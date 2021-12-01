/**
 * @file agent-type.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#ifndef __AGENT_TYPE_H__
#define __AGENT_TYPE_H__

#include <glib.h>
#include <json-glib/json-glib.h>
#include <Windows.h>




/// <summary>
/// Socket is a object use to manage connection with host, 
/// included send and receive,
/// </summary>
typedef struct _Socket 				Socket;


/// <summary>
/// device information use to register 
/// information with host, include GPU, CPU, RAM
/// </summary>
typedef struct _DeviceInformation 	DeviceInformation;

/// <summary>
/// agent object is an object use to abstract agent module
/// </summary>
typedef struct _AgentServer			AgentServer;

/// <summary>
/// childprocess object abstract the existen of child process
/// </summary>
typedef	struct _ChildProcess		ChildProcess;


/// <summary>
/// 
/// </summary>
typedef struct _ShellSession        ShellSession;

/// <summary>
/// 
/// </summary>
typedef struct _RemoteSession       RemoteSession;

/// <summary>
/// 
/// </summary>
typedef struct _FileTransferSession FileTransferSession;

/// <summary>
/// 
/// </summary>
typedef struct _FileCompressor      FileCompressor;

/// <summary>
/// 
/// </summary>
typedef struct _FileTransceiver     FileTransceiver;

/// <summary>
/// for better maintainance, 
/// message will be use to replace JsonObject
/// </summary>
typedef		   JsonObject			Message;

/// <summary>
/// child standard input output handle, 
/// this function is settled inside a looped thread 
/// and sould be invoked when child process emit output to stdout  
/// </summary>
typedef void  (*ChildStdOutHandle)    (GBytes* buffer,
                                     AgentServer* agent,
                                     gpointer data);

/// <summary>
/// child standard input output handle, 
/// this function is settled inside a looped thread 
/// and sould be invoked when child process emit output to stdout  
/// </summary>
typedef void  (*ChildStdErrHandle)    (GBytes* buffer,
                                     AgentServer* agent,
                                     gpointer data);
/// <summary>
/// child state handle function is setteled inside a looped thread 
/// and responsible for manage state of child process thread
/// (Ex: report session disconnected to host if session core process ended)
/// </summary>
typedef void  (*ChildStateHandle)  (ChildProcess* proc,
                                    AgentServer* agent,
                                    gpointer data);

#endif