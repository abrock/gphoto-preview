#include "manager.h"

#include <gphoto2pp/camera_file_wrapper.hpp>
#include <gphoto2pp/camera_wrapper.hpp>
#include <gphoto2pp/camera_capture_type_wrapper.hpp>
#include <gphoto2pp/helper_camera_wrapper.hpp>

Manager::Manager(const std::string &_model, const std::string &_port) : model(_model), port(_port) {}

void Manager::runPreview() {
    ParallelTime last_shot;
    sleep(1);
    for (size_t ii = 0;; ++ii) {
        try {
            if (last_shot.realTime() > .3) {
                std::cout << "Trying to capture preview image #" << ii << std::endl;
                gphoto2pp::CameraFileWrapper file = cameraWrapper->capturePreview();
                file.save("test.jpg");
                cv::Mat img = cv::imread("test.jpg");
                if (img.empty()) {
                    img.create(100, 100, CV_8UC3);
                }
                cv::imshow("img", img);
                last_shot.start();
            }
            cv::waitKey(0);
            std::cout << "done." << std::endl;
        }  catch (...) {
            if (nullptr != cameraWrapper) {
                cameraWrapper.reset();
                cameraWrapper = nullptr;
            }
            waitConnect();
        }
    }
}

void Manager::waitConnect() {
    while(true) {
        if (connect()) {
            return;
        }
        sleep(5);
    }
}

bool Manager::connect() {
    try {
        cameraWrapper = std::shared_ptr<gphoto2pp::CameraWrapper>(new gphoto2pp::CameraWrapper(model, port));
        std::cout << "#####################" << std::endl;
        std::cout << "# connect to camera #" << std::endl;
        std::cout << "#####################" << std::endl;
        std::cout << "#############################" << std::endl;
        std::cout << "# Summary: " << std::endl
                  << "#############################" << std::endl
                  << cameraWrapper->getSummary() << std::endl;
        return true;
    }
    catch (const gphoto2pp::exceptions::NoCameraFoundError &e) {
        std::cout << "GPhoto couldn't detect any cameras connected to the computer" << std::endl;
        std::cout << "Exception Message: " << e.what() << std::endl;
        return false;
    }
    catch (gphoto2pp::exceptions::gphoto2_exception& e) {
        if (nullptr != cameraWrapper) {
            cameraWrapper.reset();
            cameraWrapper = nullptr;
        }
        std::cout << "GPhoto2 Exception Code: " << e.getResultCode() << std::endl;
        std::cout << "GPhoto2 Exception Message: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Manager::run_difference() {
    ParallelTime last_shot;
    sleep(1);
    cv::Mat reference;
    cv::Mat img;
    for (size_t ii = 0;; ++ii) {
        try {
            if (last_shot.realTime() > .3) {
                std::cout << "Trying to capture preview image #" << ii << std::endl;
                gphoto2pp::CameraFileWrapper file = cameraWrapper->capturePreview();
                file.save("test.jpg");
                img = cv::imread("test.jpg");
                if (img.empty()) {
                    img.create(100, 100, CV_8UC3);
                }
                cv::imshow("img", img);
                last_shot.start();
            }
            char const key = cv::waitKey(300);
            switch(key) {
            case 'd': reference = img.clone(); std::cout << "Copied image to reference" << std::endl; break;
            case 'n': reference = cv::Mat(); break;
            }
            if (reference.size() == img.size()) {
                cv::Mat diff;
                cv::absdiff(reference, img, diff);
                double maxdiff = 0;
                double mindiff = 0;
                cv::minMaxIdx(diff, &mindiff, &maxdiff);
                std::cout << "ref  norm: " << cv::norm(reference) << std::endl
                          << "img  norm: " << cv::norm(img) << std::endl
                          << "diff norm: " << cv::norm(diff) << std::endl
                          << "max diff:  " << maxdiff << std::endl;
                if (maxdiff > 0) {
                    diff *= 255.0/maxdiff;
                }
                cv::imshow("diff", diff);
            }

            std::cout << "done." << std::endl;
        }  catch (...) {
            if (nullptr != cameraWrapper) {
                cameraWrapper.reset();
                cameraWrapper = nullptr;
            }
            waitConnect();
        }
    }
}

void Manager::capture_and_download(std::string const& fn) {
    if (nullptr == cameraWrapper) {
        waitConnect();
    }
    gphoto2pp::CameraFileWrapper cameraFile;
    gphoto2pp::helper::capture(*cameraWrapper, cameraFile);
    cameraFile.save(fn);
}

std::vector<std::shared_ptr<Manager> > Manager::listCams() {
    std::vector<std::shared_ptr<Manager> > result;
    try {
        std::cout << "###########################" << std::endl;
        std::cout << "# auto-detect all cameras #" << std::endl;
        std::cout << "###########################" << std::endl;
        auto cameraList = gphoto2pp::autoDetectAll();
        for(int ii = 0; ii < cameraList.count(); ++ii) {
            std::string model = cameraList.getName(ii);
            std::string port = cameraList.getValue(ii);
            std::cout << "model: " << model << " port: " << port << std::endl;
            if (!port.starts_with("disk:") && !model.starts_with("Motorola")) { // Hacky check to avoid "connecting" to some SD card or similar.
                result.push_back(std::shared_ptr<Manager>(new Manager(model, port)));
            }
        }
    }
    catch (const gphoto2pp::exceptions::NoCameraFoundError &e) {
        std::cout << "GPhoto couldn't detect any cameras connected to the computer" << std::endl;
        std::cout << "Exception Message: " << e.what() << std::endl;
    }
    catch (gphoto2pp::exceptions::gphoto2_exception& e) {
        std::cout << "GPhoto2 Exception Code: " << e.getResultCode() << std::endl;
        std::cout << "GPhoto2 Exception Message: " << e.what() << std::endl;
    }
    std::sort(result.begin(), result.end());
    return result;
}

void Manager::detectAndSelfAssign() {
    std::vector<std::shared_ptr<Manager> > cams = listCams();
    if (!cams.empty()) {
        *this = *cams[0];
    }
}

bool operator <(const Manager &a, const Manager &b) {
    if (a.model != b.model) {
        return a.model < b.model;
    }
    return a.port < b.port;
}

bool operator <(const std::shared_ptr<Manager const> &a, const std::shared_ptr<Manager const> &b) {
    return *a < *b;
}
