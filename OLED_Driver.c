#include "OLED_Driver.h"
#include <stdint.h>
#include "Font_16x24.h"
#include "Font_16x32.h"
#include "Font_8x8.h"
#include "Font_emojis.h"
#include <string.h>
#include <stdbool.h>
#include "main.h"  // usually where hi2c2 is declared, or
extern I2C_HandleTypeDef hi2c2;
void  ssd1306_sendCommand(uint8_t cmd) {
     uint8_t buffer[2];
     buffer[0] = 0x00;  // Control byte: tells the OLED the next byte is a command
     buffer[1] = cmd;   // The actual command byte to send
     HAL_I2C_Master_Transmit(&hi2c2, SSD1306_I2C_ADDR, buffer, 2, HAL_MAX_DELAY);
 }
 void OLED_SendData(uint8_t* data, int size , int row ) {
	 ssd1306_sendCommand(0x21); // Set column address
	 ssd1306_sendCommand(0x00); // Start column 0
	 ssd1306_sendCommand(0x7F); // End column 127
	 ssd1306_sendCommand(0x22); // Set page address
	 ssd1306_sendCommand(row);
	 ssd1306_sendCommand(row);
	 uint8_t buffer[size + 1];
	    buffer[0] = 0x40;  // Control byte: data

	    memcpy(&buffer[1], data, size);

	    // Now send buffer via I2C
	    HAL_I2C_Master_Transmit(&hi2c2, SSD1306_I2C_ADDR, buffer , size + 1, HAL_MAX_DELAY);
 }
 void OLED_Init(void) {
     HAL_Delay(100);  // Wait after power on

     ssd1306_sendCommand(0xAE); // Display OFF

     ssd1306_sendCommand(0xD5); // Set display clock divide
     ssd1306_sendCommand(0x80);

     ssd1306_sendCommand(0xA8); // Set multiplex ratio
     ssd1306_sendCommand(0x3F); // 1/64

     ssd1306_sendCommand(0xD3); // Display offset
     ssd1306_sendCommand(0x00);

     ssd1306_sendCommand(0x40); // Set display start line to 0

     ssd1306_sendCommand(0x8D); // Charge pump
     ssd1306_sendCommand(0x14); // Enable

     ssd1306_sendCommand(0x20); // Memory addressing mode
     ssd1306_sendCommand(0x00); // Horizontal

     ssd1306_sendCommand(0xA1); // Segment re-map
     ssd1306_sendCommand(0xC8); // COM scan direction

     ssd1306_sendCommand(0xDA); // COM pins hardware config
     ssd1306_sendCommand(0x12);

     ssd1306_sendCommand(0x81); // Set contrast
     ssd1306_sendCommand(0xCF);

     ssd1306_sendCommand(0xD9); // Set pre-charge
     ssd1306_sendCommand(0xF1);

     ssd1306_sendCommand(0xDB); // VCOM detect
     ssd1306_sendCommand(0x40);

     ssd1306_sendCommand(0xA4); // Resume to RAM display
     ssd1306_sendCommand(0xA6); // Normal (not inverted)

     ssd1306_sendCommand(0xAF); // Display ON

     HAL_Delay(100);
 }

 const char  lookup [] = {' ', '!', '"', '#', '$', '%', '&', '"', '(', ')', '*', '+', ',', '-', '.', '/',
           '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@',
           'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
           'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`',
           'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
           'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
} ;

  uint8_t buffer[1024] ;
  uint32_t Cursor;
  bool flag [8] = {0} ;
 int getIndex(char C ) {
 for (int i = 0 ; i<95 ; i++)
 {if (lookup[i]==C) { return i ; }}
  return -1 ; }

 static const Fonts* CurrentFont;
 void OLEDWriteC_16x32(char C)
  {
  	int index = getIndex(C) ;
  	if (index==-1) return ;
  	for (int i = 0; i < 16; i++) {
  	        // Extract 4 vertical bytes (32 bits) for this column
  	        uint32_t column = FontData_16x32[index * 16 + i];

  	        uint8_t byte0 = (column >> 24) & 0xFF;
  	        uint8_t byte1 = (column >> 16) & 0xFF;
  	        uint8_t byte2 = (column >> 8) & 0xFF;
  	        uint8_t byte3 = column & 0xFF;
  	        buffer[Cursor + i + 128 * 0] = byte3;
  	        buffer[Cursor + i + 128 * 1] = byte2;
  	        buffer[Cursor + i + 128 * 2] = byte1;
  	        buffer[Cursor + i + 128 * 3] = byte0;

  		}
  	flag [Cursor/128  ] = true ;
  	flag [Cursor/128 + 1 ] = true ;
  	flag [Cursor/128 + 2 ] = true ;
  	flag [Cursor/128 + 3 ] = true ;
      Cursor += 16; // Move cursor horizontally for next character

  } ;
 void OLEDWriteC_16x24(char C)
 {
 	int index = getIndex(C) ;
 	if (index==-1) return ;
 	for (int i = 0; i < 16; i++) {
 	        // Extract 3 vertical bytes (24 bits) for this column
 	        uint32_t column = FontData_16x24[index * 16 + i]; // 3 bytes packed in one uint32_t

 	        uint8_t byte0 = (column >> 16) & 0xFF; // Top 8 pixels
 	        uint8_t byte1 = (column >> 8) & 0xFF;  // Middle 8
 	        uint8_t byte2 = column & 0xFF;
 	        buffer[Cursor + i + 128 * 0] = byte2;
 	        buffer[Cursor + i + 128 * 1] = byte1;
 	        buffer[Cursor + i + 128 * 2] = byte0;

 		}
 	 flag [Cursor/128] = true ;
 	flag [Cursor/128 + 1 ] = true ;
 	flag [Cursor/128 + 2 ] = true ;
     Cursor += 16; // Move cursor horizontally for next character

 } ;
 void OLEDWriteC_8x8(char C)
 {
 	int index = getIndex(C) ;
 	if (index==-1) return ;
 	for (int i = 0; i < 8; i++) {
 	        buffer [Cursor + i] = FontData_8x8[index*8+i];

 		}
 	flag [Cursor/128  ] = true ;
     Cursor += 8; // Move cursor horizontally for next character

 }
 void OLEDWriteC_emojis(char C)
 {
 	int index = getIndex(C) ;
 	if (index==-1) return ;
 	for (int i = 0; i < 32; i++) {
 	        // Extract 4 vertical bytes (32 bits) for this column
 	        uint32_t column = FontData_32x32[index * 32 + i];

 	        uint8_t byte0 = (column >> 24) & 0xFF;
 	        uint8_t byte1 = (column >> 16) & 0xFF;
 	        uint8_t byte2 = (column >> 8) & 0xFF;
 	        uint8_t byte3 = column & 0xFF;
 	        buffer[Cursor + i + 128 * 0] = byte3;
 	        buffer[Cursor + i + 128 * 1] = byte2;
 	        buffer[Cursor + i + 128 * 2] = byte1;
 	        buffer[Cursor + i + 128 * 3] = byte0;

 		}
 	flag [Cursor/128  ] = true ;
 	flag [Cursor/128 + 1 ] = true ;
 	flag [Cursor/128 + 2 ] = true ;
 	flag [Cursor/128 + 3 ] = true ;
     Cursor += 32; // Move cursor horizontally for next character

 } ;
 const Fonts Font_16x24 = {
     .width = 2,
     .height = 3,
	.writeChar = OLEDWriteC_16x24
 };

 const Fonts Font_emojis = {
     .width = 4,
     .height = 4,
	 .writeChar = OLEDWriteC_emojis
 };

 const Fonts Font_8x8 = {
     .width = 1,
     .height = 1,
	 .writeChar = OLEDWriteC_8x8
 };
 const Fonts Font_16x32 = {
      .width = 2,
      .height = 4,
 	 .writeChar = OLEDWriteC_16x32
  };

 void SetFont(const Fonts* font)
 {   if (font != NULL ) CurrentFont = font ;
}

void updateOLED(void){
	uint8_t disp[128];

	for(int i=0 ; i<8 ; i++){

	if (flag [i]) {
		memcpy(disp , &buffer[i*128] , 128) ;
		OLED_SendData (disp , 128 , i) ;
		flag [i] = false ;

	}}
}

void OLED_clear(void)
{
	 uint8_t clear[128] = {0};  // All zeros = all pixels off

	    for (uint8_t i = 0; i < 8; i++) {
	        OLED_SendData(clear, 128, i);  // Clear one page at a time
	    }


}


void SetCursor(int x , int y )
{ Cursor =  y*128*CurrentFont->height + x*8*CurrentFont->width ;
}




void OLEDWrite(char* str)
{ if (!CurrentFont || !str) return;
int len = strlen (str) ;
for (int i=0 ; i<len ; i++) CurrentFont-> writeChar(str[i]) ;

}
