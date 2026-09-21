// Nozomi Engine
// Commodore Amiga backend
// amiga_system.c

#include "amiga_main.h"
#include "../../game_defs.h"
#include "../../i_system.h"

#include <exec/types.h>
#include <exec/memory.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/dos.h>

struct MsgPort *TimerMP = NULL;
struct timerequest *TimerIO = NULL;
struct Device *TimerBase = NULL;

void I_StartupTimer(void)
{
    TimerMP = (struct MsgPort *)AllocMem(sizeof(struct MsgPort), MEMF_PUBLIC | MEMF_CLEAR);
    TimerMP->mp_Node.ln_Type = NT_MSGPORT;
    TimerMP->mp_Flags = PA_SIGNAL;
    TimerMP->mp_SigBit = AllocSignal(-1);
    TimerMP->mp_SigTask = FindTask(NULL);
    
    TimerMP->mp_MsgList.lh_Head = (struct Node *)&TimerMP->mp_MsgList.lh_Tail;
    TimerMP->mp_MsgList.lh_Tail = NULL;
    TimerMP->mp_MsgList.lh_TailPred = (struct Node *)&TimerMP->mp_MsgList.lh_Head;

    TimerIO = (struct timerequest *)AllocMem(sizeof(struct timerequest), MEMF_PUBLIC | MEMF_CLEAR);
    TimerIO->tr_node.io_Message.mn_ReplyPort = TimerMP;
    if (OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)TimerIO, 0) == 0)
        TimerBase = (struct Device *)TimerIO->tr_node.io_Device;
}

void I_ShutdownTimer(void)
{
    if (TimerIO) {
        if (TimerBase) {
            CloseDevice((struct IORequest *)TimerIO);
            TimerBase = NULL;
        }
        FreeMem(TimerIO, sizeof(struct timerequest));
        TimerIO = NULL;
    }

    if (TimerMP) {
        if (TimerMP->mp_SigBit != -1) {
            FreeSignal(TimerMP->mp_SigBit);
        }
        FreeMem(TimerMP, sizeof(struct MsgPort));
        TimerMP = NULL;
    }
}

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = 0;
    struct timeval tv;

    TimerIO->tr_node.io_Command = TR_GETSYSTIME;
    DoIO((struct IORequest *)TimerIO);
    tv = TimerIO->tr_time;

    ticks = (tv.tv_secs * 1000) + (tv.tv_micro / 1000);
	
	if (!base_tick)
		base_tick = ticks;
	
	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);
	
	return ticks;
}

uint32_t I_GetTime(void)
{
	return 0;
}

void I_Sleep(uint32_t ms)
{
	Delay((ms * 50) / 1000);
}

void I_printf (const char *text, ...)
{
    va_list argptr;
    char txt[2048];
    
    va_start(argptr,text);
    vsprintf(txt, text, argptr);
    va_end(argptr);    

	printf(txt);
}

void I_Error (const char *error, ...)
{
    va_list argptr;
    char txt[512];
    
    va_start(argptr,error);
    vsprintf(txt, error, argptr);
    va_end(argptr);    
	
    printf("Nozomi Engine Error:\n%s\n", txt);
    I_Sleep(5000);
	exit(-1);
}