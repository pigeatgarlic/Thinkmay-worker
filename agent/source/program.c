#include <glib.h>
#include <agent-server.h>
#include <general-constant.h>



#define GST_DEBUG               4


static gint agent_port = 2520;
static gint session_core_port = 2250;
static gint worker_manager_port = 5220;
static gchar worker_ip[20] = "192.168.1.12";
 




static GOptionEntry entries[] = {
  {"agentport", 0, 0, G_OPTION_ARG_INT, &agent_port,
      "String ID of the peer to connect to", "ID"},
  {"sessioncoreport", 0, 0, G_OPTION_ARG_INT, &session_core_port,
      "Signalling server to connect to", "URL"},
  {"managerport", 0, 0, G_OPTION_ARG_INT, &worker_manager_port,
      "Signalling server to connect to", "URL"},
  {"workerip", 0, 0, G_OPTION_ARG_STRING, &worker_ip,
      "Request that the peer generate the offer and we'll answer", "URL"},
  {NULL},
};


int
main(int argc, char* argv[])
{
    thinkmay_init(argv[0],14);


    agent_new();
}
