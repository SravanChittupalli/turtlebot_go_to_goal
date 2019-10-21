#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
using namespace std;

void move(double speed , double distance , bool Forward);
void rotate(double angular_speed , double angle , bool Clockwise);
double deg2rad(double angle);
void callback(const turtlesim::Pose::ConstPtr &pose_message);
void go_to_goal( turtlesim::Pose end_loc , double dist_tolerence);
ros::Publisher Topic;
ros::Subscriber Location;
turtlesim::Pose pose_message_recieve;



int main(int argc , char **argv)
{
	ros::init(argc , argv , "turtlesim_cleaner_publisher");

	ros::NodeHandle n;
	double speed,angular_speed , x-ord , y-ord;
	double distance,angle;
	bool Forward,Clockwise;


	Topic = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel" , 1000);
	Location = n.subscribe("/turtle1/pose" , 1000 , callback);
	ros::Rate loop_rate(0.5);


	// cout<<"enter speed: ";
	// cin>>speed;
	// cout<<"enter distance: ";
	// cin>>distance;
	// cout<<"forward?: ";
	// cin>>Forward;

	// move(speed, distance, Forward);

	// cout<<"enter angular_speed: ";
	// cin>>angular_speed;
	// cout<<"enter angle: ";
	// cin>>angle;
	// cout<<"clockwise?: ";
	// cin>>Clockwise;
	// rotate(deg2rad(angular_speed) , deg2rad(angle) , Clockwise);
	
	turtlesim::Pose pose;
	
	cout<<"enter x co-ordinate: ";
	cin>> x-ord;
	cout<<"enter y co-ordinate: ";
	cin>> y-ord;
	
	pose.x = x-ord;
	pose.y = y-ord;
	pose.theta = 0;
	go_to_goal(pose , 0.0001);

	//loop.sleep();

	ros::Rate loop(100);
	loop.sleep();
	//spiralClean();

	ros::spin();

	return 0;
}


void move(double speed , double distance , bool Forward )
{
	geometry_msgs::Twist vel_msg;

	if(Forward)
	{
		vel_msg.linear.x = abs(speed);
	}
	else
	{
		vel_msg.linear.x = -abs(speed);
	}

	vel_msg.linear.y = 0;
	vel_msg.linear.z = 0;

	vel_msg.angular.x = 0;
	vel_msg.angular.y = 0;
	vel_msg.angular.z = 0;

	double t0 = ros::Time::now().toSec();
	double current_distance = 0.0;
	ros::Rate loop_rate(1000);


	do
	{
		Topic.publish(vel_msg);
		double t1 = ros::Time::now().toSec();
		current_distance = speed * (t1-t0);
		ros::spinOnce();
		loop_rate.sleep();
		//cout<<(t1-t0)<<", "<<current_distance <<", "<<distance<<endl;
	}while(current_distance < distance);

	vel_msg.linear.x = 0;
	Topic.publish(vel_msg);
	
}

void rotate(double angular_speed , double angle , bool Clockwise)
{
	geometry_msgs::Twist ang_msg;

	ang_msg.linear.x = 0;
	ang_msg.linear.y = 0;
	ang_msg.linear.z = 0;

	ang_msg.angular.x = 0;
	ang_msg.angular.y = 0;

	if(Clockwise)
	{
		ang_msg.angular.z = -abs(angular_speed);	
	}
	else
	{
		ang_msg.angular.z = abs(angular_speed);
	}

	double t0 = ros::Time::now().toSec();
	double current_angle = 0;
	ros::Rate loop_rate(1000);
	do
	{
		Topic.publish(ang_msg);
		double t1 = ros::Time::now().toSec();
		current_angle = (t1 - t0) * angular_speed;
	}while(angle > current_angle);

	ang_msg.angular.z=0;
	Topic.publish(ang_msg);
}

double deg2rad(double angle)
{
	return (3.1415926535897 / 180.0)*angle;
}

void callback(const turtlesim::Pose::ConstPtr &pose_message)
{
	pose_message_recieve.x = pose_message->x;
	pose_message_recieve.y = pose_message->y;
	pose_message_recieve.theta = pose_message->theta;
	//printf("%lf , %lf , %lf \n",pose_message_recieve.x , pose_message_recieve.y , pose_message_recieve.theta );
}

void go_to_goal( turtlesim::Pose end_loc , double dist_tolerence)
{
	geometry_msgs::Twist vel;
	ros::Rate loop_rate(100);
	do
	{
		printf("%d\n", 1 );
		vel.linear.x = 1 * sqrt(pow((pose_message_recieve.x - end_loc.x) , 2 ) + pow((pose_message_recieve.y - end_loc.y) , 2));
		vel.linear.y = 0;
		vel.linear.z = 0;

		vel.angular.x = 0;
		vel.angular.y = 0;
		vel.angular.z = 4* (atan2((end_loc.y - pose_message_recieve.y),(end_loc.x - pose_message_recieve.x)) - pose_message_recieve.theta);
		printf("%lf , %lf \n",vel.linear.x ,vel.angular.z );
		Topic.publish(vel);

		ros::spinOnce();
		loop_rate.sleep();
	}while(sqrt(pow((pose_message_recieve.x - end_loc.x) , 2 ) + pow((pose_message_recieve.y - end_loc.y) , 2)) > dist_tolerence);

	cout<<"end move goal"<<endl;
	vel.linear.x =0;
	vel.angular.z = 0;
	Topic.publish(vel);
}
