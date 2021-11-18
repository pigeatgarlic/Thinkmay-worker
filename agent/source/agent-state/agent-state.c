#include <agent-state.h>
#include <agent-server.h>
#include <agent-type.h>
#include <glib.h>

#include <state-indicator.h>
#include <error-code.h>


static void  
default_session_initialize  ( AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_session_terminate   (AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}                             

static void
default_remote_control_disconnect( AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_remote_control_reconnect(AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}


static void
default_send_message_to_host(AgentServer* agent,
                             Message* message)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_send_message_to_local_module(AgentServer* agent,
                                     Message* message)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_connect_to_host(AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_register_to_host(AgentServer* agent)
{
    agent_report_error(agent,AGENT_STATE_CONFLICT);
}

static void
default_on_session_core_exit(AgentServer* agent)
{
    return;
}

static void
default_on_shell_process_exit(AgentServer* agent)
{
    return;
}

static gchar* 
default_get_current_state(void)
{
    return AGENT_UNDEFINED;
}

void
default_method(AgentState* state)
{
    state->session_terminate =          default_session_terminate;
    state->remote_control_disconnect =  default_remote_control_disconnect;
    state->session_initialize =         default_session_initialize;
    state->remote_control_reconnect =   default_remote_control_reconnect;
    state->send_message_to_host =       default_send_message_to_host;
    state->send_message_to_session_core = default_send_message_to_local_module;
    state->send_message_to_session_loader = default_send_message_to_local_module;
    state->connect_to_host =            default_connect_to_host;
    state->register_to_host =           default_register_to_host;
    state->on_session_core_exit =       default_on_session_core_exit;
    state->get_current_state =          default_get_current_state;
    state->on_shell_process_exit =        default_on_shell_process_exit;
}