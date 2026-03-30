# TO-DO:

Need to add Windows + WSL section (USB attach command, etc.)

# EARENDIL: A portable search-and-rescue (SAR) system
University of Kansas --- EECS 542 (Senior Design Project) --- Team 1

EARENDIL simplifies SAR operations by letting lost users know where they are, and which direction safety is in.

# 1. Hardware

## 1.1. Target platform

This project is build around Raspberry Pi's RP2040 microcontroller. 

## 1.2. Handheld components

| Subsystem | Component | Relevant standards | Cost (as of 2026-03-21) |
| :-------- | :-------- | :----------------- | :---------------------- |
| Microcontroller + Radio Transceiver | [Adafruit Feather RP2040 with RFM95 LoRa Radio - 915MHz - RadioFruit and STEMMA QT](https://www.adafruit.com/product/5714) | UART, SPI, I²C, USB | $29.95 |
| Round Display | [Adafruit 1.28" 240x240 Round TFT LCD Display with MicroSD - GC9A01A with EYESPI Connector](https://www.adafruit.com/product/6178) | SPI | $17.50
| GNSS (GPS) receiver | [Adafruit Ultimate GPS Breakout with GLONASS + GPS - PA1616D - 99 channel w/10 Hz updates](https://www.adafruit.com/product/5440) | UART | $29.95 |
| Magnetometer | [Adafruit Triple-axis Magnetometer - MMC5603 - STEMMA QT / Qwiic](https://www.adafruit.com/product/5579) | I²C | $5.95 |
| Precision Altimeter | | I²C | |
| Accelerometer + Gyroscope | | I²C | |
| MicroSD reader | | SPI | |

Other hardware includes
- Radio antenna
- Antenna connector
- PCB (design pending)
- Case (design pending)

## 1.3. Node components

| Subsystem | Component | Relevant standards | Cost (as of 2026-03-21) |
| :-------- | :-------- | :----------------- | :---------------------- |
| Microcontroller + Radio Transceiver | [Adafruit Feather RP2040 with RFM95 LoRa Radio - 915MHz - RadioFruit and STEMMA QT](https://www.adafruit.com/product/5714) | UART, SPI, I²C, USB | $29.95 |
| GNSS (GPS) receiver | [Adafruit Ultimate GPS Breakout with GLONASS + GPS - PA1616D - 99 channel w/10 Hz updates](https://www.adafruit.com/product/5440) | UART | $29.95 |
| Precision Altimeter | | I²C | |
| MicroSD reader | | SPI | |

# Software

## 2.1. Software dependencies

TO-DO: REVISE MINIMUM REQUIRED VERSIONS, DEP. VERSIONS IN GENERAL

- [**pico-sdk (2.2.0)**](https://github.com/raspberrypi/pico-sdk)
```
git clone [pico_sdk_repo]
```
```

```

- [**FreeRTOS-Kernel (11.2.0)**](https://github.com/FreeRTOS/FreeRTOS-Kernel)
- [**CMake (minimum 3.13)**](https://github.com/Kitware/CMake) => Must be available in your $PATH
- [**arm-none-eabi-gcc (version?)**]() => Must be available in your $PATH
- [**arm-none-eabi-binutils**]
- [**arm-none-eabi-newlib**]
- picotools (download precompiled executable from official repo)

In Debian based systems, do
```
sudo apt-get install build-essential gcc-arm-none-eabi
```

- [**arm-none-eabi-g++ (version?)**]() => Must be available in your $PATH
- What else? Need to make sure nothing's missing.

Make sure the GNU Arm Embedded Toolchain is installed and available in your $PATH.

[Mention the other repos that are implicitly included in EARENDIL, that they are lean-ified, and that users are free to instead use the original repo. Annotate versions too, for completion.]

## 2.2. Compilation instructions (Linux)

SUPER DUPER EXTREMELY IMPORTANT OR SERIALUSB WILL NOT WORK:
```
cd [path_to_pico_sdk]
```
```
git submodule update --init
```

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

### 2.2.1. Recompilation instructions (optional)

When recompiling the project after modifying the source files:
- **If CMakeLists.txt was modified:** Do ```rm -rf *```in /build, then proceed with Steps 1 and 2.
- **Otherwise:** Proceed with Step 2. If errors are raised, use ```make clean``` then ```make```.

## 2.3. Upload instructions

**Step 0** - Grab a USB-C-to-USB-A cable and plug it into the Feather's USB-C port.
 
**Step 1** - Press and hold the 'Boot' button on the Adafruit Feather RP2040 + RFM95.

**Step 2** - While holding the 'Boot' button, plug the USB-A end of the cable to your computer.

**Step 3** - Release the 'Boot' button. The Feather should be recognized as storage volume 'RPI-RP2'.

**Step 4** - Mount the 'RPI-RP2' storage volume and upload the project's .uf2 to it.

*Done! The Feather should be now programmed and functional.*

# 3. Team roles
[Subject to change as the project moves forward]

- **Leo**: Embedded Systems Lead, Software Engineer (?), project ideation/vision/hardware selection (?)
- **Stephen**: Project Lead, Software Engineer, ...
- **Jacob**: UI/UX Software Engineer, ...
- **Matthew**: Power Systems Engineer, ...



# 4. Remarks (if any)
