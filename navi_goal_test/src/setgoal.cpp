#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//define POSE structure
typedef struct _POSE
{
  double pos_x;
  double pos_y;
  double pos_z;
  double ori_x;
  double ori_y;
  double ori_z;
  double ori_w;
} POSE;

POSE pose1 = {1.557, 0.386, 0.0,  0.0, 0.0, -0.697, 0.717};
POSE pose2 = {1.536, -0.451, 0.0,  0.0, 0.0, -0.714, 0.7};
POSE pose3 = {0.992, -0.954, 0.0,  0.0, 0.0, -0.716, 0.698};
POSE pose4 = {0.954, -1.790, 0.0,  0.0, 0.0, -0.016, 1};
POSE pose5 = {1.541, -2.914, 0.0,  0.0, 0.0, 1.0, 0.01};

bool go_next_pose = false;
bool error = false;


void setGoal(POSE pose){
  
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = pose.pos_x;
  goal.target_pose.pose.position.y = pose.pos_y;
  goal.target_pose.pose.position.z = pose.pos_z;
  goal.target_pose.pose.orientation.x = pose.ori_x;
  goal.target_pose.pose.orientation.y = pose.ori_y;
  goal.target_pose.pose.orientation.z = pose.ori_z;
  goal.target_pose.pose.orientation.w = pose.ori_w;

  ROS_INFO("Sending goal");

  ac.sendGoal(goal);
  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Reached pose.");
    go_next_pose = true;
  }
  
  else
    ROS_INFO("The base failed to reach the pose for some reason");
    error = true;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "navi_goal_test");

  setGoal(pose1);

  if(go_next_pose == true)
  {
    setGoal(pose2);
    if(go_next_pose == true)
    {
      setGoal(pose3);
      if(go_next_pose == true)
      {
        setGoal(pose4);
        if(go_next_pose == true)
        {
          setGoal(pose5);
          if(error == true)
            return 0;
        }
        if(error == true)
          return 0;
      }
      if(error == true)
        return 0;
    }
    if(error == true)
      return 0;
  }
  if(error == true)
    return 0;

  return 0;
}
