// Nozomi Engine
// i_net.h

#ifndef I_NET_H
#define I_NET_H

#include "game_defs.h"

#pragma pack(push, 1)
typedef struct 
{
    uint32_t flags; // obj flags
	uint16_t type; // obj type
    uint16_t x; // obj x
	uint16_t y; // obj y
    uint16_t anim_timer; // current anim tick
    uint8_t health; // obj hp
    uint8_t dir_layer; // LLLLLLDD L - layer D - dir
    uint8_t anim_state; // obj anim state
    int8_t momx; // obj momx
	int8_t momy; // obj momy
} object_packet_t;

typedef struct {
    char world_name[32]; // world name for packet, don't update if it doesn't match
    uint16_t packet_num; // can loop back with no worries
    // packet buffering
    uint8_t packet_index; // current index for this "packet" (frame)
    uint8_t num_indexes; // number of indexes for this "packet" (frame)
    // objects
    uint8_t object_count;
    object_packet_t objects[16]; // update up to 16 objects at a time
} world_packet_t;
#pragma pack(pop)

void I_StartupNetwork(void);
void I_ShutdownNetwork(void);
int I_NetConnect(const char *addr, int port);
void I_NetDisconnect(void);
int I_NetSend(const void *buf, int len);
int I_NetReceive(void *buf, int len);

extern bool network_supported;

#endif
