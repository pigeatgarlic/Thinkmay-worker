#include <agent-server.h>
#include <agent-type.h>
#include <agent-session-initializer.h>
#include <agent-socket.h>
#include <agent-device.h>
#include <agent-message.h>
#include <agent-shell-session.h>
#include <agent-device.h>
#include <agent-state.h>
#include <agent-child-process.h>
#include <agent-state-unregistered.h>
#include <agent-file-transfer-service.h>



#include <stdio.h>
#include <libsoup/soup.h>
#include <glib-2.0/glib/gstdio.h>

#include <child-process-constant.h>
#include <general-constant.h>
#include <logging.h>
#include <error-code.h>
#include <message-form.h>

/// <summary>
/// agent object 
/// </summary>
struct _AgentServer
{
	/// <summary>
	/// socket object
	/// </summary>
	Socket* socket;

	/// <summary>
	/// gmainloop will be run in agent process
	/// </summary>
	GMainLoop* loop;

	/// <summary>
	/// 
	/// </summary>
	SoupServer* server;

	/// <summary>
	/// state of the slave device
	/// </summary>
	AgentState* state;

	/// <summary>
	/// Child process array, agent module are capable to create 
	/// new child process in admintrator privillege 
	/// and has full control of child process
	/// (include handle stdout, child process state)
	/// </summary>
	ChildProcess* child_process[MAX_CHILD_PROCESS];

	/// <summary>
	/// 
	/// </summary>
	FileTransferSession* file_transfer_session[MAX_FILE_TRANSFER_INSTANCE];

	/// <summary>
	/// 
	/// </summary>
	ShellSession* shell_session[MAX_POWERSHELL_INSTANCE];

	/// <summary>
	/// 
	/// </summary>
	RemoteSession* remote_session;
};











static AgentServer agent_declare = {0};


void
server_callback (SoupServer        *server,
                 SoupMessage	   *msg,
		 		 const char        *path,
                 GHashTable        *query,
		 		 gpointer           user_data);

void
do_post (SoupServer *server, 
		SoupMessage *msg, 
		const char *path);


static SoupServer*
init_agent_server()
{
	SoupServer* server = g_object_new(SOUP_TYPE_SERVER,NULL);

	soup_server_add_handler(agent_declare.server,
		"/agent/Initialize",server_callback,&agent_declare,NULL);

	soup_server_add_handler(agent_declare.server,
		"/agent/Terminate",server_callback,&agent_declare,NULL);
		
	soup_server_add_handler(agent_declare.server,
		"/agent/Disconnect",server_callback,&agent_declare,NULL);

	soup_server_add_handler(agent_declare.server,
		"/agent/Reconnect",server_callback,&agent_declare,NULL);

	soup_server_add_handler(agent_declare.server,
		"/agent/Shell",server_callback,&agent_declare,NULL);

	soup_server_listen_all(agent_declare.server,2250,0,&error);
	if(error){g_printerr(error->message); return;}
}


AgentServer*
agent_new(gint url, 
		gint session_core_port, 
		gchar* manager_url, 
		gchar* worker_ip)
{	
	GError* error = NULL;
	//set initial state of agent as unregistered	
	AgentState* unregistered = transition_to_unregistered_state();
	agent_declare.state = unregistered;
	agent_declare.server = init_agent_server();
	if(!agent_declare.server){return;}




	initialize_child_process_system(&agent_declare);
	initialize_file_transfer_service(&agent_declare);
	intialize_remote_session_service(&agent_declare);
	agent_declare.socket=initialize_socket(&agent_declare);
	// connect to host with given id
	agent_connect_to_host(&agent_declare);

	// start gmainloop, 
	agent_declare.loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(agent_declare.loop);
	return NULL;
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
	SoupHTTPVersion http_version;


	soup_message_headers_iter_init (&iter, msg->request_headers);
	while (soup_message_headers_iter_next (&iter, &name, &value))
	{
		if(g_strcmp0())
		
	}

        request_body = msg->request_body;
	if (request_body->length)
		g_print ("%s\n", request_body->data);

	file_path = g_strdup_printf (".%s", path);

	if (msg->method == SOUP_METHOD_POST)
		do_post (server, msg, file_path);
	else
		msg->status_code = SOUP_STATUS_METHOD_NOT_ALLOWED;
	g_free (file_path);

	gint status_code;
	g_object_get(msg,"status-code",&status_code,NULL);

	g_print("  -> %d \n\n", status_code);
}

