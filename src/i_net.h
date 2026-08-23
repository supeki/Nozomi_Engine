// Nozomi Engine
// i_net.h

#ifndef I_NET_H
#define I_NET_H

#include "game_defs.h"

void I_StartupNetwork(void);
void I_ShutdownNetwork(void);
int I_NetConnect(const char *addr, int port);
void I_NetDisconnect(void);
int I_NetSend(const void *buf, int len);
int I_NetReceive(void *buf, int len);

extern bool network_supported;

#endif
