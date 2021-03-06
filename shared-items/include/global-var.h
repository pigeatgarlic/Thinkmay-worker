/**
 * @file global-var.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__
#include <glib-2.0/glib.h>


const gchar*                      agent_port ;
const gchar*                      session_core_port ;
const gchar*                      cluster_ip ; 
const gchar*                      token ;
const gchar*                      turn ; 
const gchar*                      user ; 
const gchar*                      password ; 
const gchar*                      device_token ; 


/**
 * @brief 
 * agent to expose to session core and cluster manager
 */
#define			AGENT_PORT 					agent_port

/**
 * @brief 
 * session core port to expose with agent and cluster manager
 */
#define			SESSION_CORE_PORT 			session_core_port

/**
 * @brief 
 * clueter manager ip address
 */
#define			CLUSTER_IP 					cluster_ip 

/**
 * @brief 
 * user token to communication with other module
 */
#define			TOKEN 						token


/**
 * @brief 
 * worker token to communication with other module
 */
#define			DEVICE_TOKEN                device_token 

/**
 * @brief 
 * default turn connection 
 */
#define			TURN						turn

/**
 * @brief 
 * cluster owner username 
 */
#define         USER                        user 

/**
 * @brief 
 * cluster owner password
 */
#define         PASSWORD                    password 


/**
 * @brief 
 * setup defaul value
 */
void            default_var                 ();


#endif