/*
 * teclado.c
 *
 *  Created on: 13 jun. 2021
 *      Author: Braulio
 */
#include <stdio.h>
#include <stdint.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
#include "teclado.h"

//declaración de variable para el switch
volatile uint8_t L1;
volatile uint8_t L2;
volatile uint8_t L3;
volatile uint8_t L4;
volatile uint8_t C1;
volatile uint8_t C2;
volatile uint8_t C3;

//variable de entrada
volatile char data_input;

//Estructura de configuración del switch
gpio_pin_config_t sw_config = {              //Estructura de configuración para indicar si es I/O el Switch
		kGPIO_DigitalInput,
		1,
};
gpio_pin_config_t portb_config = {             //Estructura de configuración para indicar si es I/O del PortB
		kGPIO_DigitalOutput,
		0,
};
gpio_pin_config_t sw_config2 = {              //Estructura de configuración para indicar si es I/O el Switch
		kGPIO_DigitalInput,
		0,
};

//ISR para los puertos del keypad
void PORTB_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOB, 1u << PIN2);
    GPIO_PortClearInterruptFlags(GPIOB, 1u << PIN3);
    GPIO_PortClearInterruptFlags(GPIOB, 1u << PIN10);
    GPIO_PortClearInterruptFlags(GPIOB, 1u << PIN11);
    L1 = true;
    L2 = true;
    L3 = true;
    L4 = true;

	SDK_ISR_EXIT_BARRIER;
}
void PORTD_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOD, 1u << PIN0);
    GPIO_PortClearInterruptFlags(GPIOD, 1u << PIN2);
    GPIO_PortClearInterruptFlags(GPIOD, 1u << PIN3);
    C1 = true;
    C2 = true;
    C3 = true;

	SDK_ISR_EXIT_BARRIER;
}

