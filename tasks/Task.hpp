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
        
        // Tilt angle at which the PanCam should be when taking pictures
        const double tilt_angle;
        
        const double position_left;
        const double position_center;
        const double position_right;
        
        // Order in which the positions are traversed
        int position_index;
        const double * position_order[4] = {&position_center, &position_left, &position_center, &position_right};

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

