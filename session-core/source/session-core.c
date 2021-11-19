#include <session-core-signalling.h>
#include <session-core-remote-config.h>
#include <session-core-pipeline.h>
#include <session-core-data-channel.h>
#include <session-core.h>
#include <session-core-type.h>

#include <error-code.h>
#include <module-code.h>
#include <opcode.h>


#include <glib.h>
#include <logging.h>
#include <message-form.h>


struct _SessionCore
{
	Pipeline* pipe;

	WebRTCHub* hub;

	CoreState state;

	GMainLoop* loop;

	SignallingHub* signalling;

	QoE* qoe;

	IPC* ipc;
};





/// <summary>
/// setup slave session, this step include get value from json config file 
/// </summary>
/// <param name="self"></param>
static void
session_core_setup_session(SessionCore* self)
{
	pipeline_set_state(self->pipe, PIPELINE_READY);
	self->state = SESSION_INFORMATION_SETTLED;
	worker_log_output("session core setup done");
}

SessionCore*
session_core_initialize()
{
	static SessionCore core;
	worker_log_output("Session core process started");

	core.hub =				webrtchub_initialize();
	core.signalling =		signalling_hub_initialize(&core);

	core.qoe =				qoe_initialize();
	core.pipe =				pipeline_initialize(&core);

	core.state =			SESSION_CORE_INITIALIZING;
	core.loop =				g_main_loop_new(NULL, FALSE);
	 
	session_core_setup_session(&core);


	session_core_connect_signalling_server(&core);
	g_main_loop_run(core.loop);
	return &core;	
}






void
session_core_connect_signalling_server(SessionCore* self)
{
	connect_to_websocket_signalling_server_async(self);
}

void
session_core_setup_pipeline(SessionCore* self)
{
	setup_pipeline(self);
}				




void
session_core_finalize(SessionCore* self, 
					  GError* error)
{
	worker_log_output(error->message);
	g_main_loop_unref(self->loop);
}



void
report_session_core_error(SessionCore* self,
						  gchar* code)
{
	worker_log_output(code);
}








Pipeline*
session_core_get_pipeline(SessionCore* self)
{
	return self->pipe;
}

WebRTCHub*
session_core_get_rtc_hub(SessionCore* self)
{
	return self->hub;
}


QoE*
session_core_get_qoe(SessionCore* self)
{
	return self->qoe;
}


CoreState
session_core_get_state(SessionCore* self)
{
	return self->state;
}

void
session_core_set_state(SessionCore* core, CoreState state)
{
	core->state = state;
}

SignallingHub*
session_core_get_signalling_hub(SessionCore* core)
{
	return core->signalling;
}

IPC*
session_core_get_ipc(SessionCore* core)
{
	return core->ipc;
}

GMainContext*
session_core_get_main_context(SessionCore* core)
{
	return g_main_loop_get_context(core->loop);
}