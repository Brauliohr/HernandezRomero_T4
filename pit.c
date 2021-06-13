/*
 * pit.c
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */
#include "pit.h"
#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pit.h"
#include "board.h"
#include "clock_config.h"


volatile bool pitIsrFlag = false;

void PIT0_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pitIsrFlag = true;

	__DSB();
}


uint8_t PITflag_get_pit(void)
{
	return (pitIsrFlag);
}

void PITflag_set_pit(void)
{
	pitIsrFlag = false;
}

void pit_config(void)
{
	/*Configuraciones del PIT*/
		/* Structure of initialize PIT */
		pit_config_t pitConfig;
		uint32_t freq = 0;

		/* Board pin, clock, debug console init */
		BOARD_InitPins();
		BOARD_InitDebugConsole();

		/*
		 * pitConfig.enableRunInDebug = false;
		 */
		PIT_GetDefaultConfig(&pitConfig);

		/* Init pit module */
		PIT_Init(PIT, &pitConfig);

		/* Set timer period for channel 0 */
		freq = CLOCK_GetFreq(kCLOCK_BusClk);

		PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, freq));

		/* Enable timer interrupts for channel 0 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

		/* Enable at the NVIC */
		EnableIRQ(PIT0_IRQn);

		/*Start channel 0*/
		PIT_StartTimer(PIT, kPIT_Chnl_0);

}


