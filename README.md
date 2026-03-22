# EARENDIL: A portable search-and-rescue (SAR) system
University of Kansas --- EECS 542 (Senior Design Project) --- Team 1

EARENDIL simplifies SAR operations by letting lost users know where they are, and which direction safety is in.

# 1. Hardware

## 1.1. Target platform and components

This project is build around Raspberry Pi's RP2040 microcontroller. 

- **Microcontroller + Radio:** Adafruit Feather RP2040 + RFM95
- **Magnetometer:** [Adafruit TEXT_HERE]()
- **Display:** [Adafruit TEXT_HERE]()
- **GNSS receiver:** [Adafruit Ultimate GPS]()
- **Accelerometer + Gyroscope:** [Adafruit TEXT_HERE]()

| Subsystem         | Component         | Cost (as of 2026-03-21) |
| :----------- | :--------------: | :------: |
| Microcontroller + Radio | Adafruit Feather RP2040 + RFM95 | $45T 
| GNSS receiver    | Even more text   | $6B



# Software

## 2.1. Software dependencies

TO-DO: REVISE MINIMUM REQUIRED VERSIONS, DEP. VERSIONS IN GENERAL

- [**pico-sdk (2.2.0)**](https://github.com/raspberrypi/pico-sdk)
- [**FreeRTOS-Kernel (11.2.0)**](https://github.com/FreeRTOS/FreeRTOS-Kernel)
- [**CMake (minimum 3.13)**](https://github.com/Kitware/CMake) => Must be available in your $PATH
- [**arm-none-eabi-gcc (version?)**]() => Must be available in your $PATH
- [**arm-none-eabi-g++ (version?)**]() => Must be available in your $PATH
- What else? Need to make sure nothing's missing.

Make sure the GNU Arm Embedded Toolchain is installed and available in your $PATH.

[Mention the other repos that are implicitly included in EARENDIL, that they are lean-ified, and that users are free to instead use the original repo. Annotate versions too, for completion.]

# Compilation instructions (Linux)

**Step 0** - Navigate to /build in your CLI:
```
cd [path_to_earendil-SAR-system]/build
```

**Step 1** - Generate the project's Makefile using CMake and the provided CMakeLists.txt:

```
cmake .. -DPICO_SDK_PATH=[path_to_pico-sdk] -DFREERTOS_KERNEL_PATH=[path_to_FreeRTOS-Kernel]
```

**Step 2** - Make the Makefile to generate the project's .uf2 file.
```
make
```
You will use this .uf2 file to program the Adafruit Feather RP2040 + RFM95 via its on-board FLASH memory.

#

### Recompilation instructions (optional)

When recompiling the project after modifying the source files:
- **If CMakeLists.txt was modified:** Do ```rm -rf *```in /build, then proceed with Steps 1 and 2.
- **Otherwise:** Proceed with Step 2. If errors are raised, use ```make clean``` then ```make```.

# Upload instructions

**Step 0** - Grab a USB-C-to-USB-A cable and plug it into the Feather's USB-C port.
 
**Step 1** - Press and hold the 'Boot' button on the Adafruit Feather RP2040 + RFM95.

**Step 2** - While holding the 'Boot' button, plug the USB-A end of the cable to your computer.

**Step 3** - Release the 'Boot' button. The Feather should be recognized as storage volume 'RPI-RP2'.

**Step 4** - Mount the 'RPI-RP2' storage volume and upload the project's .uf2 to it.

Done! The Feather should be now programmed and functional.

# Team roles
[Subject to change as the project moves forward]

- **Leo**: Embedded Systems Lead, Software Engineer (?), project ideation/vision
- **Stephen**: Project Lead, Software Engineer, ...
- **Jacob**: UI/UX Software Engineer, ...
- **Matthew**: Power Systems Engineer, ...



# Remarks (if any)
