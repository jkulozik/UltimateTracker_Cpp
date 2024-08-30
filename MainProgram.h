#ifndef MAINPROGRAM_H
#define MAINPROGRAM_H

#include <openvr.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <iostream>
#include <chrono>

void preciseWait(double microseconds);

struct DeviceInfo {
    std::string serial;
    std::string model;
};

class MainProgram {
public:
    MainProgram();
    ~MainProgram();
    bool Initialize();
    void GetTrackerData();
    void PrintDeviceData();
    void PrintDiscoveredObjects();
    void Plot3DData(const vr::HmdMatrix34_t& matrix);
    void RenderScene();
    void InitCSV();
    void LogDataCSV(int index, std::chrono::system_clock::time_point currentTime, const vr::HmdMatrix34_t& matrix);
    void CloseCSV();
    bool CheckKeyPressed();
    void ReceiveOptiTrackData();

private:
    vr::IVRSystem* vr_system;
    vr::TrackedDevicePose_t tracked_device_pose[vr::k_unMaxTrackedDeviceCount];
    std::map<std::string, std::vector<DeviceInfo>> object_names;
    void PrintPositionalData(const vr::HmdMatrix34_t& matrix);
    void PopulateDeviceInfo();
    std::ofstream file;
    std::chrono::system_clock::time_point currentTime;
};

#endif // MAINPROGRAM_H