//Estructura Port_pin_config
    const port_pin_config_t portb_pin_config = {
		kPORT_PullDown,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTA04 */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};

    const port_pin_config_t portd_pin_config = {
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTA04 */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};

    void init_clocks(void)
    {
        //Habilitar relojes de los puertos
        CLOCK_EnableClock(kCLOCK_PortB);
        CLOCK_EnableClock(kCLOCK_PortD);
    }

    void init_config(void)
    {
        //Port_SetPinConfig
        PORT_SetPinConfig(PORTB, PIN2, &portb_pin_config);   //Configura el puerto B (Renglones)
        PORT_SetPinConfig(PORTB, PIN3, &portb_pin_config);
        PORT_SetPinConfig(PORTB, PIN10, &portb_pin_config);
        PORT_SetPinConfig(PORTB, PIN11, &portb_pin_config);
        PORT_SetPinConfig(PORTD, PIN0, &portd_pin_config);   //Configura el puerto D(Columnas) 
        PORT_SetPinConfig(PORTD, PIN2, &portd_pin_config);
        PORT_SetPinConfig(PORTD, PIN3, &portd_pin_config);
        //Interrupts config
        PORT_SetPinInterruptConfig(PORTB, PIN2, kPORT_InterruptRisingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN3, kPORT_InterruptRisingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN10, kPORT_InterruptRisingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN11, kPORT_InterruptRisingEdge);
	    PORT_SetPinInterruptConfig(PORTD, PIN0, kPORT_InterruptFallingEdge);
        PORT_SetPinInterruptConfig(PORTD, PIN2, kPORT_InterruptFallingEdge);
        PORT_SetPinInterruptConfig(PORTD, PIN3, kPORT_InterruptFallingEdge);
        
        NVIC_EnableIRQ(PORTB_IRQn);
	    /**It Sets the priority of the IRQ*/
	    NVIC_SetPriority(PORTB_IRQn, 2);

	    NVIC_EnableIRQ(PORTD_IRQn);
	    /**It Sets the priority of the IRQ*/
	    NVIC_SetPriority(PORTD_IRQn, 2);

        //GPIO_PinInit para el switcheo
        GPIO_PinInit(GPIOB, PIN2, &portb_config);           //Renglones
        GPIO_PinInit(GPIOB, PIN3, &portb_config);
        GPIO_PinInit(GPIOB, PIN10, &portb_config);
        GPIO_PinInit(GPIOB, PIN11, &portb_config);
        GPIO_PinInit(GPIOD, PIN0, &sw_config);              //Columnas - Entradas
        GPIO_PinInit(GPIOD, PIN2, &sw_config);
        GPIO_PinInit(GPIOD, PIN3, &sw_config);

        L1 = GPIO_PinRead(GPIOB, PIN2);
        L2 = GPIO_PinRead(GPIOB, PIN3);
        L3 = GPIO_PinRead(GPIOB, PIN10);
        L4 = GPIO_PinRead(GPIOB, PIN11);
        C1 = GPIO_PinRead(GPIOD, PIN0);
        C2 = GPIO_PinRead(GPIOD, PIN2);
        C3 = GPIO_PinRead(GPIOD, PIN3);
    }

    void finder_config(void)
    {
        PORT_SetPinConfig(PORTB, PIN2, &portd_pin_config);
        PORT_SetPinConfig(PORTB, PIN3, &portd_pin_config);
        PORT_SetPinConfig(PORTB, PIN10, &portd_pin_config);
        PORT_SetPinConfig(PORTB, PIN11, &portd_pin_config);
        PORT_SetPinConfig(PORTD, PIN0, &portb_pin_config);   //Configura el puerto D(Columnas) 
        PORT_SetPinConfig(PORTD, PIN2, &portb_pin_config);
        PORT_SetPinConfig(PORTD, PIN3, &portb_pin_config);

        PORT_SetPinInterruptConfig(PORTB, PIN2, kPORT_InterruptFallingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN3, kPORT_InterruptFallingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN10, kPORT_InterruptFallingEdge);
        PORT_SetPinInterruptConfig(PORTB, PIN11, kPORT_InterruptFallingEdge);
	    PORT_SetPinInterruptConfig(PORTD, PIN0, kPORT_InterruptRisingEdge);
        PORT_SetPinInterruptConfig(PORTD, PIN2, kPORT_InterruptRisingEdge);
        PORT_SetPinInterruptConfig(PORTD, PIN3, kPORT_InterruptRisingEdge);

        NVIC_EnableIRQ(PORTB_IRQn);
	    /**It Sets the priority of the IRQ*/
	    NVIC_SetPriority(PORTB_IRQn, 2);

	    NVIC_EnableIRQ(PORTD_IRQn);
	    /**It Sets the priority of the IRQ*/

	    NVIC_SetPriority(PORTD_IRQn, 2);
        GPIO_PinInit(GPIOB, PIN2, &sw_config);
        GPIO_PinInit(GPIOB, PIN3, &sw_config);
        GPIO_PinInit(GPIOB, PIN10, &sw_config);
        GPIO_PinInit(GPIOB, PIN11, &sw_config);
        GPIO_PinInit(GPIOD, PIN0, &portb_config);              //Columnas
        GPIO_PinInit(GPIOD, PIN2, &portb_config);
        GPIO_PinInit(GPIOD, PIN3, &portb_config);
        
        L1 = GPIO_PinRead(GPIOB, PIN2);
        L2 = GPIO_PinRead(GPIOB, PIN3);
        L3 = GPIO_PinRead(GPIOB, PIN10);
        L4 = GPIO_PinRead(GPIOB, PIN11);
        C1 = GPIO_PinRead(GPIOD, PIN0);
        C2 = GPIO_PinRead(GPIOD, PIN2);
        C3 = GPIO_PinRead(GPIOD, PIN3);
    }

    void column_finder(void)
    {
        if(!C1){
            l_finder1();
        }
        if(!C2)
        {
            l_finder2(); 
        }
        if(!C3)
        {
            l_finder3();
            
        }
    }

    void l_finder1()
    {
        finder_config();
        if(!L1)
        {
            data_input = '1';
            printf("%c\n", data_input);
        }

        if(!L2)
        {
            data_input = '4';
            printf("%c\n", data_input);
        }

        if(!L3)
        {
            data_input = '7';
            printf("%c\n", data_input);
        }
        if(!L4)
        {
            data_input = '.';
            printf("%c\n", data_input);
        }
    }
    void l_finder2()
    {
        finder_config();
        if(!L1)
        {
            data_input = '2';
            printf("%c\n", data_input);
        }

        if(!L2)
        {
            data_input = '5';
            printf("%c\n", data_input);
        }

        if(!L3)
        {
            data_input = '8';
            printf("%c\n", data_input);
        }
        if(!L4)
        {
            data_input = '0';
            printf("%c\n", data_input);
        }
    }
    void l_finder3()
    {
        finder_config();
        if(!L1)
        {
            data_input = '3';
            printf("%c\n", data_input);
        }

        if(!L2)
        {
            data_input = '6';
            printf("%c\n", data_input);
        }

        if(!L3)
        {
            data_input = '9';
            printf("%c\n", data_input);
        }
        if(!L4)
        {
            data_input = '#';
            printf("%c\n", data_input);
        }
    }