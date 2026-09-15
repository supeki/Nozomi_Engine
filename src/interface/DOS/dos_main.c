// Nozomi Engine
// DOS/DJGPP backend
// dos_main.c

#include "dos_main.h"

#include "../../i_system.h" 
#include "../../i_video.h" 

#include "../../game_defs.h" 
#include "../../game_main.h" 
#include "../../game_video.h" 

void I_Ticker(void)
{
	timer_ticks++;
}
END_OF_FUNCTION(I_Ticker);

int main(int argc, char **argv) 
{ 
    allegro_init();

    install_timer();
    install_keyboard();
	install_mouse();

    LOCK_VARIABLE(timer_ticks);
    LOCK_FUNCTION(I_Ticker);
    install_int_ex(I_Ticker, BPS_TO_TIMER(1000));

    gameMain(); 

    I_printf("Starting main game loop...\n"); 
    gameLoop(); 
    
    I_printf("Quitting %s...\n", GAME_NAME); 
    return 0; 
}