/*********************************
プログラムがコンパイル通り，うまく動くようにしてください．
プログラム内容は，
１，float型の配列とint型を送る，
２，String型とint型の配列をを受け取り，表示させるプログラムです．
rostopic pubやrostopic echoを使ってやり取りを確認したり，このプログラムに値を送ったりしてみましょう．
**********************************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void cb_msg_string(const std_msgs::String &msg);
inline void cb_msg_int_array(const std_msgs::Int32MultiArray::ConstPtr &msg);
　　
    /**********************************************************************
Declare variables
**********************************************************************/
    std::Float32MultiArray a;
std::Int32 b;

std::String letter;
int c[2] = {0, 0};

/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "finding_mistakes");
    ros::NodeHandle n;
    ros::Publisher pub_msg_float = n.advertise<std_msgs::Float32MultiArray>("float_signal", 100);
    ros::Publisher pub_msg_int = n.advertise<std_msgs::Int32>("int_signal", 100);
    ros::Subscriber sub_msg_string = n.subscribe("string_signal", 100, cb_msg_string);
    ros::Subscriber sub_msg_int_array = n.subscribe("int_array_signal", 100, cb_msg_int_array);

    ros::Rate loop_rate(1 / CTRL_PERIOD);
    a = 3.14159265;
    b = 5;

    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO("%d, %s", c, letter);

        pub_msg_float.publish(a);
        int_signal.publish(b);
        loop_rate.sleep();
    }
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_msg_string(const std_msgs::String &msg)
{
    letter = msg.data;
}
inline void cb_msg_int_array(const std_msgs::Int32MultiArray::ConstPtr &msg)
{
    c = msg.data;
}
