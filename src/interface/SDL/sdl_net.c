// Nozomi Engine
// SDL2 backend
// sdl_net.c

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../../game_defs.h"
#include "../../i_net.h"
#include "../../i_system.h"

bool network_supported = true;

IPaddress server_addr;
TCPsocket client_sock = NULL;

void I_StartupNetwork(void)
{
    int res;
    res = SDLNet_Init();

    if (res < 0) {
        I_printf("SDLNet_Init fail: %s\nDisabling Network features.\n", SDLNet_GetError());
        network_supported = false;
    }
}

void I_ShutdownNetwork(void)
{
    I_NetDisconnect();
    SDLNet_Quit();
}

int I_NetConnect(const char *addr, int port)
{
    int res;

    if (!network_supported) {
        I_printf("Networking is not supported on this platform.\n");
        return -1;
    }

    if (client_sock != NULL) {
        I_printf("Already connected to a server.\n");
        return -1;
    }

    res = SDLNet_ResolveHost(&server_addr, addr, port);
    if (res < 0) {
        I_printf("SDLNet_ResolveHost fail: %s\n", SDLNet_GetError());
        return -1;
    }

    client_sock = SDLNet_TCP_Open(&server_addr);
    if (!client_sock) {
        I_printf("SDLNet_TCP_Open fail: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}

void I_NetDisconnect(void)
{
    if (client_sock != NULL) {
        SDLNet_TCP_Close(client_sock);
        client_sock = NULL;
    }
}

int I_NetSend(const void *buf, int len)
{
    if (client_sock == NULL)
        return -1;

    return SDLNet_TCP_Send(client_sock, buf, len);
}

int I_NetReceive(void *buf, int len)
{
    if (client_sock == NULL)
        return -1;

    return SDLNet_TCP_Recv(client_sock, buf, len);
}
