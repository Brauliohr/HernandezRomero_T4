/*
 * switch.c
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */
#include "switch.h"
#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pit.h"
#include "board.h"
#include "clock_config.h"

uint8_t switch2;
uint8_t switch3;

gpio_pin_config_t sw_conf = {
	kGPIO_DigitalInput,
	0,
};

void PORTC_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOC, 1u << PIN6);

	switch2 = true;
	SDK_ISR_EXIT_BARRIER;
}

void PORTA_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOA, 1u << PIN4);

	switch3 = true;
	SDK_ISR_EXIT_BARRIER;
}

void switch_config(void)
{
	const port_pin_config_t porta_pin4_config = {
		  kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		  kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		  kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		  kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		  kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		  kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		  kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		};

		/*Habilitar los relojes para los puertos a utilizar*/
		CLOCK_EnableClock(kCLOCK_PortA);
		CLOCK_EnableClock(kCLOCK_PortC);

		PORT_SetPinConfig(PORTC, PIN6, &porta_pin4_config);
		PORT_SetPinConfig(PORTA, PIN4, &porta_pin4_config);

		PORT_SetPinInterruptConfig(PORTC, PIN6, kPORT_InterruptFallingEdge);
		PORT_SetPinInterruptConfig(PORTA, PIN4, kPORT_InterruptFallingEdge);

		GPIO_PinInit(GPIOC, PIN6, &sw_conf);
		GPIO_PinInit(GPIOA, PIN4, &sw_conf);

		NVIC_EnableIRQ(PORTC_IRQn);
		/**It Sets the priority of the IRQ*/
		NVIC_SetPriority(PORTC_IRQn, 2);

		NVIC_EnableIRQ(PORTA_IRQn);
		/**It Sets the priority of the IRQ*/
		NVIC_SetPriority(PORTA_IRQn, 2);
}

uint8_t sw2_get(void)
{
	return switch2;
}
void sw2_set(void)
{
	switch2 = false;
}
uint8_t sw3_get(void)
{
	return switch3;
}
void sw3_set(void)
{
	switch3 = false;
}
