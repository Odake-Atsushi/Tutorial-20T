/**********************************************************************
SampleCodeの説明
pidと台形制御のライブラリを継承しただけなので，基本的にメンバ関数と引数は変わりないです．
ただ，timerを引数に取らないようにするためtimerの関数を追加したのでmain文にはstartTimerのメンバ関数を書いておいてください
詳しく知りたい人は，僕に聞くかライブラリを見てください
**********************************************************************/
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

#define CTRL_PERIOD 0.005f

/**********************************************************************
Declare variables
**********************************************************************/
float target_x = 150.0;
float v_max = 20.0;
float t_1 = 1.5;
float t_3 = 2.0;
float timer0 = 0.0;
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
void pid_set(void);
/**********************************************************************
Declare Instances
**********************************************************************/

Timer ControlTicker;

dc_motor_1 Motor0( D3, D8, 0 );
Differential calc_velocity(0.25,CTRL_PERIOD);
trapezoid_control trapezoid_control(target_x,v_max,t_1,t_3);
rotary_encoder ENC_M0(D5,D4,NC,100,rotary_encoder::X4_ENCODING);
Serial PC(USBTX,USBRX,115200);
DigitalIn Sw0(A1);



int Is_start0 ;
/**********************************************************************
Main
**********************************************************************/
int main(){
  ControlTicker.start();

  float velocity = 0.0;
  float enc_now0 = 0.0;
  float M0Output = 0.0;
  bool finish_time = 0;

  PC.printf("確認 \r\n");
  pid_set();

  for (;finish_time == 0;){
    if ( ControlTicker.read() >= CTRL_PERIOD ){
      ControlTicker.reset();

      enc_now0 = ENC_M0.getRad();
      velocity = calc_velocity.filter(enc_now0);
      if(Sw0.read() == 0){
        Is_start0 = 1;
      }
      if(Is_start0 == 1){
        M0Output = trapezoid_control.control(velocity);
        Motor0.drive(M0Output);

        PC.printf("%2.4f %2.4f %2.4f %2.4f %2.4f %d \r\n",trapezoid_control.getTimer(),velocity,M0Output,enc_now0,trapezoid_control.filter(),trapezoid_control.checkEnd());
        if(trapezoid_control.getTimer() >= trapezoid_control.getFinishTime()){
          finish_time = 1;
          M0Output = 0.0;
          Motor0.drive(M0Output);
        }
      }//Is_start0=1
    }//0.02s
  }//for
}//main

/**********************************************************************
Functions
**********************************************************************/
void pid_set(){
  trapezoid_control.PID::set(CTRL_PERIOD);
  trapezoid_control.setGain(1.0,20.0,0.0);
  trapezoid_control.setFirstPosition(0.0);
  trapezoid_control.setCtrlPeriodSec( CTRL_PERIOD);
  //trapezoid_control.setLimitIOutput(100.0);
  // trapezoid_control.setLimitOutput(60.0);
}
