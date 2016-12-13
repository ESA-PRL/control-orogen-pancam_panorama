#ifndef pancam_panorama_TYPES_HPP
#define pancam_panorama_TYPES_HPP

#include <base/Time.hpp>

namespace pancam_panorama {
    // Timestamped PTU tilt and pan angles structure
    struct TimestampedOrientationPTU
    {
        base::Time time;
        // Angles are read direcrtly from the PTU as step values
        double angle_tilt;
        double angle_pan;
        // Angles converted in degrees
        double angle_tilt_degrees;
        double angle_pan_degrees;
    };
}

#endif

