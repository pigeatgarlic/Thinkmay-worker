/**
 * @file session-core.c
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <session-core-signalling.h>
#include <session-core-remote-config.h>
#include <session-core-pipeline.h>
#include <session-core-data-channel.h>
#include <session-core.h>
#include <session-core-type.h>
#include <session-core-qos.h>

#include <error-code.h>
#include <module-code.h>
#include <opcode.h>
#include <logging.h>
#include <message-form.h>
#include <global-var.h>
#include <token-validate.h>
#include <development.h>
#include <human-interface-opcode.h>


#include <glib.h>

#ifdef G_OS_WIN32
#include <Windows.h>

#endif


struct _SessionCore
{
	/**
	 * @brief 
	 * Soup server for receiving ping from cluster manager
	 */
	SoupServer* server;

	/**
	 * @brief 
	 * pipeline of the stream
	 */
	Pipeline* pipe;

	/**
	 * @brief 
	 * webrtchub to communicate with client
	 */
	WebRTCHub* hub;

	/**
	 * @brief 
	 * mainloop run throughout the stream
	 */
	GMainLoop* loop;

	/**
	 * @brief 
	 * signalling hub for connection with signalling server
	 */
	SignallingHub* signalling;

	/**
	 * @brief 
	 * StreamConfig of the stream
	 */
	StreamConfig* qoe;

	/**
	 * @brief 
	 * 
	 */
	DeviceType peer_device;

	/**
	 * @brief 
	 * 
	 */
	CoreEngine peer_engine;
};





/**
 * @brief 
 * callback function used to handle soup message from client,
 * those message will only come from agent and cluster manager 
 * @param server session core soup server
 * @param msg message that need to handle
 * @param path 
 * @param query 
 * @param user_data pointer to session core
 */
void	   server_callback (SoupServer        *server,
							SoupMessage	   	  *msg,
							const char        *path,
							GHashTable        *query,
							gpointer           user_data);





/**
 * @brief 
 * setup session by request input from database with worker token given by agent
 * @param self session core
 */
