/*
 * teclado.h
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */

#ifndef TECLADO_H_
#define TECLADO_H_

#include <stdio.h>
#include <stdint.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"

//Definición de macros a utilizar
#define PIN2   2u       
#define PIN0   0u 
#define PIN22  22u 
#define PIN2    2u
#define PIN3    3u
#define PIN10  10u
#define PIN11  11u

//Funciones para el barrido
void l_finder1();
void l_finder2();
void l_finder3();

void PORTB_IRQHandler(void);
void PORTD_IRQHandler(void);

void init_clocks(void);
void init_config(void);
void finder_config(void);
void column_finder(void);
void l_finder1(void);
void l_finder2(void);
void l_finder3(void);


#endif /* TECLADO_H_ */
