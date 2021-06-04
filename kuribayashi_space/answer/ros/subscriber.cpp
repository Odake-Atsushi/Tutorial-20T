/*********************************
publiserとsubscriberのプログラムの練習1
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
inline void cb_msg_float(const std_msgs::Float32::ConstPtr &msg);
inline void cb_msg_int(const std_msgs::Int32MultiArray::ConstPtr &msg);

/**********************************************************************
Declare variables
**********************************************************************/
std_msgs::String count;

float a = 0.0f;
std::vector<int> c = {0, 0}; //65行目のような代入方法を用いる時は，配列は普通の配列ではなく動的なvector配列を用いる
int b[2] = {0, 0};           //66行目以降のような代入方法を用いる場合

/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle n;
    ros::Publisher pub_msg_int = n.advertise<std_msgs::String>("count_signal", 100);
    ros::Subscriber sub_msg_float = n.subscribe("float_signal", 100, cb_msg_float);
    ros::Subscriber sub_msg_int = n.subscribe("int_signal", 100, cb_msg_int);

    ros::Rate loop_rate(1 / CTRL_PERIOD);
    count.data = "hello"; //string型に代入するときは" "で囲む

    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO("%d, %d,%d, %d, %f", c[0], c[1], b[0], b[1], a);

        pub_msg_int.publish(count);
        loop_rate.sleep();
    }
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_msg_float(const std_msgs::Float32::ConstPtr &msg)
{
    a = msg->data;
}
inline void cb_msg_int(const std_msgs::Int32MultiArray::ConstPtr &msg)
{
    //配列の一括代入ができる
    c = msg->data;

    //for文を使って要素ずつ代入でも可
    for (int i = 0; i < 2; i++)
    {
        b[i] = msg->data[i];
    }
}