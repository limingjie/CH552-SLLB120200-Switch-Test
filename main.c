#include <ch554.h>
#include <gpio.h>
#include <oled.h>    // OLED
#include <stdio.h>   // sprintf
#include <system.h>  // mcu_config()
#include <time.h>    // millis(), delay()

#define SW_VALUE_MIN 0
#define SW_VALUE_MAX 100

#define SW_1 P14
#define SW_2 P15
#define SW_T P34
#define SW_3 P33
#define SW_4 P11

__data uint32_t last_sw_change_time = 0;

void startup()
{
    // Set MCU Frequency
    mcu_config();
    delay(5);

    timer_init();
    OLED_init();

    // Init SLLB120200 pins
    PIN_input_PU(SW_1);
    PIN_input_PU(SW_2);
    PIN_input_PU(SW_T);
    PIN_input_PU(SW_3);
    PIN_input_PU(SW_4);
}

void main()
{
    __xdata char buf[25];

    startup();

    __bit           b_SW_1       = 1;
    __bit           b_SW_2       = 1;
    __bit           b_SW_3       = 1;
    __bit           b_SW_4       = 1;
    __bit           b_SW_T       = 1;
    __bit           SW_was_left  = 0;
    __bit           SW_was_right = 0;
    __data uint8_t  SW_value     = 0;
    __data uint8_t  slider_pos   = 0;
    __data uint16_t SW_interval  = 500;

    OLED_printxy(0, 0, "===SLLB120200 Test===");
    OLED_printxy(0, 2, "PIN  1 2 T 3 4 C");
    OLED_printxy(0, 3, "               G");
    OLED_printxy(0, 5, "Val =        SW = OFF");
    OLED_printxy(0, 6, "--------------------+");
    OLED_printxy(0, 7, "#====================");

    while (1)
    {
        b_SW_1 = PIN_read(SW_1);
        b_SW_2 = PIN_read(SW_2);
        b_SW_3 = PIN_read(SW_3);
        b_SW_4 = PIN_read(SW_4);
        b_SW_T = PIN_read(SW_T);

        OLED_printxy(5, 3, b_SW_1 ? "1" : "0");
        OLED_printxy(7, 3, b_SW_2 ? "1" : "0");
        OLED_printxy(9, 3, b_SW_T ? "1" : "0");
        OLED_printxy(11, 3, b_SW_3 ? "1" : "0");
        OLED_printxy(13, 3, b_SW_4 ? "1" : "0");

        if (!b_SW_2)  // On Far-Left
        {
            if (millis() - last_sw_change_time > SW_interval && SW_value > SW_VALUE_MIN)
            {
                last_sw_change_time = millis();
                SW_value--;
                SW_interval = 50;
            }
        }
        else if (!b_SW_1)  // On Left
        {
            if (!SW_was_left)
            {
                SW_was_left = 1;
                if (SW_value > SW_VALUE_MIN)
                {
                    last_sw_change_time = millis();
                    SW_value--;
                }
            }
        }
        else if (!b_SW_3)  // On Far-Right
        {
            if (millis() - last_sw_change_time > SW_interval && SW_value < SW_VALUE_MAX)
            {
                last_sw_change_time = millis();
                SW_value++;
                SW_interval = 50;
            }
        }
        else if (!b_SW_4)  // On Right
        {
            if (SW_was_right != 1)
            {
                SW_was_right = 1;
                if (SW_value < SW_VALUE_MAX)
                {
                    last_sw_change_time = millis();
                    SW_value++;
                }
            }
        }
        else
        {
            SW_was_left  = 0;
            SW_was_right = 0;
            SW_interval  = 500;
        }

        sprintf(buf, "%03d", SW_value);
        OLED_printxy(6, 5, buf);
        OLED_printxy(18, 5, b_SW_T ? "OFF" : "ON ");
        if (SW_value / 5 != slider_pos)
        {
            OLED_printxy(20 - slider_pos, 6, "-");
            OLED_printxy(slider_pos, 7, "=");
            slider_pos = SW_value / 5;
            OLED_printxy(20 - slider_pos, 6, "+");
            OLED_printxy(slider_pos, 7, "#");
        }
    }
}
