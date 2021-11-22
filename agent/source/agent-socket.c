#include <agent-socket.h>
#include <agent-type.h>
#include <agent-session-initializer.h>
#include <agent-device.h>
#include <agent-server.h>

#include <error-code.h>
#include <logging.h>
#include <message-form.h>
#include <global-var.h>

#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

/// <summary>
/// contain information about websocket socket with host
/// </summary>
struct _Socket
{
    /// <summary>
    /// 
    /// </summary>
    SoupSession* session;
    
	/// <summary>
	/// 
	/// </summary>
	gchar* cluster_url;
};


static Socket socket_declare = {0};






gboolean
send_message_to_host(AgentServer* object,
                     gchar* message)
{
    Socket* socket = agent_get_socket(object);
    SoupMessage* soupMessage = soup_message_new(SOUP_METHOD_POST,socket->cluster_url);

    soup_message_headers_append(soupMessage->request_headers,"Authorization",TOKEN);
    soup_message_set_request(soupMessage,"application/json",
        SOUP_MEMORY_COPY,message,strlen(message));
    soup_session_send_async(socket->session,soupMessage,NULL,NULL,NULL);
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
initialize_socket(gchar* token)
{
    const gchar* http_aliases[] = { "http", NULL };

    GString* string = g_string_new("http://");
    g_string_append(string,CLUSTER_IP);
    g_string_append(string,":2220");


    socket_declare.cluster_url = g_string_free(string,FALSE);
    socket_declare.session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    return &socket_declare;
}