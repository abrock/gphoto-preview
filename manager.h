#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <unistd.h>
#include <string>

#include <gphoto2pp/helper_camera_wrapper.hpp>
#include <gphoto2pp/camera_wrapper.hpp>
#include <gphoto2pp/camera_file_wrapper.hpp>
#include <gphoto2pp/camera_capture_type_wrapper.hpp>
#include <gphoto2pp/exceptions.hpp>

#include <gphoto2pp/helper_gphoto2.hpp>
#include <gphoto2pp/camera_list_wrapper.hpp>
#include <gphoto2pp/window_widget.hpp>
#include <gphoto2pp/toggle_widget.hpp>
#include <gphoto2pp/camera_event_type_wrapper.hpp>

#include <opencv2/highgui.hpp>

#include <ParallelTime/paralleltime.h>

class Manager {
    gphoto2pp::CameraWrapper * cameraWrapper;

public:
    Manager() {}

    void run();

    void waitConnect();

    bool connect();

    void run_difference();
};

#endif // MANAGER_H
