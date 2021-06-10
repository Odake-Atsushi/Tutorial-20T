/***************************memo***************************
エンコーダの角度を見るプログラム
**********************************************************/

#include "mbed.h"
#include "rotary_encoder.h"

#define CONTROL_CYCLE 0.02f

/**********************************エンコーダの設定**********************************/
rotary_encoder ENC_1(D4, D5, NC, 512, rotary_encoder::X4_ENCODING); //ENC_0
Timer control_ticker;

Serial PC(USBTX, USBRX, 115200);

int main()
{
    control_ticker.start();
    for (;;)
    {
        // hoge_monitor.printf("motor_output %f\r\n", motor_cycleout);
        if (control_ticker.read() >= CONTROL_CYCLE)
        {
            control_ticker.reset();
            /********************* エンコーダー ********************/
            PC.printf("ENC==> %f\r\n", ENC_1.getRad());
        } //制御周期
    }     //while文
} //main文