static void
session_core_setup_session(SessionCore* self)
{
	JsonParser* token_parser = json_parser_new();
	gchar* remote_token;


	if(DEVELOPMENT_ENVIRONMENT)
	{
		remote_token = DEFAULT_CORE_TOKEN;
	}
	else
	{
		const char* http_aliases[] = { "http", NULL };
		SoupSession* http_session = soup_session_new_with_options(
				SOUP_SESSION_SSL_STRICT, FALSE,
				SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
				SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

		GString* token_url= g_string_new("http://");
		g_string_append(token_url,	CLUSTER_IP);
		g_string_append(token_url,":5000/session/token");
		gchar* token_str = g_string_free(token_url,FALSE);

		worker_log_output("getting remote token from server\n");
		worker_log_output(token_str);


		SoupMessage* token_message = soup_message_new(SOUP_METHOD_GET,token_str);


		worker_log_output("registering with device token\n");
		worker_log_output(DEVICE_TOKEN);

		soup_message_headers_append(token_message->request_headers,
			"Authorization",DEVICE_TOKEN);
		soup_session_send_message(http_session,token_message);

		if(token_message->status_code == SOUP_STATUS_OK )
		{
			GError* error = NULL;
			JsonObject* json_infor = get_json_object_from_string(token_message->response_body->data,error,token_parser);
			remote_token = json_object_get_string_member(json_infor,"token");
		}
		else 
		{
			g_printerr ("got response code %d\n",token_message->status_code);
			GError* error = malloc(sizeof(GError));
			error->message = "fail to get session information";
			session_core_finalize(self,error);
			return;
		}
	}


	if (DEVELOPMENT_ENVIRONMENT)
	{
		signalling_hub_setup(self->signalling,
			DEFAULT_TURN,
			DEVELOPMENT_SIGNALLING_URL,
			NULL,
			remote_token);

		qoe_setup(self->qoe,
					1920,
					1080,
					OPUS_ENC,
					CODEC_H265,
					HIGH_CONST);
		
		self->peer_device = WINDOW_APP;
	}
	else
	{
		worker_log_output("got remote token\n");
		worker_log_output(remote_token);

		const char* https_aliases[] = { "https", NULL };
		SoupSession* https_session = soup_session_new_with_options(
				SOUP_SESSION_SSL_STRICT, FALSE,
				SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
				SOUP_SESSION_HTTPS_ALIASES, https_aliases, NULL);

		GString* infor_url = g_string_new(SESSION_INFOR_VALIDATE_URL);
		g_string_append(infor_url,	"?token=");
		g_string_append(infor_url,	remote_token);


		gchar* infor_str = g_string_free(infor_url,FALSE);
		SoupMessage* infor_message = soup_message_new(SOUP_METHOD_GET,infor_str);

		soup_session_send_message(https_session,infor_message);


		if(infor_message->status_code == SOUP_STATUS_OK)
		{
			GError* error = NULL;
			JsonParser* parser = json_parser_new();
			JsonObject* json_infor = get_json_object_from_string(infor_message->response_body->data,error,parser);


			signalling_hub_setup(self->signalling,
				json_object_get_string_member(json_infor,"turn"),
				json_object_get_string_member(json_infor,"signallingurl"),
				json_object_get_array_member(json_infor,"stuns"),
				remote_token);

			qoe_setup(self->qoe,
						json_object_get_int_member(json_infor,"screenwidth"),
						json_object_get_int_member(json_infor,"screenheight"),
						json_object_get_int_member(json_infor,"audiocodec"),
						json_object_get_int_member(json_infor,"videocodec"),
						json_object_get_int_member(json_infor,"mode"));
			

			self->peer_device =	json_object_get_int_member(json_infor,"clientdevice");
			self->peer_engine =	json_object_get_int_member(json_infor,"clientengine");
			g_object_unref(parser);
		}
		else 
		{
			GError* error = malloc(sizeof(GError));
			error->message = "fail to get session information";
			session_core_finalize(self,error);
			return;
		}
	}
	
		
	worker_log_output("session core setup done");
}







/**
 * @brief 
 * initialize session core with message handler
 * @param core 
 * @return SoupServer* 
 */
static SoupServer*
init_session_core_server(SessionCore* core)
{
	GError* error = NULL;
	SoupServer* server = soup_server_new(NULL);

	soup_server_add_handler(server,
		"/",server_callback,core,NULL);

	soup_server_listen_all(server,2250,0,&error);
	if(error){g_printerr(error->message); return;}
	return server;
}



void
server_callback (SoupServer        *server,
                 SoupMessage	   *msg,
		 		 const char        *path,
                 GHashTable        *query,
				 SoupClientContext *ctx,
		 		 gpointer           user_data)
{
	char *file_path;
	SoupMessageHeadersIter iter;
	SoupMessageBody *request_body;
	const char *name, *value;
	SessionCore* core = (SessionCore*) user_data;
	SoupURI* uri = soup_message_get_uri(msg);

	soup_message_headers_iter_init (&iter, msg->request_headers);
	while (soup_message_headers_iter_next (&iter, &name, &value))
	{
		if(!g_strcmp0(uri->path,"/cluster/ping"))
		{
			gchar* response = "ping";
			soup_message_set_response(msg,
				"application/json",SOUP_MEMORY_COPY,response,strlen(response));
			msg->status_code = SOUP_STATUS_OK;
			return;
		}
		if(!g_strcmp0(name,"Authorization"))
		{ 
			if(!validate_token(value))
			{
				msg->status_code = SOUP_STATUS_UNAUTHORIZED;
				return;
			}
		}
	}
	if(!g_strcmp0(uri->path,"/agent/message"))
	{
		msg->status_code = SOUP_STATUS_OK;
		return;
	}
}



SessionCore*
session_core_initialize()
{
	worker_log_output("Session core process started");
	SessionCore* core = malloc(sizeof(SessionCore));

	core->server = 				init_session_core_server(core);
	core->hub =					webrtchub_initialize();
	core->signalling =			signalling_hub_initialize(core);
	core->qoe =					qoe_initialize();
	core->pipe =				pipeline_initialize(core);
	core->loop =				g_main_loop_new(NULL, FALSE);

	session_core_setup_session(core);
	signalling_connect(core);
	g_main_loop_run(core->loop);
	return core;	
}











void
session_core_finalize(SessionCore* self, 
					  GError* error)
{
	if(error)
		worker_log_output(error->message);

	if(self->loop)
		g_main_loop_unref(self->loop);

#ifdef G_OS_WIN32
	ExitProcess(0);
#endif

}




void
report_session_core_error(SessionCore* self,
						  gchar* code)
{
	worker_log_output(code);
}








Pipeline*
session_core_get_pipeline(SessionCore* self)
{
	return self->pipe;
}

WebRTCHub*
session_core_get_rtc_hub(SessionCore* self)
{
	return self->hub;
}


StreamConfig*
session_core_get_qoe(SessionCore* self)
{
	return self->qoe;
}


SignallingHub*
session_core_get_signalling_hub(SessionCore* core)
{
	return core->signalling;
}

DeviceType		
session_core_get_client_device(SessionCore* self)
{
	return self->peer_device;
}

CoreEngine
session_core_get_client_engine(SessionCore* self)
{
	return self->peer_engine;
}