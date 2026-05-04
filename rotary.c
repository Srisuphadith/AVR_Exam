// #define F_CPU 8000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include <avr/interrupt.h>
// #include "./include/pin_module.h"

// int ind = 0;
// int a = 0, b = 0, prev_a = 0, prev_b = 0;
// int arr[5][2];
// int arr_plus[5][2] = {{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}};
// int arr_minus[5][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}};
// int ok = 0;
// int arr_null[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

// int rotary = 0;

// void setup()
// {
//     pin_config(B, 0, I);
//     pin_config(B, 1, I);
//     pin_config(B, 2, O);
// }
// void loop()
// {
//     int state_plus = 1, state_minus = -1;
//     if (a == prev_a && b == prev_b)
//     {
//     }
//     else
//     {

//         a = digital_input(B, 0);
//         b = digital_input(B, 1);

//         if (a == 0 && b == 0 && ind == 0)
//         {
//             ok = 1;
//         }
//         else if (a == 0 && b == 0 && ind == 4)
//         {
//             ok = 0;
//         }
//         if (ok == 1)
//         {
//             arr[ind][0] = a;
//             arr[ind][1] = b;
//             ind++;

//             if (ind == 4) // อาจจะสาม
//             {
//                 for (int i = 0; i < 5; i++)
//                 {
//                     if (!(arr[i][0] == arr_plus[i][0] && arr[i][1] == arr_plus[i][1]))
//                     {
//                         state_plus = 0;
//                     }
//                     if (!(arr[i][0] == arr_minus[i][0] && arr[i][1] == arr_minus[i][1]))
//                     {
//                         state_minus = 0;
//                     }
//                 }

//                 if (state_plus == 1 && state_minus == -1){
//                     for (int i = 0; i < 5; i++){
//                         arr[i][0]=-1;
//                         arr[i][1]=-1;
//                     }
//                 }
//             }
//         }
//     }
//     rotary += (state_plus + state_minus);
//     if (a)
//     {
//         digital_event(B, 2, 1);
//     }
//     else
//     {
//         digital_event(B, 2, 0);
//     }
// }

// void main()
// {
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./include/pin_module.h"

int ind = 0;
int a = 0, b = 0, prev_a = 0, prev_b = 0;
int arr[5][2];
int arr_plus[5][2] = {{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}};
int arr_minus[5][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}};
int ok = 0;

int rotary = 0;

void setup()
{
    // pin_config(B, 0, I);
    // pin_config(B, 1, I);
    pin_config(B, 2, O);
}

void loop()
{
    int state_plus = 1, state_minus = -1;

    a = digital_input(B, 0);
    b = digital_input(B, 1);
    if (a != prev_a || b != prev_b)
    {
        prev_a = a;
        prev_b = b;

        if (a == 0 && b == 0 && ind == 0)
        {
            ok = 1;
        }

        if (ok == 1)
        {
            arr[ind][0] = a;
            arr[ind][1] = b;
            ind++;

            if (ind == 5)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (!(arr[i][0] == arr_plus[i][0] && arr[i][1] == arr_plus[i][1]))
                    {
                        state_plus = 0;
                    }
                    if (!(arr[i][0] == arr_minus[i][0] && arr[i][1] == arr_minus[i][1]))
                    {
                        state_minus = 0;
                    }
                }

                if (state_plus == 1)
                {
                    rotary++;
                }
                else if (state_minus == -1)
                {
                    rotary--;
                }

                ind = 0;
                ok = 0;
            }
        }
    }

    if (rotary > 10)
    {
        digital_event(B, 2, 1);
    }
    else
    {
        digital_event(B, 2, 0);
    }
}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}