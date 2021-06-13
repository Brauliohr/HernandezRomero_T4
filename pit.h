/*
 * pit.h
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */

#ifndef PIT_H_
#define PIT_H_

#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"


void PIT0_IRQHandler(void);


uint8_t PITflag_get_pit(void);
void PITflag_set_pit(void);

void pit_config(void);




#endif /* PIT_H_ */
