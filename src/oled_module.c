#define F_CPU 8000000UL
#include <avr/io.h>
#include "../include/twi_module.h"
#define SSD1306_ADDR 0x3C

uint8_t buffer[1024];

void OLED_command(uint8_t cmd)
{
    TWI_Start();
    TWI_Write(SSD1306_ADDR << 1); // write mode
    TWI_Write(0x00);              // control byte: command
    TWI_Write(cmd);
    TWI_Stop();
}

void OLED_data(uint8_t data)
{
    TWI_Start();
    TWI_Write(SSD1306_ADDR << 1);
    TWI_Write(0x40); // control byte: data
    TWI_Write(data);
    TWI_Stop();
}
void OLED_init(void)
{
    TWI_Init();

    OLED_command(0xAE); // display off
    OLED_command(0xD5);
    OLED_command(0x80); // clock
    OLED_command(0xA8);
    OLED_command(0x3F); // multiplex (64)
    OLED_command(0xD3);
    OLED_command(0x00); // offset
    OLED_command(0x40); // start line

    OLED_command(0x8D);
    OLED_command(0x14); // charge pump
    OLED_command(0x20);
    OLED_command(0x02); // addressing mode (horizontal)

    OLED_command(0xA1); // segment remap
    OLED_command(0xC8); // COM scan direction

    OLED_command(0xDA);
    OLED_command(0x12); // COM pins
    OLED_command(0x81);
    OLED_command(0xCF); // contrast

    OLED_command(0xD9);
    OLED_command(0xF1); // pre-charge
    OLED_command(0xDB);
    OLED_command(0x40); // vcom detect

    OLED_command(0xA4); // display all on resume
    OLED_command(0xA6); // normal display

    OLED_command(0xAF); // display on
}
void OLED_clear(void)
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        OLED_data(0x00);
    }
}

void OLED_drawPixel(uint8_t x, uint8_t y)
{
    if (x >= 128 || y >= 64)
        return;

    uint16_t index = x + (y / 8) * 128;
    buffer[index] |= (1 << (y % 8));
}
void OLED_update(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_command(0xB0 + page); // set page
        OLED_command(0x00);        // lower col
        OLED_command(0x10);        // higher col

        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_data(buffer[page * 128 + col]);
        }
    }
}

void OLED_clear_buffer()
{
    for (int i = 0; i < 1024; i++)
    {
        buffer[i] = 0;
    }
}
#include <avr/pgmspace.h> // ต้องใช้ไลบรารีนี้เพื่อเก็บฟอนต์ไว้ใน Flash Memory (ไม่ให้ RAM เต็ม)

// 1. สร้างตาราง Font ขนาด 5x7 พิกเซล (เฉพาะตัวเลข 0-9, ตัวอักษรบางตัว และสัญลักษณ์)
// เก็บไว้ใน PROGMEM เพื่อประหยัด RAM
const uint8_t font5x7[33][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0: Space (เว้นวรรค)
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 1: '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 2: '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 3: '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 4: '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 5: '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 6: '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 7: '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 8: '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 9: '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 10: '9'
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 11: 'V'
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // 12: 'A'
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 13: '.'
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 14: '+' 
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 15: '-'
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 16: 'X'
    {0x03, 0x04, 0x78, 0x04, 0x03}, // 17: 'Y'
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 18: 'H'
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 19: 'z' 
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 20: 'k'
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // 21: 'M'
    {0x7C, 0x04, 0x78, 0x04, 0x78}, // 22: 'm'
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 23: 'u'
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 24: 's'
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 25: 'T'
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 26: 'C'
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 27: 'R'
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 28: 'F'
    {0x08, 0x04, 0x08, 0x10, 0x08},  // 29: '~'
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 30: 'D'
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 31: 'I'
    {0x20, 0x10, 0x08, 0x04, 0x02}  // 32: '/'
};


// ฟังก์ชันแปลงตัวอักษรเป็น Index ของฟอนต์
uint8_t getFontIndex(char c) {
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return c - '0' + 1;
    if (c == 'V') return 11;
    if (c == 'A') return 12;
    if (c == '.') return 13;
    if (c == '+') return 14;
    if (c == '-') return 15;
    if (c == 'X') return 16;
    if (c == 'Y') return 17;
    if (c == 'H') return 18;
    if (c == 'z') return 19;
    if (c == 'k') return 20;
    if (c == 'M') return 21;
    if (c == 'm') return 22;
    if (c == 'u') return 23;
    if (c == 's') return 24;
    if (c == 'T') return 25;
    if (c == 'C') return 26;
    if (c == 'R') return 27;
    if (c == 'F') return 28;
    if (c == '~') return 29;
    if (c == 'D') return 30;
    if (c == 'I') return 31;
    if (c == '/') return 32;
    return 0; 
}


// 2. ฟังก์ชันสำหรับวาดตัวอักษร 1 ตัว
void OLED_drawChar(uint8_t x, uint8_t y, char c) {
    uint8_t index = getFontIndex(c);
    for (uint8_t i = 0; i < 5; i++) {
        // อ่านข้อมูลพิกเซลจาก Flash Memory
        uint8_t line = pgm_read_byte(&(font5x7[index][i])); 
        for (uint8_t j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                OLED_drawPixel(x + i, y + j);
            }
        }
    }
}

// 3. ฟังก์ชันสำหรับวาดข้อความยาวๆ
void OLED_printString(uint8_t x, uint8_t y, const char* str) {
    while (*str) {
        OLED_drawChar(x, y, *str);
        x += 6; // เลื่อนแกน X ไปทีละ 6 พิกเซล (ความกว้างฟอนต์ 5 + ช่องว่าง 1)
        str++;
    }
}

static int abs_int(int v)
{
    return (v < 0) ? -v : v;
}

void OLED_drawLine(uint8_t x0, uint8_t y0,
                   uint8_t x1, uint8_t y1)
{
    if (x0 >= 128 || y0 >= 64 || x1 >= 128 || y1 >= 64 )
        return;
    int dx = abs_int(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs_int(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;
    int e2;

    while (1)
    {
        OLED_drawPixel(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}