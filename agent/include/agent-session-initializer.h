#ifndef __AGENT_SESSION_INITIALIZER_H__ 
#define __AGENT_SESSION_INITIALIZER_H__ 

#include <glib.h>
#include <agent-server.h>
#include <agent-type.h>

#include <Windows.h>


/// <summary>
/// (ATOMIC function)
/// send message to session core using standard input output (stdio)
/// should only be used by send_message function
/// </summary>
/// <param name="self"></param>
/// <param name="buffer"></param>
/// <returns></returns>
gboolean			send_message_to_core							(AgentServer* self,
																	 gchar* buffer);

/// <summary>
/// initialize session, include create std pipe and session core and session loader child process
/// should be invoked when there is a request from host
/// </summary>
/// <param name="object"></param>
/// <returns></returns>
gboolean			session_initialize   							(AgentServer* object);

/// <summary>
/// terminate session, included termination of two child process and cleaning some garbagge
/// and assign NULL value to session 
/// </summary>
/// <param name="object"></param>
/// <returns></returns>
gboolean 			session_terminate   							(AgentServer* object);

/// <summary>
/// disconnect remote control with user, unlike session terminate,
/// disconnect process do not assign NULL to agent->session
/// </summary>
/// <param name="object"></param>
/// <returns></returns>
gboolean			remote_control_disconnect						(AgentServer* object);

/// <summary>
/// initialize remote control based on session structure in agent_object
/// </summary>
/// <param name="object"></param>
/// <returns></returns>
gboolean			remote_control_reconnect						(AgentServer* object);

/// <summary>
/// 
/// </summary>
/// <param name="agent"></param>
RemoteSession*		intialize_remote_session_service				(AgentServer* agent,
                                 								     gchar* session_core_port);

/// <summary>
/// 
/// </summary>
/// <param name="agent"></param>
/// <param name="data"></param>
void				setup_session									(AgentServer* agent, 
																	 gchar* data);
																	 
#endif