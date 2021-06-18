/*********************
30rad/sで回転するようなpid制御を行うプログラム
mbed研修の課題2－2
**********************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include "lpf.h"
#include "dc_motor_1.h"
#include "rotary_encoder.h"
#include "pid.h"
#include "differential.h"
#include "integral.h"
/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
void pid_init(void);
/**********************************************************************
Declare Instances
**********************************************************************/
DigitalIn Sw(A1);
DigitalOut Led0(A3);
DigitalOut Led1(A0);
dc_motor_1 Motor(D3, D8, 1);
PID pid(CTRL_PERIOD);
Differential diff_enco(0.05, CTRL_PERIOD); //微分のライブラリの宣言
rotary_encoder ENC_M(D4, D5, NC, 50, rotary_encoder::X4_ENCODING);
Serial PC(USBTX, USBRX, 115200);
/**********************************************************************
Declare variables
**********************************************************************/
Timer ControlTicker;
float target = 30.0;
float enco_now = 0.0;
float M0Output = 0.0;
float rad_velocity = 0.0f;
bool is_start = 0;
/**********************************************************************
Main
**********************************************************************/
int main()
{
    ControlTicker.start();

    pid_init();
    for (;;)
    {
        if (ControlTicker.read() >= CTRL_PERIOD)
        {
            ControlTicker.reset();
            if (Sw == 0)
            {
                is_start = 1;
            }
            if (is_start == 1)
            {
                enco_now = ENC_M.getRad();
                rad_velocity = diff_enco.filter(enco_now); //角度を微分して角速度を求める
                M0Output = pid.control(rad_velocity);      //現在の角速度を確認してpid制御

                Motor.drive(M0Output);
                PC.printf("%f %f\r\n", rad_velocity, M0Output);

            } //flag
        }     //制御周期
    }         //for
} //main
/*****************************************************************
Functions
*****************************************************************/
void pid_init() //pid制御の設定を関数としてまとめる
{
    pid.setTarget(target);
    pid.setGain(7.0, 3.0, 0.0);
    pid.setEndStatus(0.5, 0.5);
    pid.setFirstPosition(0.0);
}