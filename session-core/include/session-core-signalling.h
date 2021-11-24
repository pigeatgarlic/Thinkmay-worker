                /// <summary>
/// @file session-core-signalling.h
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-09-05
/// 
/// @copyright Copyright (c) 2021
/// 
#ifndef __SESSION_CORE_SIGNALLING_H__
#define __SESSION_CORE_SIGNALLING_H__

#include "session-core-type.h"
#include "session-core.h"
#include "session-core-type.h"


#include <message-form.h>

#include <libsoup/soup.h>
#include <glib.h>
#include <gst/gst.h>

/*Used as webrtcbin callback function*/

/// <summary>
/// send ice candidate to peer through signalling server
/// typically emitted when pipeline is set to PLAYING 
/// </summary>
/// <param name="webrtc"></param>
/// <param name="mlineindex"></param>
/// <param name="candidate"></param>
/// <param name="core"></param>
void                            send_ice_candidate_message                              (GstElement* webrtc,
                                                                                        guint mlineindex,
                                                                                        gchar* candidate,
                                                                                        SessionCore* core);

/// <summary>
/// handle on negotiation signal from webrtcbin element, 
/// typically emitted when pipeline is set to PLAYING 
/// <param name="element"></param>
/// <param name="core"></param>
void                            on_negotiation_needed                                   (GstElement* element,
                                                                                        SessionCore* core);


/// <summary>
/// handle on ice gathering state from webrtcbin element,
/// typically emitted when pipeline is set to PLAYING 
/// </summary>
/// <param name="webrtcbin"></param>
/// <param name="pspec"></param>
/// <param name="user_data"></param>
void                            on_ice_gathering_state_notify                           (GstElement* webrtcbin,
                                                                                        GParamSpec* pspec,
                                                                                        gpointer user_data);

/// <summary>
/// initialize signalling hub 
/// </summary>
/// <param name="core"></param>
/// <returns></returns>
SignallingHub*                  signalling_hub_initialize                               (SessionCore* core);


/// <summary>
/// connect to signalling server, 
/// this function should be triggered in initialize process
/// </summary> 
/// <param name="core">session core object</param>
void                            connect_to_websocket_signalling_server_async            (SessionCore* core);

/// <summary>
/// 
/// </summary>
/// <param name="hub"></param>
/// <param name="turn"></param>
/// <param name="url"></param>
/// <param name="session_slave_id"></param>
void                            signalling_hub_setup                                    (SignallingHub* hub,
                                                                                        gchar* turn, 
                                                                                        gchar* url);

/// <summary>
/// close websocket connection with signalling server 
/// (it means session disconnnected event will be triggered)
/// </summary>
/// <param name="hub"></param>
/// <returns></returns>
gboolean                        signalling_close                                        (SignallingHub* hub);

/// <summary>
/// get stun server from signallinghub, 
/// stun server will follow gstreamer format 
/// (Ex: stun://stun.l.google.com:3475)
/// </summary>
/// <param name="hub"></param>
/// <returns></returns>
gchar*                          signalling_hub_get_turn_server                          (SignallingHub* hub);


#endif // !__SESSION_CORE_SIGNALLING_H