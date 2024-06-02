/*
 ============================================================================
 Name        : Ali_Mamdouh_Mini_Project3.c
 Author      : Ali Mamdouh
 Description : Fan which is is automatically turned ON or OFF and change its Speed according to the Temperature
 Date        : 12/10/2022
 ============================================================================
 */
#include "lcd.h"
#include "lm35_sensor.h"
#include "adc.h"
#include "motor.h"

int main(void)
{
	uint8 temp = 0;
	ADC_ConfigType adcConfig = {ADC_REF_VOLT_CODE, F_CPU_8};
	DcMotor_State motorState = CW;
	uint8 motorSpeed = 100;
	LCD_init(); /* Initialize the LCD */
	ADC_init(&adcConfig);
	DcMotor_Init();
	while(1)
	{
		/*
		 *The coming lines is to display temperature
		 */
		LCD_moveCursor(0,0); //To write in the same place in LCD in the next iteration
		LCD_displayString("Temp is ");
		temp = LM35_getTemperature();

		if(temp >= 100)
		{
			LCD_intgerToString(temp);
			LCD_displayString("C");
		}
		else if(temp >= 10)
		{
			LCD_intgerToString(temp);
			LCD_displayString(" C"); //In case the digital value is two digits print space and C in the next digit place
		}
		else
		{
			LCD_intgerToString(temp);
			LCD_displayString("  C"); //In case the digital value is one digit print space and C in the next digit place
		}

		/*
		 *The coming lines is to control motor speed with temperature
		 */
		if(temp < 30)
		{
			DcMotor_Rotate(STOP,0);
		}
		else if(temp < 60 ) //Temperature greater than or equal 30
		{
			DcMotor_Rotate(CW,25);
		}
		else if(temp < 90 ) //Temperature greater than or equal 60
		{
			DcMotor_Rotate(CW,50);
		}
		else if(temp < 120 ) //Temperature greater than or equal 90
		{
			DcMotor_Rotate(CW,75);
		}
		else //Temperature greater than or equal 120
		{
			DcMotor_Rotate(CW,100);
		}
	}



}
