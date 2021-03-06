#include <ros/ros.h>
#include<std_msgs/String.h>
#include<sstream>

using namespace std;
//string topic[num];

  int me = 0 ;    //当前节点, 为0则是master
  int nodenum = 6;     //一共有多少个节点
  int num = 20;      //每个节点发布topic数量

void topicCallback(const std_msgs::String::ConstPtr &msg)
{  
    ROS_INFO("Node%d [ I heard ( %s ) ]", me, msg->data.c_str());
}

int main(int argc, char **argv)
{

  std::string myname = "node" ;
  string s = to_string(me);
   myname+=s;
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv,myname);

  //实例化句柄，初始化node
  ros::NodeHandle nh;

       //定义发布的频率 
      ros::Rate loop_rate(1.0);

      //发布topic数量
      ros::Publisher pub[num+1];
      std_msgs::String msg[num+1];

  //接收msg数量
  int subnum;
  if(me!=0)  
  { 
      subnum = (nodenum-1)*num;  

          for(int i = 1; i <=num; i++) 
        {
        //创建publisher    
        //std_msgs::String为需要public的类型
        //topic1   topic的名称，1为缓存队列
        std::string temp = "topic_" ;
        string s1 = to_string(me);
        temp+=s1;
        temp+="_";
        string s2 = to_string(i);
        temp+=s2;
        pub[i] = nh.advertise<std_msgs::String>(temp, 1);   
         }
    }
      else 
      {  
                subnum = nodenum*num; 
       }
  ros::Subscriber sub[subnum+1];

//创建Subscriber
int count = 1;
for(int j = 1; j <=nodenum; j++)
{
   for(int i = 1; i <=num; i++) 
 {    
    if(j ==me) break;
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_" ;
    string s3 = to_string(j);
    temp+=s3;
    temp+="_";
    string s4 = to_string(i);
    temp+=s4;
    sub[count] = nh.subscribe(temp, 1, topicCallback);
    count ++;
 }
}

  //循环发布msg
  //ros::ok表示ros没有关就一直执行循环
  while (ros::ok())
  {
    if(me!=0)
    {
          for(int i = 1; i <=num; i++) 
          {
              std::string temp = myname ;
              temp+=" : topic_";
              string s5 = to_string(me);
              temp+=s5;
              temp+="_";
              string s6 = to_string(i);
              temp+=s6;

              msg[i].data = temp;
              ROS_INFO("%s ",  msg[i].data.c_str());     //相当于printf，显示数据
              //以1Hz的频率发布msg
              pub[i].publish(msg[i]);
          }
    }
    ros::spinOnce();
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 

