/// <summary>
/// @file session-core-remote-config.c
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-09-06
/// 
/// @copyright Copyright (c) 2021
/// 
#include <session-core-remote-config.h>
#include <session-core-type.h>
#include <session-core-pipeline.h>

#include <opcode.h>
#include <qoe.h>

#include <gst/gst.h>
#include <glib-2.0/glib.h>


#include <stdio.h>
#include <Windows.h>




struct _QoE
{
	/*non volatile value, 
	*determine in session initialize time*/
	gint screen_height;
	gint screen_width;

	/*quality control mode*/
	QoEMode mode;

	/*codec audio*/
	Codec codec_audio;
	Codec codec_video;

};


QoE*
qoe_initialize()
{
	static QoE qoe;
	memset(&qoe, 0, sizeof(QoE));
	return &qoe;
}


void
display_setting_get_and_set(gint* screen_width,
							gint* screen_height)
{
	// resize window to fit user's window
	DEVMODE devmode;
    devmode.dmPelsWidth = *screen_width;
    devmode.dmPelsHeight = *screen_height;
    devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    devmode.dmSize = sizeof(DEVMODE);

    long result = ChangeDisplaySettings(&devmode, 0);

	gint x = GetSystemMetrics(SM_CXSCREEN);
	gint y = GetSystemMetrics(SM_CYSCREEN);


	memcpy(screen_height,&y,sizeof(gint));
	memcpy(screen_width,&x,sizeof(gint));
}


void
qoe_setup(QoE* qoe,
		  Message* request_result)
{
	
	qoe->screen_width =  json_object_get_int_member(request_result,"ScreenWidth"),
	qoe->screen_height =  json_object_get_int_member(request_result,"ScreenHeight"),

	display_setting_get_and_set(&qoe->screen_width,&qoe->screen_height);

	qoe->codec_audio = json_object_get_int_member(request_result,"AudioCodec");
	qoe->codec_video = json_object_get_int_member(request_result,"VideoCodec");

	qoe->mode = json_object_get_int_member(request_result,"QoEMode");
}








Codec
qoe_get_audio_codec(QoE* qoe)
{
	return qoe->codec_audio;
}

Codec
qoe_get_video_codec(QoE* qoe)
{
	return qoe->codec_video;
}

gint 
qoe_get_screen_width(QoE* qoe)
{
	return qoe->screen_width;
}

gint 
qoe_get_screen_height(QoE* qoe)
{
	return qoe->screen_height;
}