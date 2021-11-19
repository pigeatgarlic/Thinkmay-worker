#ifndef __LOGGING_H__
#define __LOGGING_H__
#include <glib.h>



/// <summary>
/// write to log file,
/// content of log file may contain: error report, message receive, state change, 
/// </summary>
/// <param name="file_name"></param>
/// <param name="text"></param>
void                worker_log_output           (gchar* text);


#endif