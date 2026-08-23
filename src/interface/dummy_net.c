// Nozomi Engine
// dummy network backend
// dummy_net.c

#include "../game_defs.h"
#include "../i_net.h"

bool network_supported = false;

void I_StartupNetwork(void)
{

}

void I_ShutdownNetwork(void)
{

}

int I_NetConnect(const char *addr, int port)
{
    return -1;
}

void I_NetDisconnect(void)
{

}

int I_NetSend(const void *buf, int len)
{
    return -1;
}

int I_NetReceive(void *buf, int len)
{
    return -1;
}

