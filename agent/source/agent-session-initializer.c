#include <agent-session-initializer.h>
#include <agent-socket.h>
#include <agent-type.h>
#include <agent-child-process.h>
#include <state-indicator.h>

#include <error-code.h>
#include <message-form.h>
#include <general-constant.h>
#include <global-var.h>

#include <gmodule.h>
#include <Windows.h>
#include <stdio.h>

#define BUFFER_SIZE 10000


struct _RemoteSession
{
    ChildProcess* process;

    gchar* session_core_url;

    SoupSession* session;
};

static RemoteSession remote_session_singleton;

RemoteSession*
intialize_remote_session_service()
{
    GString* base_url = g_string_new("http://localhost:");
    g_string_append(base_url,SESSION_CORE_PORT);
    g_string_append(base_url,"/agent");
    gchar* url = g_string_free(base_url,FALSE);

    const gchar* http_aliases[] = { "http", NULL };
    remote_session_singleton.session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    remote_session_singleton.process = NULL;
    remote_session_singleton.session_core_url = url;
    return &remote_session_singleton;
}


static void
handler_session_core_state_function(ChildProcess* proc,
                                    AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);
    GString* string = g_string_new("http://localhost:");
    g_string_append(string,SESSION_CORE_PORT);
    session->session_core_url = g_string_free(string,FALSE);

    SoupMessage* message = soup_message_new(SOUP_METHOD_POST,session->session_core_url);
    soup_message_headers_append(message->request_headers,"Authorization",TOKEN);

    SoupMessageBody* body = message->request_body;
    
    soup_session_send(session->session,message,NULL,NULL);
}


static void
handle_session_core_error(GBytes* buffer,
    AgentServer* agent,
    gpointer data)
{
    gchar* message = g_bytes_get_data(buffer, NULL);
}

static void
handle_session_core_output(GBytes* buffer,
    AgentServer* agent,
    gpointer data)
{
    gchar* message = g_bytes_get_data(buffer, NULL);
}

gboolean
session_reconnect(AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);

    GString* core_script = g_string_new(SESSION_CORE_BINARY);
    g_string_append(core_script,"--token=");
    g_string_append(core_script,TOKEN);

    session->process =
    create_new_child_process(g_string_free(core_script,FALSE),
        (ChildStdErrHandle)handle_session_core_error,
        (ChildStdOutHandle)handle_session_core_output,
        (ChildStateHandle)handler_session_core_state_function, agent,NULL);
}

gboolean
session_disconnect(AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);
    childprocess_force_exit(session->process);
    clean_childprocess(session->process);
}

gboolean
session_terminate(AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);
    childprocess_force_exit(session->process);
    clean_childprocess(session->process);
}

gboolean
session_initialize(AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);

    session->process =
    create_new_child_process(SESSION_CORE_BINARY,
        (ChildStdErrHandle)handle_session_core_error,
        (ChildStdOutHandle)handle_session_core_output,
        (ChildStateHandle)handler_session_core_state_function, agent,NULL);
}

gboolean
send_message_to_core(AgentServer* agent, gchar* buffer)
{
    RemoteSession* session = agent_get_remote_session(agent);
    GString* string = g_string_new("http://localhost:");
    g_string_append(string,SESSION_CORE_PORT);
    session->session_core_url = g_string_free(string,FALSE);

    SoupMessage* message = soup_message_new(SOUP_METHOD_POST,session->session_core_url);
    soup_message_headers_append(message->request_headers,"Authorization",TOKEN);

    SoupMessageBody* body = message->request_body;
    soup_message_body_append_take(body,buffer,strlen(buffer));
    
    soup_session_send(session->session,message,NULL,NULL);
}