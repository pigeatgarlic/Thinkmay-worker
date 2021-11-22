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

#ifdef G_OS_WIN32
#include <Windows.h>
#endif


struct _ChildProcess
{
    AgentServer* agent;

#ifndef G_OS_WIN32
    GSubprocess* process;
    GInputStream* process_stderr;
    GInputStream* process_stdout;
#else
    HANDLE process;
    HANDLE process_stderr;
    HANDLE process_stdout;
#endif

    ChildStdOutHandle stdout_handler;
    ChildStdErrHandle stderr_handler;
    ChildStateHandle handler;

    GThread* statehdl;

    gboolean completed;
    gpointer data;
};






#ifndef G_OS_WIN32

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

#endif



static gpointer 
handle_child_process_state(gpointer data)
{
    ChildProcess* proc = (ChildProcess*)data;
    while (TRUE)
    {
#ifndef G_OS_WIN32
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
#else
        DWORD code;
        GetExitCodeProcess(proc->process, &code);
        if(code != STILL_ACTIVE){
            proc->handler(proc,proc->agent,proc->data);
            return;
        }

        
        gchar* buffer_stderr = malloc(200);
        gchar* buffer_stdout = malloc(200);
        gchar* temp_err = buffer_stderr;
        gchar* temp_out = buffer_stdout;

        while(TRUE) {
            DWORD result,dwWritten;
            result = ReadFile(proc->process_stderr, temp_err, 1, &dwWritten, NULL);
            if(result && dwWritten) {
                if(!*temp_err)
                    break;
                else
                    temp_err++;
        }}
        while(TRUE) {
            DWORD result,dwWritten;
            result = ReadFile(proc->process_stdout, temp_out, 1, &dwWritten, NULL);
            if(result && dwWritten) {
                if(!*temp_out)
                    break;
                else
                    temp_out++;
        }}

        if(*buffer_stderr) {
            GBytes* byte = g_bytes_new(buffer_stderr,strlen(buffer_stderr));
            proc->stderr_handler(byte,proc->agent,proc->data);
        } if(*buffer_stdout) {
            GBytes* byte = g_bytes_new(buffer_stdout,strlen(buffer_stdout));
            proc->stdout_handler(byte,proc->agent,proc->data);
        }

        free(buffer_stderr);
        free(buffer_stdout);
#endif
    }
}


void
clean_childprocess(ChildProcess* proc)
{
#ifndef G_OS_WIN32
    g_input_stream_close(proc->process_stderr,NULL,NULL);
    g_input_stream_close(proc->process_stdout,NULL,NULL);
#endif
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
    TerminateProcess(proc->process, 1);
}

#ifdef G_OS_WIN32

typedef struct _ChildPipe
{
    HANDLE standard_out;
    HANDLE standard_err;
}ChildPipe;

static ChildPipe*
initialize_process_handle(ChildProcess* self,
                          AgentServer* agent)
{
    ChildPipe* ret = malloc(sizeof(ChildPipe));

    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = TRUE;
    attr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&self->process_stdout, &ret->standard_out, &attr, 0))
        return NULL;
    if (!CreatePipe(&self->process_stderr, &ret->standard_err, &attr, 0))
        return NULL;

    SetHandleInformation(self->process_stderr, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(self->process_stdout, HANDLE_FLAG_INHERIT, 0);
    return ret;
}
#endif




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
#ifndef G_OS_WIN32
    child_process->process = g_subprocess_new(G_SUBPROCESS_FLAGS_STDERR_PIPE | 
                                              G_SUBPROCESS_FLAGS_STDOUT_PIPE,
                                              &error,process_name,NULL);
    if(error)        
        return;
    child_process->process_stderr = g_subprocess_get_stderr_pipe(child_process->process);
    child_process->process_stdout = g_subprocess_get_stdout_pipe(child_process->process);
#else


    ChildPipe* hdl = initialize_process_handle(child_process,agent);
    if(!hdl){ return NULL; }

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    /*setup startup infor(included standard input and output)*/
    STARTUPINFO startup_infor;
    ZeroMemory(&startup_infor, sizeof(startup_infor));
    startup_infor.cb = sizeof(STARTUPINFO);
    startup_infor.dwFlags |= STARTF_USESTDHANDLES;
    startup_infor.hStdOutput = hdl->standard_out;
    startup_infor.hStdError = hdl->standard_err;
    free(hdl);

    /*START process, all standard input and output are controlled by agent*/
    gboolean result = CreateProcess(NULL,
        process_name,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startup_infor, &pi);

    if(!result)
        return NULL;        
    else    
        child_process->process = pi.hProcess;
#endif

    child_process->statehdl =   g_thread_new("handle",handle_child_process_state,child_process);
    return child_process;
}


