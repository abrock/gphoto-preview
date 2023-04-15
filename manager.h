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
    std::shared_ptr<gphoto2pp::CameraWrapper> cameraWrapper = nullptr;

public:
    std::string model;
    std::string port;

    Manager() {}

    Manager(std::string const& _model, std::string const& _port);

    void runPreview();

    void waitConnect();

    bool connect();

    void run_difference();

    void capture_and_download(const std::string &fn);

    static std::vector<std::shared_ptr<Manager> > listCams();

    void detectAndSelfAssign();
};

bool operator < (Manager const& a, Manager const& b);

bool operator < (std::shared_ptr<gphoto2pp::CameraWrapper> const& a, std::shared_ptr<gphoto2pp::CameraWrapper> const& b);


#endif // MANAGER_H
