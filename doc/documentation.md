# LIB pin_module

include
```
    #include "./include/pin_module.h"
```
Ex. กำหนด PB0 เป็น input เเบบ pull-up
```
    pin_config(B,0,I);
```
Ex. กำหนด PD3 เป็น output
```
    pin_config(D,3,O);
```
Ex. เปิด PD3 ให้มีค่า HIGH
```
    digital_event(D,3,1);
```
Ex. อ่านค่าจาก pin PB6
```
    uint8_t = digital_input(B,6);
```

---

# delay_module

include
```
    #include "./include/delay_module.h"
```
ต้องเรียกใช้ ก่อนใน setup
```
    delay_module();
```
ถึงจะใช้ได้
```
    my_delay(1000); //หน่วย ms ต่ำสุด 1 ms
    my_delay_us(10); //หน่วย us ต่ำสุด 1 us
```

---

# analog_module

include
```
    #include "./include/analog_module.h"
```
Ex. อ่านค่า analog จาก ADC3
```
    uint16_t value = analog_read(3);
```

---

# lcd_module
***กำหนด pin ใน ./src/lcd_module.c***
include
```
    #include "./include/lcd_module.h"
```
ต้องเรียกใช้ ก่อนใน setup
```
    lcd_module();
```
ถึงจะใช้งานได้
```
    lcdPrintString("Hello world");
    lcdPrintChar('M');
    lcdHome();
    lcdLine2();
    lcdClear();
```

---

# keypad_module

***กำหนด pin ใน ./src/keypad_module.c***
include
```
    #include "./include/keypad_module.h"
```
เรียกใช้ได้เลย
```
    char buff = keypad_read() //ถ้าไม่มีการกดจะได้ค่า 'N'
```

---

# ultrasonic_module

PB0 -> echo
PB3 -> trigger

include
```
    #include "./include/ultrasonic_module.h"
```
เรียกใช้ก่อนใน setup
```
    ultrasonic_begin();
```
เเล้วสามารถเรียกค่ามาใช้ใน loop ได้เลย
```
    float dis = distance;
```

---

# mcp_module (ADC 12 bit resolution MCP3201)

PB5(CLK) -> 7(CL)MCP
PB4(MISO) -> 6(Dout) MCP
PB2(SS) -> 5(CS) MCP

include
```
    #include "./include/mcp_module.h"
```
เรียกใช้ใน loop ได้เลย
```
    uint16_t value = mcp_read();
```

---

# ds1307_module (Real Time Clock)
PC5(SCL) -> 6(SCL) DS1307
PC4(SDA) -> 5(SDA) DS1307

include
```
    #include "./include/ds1307_module.h"
```
เรียกใช้ใน setup ก่อน 
```
    rtc_module();
```
เรียกใช้ได้เลย
```
    void rtc_setDateTime(Hour, min, sec, date, month, year);//เรียกใน setup เราจะ set เวลาเเค่รอบเดียว
    void rtc_getDateTime(&Hour, &min, &sec, &date, &month, &year); //ส่งเป็น pointer
    char mo = monthName(month); // ได้เดือนเป็น Ex. Jan Feb ....
```
