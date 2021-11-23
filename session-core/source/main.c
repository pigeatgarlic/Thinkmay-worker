#include <session-core.h>
#include <session-core-type.h>

#include <gst/gst.h>
#include <glib-2.0/glib.h>
#include <global-var.h>

#define GST_USE_UNSTABLE_API

#define GST_DEBUG       4




static GOptionEntry entries[] = {
  {"agentport", 0, 0, G_OPTION_ARG_INT, &AGENT_PORT,
      "String ID of the peer to connect to", "ID"},
  {"sessioncoreport", 0, 0, G_OPTION_ARG_INT, &SESSION_CORE_PORT,
      "Signalling server to connect to", "URL"},
  {"clusterip", 0, 0, G_OPTION_ARG_INT, &CLUSTER_IP,
      "Signalling server to connect to", "URL"},
  {"token", 0, 0, G_OPTION_ARG_STRING, &TOKEN,
      "Signalling server to connect to", "URL"},
  {NULL},
};

int
main(int argc, char* argv[])
{
    default_var();

    GOptionContext *context;
    GError *error = NULL;

    context = g_option_context_new ("- thinkmay gstreamer client");
    g_option_context_add_main_entries (context, entries, NULL);
    g_option_context_add_group (context, gst_init_get_option_group ());
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_printerr ("Error initializing: %s\n", error->message);
        return -1;
    }

    session_core_initialize();

    return;
}
