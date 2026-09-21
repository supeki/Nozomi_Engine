// Nozomi Engine
// Commodore Amiga backend
// amiga_main.c

#include "amiga_main.h"

#include "../../i_system.h" 
#include "../../i_video.h" 

#include "../../game_defs.h" 
#include "../../game_main.h" 
#include "../../game_video.h" 

int main(int argc, char **argv) 
{ 
    I_StartupTimer();
    gameMain(); 

    I_printf("Starting main game loop...\n"); 
    gameLoop(); 
    
    I_printf("Quitting %s...\n", GAME_NAME); 
    I_ShutdownTimer();
    return 0; 
}