#ifndef IK_SOLVER_DEMO_NODE_H
#define IK_SOLVER_DEMO_NODE_H

#include "ompl_solver/OMPLsolver.h"
#include <ros/ros.h>
#include <ros/package.h>
#include <exotica/Initialiser.h>
#include <sensor_msgs/JointState.h>

class OMPLSolverDemoNode
{
public:
    OMPLSolverDemoNode();
private:
    ros::NodeHandle nh_;
    ros::NodeHandle nhg_;
    std::string resource_path_;
    ros::Publisher jointStatePublisher_;
};

#endif // IK_SOLVER_DEMO_NODE_H