// Proyecto 1
// Autor: Braulio Hernández Romero
// Diseño con microcontroladores
// Diplomado "Diseño de circuitos" Cinvestav-IECA
// Fecha de entrega: 13 de junio de 2021

#include <stdio.h>
#include <stdint.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
#include "fsl_dac.h"
#include "teclado.h"
#include "pit.h"
#include "switch.h"

#define ONE   (0x01u)
#define TWO   (0x02u)

typedef enum{
	Inicial,
	Freq_change,
	V_change,
}State_name_t;


int main(void)
{
    //Configuración de los switch
	switch_config();
	//Pit config
	pit_config();

	//DAC init
	uint8_t index;
	dac_config_t dacConfigStruct;
	uint32_t dacValue;

	BOARD_InitPins();
	BOARD_InitDebugConsole();

	/* Configure the DAC. */
	/*
	* dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
	* dacConfigStruct.enableLowPowerMode = false;
	*/
	DAC_GetDefaultConfig(&dacConfigStruct);
	DAC_Init(DAC0, &dacConfigStruct);
	DAC_Enable(DAC0, true);             /* Enable output. */
	DAC_SetBufferReadPointer(DAC0, 0U); /* Make sure the read pointer to the start. */
	                                                     /*
	                                                      * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
	                                                      * and itemss can be written manually by user.
	                                                      */


	uint8_t input_port_a, input_port_c, total_input;
	State_name_t current_state = Inicial;

	while(1)
    {
        input_port_a = sw3_get();
		input_port_c = sw2_get();

		input_port_a = input_port_a << 1;

		total_input = input_port_a | input_port_c;

		sw2_set();
		sw3_set();

        switch (current_state) {

		      case Inicial:

		    	//freq y volt inicial

		    	if(ONE == total_input)
		    	{
		    	  current_state = Freq_change ;
		    	}
		    	else
		    	{
		    	  current_state = Inicial;
		    	}
		    	break;

		      case Freq_change:

		      	//cambio de freq
                  column_finder();


		  		if(TWO == total_input)
		  		{
		  			current_state = V_change ;
		  		}
		  		else
		  		{
		  			current_state = Freq_change;
		  		}
		  		break;

		  	 case V_change:
		  		
                //Volt change
                column_finder();

		  		if(ONE == total_input)
		  		{
		  			current_state = Freq_change ;
		  		}
		  		else
		  		{
		  			current_state = V_change;
		  		}
		  		break;

		  	default:
		  		current_state = Inicial;
		  		break;
		    }

    }


    return 0;
}
