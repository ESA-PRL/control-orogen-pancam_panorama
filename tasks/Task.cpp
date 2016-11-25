#include "Task.hpp"

using namespace pancam_panorama;

Task::Task(std::string const& name):
    TaskBase(name),
    enable(false),
    save_frame(false),
    left_frame_saved(false),
    right_frame_saved(false),
    position_index(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine):
    TaskBase(name, engine),
    enable(false),
    save_frame(false),
    left_frame_saved(false),
    right_frame_saved(false),
    position_index(0)
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
    
    tilt_angle = _positionTilt.get();
    position_left = _positionLeft.get();
    position_center = _positionCenter.get();
    position_right = _positionRight.get();
    position_error_margin = _positionErrorMargin.get();
    position_goal = position_order[position_index];
    frame_delay_um.microseconds = _frameDelayTimeMs.get() * 1000LL;
    
    pan_target_set = false;
    tilt_target_set = false;
    
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
        // Got joystick data
        // The reading already sets the variable
        if(joystick_command.buttonValue[Y])
        {
            // Toggle the PanCam panorama mode with Y button
            enable = !enable;
            
            // Reset the target flags
            if(!enable)
            {
                pan_target_set = false;
                tilt_target_set = false;
            }
        }
    }
    
    if(_pan_angle_in.read(pan_angle_in) == RTT::NewData && enable)
    {
        // Got new data on the pan position
        // Check if the pan and tilt angles has arrived to requested positions
        if(fabs(pan_angle_in - (*position_goal)) < position_error_margin && fabs(tilt_angle_in - tilt_angle) < position_error_margin)
        {
            if(!save_frame)
            {
                // The pan and tilt have reached their destination, now the cameras can take a picture set
                goal_arrival_time = base::Time::now();
                save_frame = true;
            }
            else if(left_frame_saved && right_frame_saved)
            {
                // Pictures have been taken, proceed to the next position
                // Loop back to 0 instead of going to 4
                position_index = (position_index + 1) % 4;
                
                // Set the pointer to the next goal
                position_goal = position_order[position_index];
                
                // Send signal to move to the next position to the PTU
                _pan_angle_out.write(*position_goal);
                // Target flags do not need to be reset as the command is sent out immediately
                
                // Reset flags
                save_frame = false;
                left_frame_saved = false;
                right_frame_saved = false;
            }
        }
        else if(!pan_target_set)
        {
            _pan_angle_out.write(*position_goal);
            pan_target_set = true;
        }
    }
    
    if(_tilt_angle_in.read(tilt_angle_in) == RTT::NewData && enable)
    {
        // Got new data on the tilt position
        if(fabs(tilt_angle_in - tilt_angle) > position_error_margin && !tilt_target_set)
        {
            // Send the signal to the PTU until it reaches the requested tilt position
            _tilt_angle_out.write(tilt_angle);
            tilt_target_set = true;
        }
    }
    
    if(_left_frame_in.read(left_frame) == RTT::NewData && enable && save_frame)
    {
        if(left_frame->time > goal_arrival_time + frame_delay_um)
        {
            // Save the timestamped PTU angles (this only needs to be done as images come in pairs)
            ptu_timestamped_angles.time = left_frame->time;
            ptu_timestamped_angles.angle_pan = pan_angle_in;
            ptu_timestamped_angles.angle_tilt = tilt_angle_in;
            _ptu_angles.write(ptu_timestamped_angles);
            
            _left_frame_out.write(left_frame);
            left_frame_saved = true;
        }
    }
    
    if(_right_frame_in.read(right_frame) == RTT::NewData && enable && save_frame)
    {
        // Frames always come in pairs, no frame synchronisation is required
        if(right_frame->time > goal_arrival_time + frame_delay_um)
        {
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

