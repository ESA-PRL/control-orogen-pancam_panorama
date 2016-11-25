#ifndef PANCAM_PANORAMA_TASK_TASK_HPP
#define PANCAM_PANORAMA_TASK_TASK_HPP

#include "pancam_panorama/TaskBase.hpp"
#include "controldev/JoystickTaskBase.hpp"

namespace pancam_panorama
{
    // Button names are in order in which they appear in the vector
    enum ButtonName
    {
        X,
        A,
        B,
        Y,
        LB,
        RB,
        LT,
        RT,
        BACK,
        START,
        LJOY,
        RJOY
    };
    
    class Task : public TaskBase
    {
	friend class TaskBase;
    protected:
        controldev::RawCommand joystick_command;
        
        // Enable the automatic panorama capture (dynamic property)
        bool enable;
        
        // PTU pan and tilt angles from the PTU module (inputs for feedback)
        double pan_angle_in;
        double tilt_angle_in;
        
        // Position error margin for the pan and tilt positions
        double position_error_margin;
        
        // Tilt angle at which the PanCam should be when taking pictures
        double tilt_angle;
        
        double position_left;
        double position_center;
        double position_right;
        
        // Order in which the positions are traversed
        int position_index;
        double * position_order[4] = {&position_center, &position_left, &position_center, &position_right};
        // Pointer to a double to mark the next goal
        double * position_goal;
        bool pan_target_set;
        bool tilt_target_set;
        
        base::Time goal_arrival_time;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> left_frame, right_frame;
        
        base::Time frame_delay_um;
        bool save_frame, left_frame_saved, right_frame_saved;
        
        // Structure for saving the timestamped pan and tilt angles for logging
        TimestampedOrientationPTU ptu_timestamped_angles;

    public:
        Task(std::string const& name = "pancam_panorama::Task");
        Task(std::string const& name, RTT::ExecutionEngine* engine);
	    ~Task();
        bool configureHook();
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();
    };
}

#endif

