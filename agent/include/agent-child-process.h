#ifndef __AGENT_CHILD_PROCESS_H__
#define __AGENT_CHILD_PROCESS_H__
#include "agent-type.h"
#include "glib.h"


/// <summary>
/// send message to child process
/// </summary>
/// <param name="self">child process to send message</param>
/// <param name="buffer">content send to child process through stdin</param>
/// <param name="size">size of content</param>
/// <returns></returns>
gboolean			send_message_to_child_process				(ChildProcess* self,
																 gchar* buffer,
																 gint size);

/// <summary>
/// close child process
/// </summary>
/// <param name="proc">child process which will be close</param>
void				wait_for_childproces						(ChildProcess* process);



/// <summary>
/// create new child process with given binary and handle function
/// </summary>
/// <param name="binary_name">name of binary</param>
/// <returns></returns>
ChildProcess*		create_new_child_process					(gchar* binary_name,
																 ChildStdErrHandle stderrhdl,
																 ChildStdOutHandle stdouthdl,
																 ChildStateHandle handler,
																 AgentServer* agent);


/// <summary>
/// 
/// </summary>
/// <param name="process"></param>
/// <returns></returns>
void 				childprocess_force_exit						(ChildProcess* proc);

/// <summary>
/// 
/// </summary>
/// <param name="proc"></param>
void 				clean_childprocess							(ChildProcess* proc);
#endif