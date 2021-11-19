#include <glib.h>
#include <agent-server.h>


#include <gst/gst.h>
#include <glib-2.0/glib.h>

#define GST_DEBUG               4


static gint agent_port = 2520;
static gchar session_core_port[10] = "2250";
static gchar worker_ip[20] = "192.168.1.12";
static gchar manager_url[20] = "192.168.1.12";
static gchar token[200] = {0}; 




static GOptionEntry entries[] = {
  {"token", 0, 0, G_OPTION_ARG_STRING, &token,
      "token register with worker manager", "TOKEN"},
  {"agentport", 0, 0, G_OPTION_ARG_INT, &agent_port,
      "String ID of the peer to connect to", "ID"},
  {"sessioncoreport", 0, 0, G_OPTION_ARG_INT, &session_core_port,
      "Signalling server to connect to", "URL"},
  {"managerurl", 0, 0, G_OPTION_ARG_STRING, &manager_url,
      "Signalling server to connect to", "URL"},
  {"workerip", 0, 0, G_OPTION_ARG_STRING, &worker_ip,
      "Request that the peer generate the offer and we'll answer", "URL"},
  {NULL},
};


int
main(int argc, char* argv[])
{
    GOptionContext *context;
    GError *error = NULL;

    context = g_option_context_new ("- gstreamer webrtc sendrecv demo");
    g_option_context_add_main_entries (context, entries, NULL);
    g_option_context_add_group (context, gst_init_get_option_group ());
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_printerr ("Error initializing: %s\n", error->message);
        return -1;
    }

    agent_new(agent_port, 
            session_core_port, 
            token,
            manager_url,
            worker_ip);
    return;
}
