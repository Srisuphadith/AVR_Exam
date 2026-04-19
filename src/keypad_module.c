#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/pin_module.h"

//เเก้ขาตรงนี้นะะะ!!!!!!
//              r1,r2,r3,r4 //  ดำ
char row_s[4] = {B, D, D, D};
int row_pn[4] = {7, 5, 6, 7};

//              c1,c2,c3,c4 // ขาว
char col_s[4] = {B, B, B, B};
int col_pn[4] = {0, 1, 2, 3};

char keyData[4][4] = {{'1', '2', '3', 'A'},
                      {'4', '5', '6', 'B'},
                      {'7', '8', '9', 'C'},
                      {'*', '0', '#', 'D'}};

char keypad_read()
{
    int r = -1, c = -1;
    for (int i = 0; i < 4; i++)
    {
        pin_config(row_s[i], row_pn[i], I);
        pin_config(col_s[i], col_pn[i], O);
    }
    for (int i = 0; i < 4; i++)
    {
        pin_config(row_s[i], row_pn[i], O);
        for (int j = 0; j < 4; j++)
        {
            pin_config(col_s[j], col_pn[j], I);
            if (!digital_input(col_s[j], col_pn[j]))
            {
                r = i;
                c = j;
            }
            pin_config(col_s[j], col_pn[j], O);
        }
        pin_config(row_s[i], row_pn[i], I);
    }
    if (r == -1 || c == -1)
    {
        return 'N';
    }
    else
    {
        return keyData[r][c];
    }
}

// void setup()
// {
// }
// void loop()
// {
// }

// void main()
// {
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }