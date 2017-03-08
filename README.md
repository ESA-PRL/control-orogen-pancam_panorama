# PanCam panorama

## Overview

This package automatically takes pictures with the PanCam in different positions. When the routine is enabled the module instructs the PanCam to a predefined position, waits until it reaches this position and saves the next incoming image frames to make sure the images are not blurred because of the PTU movement.

The number of pan/tilt positions is arbitrary and defined via the parameters.

**Authors: Karl Kangur  
Contact: Martin Azkarate  
Affiliation: Automation and Robotics Laboratories, ESTEC, ESA**


## Installation

### Dependencies

This package does not depend on any other packages.

### Building

In order to install, clone the latest version from this repository into your workspace under `control/orogen/pancam_panorama`, add the following line to `autoproj/manifest` under `layout:`

    - control/orogen/pancam_panorama

Execute the following to build the package:

    $ autoproj build


## Basic Usage

### motion_translator

#### Inputs

* **`pan_angle_in`** (/double)

Feedback from the PTU pan angle, for example for the [ptu_directedperception](https://github.com/rock-drivers/drivers-orogen-ptu_directedperception) package.

* **`tilt_angle_in`** (/double)

Feedback from the PTU tilt angle, for example for the [ptu_directedperception](https://github.com/rock-drivers/drivers-orogen-ptu_directedperception) package.

* **`left_frame_in`** (base::samples::frame::Frame)

Input of the left camera of the PTU.

* **`right_frame_in`** (base::samples::frame::Frame)

Input of the right camera of the PTU.

#### Outputs

* **`pan_angle_out`** (/double)

Pan motion command directed to a pan-tilt unit, for example for the [ptu_directedperception](https://github.com/rock-drivers/drivers-orogen-ptu_directedperception) package.

* **`tilt_angle_out`** (/double)

Tilt motion command directed to a pan-tilt unit, for example for the [ptu_directedperception](https://github.com/rock-drivers/drivers-orogen-ptu_directedperception) package.

* **`left_frame_out`** (base::samples::frame::Frame)

Output of the left camera of the PTU.

* **`right_frame_out`** (base::samples::frame::Frame)

Output of the right camera of the PTU.

* **`pan_angle_out_degrees`** (/double)

PTU pan angle in degrees.

* **`tilt_angle_out_degrees`** (/double)

PTU tilt angle in degrees.

#### Parameters

* **`cameraPositions`** (/std/vector\</base/Vector2d\>)

Camera pan/tilt orientations for pictures, in degrees.

In the `.yml` configuration file this parameter looks like the following:

    cameraPositions:
      - data:
        - -30.0
        - 20.0
      - data:
        - 0.0
        - 20.0
      - data:
        - 30.0
        - 20.0
      - data:
        - 0.0
        - 20.0

* **`positionErrorMargin`** (/double)

PanCam pan position error margin in degrees.

* **`frameDelayTimeMs`** (/double)

Minimum delay in milliseconds before saving a frame after the PanCam reaches its goal position.

