#include "multi-view-manager.h"

#include <chrono>
#include <thread>

MultiViewManager::MultiViewManager() {
    cams = Manager::listCams();
    std::cout << "List of cams: " << std::endl;
    for (size_t ii = 0; ii < cams.size(); ++ii) {
        std::cout << "#" << ii << ": model " << cams[ii]->model << ", port " << cams[ii]->port << std::endl;
    }
}

void MultiViewManager::captureAll() {
    if (cams.empty()) {
        std::cout << "Not capturing since no cams were found" << std::endl;
        return;
    }
    std::chrono::microseconds duration(int(std::round(1000*delay_ms)));
    std::cout << "Capturing with delay " << delay_ms << std::endl;
    std::vector<std::thread*> threads;
    threads.push_back(new std::thread(&Manager::capture_and_download, *cams[0], "multiview-c0.arw"));
    std::this_thread::sleep_for(duration);
    for (size_t ii = 1; ii < cams.size(); ++ii) {
        threads.push_back(new std::thread(&Manager::capture_and_download, *cams[ii], "multiview-c" + std::to_string(ii) + ".arw"));
    }
    std::cout << "Started all exposures" << std::endl;

    for (std::thread* t : threads) {
        t->join();
    }
    std::cout << "Finished all exposures" << std::endl;
}

void MultiViewManager::setDelay(const QString &str) {
    std::stringstream _str(str.toStdString());
    _str >> delay_ms;
}

MultiViewManager::~MultiViewManager() {}
