#include "Task.hpp"

using namespace pancam_panorama;

Task::Task(std::string const& name):
    TaskBase(name),
    enable(false),
    position_index(0),
    tilt_angle(_positionTilt.get()),
    position_left(_positionLeft.get()),
    position_center(_positionCenter.get()),
    position_right(_positionRight.get())
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine):
    TaskBase(name, engine),
    enable(false),
    position_index(0),
    tilt_angle(_positionTilt.get()),
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
    
    if(_raw_command.read(joystick_command) == RTT::NewData)
    {
        // The reading already sets the variable
        if(joystick_command.buttonValue[Y])
        {
            // Toggle the PanCam panorama mode with Y button
            enable = !enable;
        }
    }
    
    if(_pan_angle_in.read(pan_angle_in) == RTT::NewData && enable)
    {
        // Got new data on the pan position
        const double * position_goal = position_order[position_index];
        
        // Check if the pan angle has arrived to predefined position
        if(fabs(pan_angle_in - (*position_goal)) < 0.1f && fabs(tilt_angle_in - tilt_angle) < 0.1f)
        {
            // TODO perhaps wait some time here to make sure the PTU is at rest when the picture is taken
            // TODO trigger the cameras on the PTU here
            
            // TODO wait for the picture confirmation (how to do that?)
            
            // Move to the next position, loop back to 0 instead of going to 4
            position_index = (position_index + 1) % 4;
            
            // Send the signal to the PTU
            _pan_angle_out.write(*(position_order[position_index]));
        }
        else
        {
            // Continously send the signal to the PTU until it reaches the position
            _pan_angle_out.write(*(position_order[position_index]));
        }
    }
    
    if(_tilt_angle_in.read(tilt_angle_in) == RTT::NewData && enable)
    {    
        // Got new data on the tilt position
        if(fabs(tilt_angle_in - tilt_angle) > 0.1f)
        {
            // Continously send the signal to the PTU until it reaches the position
            _tilt_angle_out.write(tilt_angle);
        }
    }
}

void Task::errorHook()
{
    TaskBase::errorHook();
}

void Task::stopHook()
{
    TaskBase::stopHook();
    
    // Reset PanCam position index so it would start from the beginning next time
    position_index = 0;
}

void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}

