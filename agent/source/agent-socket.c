/**
 * @file agent-socket.c
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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
	gchar cluster_url[50];
};








gboolean
send_message_to_host(AgentServer* object,
                     gchar* message)
{
    Socket* socket = agent_get_socket(object);
    SoupMessage* soupMessage = soup_message_new(SOUP_METHOD_POST,socket->cluster_url);

    soup_message_headers_append(soupMessage->request_headers,"Authorization",DEVICE_TOKEN);
    soup_message_set_request(soupMessage,"application/json",
        SOUP_MEMORY_COPY,message,strlen(message));
    soup_session_send_async(socket->session,soupMessage,NULL,NULL,NULL);
}



gboolean
register_with_host(AgentServer* agent)
{
    GError* error = NULL;
    Socket* socket = agent_get_socket(agent);
    worker_log_output("Registering with host");

    gchar* package = get_registration_message();
    SoupMessage* soupMessage = soup_message_new(SOUP_METHOD_POST,socket->cluster_url);

    soup_message_headers_append(soupMessage->request_headers,"Authorization",TOKEN);
    soup_message_set_request(soupMessage,"application/json", SOUP_MEMORY_COPY,package,strlen(package));

    soup_session_send_message(socket->session,soupMessage);

    JsonParser* parser = json_parser_new();
    JsonObject* result_json = get_json_object_from_string(soupMessage->response_body->data,&error,parser);
    gchar* token_result = json_object_get_string_member(result_json,"token");
    g_object_unref(parser); 
    if(!token_result) { return FALSE; }


    memcpy(DEVICE_TOKEN,token_result,strlen(token_result));
    return TRUE; 
}






/*START get-set-function for Socket*/

Socket*
initialize_socket()
{
    Socket* socket = malloc(sizeof(Socket));
    memset(socket,0,sizeof(Socket)); 
    const gchar* http_aliases[] = { "http", NULL };

    GString* string = g_string_new("http://");
    g_string_append(string,CLUSTER_IP);
    g_string_append(string,":2220");
    gchar* url = g_string_free(string,FALSE);

    memcpy( socket->cluster_url,url,strlen(url)); 
    socket->session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    return socket;
}