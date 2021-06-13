/*
 * switch.h
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pit.h"
#include "board.h"
#include "clock_config.h"

#define PIN6  6u
#define PIN4  4u

void PORTA_IRQHandler(void);
void PORTC_IRQHandler(void);
void switch_config(void);

uint8_t sw2_get(void);
void sw2_set(void);
uint8_t sw3_get(void);
void sw3_set(void);

#endif /* SWITCH_H_ */
