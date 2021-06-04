/*********************************
ppubliserとsubscriberのプログラムの練習1
**********************************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/String.h"

/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f

/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void cb_count(const std_msgs::String::ConstPtr &msg);

/**********************************************************************
Declare variables
**********************************************************************/
std_msgs::Float32 msg_float;       //node(ファイル)間の通信を行うからrosの型
std_msgs::Int32MultiArray msg_int; //node(ファイル)間の通信を行うからrosの型
std::string letter;                //c++の型,std::は付ける
/**********************************************************************
Main
**********************************************************************/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "publisher");
    ros::NodeHandle n;
    ros::Publisher pub_float_msg = n.advertise<std_msgs::Float32>("float_signal", 100);     //float型の変数を送る
    ros::Publisher pub_int_msg = n.advertise<std_msgs::Int32MultiArray>("int_signal", 100); //int型の配列を送る
    ros::Subscriber sub_count = n.subscribe("count_signal", 100, cb_count);                 //送った回数を受け取る

    ros::Rate loop_rate(1 / CTRL_PERIOD);

    msg_int.data.resize(2);
    msg_float.data = 1.0f;
    msg_int.data[0] = 0;
    msg_int.data[1] = 1;

    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO("%s", letter.c_str()); //string型を表示させるときにはc_str()をつける

        msg_float.data = msg_float.data + 0.01; //値が変わるようにしている

        pub_float_msg.publish(msg_float); //送る
        pub_int_msg.publish(msg_int);     //送る

        loop_rate.sleep();
    }

    return 0;
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_count(const std_msgs::String::ConstPtr &msg)
{
    letter = msg->data;
}