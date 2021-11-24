/// <summary>
/// @file session-core-remote-config.h
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-09-05
/// 
/// @copyright Copyright (c) 2021
/// 
#include "session-core-type.h"
#include "session-core.h"
#include <gst/gst.h>
#include <qoe.h>





/// <summary>
/// initialize qoe metric measurement
/// </summary> 
/// <returns></returns>
QoE*			qoe_initialize						();

/// <summary>
/// setup qoe metric measurement
/// </summary> 
/// <param name="qoe"></param>
/// <param name="screen_width"></param>
/// <param name="screen_height"></param>
/// <param name="audio_codec"></param>
/// <param name="video_codec"></param>
/// <param name="qoe_mode"></param>
void			qoe_setup							(QoE* qoe,
													 Message* request_result );


/// <summary>
/// get audio codec from current remote session
/// </summary>
/// <param name="qoe"></param>
/// <returns></returns>
Codec			qoe_get_audio_codec					(QoE* qoe);

/// <summary>
/// get video codec from current remote session
/// </summary>
/// <param name="qoe"></param>
/// <returns></returns>
Codec			qoe_get_video_codec					(QoE* qoe);




/// <summary>
/// 
/// </summary>
/// <param name="qoe"></param>
/// <returns></returns>
gint			qoe_get_screen_height				(QoE* qoe);



/// <summary>
/// 
/// </summary>
/// <param name="qoe"></param>
/// <returns></returns>
gint			qoe_get_screen_width				(QoE* qoe);



