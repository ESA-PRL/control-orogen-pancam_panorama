#include "Task.hpp"

using namespace pancam_panorama;

Task::Task(std::string const& name):
    TaskBase(name),
    enable(false),
    save_frame(false),
    left_frame_saved(false),
    right_frame_saved(false),
    position_index(0),
    tilt_angle(_positionTilt.get()),
    position_left(_positionLeft.get()),
    position_center(_positionCenter.get()),
    position_right(_positionRight.get()),
    position_goal(position_order[position_index])
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine):
    TaskBase(name, engine),
    enable(false),
    save_frame(false),
    left_frame_saved(false),
    right_frame_saved(false),
    position_index(0),
    tilt_angle(_positionTilt.get()),
    position_left(_positionLeft.get()),
    position_center(_positionCenter.get()),
    position_right(_positionRight.get()),
    position_goal(position_order[position_index])
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
        // Check if the pan angle has arrived to predefined position
        if(fabs(pan_angle_in - (*position_goal)) < 0.1f && fabs(tilt_angle_in - tilt_angle) < 0.1f)
        {
            // TODO perhaps wait some time here to make sure the PTU is at rest when the picture is taken
            if(!save_frame)
            {
                // TODO add timeout to avoid blurry pictures, before or after reaching the goal position?
                save_frame = true;
            }
            else if(left_frame_saved && right_frame_saved)
            {
                // Move to the next position, loop back to 0 instead of going to 4
                position_index = (position_index + 1) % 4;
                
                // Set the pointer to the next goal
                position_goal = position_order[position_index];
                
                // Send signal to move to the next position to the PTU
                _pan_angle_out.write(*position_goal);
                
                // Reset flags
                save_frame = false;
                left_frame_saved = false;
                right_frame_saved = false;
            }
        }
        else
        {
            // Continously send the signal to the PTU until it reaches the position
            // Without this the task will not properly start
            _pan_angle_out.write(*position_goal);
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
    
    if(_left_frame_in.read(left_frame) == RTT::NewData)
    {
        // A new left frame arrived
        if(save_frame)
        {
            // TODO add the position in the file name or meta data of the picture
            _left_frame_out.write(left_frame);
            left_frame_saved = true;
        }
    }
    
    if(_right_frame_in.read(right_frame) == RTT::NewData)
    {
        // A new right frame arrived
        if(save_frame)
        {
            // TODO add the position in the file name or meta data of the picture
            _right_frame_out.write(right_frame);
            right_frame_saved = true;
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

