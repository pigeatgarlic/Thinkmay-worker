/**
 * @file human-interface-opcode.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __HUMAN_INTERFACE_OPCODE_H__
#define __HUMAN_INTERFACE_OPCODE_H__


/**
 * @brief 
 * HID Opcode for communication with client 
 */
typedef enum
{
	KEYUP,
	KEYDOWN,

	MOUSE_WHEEL,
	MOUSE_MOVE,
	MOUSE_UP,
	MOUSE_DOWN,

	POINTER_LOCK,

	KEYRESET,
	QOE_REPORT


}HidOpcode;


#endif