/// <summary>
/// @file agent-shell-session.c
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-11-19
/// 
/// @copyright Copyright (c) 2021
/// 
#include <agent-shell-session.h>
#include <agent-type.h>
#include <agent-child-process.h>
#include <agent-socket.h>


#include <logging.h>
#include <general-constant.h>
#include <message-form.h>
#include <error-code.h>
#include <agent-server.h>
#include <child-process-constant.h>
#include <child-process-resources-assign.h>

   
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
    ShellSession* session = (ShellSession*) data;
    
    gsize size;
    gchar* buffer = g_bytes_get_data(output,&size);

    SoupMessageBody* body = session->message->response_body;
    soup_message_body_append_take(body,buffer,size);
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

    SoupBuffer* buffer = soup_message_body_get_chunk(message->request_body,0);
    g_file_set_contents(file_name,
        buffer->data,
        buffer->length,&error);
    if(error){ return; }


    GString* string = g_string_new("powershell ");
    g_string_append(string,file_name);
    gchar* script = g_string_free(string,FALSE); 

    session->process = create_new_child_process(string,NULL,
        handle_powershell_output,NULL,agent);
    
    wait_for_childproces(session->process);
    g_file_delete(session->file,NULL,NULL);
    clean_childprocess(session->process); 
    g_free(string);
    free(session);
}