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


#define			AGENT_PORT 					agent_port
#define			SESSION_CORE_PORT 			session_core_port
#define			CLUSTER_IP 					cluster_ip 
#define			TOKEN 						token
#define			DEVICE_TOKEN                device_token 
#define			TURN						turn
#define         USER                        user 
#define         PASSWORD                    password 


void            default_var                 ();


#endif