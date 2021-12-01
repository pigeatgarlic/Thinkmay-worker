/**
 * @file agent-socket.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __SOCKET_H__
#define __SOCKET_H__


#include <glib.h>
#include <libsoup/soup.h>
#include <json-glib-1.0/json-glib/json-glib.h>
#include <agent-server.h>

#include <Windows.h>










/**
 * @brief 
 * register device with host through cluster manager
 * @param self 
 * @return gboolean 
 */
gboolean                                register_with_host                  (AgentServer* self);


gboolean								send_message_to_host				(AgentServer* self,
                                                                             gchar* data);

/// <summary>
/// connect to host asynchronously,
/// this function create new soup message and session,
/// wrap soup_websocket_connect_async function
/// </summary>
/// <param name="self"></param>
void									connect_to_host_async				(AgentServer* self);


/**
 * @brief 
 * initialize socket
 * @return Socket* pointer to socket instance
 */
Socket*                                 initialize_socket                   ();

/**
 * @brief 
 * close socket
 * @param socket 
 */
void                                    socket_close                        (Socket* socket);

/*END get-set function for Socket*/

#endif