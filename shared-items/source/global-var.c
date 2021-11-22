#include <global-var.h>



void
default_var()
{
    AGENT_PORT = malloc(10);
    SESSION_CORE_PORT = malloc(10);
    TOKEN = malloc(500);
    CLUSTER_IP = malloc(200);
    USER = malloc(100);
    PASSWORD = malloc(100);

    memset(AGENT_PORT,0,10);
    memset(SESSION_CORE_PORT,0,10);
    memset(TOKEN,0,500);
    memset(CLUSTER_IP,0,200);
    memset(USER,0,100);
    memset(PASSWORD,0,100);

    SESSION_CORE_PORT = "3330";
    AGENT_PORT = "2220";
}
