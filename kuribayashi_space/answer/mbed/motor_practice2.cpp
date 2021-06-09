/*********************
スイッチを1回押すと，モータが正回転に動き，もう一回押すとモータが負の回転に動く．
また，スイッチを押していないときは止まるプログラム．
mbed研修の課題2－1
**********************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include "dc_motor_1.h"
#define CONTROL_CYCLE 0.02f
/**********************************************************************
Declare Instances
**********************************************************************/
DigitalIn SW(A1);
dc_motor_1 M0(D3, D8, 0);
DigitalOut led_0(LED1); //入力するときは，Outを使う．Inはこちらから入力できない(読み取り専用　ex.スイッチとか)
AnalogOut led_1(A3);
Serial PC(USBTX, USBRX, 115200);
/**********************************************************************
Declare variables
**********************************************************************/
Timer control_ticker;
bool b = 0;
float motor_cycleout = 0.0;
/**********************************************************************
Main
**********************************************************************/
int main()
{
    control_ticker.start();
    while (1)
    {
        if (control_ticker.read() >= CONTROL_CYCLE)
        {
            control_ticker.reset();

            if (SW.read() == 0)
            {
                if (b == 0 && motor_cycleout == 0.0f)
                {
                    motor_cycleout = 30.0;
                    b = 1;
                    led_0 = 1; //つく
                    led_1 = 0; //つかない
                }
                else if (b == 1 && motor_cycleout == 0.0f)
                {
                    motor_cycleout = -30.0;
                    b = 0;
                    led_0 = 0; //つく
                    led_1 = 1; //つかない
                }
            }
            else
            {
                motor_cycleout = 0;
                led_0 = 0; //つかない
                led_1 = 0; //つかない
            }
            M0.drive(motor_cycleout);
        }
    }
}
