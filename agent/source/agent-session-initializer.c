#include <agent-session-initializer.h>
#include <agent-socket.h>
#include <agent-type.h>
#include <agent-child-process.h>
#include <state-indicator.h>

#include <child-process-constant.h>
#include <error-code.h>
#include <message-form.h>
#include <general-constant.h>

#include <gmodule.h>
#include <Windows.h>
#include <stdio.h>

#define BUFFER_SIZE 10000


struct _RemoteSession
{
    ChildProcess* process;

    gchar session_core_url[20];

    SoupSession* session;
};

static RemoteSession remote_session_singleton;

RemoteSession*
intialize_remote_session_service(AgentServer* agent,
                                 gchar* session_core_port)
{
    GString* base_url = g_string_new("http://localhost:");
    g_string_append(base_url,session_core_port);
    g_string_append(base_url,"/agent");
    gchar* url = g_string_free(base_url,FALSE);

    const gchar* http_aliases[] = { "http", NULL };
    remote_session_singleton.session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    remote_session_singleton.process = NULL;
    memcpy(remote_session_singleton.session_core_url,url,strlen(url));
    return &remote_session_singleton;
}


static void
handler_session_core_state_function(ChildProcess* proc,
                                    AgentServer* agent)
{
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
session_terminate(AgentServer* agent)
{
    RemoteSession* session = agent_get_remote_session(agent);

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
    SoupMessage* message = soup_message_new(SOUP_METHOD_POST,session->session_core_url);

    gchar* token = agent_get_token(agent);
    soup_message_headers_append(message->request_headers,"Authorization",token);

    SoupMessageBody* body = message->request_body;
    soup_message_body_append_take(body,buffer,strlen(buffer));
    
    soup_session_send(session->session,message,NULL,NULL);
}