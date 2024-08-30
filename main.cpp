#include "MainProgram.h"
#include <chrono>
#include <conio.h>

/**
 * @brief Main function to run the VR tracking program
 * 
 * This program initializes a VR system, tracks the generic trackers (ULTIMATE Trackers in this case),
 * and logs their positional data at a specified frequency.
 * 
 * @return int Returns 0 on successful execution
 */

int main() {
    MainProgram program;
    //timeBeginPeriod(1); // Set timer resolution to 1 ms
    if (program.Initialize()) {
        std::cout << "VR system initialized." << std::endl;
        program.PrintDiscoveredObjects();
        const int targetFrequency = 5000; // Target frequency in Hz
        const std::chrono::microseconds targetPeriod(1000000 / targetFrequency); // Target period in microseconds
        int iterations = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        std::cout << "Starting capture..." << std::endl;
        std::cout << "Press 'q' to stop capture." << std::endl;
        while (true) { // Main loop
            auto iterationStartTime = std::chrono::high_resolution_clock::now();
            program.GetTrackerData();

            // Check for 'q' key press to quit the program
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'q' || ch == 'Q') {
                    std::cout << "Quitting program..." << std::endl;
                    break;
                }
            }

            auto iterationEndTime = std::chrono::high_resolution_clock::now();
            auto iterationElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(iterationEndTime - iterationStartTime);
            auto sleepTime = targetPeriod - iterationElapsedTime;
            preciseWait(sleepTime.count()); // a busy sleep that avoids Windows 15 ms internal clock resolution
        }
        std::cout << "Capture stopped." << std::endl;

        program.CloseCSV();
        
    }
    else {
        std::cerr << "Failed to initialize VR system." << std::endl;
    }
    return 0;
}
