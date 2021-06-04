/**********************************************************************
File    kadai_1.cpp
Author  Yusuke Kuribayashi
Micro Controller    NUCLEO_F303K8
Schematic       NUCLEO_F303K8_mainboard_2018
StartDay 2021/5/17
FinishDay 2021/5/18
**********************************************************************/
/**********
Mbedとの通信のプログラム
***********/
/**********************************************************************
Include Libraries
**********************************************************************/
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f //制御周期
#define time_target 0.2f  //ledの点灯時間
/**********************************************************************
Proto_type_Declare functions（関数の宣言）
**********************************************************************/
inline void cb_counter(const std_msgs::Int32::ConstPtr &msg_counter);
inline void cb_chatter(const std_msgs::String::ConstPtr &msg_chatter);

/**********************************************************************
Declare variables(変数宣言)
**********************************************************************/
float target_led; //ledの点灯時間を送る
int a;            //送り返された点灯時間を格納する
std::string test; //送られた文字を格納.
//動的に配列のサイズを変換できるためchar型でなくstring型を用いる/stdは名前空間
std_msgs::Float32 msg_float; //mbedを送る

/**********************************************************************
Main
**********************************************************************/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "kadai_1");
    ros::NodeHandle n;
    ros::Publisher pub_counter = n.advertise<std_msgs::Float32>("reference", 100); //float型の変数を送る_ledの点灯時間を送る
    ros::Subscriber sub_counter = n.subscribe("counter", 100, cb_counter);         //送った回数を受け取る
    ros::Subscriber sub_chatter = n.subscribe("chatter", 100, cb_chatter);         //”test”の表示を受け取る

    ros::Rate loop_rate(1 / CTRL_PERIOD);

    msg_float.data = time_target; //ledの点灯時間を代入

    while (ros::ok())
    {
        ros::spinOnce();                     //callback関数を読み込む
        ROS_INFO("%d, %s", a, test.c_str()); //ターミナルに表示/string型は[変数名].c_str()で表示

        pub_counter.publish(msg_float); //mbedに送る

        loop_rate.sleep(); //処理が制御周期より早く終わった時，ここで止まって残った時間を使う
    }                      //制御周期
} //main文

/*****************************************************************
Functions <Call Back>
*****************************************************************/
//::ConstPtrを書かない時はいつものように".data"で表すことができる．
//::ConstPtrを書くときはメンバ変数にアクセスするような形で"->data"を表す.
inline void cb_counter(const std_msgs::Int32::ConstPtr &msg_counter)
{
    a = msg_counter->data;
}
inline void cb_chatter(const std_msgs::String::ConstPtr &msg_chatter)
{
    test = msg_chatter->data;
}