#include <agent-child-process.h>
#include <agent-type.h>
#include <agent-server.h>


#include <state-indicator.h>
#include <logging.h>
#include <error-code.h>

#include <glib.h>

#include <general-constant.h>
#include <error-code.h>

#define BUFSIZE 5000


struct _ChildProcess
{
    AgentServer* agent;

    GSubprocess* process;
    GInputStream* process_stderr;
    GInputStream* process_stdout;

    ChildStdOutHandle stdout_handler;
    ChildStdErrHandle stderr_handler;
    ChildStateHandle handler;

    GThread* statehdl;

    gboolean completed;
    gpointer data;
};








static gpointer
handle_child_process_stdout(GInputStream* stream,
                            GAsyncResult* result,
                            gpointer data)
{
    GError* error = NULL;
    ChildProcess* proc = (ChildProcess*) data;

    GBytes* output = g_input_stream_read_bytes_finish(proc->process_stderr,result,&error);    
    proc->stdout_handler(output, proc, proc->agent);
    g_bytes_unref(output);
}


static gpointer
handle_child_process_stderr(GInputStream* stream,
                            GAsyncResult* result,
                            gpointer data)
{
    GError* error = NULL;
    ChildProcess* proc = (ChildProcess*) data;

    GBytes* output = g_input_stream_read_bytes_finish(proc->process_stderr,result,&error);    
    proc->stderr_handler(output, proc, proc->agent,proc->data);
    g_bytes_unref(output);
}




static gpointer 
handle_child_process_state(gpointer data)
{
    ChildProcess* proc = (ChildProcess*)data;
    while (TRUE)
    {
        gboolean exited = g_subprocess_get_if_exited(proc->process);
        if(exited)
        {
            proc->handler(proc,proc->agent,proc->data);
            return;
        }
        g_input_stream_read_bytes_async(proc->process_stderr,BUFSIZE,G_PRIORITY_LOW,NULL,
            (GAsyncReadyCallback)handle_child_process_stderr,proc);

        g_input_stream_read_bytes_async(proc->process_stdout,BUFSIZE,G_PRIORITY_LOW,NULL,
            (GAsyncReadyCallback)handle_child_process_stdout,proc);
    }
}


void
clean_childprocess(ChildProcess* proc)
{
    g_input_stream_close(proc->process_stderr,NULL,NULL);
    g_input_stream_close(proc->process_stdout,NULL,NULL);
    free(proc);
}

void
wait_for_childproces(ChildProcess* process)
{
    g_thread_join(process->statehdl);
}


void
childprocess_force_exit(ChildProcess* proc)
{
    g_subprocess_force_exit(proc->process);
}

ChildProcess*
create_new_child_process(gchar* process_name,
                        ChildStdErrHandle stderrhdl,
                        ChildStdOutHandle stdouthdl,
                        ChildStateHandle handler,
                        AgentServer* agent,
                        gpointer data)
{
    GError* error = NULL;
    ChildProcess* child_process = malloc(sizeof(ChildProcess));
    child_process->data = data;
    child_process->agent = agent;
    child_process->stderr_handler = stderrhdl,
    child_process->stdout_handler = stdouthdl,
    child_process->handler = handler;
    child_process->process = g_subprocess_new(G_SUBPROCESS_FLAGS_STDERR_PIPE | 
                                              G_SUBPROCESS_FLAGS_STDOUT_PIPE,
                                              &error,POWERSHELL_BINARY,NULL);
    if(error)        
        return NULL;
    child_process->process_stderr = g_subprocess_get_stderr_pipe(child_process->process);
    child_process->process_stdout = g_subprocess_get_stdout_pipe(child_process->process);

    child_process->statehdl =   g_thread_new("handle",handle_child_process_state,child_process);
    return child_process;
}


