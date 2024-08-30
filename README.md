# UltimateTracker_CPP

This project is designed to read orientation and position data from the VIVE Ultimate Tracker on Windows using C++. 

## Pre-requirements

Before running the program, make sure the following setup steps are completed:

1. **Install VIVE Streaming Hub**: 
   - Download and install from the official VIVE website. [VIVEStreamingHub](https://www.vive.com/us/vive-hub/download/)
   - Enable Beta mode for "PC UltimateTracker" using the code: `VIVEUTRCPreview` (valid as of 08/2024).
   - Follow the instructions to create a map. Ignore the last step demanding a SteamVR headset connection.

2. **Install SteamVR**: 
   - Download and install SteamVR.
   - Enable the null driver for a virtual headset using this tool: [SteamVRNoHeadset](https://github.com/username223/SteamVRNoHeadset).

3. **Run VIVE Streaming Hub**: 
   - Ensure that the connected tracker status is "Ready."

4. **Run SteamVR**: 
   - Necessary for the C++ program to work (Note: The Python version only requires SteamVR to be installed with the null driver enabled; it does not need to be running).

## Building and Running the Program

1. **Install OpenVR SDK**: 
   - Download and install the OpenVR SDK from the official repository.

2. **Install CMake**: 
   - Download and install CMake from the official website.

3. **Configure CMakeLists.txt**: 
   - Modify the `CMakeLists.txt` file to match your project's directory structure and paths.

4. **Create a Build Directory and Run CMake**: 
   - Open a terminal and execute the following commands:
     ```sh
     mkdir build
     cd build
     cmake ..
     cmake --build .
     ```

5. **Copy the Required DLL**: 
   - Copy `openvr_api.dll` into the `/build/bin` directory (the same directory as the executable) to ensure the correct library is used. The DLL can be found in the `OpenVR\lib\win64` directory.

6. **Run the Program**: 
   - Navigate to the bin directory and run the executable:
     ```sh
     cd bin
     UltimateTrackerReader.exe
     ```

7. **Data Logging**: 
   - Your tracking data will be stored in the `/log/UltimateTrackerData.csv` file.

## Important Notes

- The C++ program requires SteamVR to be running, while the Python version does not. However, both require SteamVR and the null driver to be installed.
- Ensure all dependencies are correctly installed and paths are properly set in your environment.

## Contact

For any questions or issues, please contact [kulozik@isir.upmc.fr].

## License

This code is licensed under the MIT License.

## Attribution

When using this code, please cite the following:

[Julian Kulozik]. (2024). UltimateTracker_CPP. GitHub repository. URL: [https://github.com/jkulozik/UltimateTracker_CPP]