void
do_post (SoupServer *server, 
		SoupMessage *msg, 
		const char *path)
{
	GStatBuf st;
	FILE *f;
	gboolean created = TRUE;
	SoupMessageBody *request_body;

	if (g_stat (path, &st) != -1) {
		const char *match = soup_message_headers_get_one (msg->request_headers, "If-None-Match");
		if (match && !strcmp (match, "*")) {
			msg->status_code = SOUP_STATUS_CONFLICT;
			return;
		}

		if (!g_file_test (path, G_FILE_TEST_IS_REGULAR)) {
			msg->status_code = SOUP_STATUS_CONFLICT;
			return;
		}

		created = FALSE;
	}

	f = fopen (path, "w");
	if (!f) {
		msg->status_code = SOUP_STATUS_INTERNAL_SERVER_ERROR;
		return;
	}

	request_body = msg->request_body;
	fwrite (request_body->data, 1, request_body->length, f);
	fclose (f);

	msg->status_code = SOUP_STATUS_OK;
}


void
agent_finalize(AgentServer* self)
{
	socket_close(self->socket);
	agent_session_terminate(self);
	if (self->loop)
	{
		g_main_loop_quit(self->loop);
	}
}









void
agent_report_error(AgentServer* self,
				   gchar* message)
{
	JsonParser* parser = json_parser_new();
	json_parser_load_from_file(parser, HOST_CONFIG_FILE,NULL);
	JsonNode* root = json_parser_get_root(parser);
	JsonObject* json = json_node_get_object(root);
	gint SlaveID = json_object_get_int_member(json,DEVICE_ID);


	JsonObject* obj = json_object_new();
	json_object_set_int_member(obj,
		"SlaveID",SlaveID);
	json_object_set_int_member(obj,
		"Module",AGENT_MODULE);	
	json_object_set_string_member(obj,
		"ErrorMessage",message);
		

	Message* msg = message_init(AGENT_MODULE,
		HOST_MODULE, ERROR_REPORT, obj);
	agent_send_message(self,msg);
}


void
agent_register_with_host(AgentServer* self)
{
	self->state->register_to_host(self);
}


void
agent_connect_to_host(AgentServer* self)
{
	self->state->connect_to_host(self);
}

void
agent_on_shell_process_terminate(AgentServer* self, gint process_id)
{
	self->state->on_shell_process_exit(self, process_id);
}

void
agent_send_message(AgentServer* self,
	Message* message)
{
	// write_to_log_file(AGENT_GENERAL_LOG, 
	// 	get_string_from_json_object(message));
	send_message(self, message);
}

void
agent_send_message_to_host(AgentServer* self,
	gchar* message)
{
	self->state->send_message_to_host(self, message);
}  

void
agent_send_message_to_session_core(AgentServer* self,
	gchar* message)
{
	self->state->send_message_to_session_core(self, message);
}

void
agent_send_message_to_session_loader(AgentServer* self,
	gchar* message)
{
	self->state->send_message_to_session_loader(self, message);
}

void
agent_session_initialize(AgentServer* self)
{
	self->state->session_initialize(self);
}

void										
agent_session_terminate(AgentServer* self)
{
	self->state->session_terminate(self);
}

void										
agent_remote_control_disconnect(AgentServer* self)
{
	self->state->remote_control_disconnect(self);
}

void										
agent_remote_control_reconnect(AgentServer* self)
{
	self->state->remote_control_reconnect(self);
}

void
agent_on_session_core_exit(AgentServer* self)
{
	self->state->on_session_core_exit(self);
}

gchar*
agent_get_current_state_string(AgentServer* self)
{
	return self->state->get_current_state();
}

void
agent_start_file_transfer(gchar* server_commmand)
{
	FileTransferSession* session = setup_file_transfer_session(server_commmand);
	start_file_transfer(session);
}





















/*START get-set function*/



/*START get-set function*/
Socket*
agent_get_socket(AgentServer* self)
{
	return self->socket;
}

void
agent_set_socket(AgentServer* self, Socket* socket)
{
	self->socket = socket;
}


void
agent_set_state(AgentServer* object, AgentState* state)
{
	object->state = state;
}

AgentState*
agent_get_state(AgentServer* self)
{
	return self->state;
}


ChildProcess*
agent_get_child_process(AgentServer* self, gint position)
{
	return self->child_process[position];
}

void
agent_set_child_process(AgentServer* self,
	gint postion,
	ChildProcess* process)
{
	self->child_process[postion] = process;
}

void
agent_set_file_transfer_service(AgentServer* self,
	gint position,
	FileTransferSession* session)
{
	self->file_transfer_session[position] = session;
}


void
agent_set_main_loop(AgentServer* self,
	GMainLoop* loop)
{
	self->loop = loop;
}

GMainLoop*
agent_get_main_loop(AgentServer* self)
{
	return self->loop;
}


RemoteSession*
agent_get_remote_session(AgentServer* self)
{
	return self->remote_session;
}

void
agent_set_remote_session(AgentServer* self, 
						 RemoteSession* session)
{
	self->remote_session = session;
}
