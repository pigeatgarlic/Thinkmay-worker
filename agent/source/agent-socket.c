#include <agent-socket.h>
#include <agent-type.h>
#include <agent-session-initializer.h>
#include <agent-device.h>
#include <agent-server.h>

#include <error-code.h>
#include <logging.h>
#include <message-form.h>

#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

/// <summary>
/// contain information about websocket socket with host
/// </summary>
struct _Socket
{
    SoupSession* session;

    /// <summary>
    /// 
    /// </summary>
    gchar* token;
    
	/// <summary>
	/// 
	/// </summary>
	gchar* cluster_url;
};


static Socket socket_declare = {0};



void
get_worker_token(GObject* object, 
                 GAsyncResult* result,
                 gpointer data)
{
    gsize read;
    GError* error = NULL;
    AgentServer* agent = (AgentServer*) data;
    GInputStream* stream = soup_session_send_finish(object,result,&error);
    if(error){agent_finalize(data); return;}

    gchar *token = malloc(500); 
    gchar *temp = token;
    while (g_input_stream_read_all (stream, &temp, 1, &read, NULL, NULL) && read == 1) {
        // break if character is null
        if(!*temp)
            break;
        else
            temp++;
    } 

    agent_server_set_token(agent,token);
}



void 
send_message_to_host(AgentServer* object,
                     gchar* message)
{
    Socket* socket = agent_get_socket(object);

    SoupMessage* soupMessage = soup_message_new(SOUP_METHOD_POST,socket->cluster_url);

    if(socket->token)
    {
        SoupMessageBody* body = soupMessage->request_body;
        soup_message_headers_append(soupMessage->request_headers,"Authorization",socket->token);
        soup_message_body_append_take(body,message,strlen(message));
        soup_session_send_message(socket->session,soupMessage);
    }
    else
    {
        SoupMessageBody* body = soupMessage->request_body;
        soup_message_body_append_take(body,message,strlen(message));
        soup_session_send_async(socket->session,soupMessage,NULL,(GAsyncReadyCallback)get_worker_token,object);
    }
}



gboolean
register_with_host(AgentServer* agent)
{
    worker_log_output("Registering with host");
    gchar* package = get_registration_message();
    send_message_to_host(agent, package); 
    return TRUE;     
}






/*START get-set-function for Socket*/





Socket*
initialize_socket(gchar* token,
                  gchar* cluster_ip)
{
    const gchar* http_aliases[] = { "http", NULL };

    GString* string = g_string_new("http://");
    g_string_append(string,cluster_ip);
    g_string_append(string,":2220");


    socket_declare.cluster_url = g_string_free(string,FALSE);
    socket_declare.token = token;
    socket_declare.session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    return &socket_declare;
}



void
socket_set_token(Socket* socket,
                 gchar* token)
{
    socket->token = token;
}