/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef PANCAM_PANORAMA_TASK_TASK_HPP
#define PANCAM_PANORAMA_TASK_TASK_HPP

#include "pancam_panorama/TaskBase.hpp"

namespace pancam_panorama
{
    class Task : public TaskBase
    {
	friend class TaskBase;
    protected:
        // Enable the automatic panorama capture
        bool enable;
        
        // PTU pan and tilt angles from the PTU module (for feedback)
        double pan_angle_in;
        double tilt_angle_in;
        
        int position_index;
        const double position_left;
        const double position_center;
        const double position_right;
        // Order in which the positions are traversed
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

