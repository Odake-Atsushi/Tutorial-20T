/*********************
スイッチを1回押すと，LEDが点灯して，もう一回押すとLEDが消えるプログラム．（LEDチカ）
**********************/
#include "mbed.h"
#define CONTROL_CYCLE 0.02f

DigitalIn SW(A1);
//DigitalOut myled(LED1);
AnalogOut led(A3);
Timer control_ticker;
int a = 0;

Serial PC(USBTX, USBRX, 115200);

int main()
{
    control_ticker.start();
    while (1)
    {
        if (control_ticker.read() >= CONTROL_CYCLE)
        {
            control_ticker.reset();

            if (SW.read() == 0 && a == 1)
                led = !led;

            a = SW.read();
        }
    }
}
