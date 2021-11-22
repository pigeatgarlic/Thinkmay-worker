/// <summary>
/// @file session-core-type.h
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-09-05
/// 
/// 
/// @copyright Copyright (c) 2021
#pragma once
#ifndef  __SESSION_CORE_TYPE_H__
#define __SESSION_CORE_TYPE_H__

#include <json-glib-1.0/json-glib/json-glib.h>
#include <glib-2.0/glib.h>




/// <summary>
/// Pipeline is a struct contain all GstElement neccessary for
/// session core to encode video and audio
/// </summary> 
typedef struct 			_Pipeline 				                Pipeline;

/// <summary>
/// Session core is a struct represent for session core module
/// </summary> 
typedef struct 			_SessionCore 			                SessionCore;

/// <summary>
/// qoe struct responsible for store recorded qoe metrict and other variable related to 
/// quality of experience of session core 
/// </summary> 
typedef struct 			_QoE					                QoE;

/// <summary>
/// webrtchub struct responsible for handle datachannel message from client
/// </summary>
typedef struct			_WebRTCHub				                WebRTCHub;

/// <summary>
/// signalling hub responsible for handle ice candidate and sdp negotiation with client module 
/// through signalling server.
/// </summary>
typedef struct 			_SignallingHub			                SignallingHub;


/// <summary>
/// session core state in form of string
/// </summary>
typedef					gchar*					                CoreState;

/// <summary>
/// PipelineState in form of string
/// </summary>
typedef					gchar*					                PipelineState;

/// <summary>
/// signalling state in form of string
/// </summary>
typedef					gchar*					                SignallingServerState;

/// <summary>
/// peer call state in form of string
/// </summary>
typedef					gchar*					                PeerCallState;


#endif // ! __SESSION_CORE_TYPE_H__


