/// <summary>
/// @file agent-file-transfer-service.c
/// @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
/// </summary>
/// @version 1.0
/// @date 2021-10-01
/// 
/// @copyright Copyright (c) 2021
/// 
#include <agent-file-transfer-service.h>
#include <agent-file-transceiver.h>
#include <agent-file-compressor.h>
#include <agent-file-transceiver.h>
#include <agent-type.h>

#include <message-form.h>
#include <agent-server.h>

#include <child-process-constant.h>


#define SIGNALLING_URL  "SignallingUrl"
#define TURN_STRING     "TurnConnection"
#define SESSION_ID      "SessionSlaveID"
#define INPUT_FILE      "InputPath"

struct _FileTransferSession
{
    FileCompressor* compressor;

    FileTransceiver* transceiver;

    gchar* input_file;

    gchar* zip_file;

    gchar* signalling_url;

    gchar* turn_connection;

    gint SessionSlaveID;

    gboolean completed;

    AgentServer* agent;
};


void
start_file_transfer(FileTransferSession* session)
{
}

void
on_file_compress_completed(gint SessionSlaveID)
{
    // get session from pool with session slave id
}

static void 
file_transfer_session_finalize(FileTransferSession* session)
{
    session->compressor = NULL;
    session->transceiver = NULL;
    session->SessionSlaveID = 0;
    session->completed = TRUE;
}


void
on_file_transceive_completed(gint SessionSlaveID)
{
    // get session from pool with session slave id
}


FileTransferSession*
setup_file_transfer_session(gchar* server_command)
{
}



gchar* 
file_transfer_session_get_turnconnection(FileTransferSession* session)
{
    return session->turn_connection;
}

gchar* 
file_transfer_session_get_input_file(FileTransferSession* session)
{
    return session->input_file;
}

gchar* 
file_transfer_session_get_zip_file(FileTransferSession* session)
{
    return session->zip_file;
}

void
file_transfer_session_set_zip_file(FileTransferSession* session, 
                                   gchar* path)
{
    session->zip_file = path;
}


gchar*
file_transfer_session_get_signalling_url(FileTransferSession* session)
{
    return session->signalling_url;
}

gchar* 
file_transfer_session_get_session_id(FileTransferSession* session)
{
    return session->SessionSlaveID;
}