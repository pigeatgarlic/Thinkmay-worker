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

#define USE_DEFAULT_DEVICE_TOKEN    FALSE
#define USE_DEFAULT_TOKEN           FALSE
#define USE_DEFAULT_CLUSTER_IP      FALSE

#define DEFAULT_DEVICE_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpcCI6IjE5Mi4xNjguMS44IiwiQ1BVIjoiQU1EIFJ5emVuIDUgMzUwMCA2LUNvcmUgUHJvY2Vzc29yICAgICAgICAgICAgICAiLCJHUFUiOiJOVklESUEgR2VGb3JjZSBHVFggMTA2MCA2R0IiLCJPUyI6IldpbmRvdzEwIFZlcnNpb24gNi4yIiwiUkFNY2FwYWNpdHkiOiI4MTIzIiwibmJmIjoxNjM4ODQzNDQ4LCJleHAiOjE2Mzg4Nzk0NDgsImlhdCI6MTYzODg0MzQ0OH0.43EE-gEyNb1q_H2vNq3TUc6A0vGpKIYxdYEYS6khDP0";
#define DEFAULT_SESSION_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjEwMCIsIkNsaWVudElEIjoiMiIsIldvcmtlcklEIjoiOSIsIk1vZHVsZSI6IjAiLCJuYmYiOjE2Mzg4MzY4NzAsImV4cCI6MTYzODg3Mjg3MCwiaWF0IjoxNjM4ODM2ODcwfQ.tyvIWotHAFBha75xv9xA_9Hnjf9VydAooqBEpjub94w"
#define SESSION_INFOR_VALIDATE_URL "https://host.thinkmay.net/Session/Setting"

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