/*********************
trapezoid_kadai_2.cppのプログラムの別解です．
現在のロボコンの上物機構のプログラムを参考にしたモノです．
この書き方は，かなり使えるので習得しておきましょう．
mbed研修の課題5-1
**********************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include "dc_motor_1.h"
#include "rotary_encoder.h"
#include "differential.h"
#include "trapezoid_control.h"
/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.005f //短くすることでフィートバックする回数を増やし，台形の精度を上げる
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void trapezoid_init(void);
inline int trapezoid_start();
/**********************************************************************
Declare variables
**********************************************************************/
Timer ControlTicker;
int a = 1;                //カウンタ変数
float target_x = 157.0;   //目標角度[rad](＝6.28 * 25)
float v_max = 20.0;       //最大角速度[rad/s]
float t_1 = 1.5;          //加速時間[s]
float t_3 = 2.0;          //減速時間[s]
float enco_now = 0.0;     //現在のタイヤの角度
float rad_velocity = 0.0; //現在のタイヤの角速度
float M0Output = 0.0;     //モータの出力
bool is_start = 0;        //スイッチを押したか判断する変数

/**********************************************************************
Declare Instances
**********************************************************************/
DigitalIn Sw(A1);
dc_motor_1 Motor(D3, D8, 1);
Differential calc_velocity(0.01, CTRL_PERIOD);
trapezoid_control trapezoid_control(target_x, v_max, t_1, t_3);
rotary_encoder ENC_M(D5, D4, NC, 100, rotary_encoder::X4_ENCODING);
Serial PC(USBTX, USBRX, 115200);

/**********************************************************************
Main
**********************************************************************/
int main()
{
    ControlTicker.start();

    trapezoid_init();
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
                if (trapezoid_start() == 1) //台形制御が終了
                {
                    is_start = 0; //スイッチのif文を抜ける
                }
                else
                {
                    is_start = 1;
                }
                Motor.drive(M0Output);

            } //flag
        }     //制御周期
    }         //for
} //main
/*****************************************************************
Functions
*****************************************************************/
inline void trapezoid_init() //台形制御の処理を関数にまとめる
{
    trapezoid_control.reset();                            //いったん台形制御の設定をリセットする(台形制御のライブラリから何がリセットされているのか確認する)
    trapezoid_control.set(target_x * a, v_max, t_1, t_3); //目標値を157×n（nは自然数）
    trapezoid_control.PID::set(CTRL_PERIOD);
    trapezoid_control.setGain(1.0, 20.0, 0.0);
    trapezoid_control.setFirstPosition(0.0);
    trapezoid_control.setCtrlPeriodSec(CTRL_PERIOD);
}
inline int trapezoid_start() //台形制御の処理をintの関数にまとめた．
{
    enco_now = ENC_M.getRad();                                             //角度の取得
    rad_velocity = calc_velocity.filter(enco_now);                         //角速度の取得
    M0Output = trapezoid_control.control(rad_velocity);                    //台形制御をかける(台形制御のライブラリから台形制御においてpid制御がどのように使われているのかを確認する)
    if (trapezoid_control.getTimer() >= trapezoid_control.getFinishTime()) //台形制御が終了したとき
    {
        PC.printf("finish\r\n"); //pidが終了したかの確認

        M0Output = 0.0;
        a = a * (-1);     //trapezoid.targetのaの部分を台形制御が終わるごとに向きを反転させる
        trapezoid_init(); //つぎの台形制御の設定を変更

        return 1; //end
    }
    else
    {
        return 0; //continue
    }
}