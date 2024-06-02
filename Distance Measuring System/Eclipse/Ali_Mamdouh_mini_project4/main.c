/*
 ============================================================================
 Name        : Ali_Mamdouh_mini_project4.c
 Author      : Ali Mamdouh
 Description : Measuring distance by ultrasonic and display distance measured on lcd
 Date        : 18/10/2022
 ============================================================================
 */
#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>


int main(void)
{
	uint16 distance = 0;
	SREG |= (1<<7);
	LCD_init(); /* Initialize the LCD */
	Ultrasonic_init();

	//LCD_displayStringRowColumn(1, 0, "MT Emb. Dipl");
	distance = Ultrasonic_readDistance();
	LCD_intgerToString(distance);



	while(1)
	{
		LCD_moveCursor(0,0);
		LCD_displayString("Distance= ");
		distance = Ultrasonic_readDistance();

		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayCharacter(' ');
		}

		LCD_displayStringRowColumn(0,13,"cm");

	}

}

