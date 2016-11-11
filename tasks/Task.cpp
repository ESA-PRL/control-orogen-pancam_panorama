#include "Task.hpp"

using namespace pancam_panorama;

Task::Task(std::string const& name):
    TaskBase(name),
    enable(_enable.get()),
    position_index(0),
    position_left(_positionLeft.get()),
    position_center(_positionCenter.get()),
    position_right(_positionRight.get())
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine):
    TaskBase(name, engine),
    enable(_enable.get()),
    position_index(0),
    position_left(_positionLeft.get()),
    position_center(_positionCenter.get()),
    position_right(_positionRight.get())
{
}

Task::~Task()
{
}

bool Task::configureHook()
{
    if(!TaskBase::configureHook())
    {
        return false;
    }
    return true;
}
bool Task::startHook()
{
    if(!TaskBase::startHook())
    {
        return false;
    }
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();
    
    if(_pan_angle_in.read(pan_angle_in) == RTT::NewData && enable)
    {
        // Got new data on the pan position
        //std::cout << "Pan angle: " << pan_angle_in << std::endl;
        const double * position_goal = position_order[position_index];
        
        // Check if the pan angle has arrived to predefined position
        if(fabs(pan_angle_in - (*position_goal)) < 0.1f)
        {
            // TODO perhaps wait some time here to make sure the PTU is at rest when the picture is taken
            // TODO trigger the cameras on the PTU here
            
            // TODO wait for the picture confirmation (how to do that?)
            
            // Move to the next position
            position_index = (position_index + 1) % 4;
            
            // Send the signal to the PTU
            _pan_angle_out.write(*(position_order[position_index]));
        }
    }
    
    if(_tilt_angle_in.read(tilt_angle_in) == RTT::NewData)
    {
        // Got new data on the tilt position
        //std::cout << "Tilt angle: " << tilt_angle_in << std::endl;
    }
}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}

