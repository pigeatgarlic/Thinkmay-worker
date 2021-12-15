/**
 * @file logging.c
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <glib.h>
#include <logging.h>
#include <gio/gio.h>
#include <string.h>

#include <libsoup/soup.h>
#include <global-var.h>
#include <development.h>







void
worker_log_output(gchar* text)
{

    if(!DEVELOPMENT_ENVIRONMENT)
    {
        if(!strlen(DEVICE_TOKEN))
            return;

        const gchar* http_aliases[] = { "http", NULL };
        SoupSession* session = soup_session_new_with_options(
                SOUP_SESSION_SSL_STRICT, FALSE,
                SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
                SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

        // get log url from clusterip
        GString* string = g_string_new("http://");
        g_string_append(string,CLUSTER_IP);
        g_string_append(string,":5000/log");
        gchar* log_url = g_string_free(string,FALSE);
        SoupMessage* message = soup_message_new(SOUP_METHOD_POST,log_url);





        // copy from buffer to soup message
        soup_message_set_request(message,"application/json",
            SOUP_MEMORY_COPY,text,strlen(text));


        soup_session_send_async(session,message,NULL,NULL,NULL);    
    }

    if(g_utf8_validate(text,-1,NULL))
    {
        g_print(text);
        g_print("\n");
    }
}                  