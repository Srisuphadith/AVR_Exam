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
```

---

# analog_module

include
```
    #include "./include/analog_module.h"
```
Ex. อ่านค่า analog จาก ADC3
```
    uint8_t value = analog_read(3);
```

---

# lcd_module

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

PB0 -> trigger
PB3 -> echo

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

# mcp_module (อ่าน analog 12 bit resolution จาก MCP3201)

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
