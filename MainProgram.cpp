#include "MainProgram.h"

void preciseWait(double microseconds) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    double elapsedMicroseconds = 0;
    do {
        QueryPerformanceCounter(&end);
        elapsedMicroseconds = static_cast<double>(end.QuadPart - start.QuadPart) * 1e6 / frequency.QuadPart;
    } while (elapsedMicroseconds < microseconds);
}

MainProgram::MainProgram() : vr_system(nullptr) {} //, window(nullptr)

MainProgram::~MainProgram() {
    if (vr_system) {
        vr::VR_Shutdown();
        vr_system = nullptr;
    }
}

bool MainProgram::Initialize() {
    try {
        vr::EVRInitError eError = vr::VRInitError_None;
        vr_system = vr::VR_Init(&eError, vr::VRApplication_Background);
        if (eError != vr::VRInitError_None) {
            std::cerr << "VR initialization error: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during VR initialization: " << e.what() << std::endl;
        return false;
    }

    std::cout << "VR system initialized." << std::endl;
    InitCSV();
    PopulateDeviceInfo();
    return true;
}

void MainProgram::PopulateDeviceInfo() {
    const int stringSize = 128;
    for (vr::TrackedDeviceIndex_t deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex) {
        if (vr::VRSystem()->IsTrackedDeviceConnected(deviceIndex)) {
            vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(deviceIndex);
            std::string deviceType;
            if (trackedDeviceClass == vr::TrackedDeviceClass_HMD) {
                deviceType = "HMD";
            } else if (trackedDeviceClass == vr::TrackedDeviceClass_Controller) {
                deviceType = "Controller";
            } else if (trackedDeviceClass == vr::TrackedDeviceClass_GenericTracker) {
                deviceType = "Generic Tracker";
            } else if (trackedDeviceClass == vr::TrackedDeviceClass_TrackingReference) {
                deviceType = "Tracking Reference";
            } else {
                continue;
            }
            // get the serial number of the device
            char serial[stringSize];
            vr::VRSystem()->GetStringTrackedDeviceProperty(deviceIndex, vr::Prop_SerialNumber_String, serial, sizeof(serial));
            // get the model number of the device
            char model[stringSize];
            vr::VRSystem()->GetStringTrackedDeviceProperty(deviceIndex, vr::Prop_ModelNumber_String, model, sizeof(model));
            // store the device info
            DeviceInfo info = {serial, model};
            object_names[deviceType].push_back(info);
        }
    }
}

void MainProgram::GetTrackerData() {
    //if (!vr_system) return;

    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, tracked_device_pose, vr::k_unMaxTrackedDeviceCount);
    // get timestamp from system
    uint64_t frameCounter;
    currentTime = std::chrono::system_clock::now();
    for (vr::TrackedDeviceIndex_t deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex) {
        if (vr::VRSystem()->IsTrackedDeviceConnected(deviceIndex)) {
            vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(deviceIndex);
            //only track generic trackers (in our case: ULTIMATE Trackers)
            if (trackedDeviceClass == vr::TrackedDeviceClass_GenericTracker) {
                LogDataCSV(deviceIndex, currentTime, tracked_device_pose[deviceIndex].mDeviceToAbsoluteTracking);
                //PrintPositionalData(tracked_device_pose[deviceIndex].mDeviceToAbsoluteTracking);
            }
        }
    }
}
void MainProgram::InitCSV()
{
    // Open CSV file
    file.open("../../log/Ultimate_tracker_data_120Hz.csv", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }
    // Write header to CSV file
    file << "TrackerIndex,Time,PositionX,PositionY,PositionZ,RotationW,RotationX,RotationY,RotationZ\n";

}
void MainProgram::LogDataCSV(int tracker_index, std::chrono::system_clock::time_point currentTime, const vr::HmdMatrix34_t& matrix)
{
    if (!file.is_open()) {
        std::cerr << "Log file is not open." << std::endl;
        return;
    }
    // Calculate position
    vr::HmdVector3_t position = {
        matrix.m[0][3],
        matrix.m[1][3],
        matrix.m[2][3]
    };

    // Calculate quaternion (rotation)
    vr::HmdQuaternion_t rotation = {
        sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2,
        (matrix.m[2][1] - matrix.m[1][2]) >= 0 ? sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2 : -sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2,
        (matrix.m[0][2] - matrix.m[2][0]) >= 0 ? sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2 : -sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2,
        (matrix.m[1][0] - matrix.m[0][1]) >= 0 ? sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2 : -sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2
    };

    // Convert current time to microseconds since epoch
    auto duration = currentTime.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    // Write data to CSV file
    file << tracker_index << ","
         << microseconds << ","
         << position.v[0] << "," << position.v[1] << "," << position.v[2] << ","
         << rotation.w << "," << rotation.x << "," << rotation.y << "," << rotation.z << "\n";
}
void MainProgram::CloseCSV()
{
    if (file.is_open()) {
        file.close();
    }
}
void MainProgram::PrintPositionalData(const vr::HmdMatrix34_t& matrix) {
    vr::HmdVector3_t position = {
        matrix.m[0][3],
        matrix.m[1][3],
        matrix.m[2][3]
    };

    vr::HmdQuaternion_t rotation = {
        sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2,
        sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2,
        sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2,
        sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2
    };

    rotation.x = copysign(rotation.x, matrix.m[2][1] - matrix.m[1][2]);
    rotation.y = copysign(rotation.y, matrix.m[0][2] - matrix.m[2][0]);
    rotation.z = copysign(rotation.z, matrix.m[1][0] - matrix.m[0][1]);

    std::cout << "Position: (" << position.v[0] << ", " << position.v[1] << ", " << position.v[2] << ")\n";
              //<< "Rotation: (" << rotation.w << ", " << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
    file.flush();
}
void MainProgram::PrintDiscoveredObjects() {
    for (const auto& [device_type, devices] : object_names) {
        std::string plural = device_type;
        if (devices.size() != 1) {
            plural += "s";
        }
        std::cout << "Found " << devices.size() << " " << plural << std::endl;
        for (const auto& device : devices) {
            std::cout << "  " << device.model << " (" << device.serial;
            if (device_type == "Tracking Reference") {
                std::cout << ", Mode " << device.model << ", " << device.model;
            }
            std::cout << ")" << std::endl;
        }
    }
}

