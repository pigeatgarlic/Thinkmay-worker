#ifndef __AGENT_STATE_H__
#define __AGENT_STATE_H__
#include <glib.h>
#include <agent-type.h>



typedef void            (*SessionTerminate)             (AgentServer* agent);

typedef void            (*SessionInitialize)            (AgentServer* agent);

typedef void            (*RemoteControlDisconnect)      (AgentServer* agent);

typedef void            (*RemoteControlReconnect)       (AgentServer* agent);

typedef void            (*SendMessageToHost)            (AgentServer* agent, gchar* message);

typedef void            (*SendMessageToSessionCore)     (AgentServer* agent,gchar* message);

typedef void            (*SendMessageToSessionLoader)   (AgentServer* agent, gchar* message);

typedef void            (*ConnectToHost)                (AgentServer* agent);

typedef void            (*RegisterToHost)               ( AgentServer* agent);

typedef void            (*OnSessionCoreExit)            (AgentServer* agent);

typedef void            (*OnShellSessionExit)            (AgentServer* agent, 
                                                          gint process_id);

typedef gchar*          (*GetCurrentState)              (void);

void                    default_method                  (AgentState* state);

struct _AgentState
{
    SessionTerminate session_terminate;

    SessionInitialize session_initialize;

    RemoteControlDisconnect remote_control_disconnect;

    RemoteControlReconnect remote_control_reconnect;

    SendMessageToHost send_message_to_host;

    SendMessageToSessionCore send_message_to_session_core;

    SendMessageToSessionLoader send_message_to_session_loader;

    OnSessionCoreExit on_session_core_exit;

    ConnectToHost connect_to_host;

    RegisterToHost register_to_host;

    OnShellSessionExit on_shell_process_exit;

    GetCurrentState get_current_state;
};

#endif