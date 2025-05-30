# STM32 OLED Display Driver via I2C

A lightweight, reusable OLED driver library written in C for STM32 microcontrollers using the I2C interface. Designed to display simple texts and emojis on 128x64 OLED screens (SSD1306).

##  Hardware

| Component        | Details                      |
|------------------|------------------------------|
| Microcontroller  | STM32   |
| Display          | OLED 128x64 (SSD1306)        |
| Interface        | I2C                          |
| Power            | 3.3V or 5V                   |

##  File Structure
/OLED_Driver/
│── OLED_Driver.c
│── OLED_Driver.h
│── Font_8x8.c
│── Font_8x8.h
│── Font_16x24.c
│── Font_16x24.h
│── Font_16x32.c
│── Font_16x32.h
│── Font_emojis.c
│── Font_emojis.h
/main/
│── main.c
│── main.h


## Wiring 
     SCL --- > PB10
     SDA --- > PB11

##  Usage Example


#include "OLED_Driver.h"

int main(void)
{ // HAL, clock, and I2C2 must be initialized before this

  

           SetFont(& Font_8x8) ; // 1 byte wide 1 byte tall
           SetCursor(0 , 0 );
	   char str [] = "Nabil" ;
	   OLEDWrite(str) ; // writing to buffer 
	   updateOLED(); // sending the overwritten page only to the OLED
	   HAL_Delay(2000) ;
	   SetFont(& Font_emojis) ; // using emojis bitmap
	   SetCursor(0 , 1 );
          char str1 []="N" ;
           OLEDWrite(str1) ;
	   updateOLED();
    while (1) { }
}    
 Full example in `main/src/main.c`
  
