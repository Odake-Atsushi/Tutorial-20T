/*********************
1,スイッチを押すと，モータが6.28rad回転する．
2,もう一回スイッチを押すと1と同じような動作する
mbed研修の課題4－2
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
dc_motor_1 Motor(D3, D8, 0);
LPF lpf_0(CTRL_PERIOD, 0.3);
PID pid(CTRL_PERIOD);
rotary_encoder ENC_M(D4, D5, NC, 100, rotary_encoder::X4_ENCODING);
Serial PC(USBTX, USBRX, 115200);
/**********************************************************************
Declare variables
**********************************************************************/
Timer ControlTicker;
int a = 1;
float target = 6.28;
/**********************************************************************
Main
**********************************************************************/
int main()
{
    ControlTicker.start();

    float enco_now = 0.0;
    float M0Output = 0.0;
    bool is_start = 0;
    pid_init();
    for (;;)
    {
        if (ControlTicker.read() >= CTRL_PERIOD)
        {
            ControlTicker.reset();
            PC.printf("%f %f\r\n", enco_now, M0Output);
            if (Sw == 0) //スイッチ押す前
            {
                is_start = 1;
            }
            if (is_start == 1) //スイッチを押した後
            {
                enco_now = ENC_M.getRad();
                M0Output = pid.control(enco_now); //pid制御をかける

                if (pid.checkEnd() == 0) //pidが終了したとき
                {
                    PC.printf("finish\r\n"); //pidが終了したかの確認

                    is_start = 0; //スイッチを押したらもう一回回転するようなプログラムなのでスイッチを押す前の状態に戻す
                    a++;          //pid.targetのaの部分をpidが終わるごとに増やす(a=a+1と等価)
                    pid_init();   //つぎのpidの設定を変更
                }
                Motor.drive(M0Output);
            } //flag
        }     //制御周期
    }         //for
} //main
/*****************************************************************
Functions
*****************************************************************/
void pid_init() //pidの処理を関数にまとめる
{
    pid.reset();               //いったんpidの設定をリセットする
    pid.setTarget(target * a); //ターゲットが6.28×n（nは自然数）
    pid.setGain(10.0, 5.0, 0.0);
    pid.setEndStatus(0.5, 0.5);
    pid.setFirstPosition(0.0);
}