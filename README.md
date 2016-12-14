# PanCam panorama

## Overview

This package automatically takes pictures with the PanCam in 3 different positions. When the routine is enabled the module instructs the PanCam to a predefined position, waits until it reaches this position and saves the next incoming image frames to make sure the images are not blurred because of the PTU movement. Then in proceeds to the next position and loops over the 3 positions.

The pan positions are defined via the parameters. The positions are named left, center and right.

For logging the package has a special structure that contains the timestamped left and right pictures with the PTU angles.

During the routine the PanCam loops trough the positions in the following way: center, left, center, right.

**Authors: Karl Kangur  
Contact: Martin Azkarate  
Affiliation: Automation and Robotics Laboratories, ESTEC, ESA**


## Installation

### Dependencies

This package depends on the following packages:

* [drivers/controldev](https://github.com/rock-drivers/drivers-controldev)
* [drivers/orogen/controldev](https://github.com/rock-drivers/drivers-orogen-controldev)

### Building

In order to install, clone the latest version from this repository into your workspace under `control/orogen/motion_translator`, add the following line to `autoproj/manifest` under `layout:`

    - control/orogen/pancam_panorama

Execute the following to build the package:

    $ autoproj build


## Basic Usage

### motion_translator

#### Inputs

* **`raw_command`** (/controldev/RawCommand)

Raw messages coming from a joystick or gamepad, used to toggle the panorama mode.

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

* **`frame`** (/pancam_panorama/PanCamTimestampedFrame)

Output of a structure containing both images as well as the PTU pan and tilt angles with a timestamp, useful for logging.

#### Parameters

* **`positionTilt`** (/double)

PanCam tilt position for all positions in degrees.

* **`positionLeft`** (/double)

PanCam pan position left in degrees.

* **`positionCenter`** (/double)

PanCam pan position center in degrees.

* **`positionRight`** (/double)

PanCam pan position right in degrees.

* **`positionErrorMargin`** (/double)

PanCam pan position error margin in degrees.

* **`frameDelayTimeMs`** (/double)

Minimum delay in milliseconds before saving a frame after the PanCam reaches its goal position.

* **`resolutionPerPosition`** (/double)

Axis resolution per position moved (in seconds/arc), used to transform degrees to motor steps, see the datasheet for the value.

