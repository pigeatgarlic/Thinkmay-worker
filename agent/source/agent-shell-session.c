/**
 * @file agent-shell-session.c
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <agent-shell-session.h>
#include <agent-type.h>
#include <agent-child-process.h>
#include <agent-socket.h>


#include <logging.h>
#include <message-form.h>
#include <error-code.h>
#include <agent-server.h>

   
struct _ShellSession
{
    ChildProcess* process;

    GFile* file;

    SoupMessage* message;
};


static void
character_remover(gchar** string, gchar* character)
{
    char **split = g_strsplit(*string, character, -1);
    memset(*string,0,strlen(*string));
    *string = g_strjoinv("", split);
    g_strfreev(split);
}

static void
handle_powershell_output(GBytes* output,
                         AgentServer* agent,
                         gpointer data)
{
    gsize size;
    ShellSession* session = (ShellSession*) data;
    gchar* buffer = g_bytes_get_data(output,&size);
    soup_message_set_response(session->message,
        "application/text",SOUP_MEMORY_COPY,buffer,size);
}

void
initialize_shell_session(AgentServer* agent,
                         SoupMessage* message)
{
    GError* error = NULL;
    ShellSession* session = malloc(sizeof(ShellSession));
    session->message = message;

    session->file = g_file_new_for_path(g_get_current_dir());
    gchar* file_name = g_file_get_path(session->file);

    message->status_code = SOUP_STATUS_OK;
    SoupBuffer* buffer = soup_message_body_get_chunk(message->request_body,0);
    g_file_set_contents(file_name,
        buffer->data,
        buffer->length,&error);
    if(error){ return; }


    GString* string = g_string_new("powershell ");
    g_string_append(string,file_name);
    gchar* script = g_string_free(string,FALSE); 

    session->process = create_new_child_process(string,
                                                NULL,
                                                handle_powershell_output,
                                                NULL,agent,session);
    
    wait_for_childproces(session->process);
    g_file_delete(session->file,NULL,NULL);
    clean_childprocess(session->process); 
    g_free(string);
    free(session);
}