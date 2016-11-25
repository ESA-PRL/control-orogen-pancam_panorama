#ifndef pancam_panorama_TYPES_HPP
#define pancam_panorama_TYPES_HPP

#include <base/Time.hpp>

namespace pancam_panorama {
    // Timestamped PTU tilt and pan angles structure
    struct TimestampedOrientationPTU
    {
        base::Time time;
        // Angles are read direcrtly from the PTU, so unit conversion might be
        // needed as PTU units might not correspond to actual angles in radians
        // or degrees
        double angle_tilt;
        double angle_pan;
    };
}

#endif

