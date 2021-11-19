#include <glib.h>
#include <logging.h>
#include <gio/gio.h>
#include <string.h>

#include <libsoup/soup.h>

#include <message-form.h>






void
worker_log_output(gchar* text)
{
    const gchar* http_aliases[] = { "http", NULL };
    SoupSession* session = soup_session_new_with_options(
            SOUP_SESSION_SSL_STRICT, FALSE,
            SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
            SOUP_SESSION_HTTPS_ALIASES, http_aliases, NULL);

    SoupMessage* message = soup_message_new(SOUP_METHOD_POST,"http://192.168.1.12/Log");
    soup_message_set_request(message,"application/text",SOUP_MEMORY_COPY,text,strlen(text));
    soup_session_send(session,message,NULL,NULL);    
}                  
