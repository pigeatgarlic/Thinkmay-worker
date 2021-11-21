#include <session-core-signalling.h>
#include <session-core-remote-config.h>
#include <session-core-pipeline.h>
#include <session-core-data-channel.h>
#include <session-core.h>
#include <session-core-type.h>

#include <error-code.h>
#include <module-code.h>
#include <opcode.h>


#include <glib.h>
#include <logging.h>
#include <message-form.h>


struct _SessionCore
{
	SoupServer* server;

	Pipeline* pipe;

	WebRTCHub* hub;

	GMainLoop* loop;

	SignallingHub* signalling;

	QoE* qoe;

	gchar* token;
};




static SessionCore core_declare;

void	   server_callback (SoupServer        *server,
							SoupMessage	   *msg,
							const char        *path,
							GHashTable        *query,
							gpointer           user_data);
/// <summary>
/// setup slave session, this step include get value from json config file 
/// </summary>
/// <param name="self"></param>
static void
session_core_setup_session(SessionCore* self)
{
	pipeline_set_state(self->pipe, PIPELINE_READY);
	worker_log_output("session core setup done");
}

static SoupServer*
init_session_core_server()
{
	GError* error = NULL;
	SoupServer* server = soup_server_new(NULL);

	soup_server_add_handler(server,
		"/",server_callback,&core_declare,NULL);

	soup_server_listen_all(server,2250,0,&error);
	if(error){g_printerr(error->message); return;}
	return server;
}



void
server_callback (SoupServer        *server,
                 SoupMessage	   *msg,
		 		 const char        *path,
                 GHashTable        *query,
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
				"application/text",SOUP_MEMORY_COPY,response,strlen(response));
			msg->status_code = SOUP_STATUS_OK;
			return;
		}
		if(!g_strcmp0(name,"Authorization") &&
		   !g_strcmp0(value,core->token))
		{
			if(!g_strcmp0(uri->path,"/agent/Message"))
			{

				msg->status_code = SOUP_STATUS_OK;
				return;
			}
		}
	}
	msg->status_code = SOUP_STATUS_UNAUTHORIZED;
}



SessionCore*
session_core_initialize()
{
	worker_log_output("Session core process started");

	core_declare.server = 			init_session_core_server();
	core_declare.hub =				webrtchub_initialize();
	core_declare.signalling =		signalling_hub_initialize(&core_declare);
	core_declare.qoe =				qoe_initialize();
	core_declare.pipe =				pipeline_initialize(&core_declare);
	core_declare.loop =				g_main_loop_new(NULL, FALSE);

	session_core_setup_session(&core_declare);
	session_core_connect_signalling_server(&core_declare);
	g_main_loop_run(core_declare.loop);
	return &core_declare;	
}






void
session_core_connect_signalling_server(SessionCore* self)
{
	connect_to_websocket_signalling_server_async(self);
}

void
session_core_setup_pipeline(SessionCore* self)
{
	setup_pipeline(self);
}				




void
session_core_finalize(SessionCore* self, 
					  GError* error)
{
	worker_log_output(error->message);
	g_main_loop_unref(self->loop);
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


QoE*
session_core_get_qoe(SessionCore* self)
{
	return self->qoe;
}


SignallingHub*
session_core_get_signalling_hub(SessionCore* core)
{
	return core->signalling;
}
