#ifndef OLED_Driver_H
#define OLED_Driver_H

#include <stdint.h>

#define SSD1306_I2C_ADDR 0x78

typedef struct {
    uint8_t width;
    uint8_t height;
    void (*writeChar)(char c);
} Fonts;


extern const Fonts Font_16x24;
extern const Fonts Font_emojis;
extern const Fonts Font_8x8;
extern const Fonts Font_16x32;


void OLED_Init(void);
void ssd1306_sendCommand(uint8_t cmd);
void OLED_SendData(uint8_t* data, int size , int row );
int getIndex(char C);
void OLEDWriteC_16x32(char C);
void OLEDWriteC_16x24(char C);
void OLEDWriteC_8x8(char C);
void OLEDWriteC_emojis(char C);
void SetFont(const Fonts* font);
void updateOLED(void);
void OLED_clear(void);
void SetCursor(int x , int y);
void OLEDWrite(char* str);

#endif
