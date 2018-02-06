#ifndef PANCAM_PANORAMA_TASK_TASK_HPP
#define PANCAM_PANORAMA_TASK_TASK_HPP

#include "pancam_panorama/TaskBase.hpp"
//#include "controldev/JoystickTaskBase.hpp"

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
    
    // Pan-tilt axis names
    enum Axis
    {
        PAN = 0,
        TILT = 1
    };
    
    class Task : public TaskBase
    {
	friend class TaskBase;
    protected:
        static constexpr double DEG2RAD = 3.14159/180;
        // For tilt angles they need to be multiplied by 4 to get them in proper units because of the gearing
        static constexpr double TILT_MULTIPLIER = 4;
        
        // PTU pan and tilt angles from the PTU module (inputs for feedback)
        double pan_angle_in;
        double tilt_angle_in;
        // The tilt angle needs a multiplier, the value is stored in tilt_angle_temp
        // as tilt_angle_in in keeps getting overwritten for some reason...
        double tilt_angle_temp;
        
        // Position error margin for the pan and tilt positions
        double position_error_margin;
        
        // Vector containing the pan and tilt position for every picture
        std::vector<base::Vector2d> camera_positions;
        // Current position index
        unsigned int position_index;
        double pan_angle_goal;
        double tilt_angle_goal;
        
        base::Time goal_arrival_time;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> left_frame, right_frame;
        
        base::Time frame_delay_um;
        bool save_frame;
        bool left_frame_saved;
        bool right_frame_saved;

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

