# hh_proto — Build & Flash Guide for Windows 11

---

## 1. Install arduino-cli

Download the Arduino-CLI .zip file for your system. (You likely need the Windows exe 64-bit):
https://docs.arduino.cc/arduino-cli/installation/

Extract the .zip file to the location of your choosing. e.g. C:\Users\Your_User\Your_Arduino_CLI_Directory

Verify the installation:

```
> arduino-cli version
```

---

## 2. Configure the RP2040 Core

Add the Earlephilhower RP2040 board manager URL:

```
> arduino-cli config init
> arduino-cli config add board_manager.additional_urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

Then install the core:

```
> arduino-cli core update-index
> arduino-cli core install rp2040:rp2040
```

Then configure your arduino-cli.yaml file to install libraries to somewhere other than OneDrive\Documents (bleh):

```
> arduino-cli config set directories.user C:\Users\Your_User\AppData\Local\Arduino15
> arduino-cli config set directories.data C:\Users\Your_User\AppData\Local\Arduino15
> arduino-cli config set directories.downloads C:\Users\Your_User\AppData\Local\Arduino15\staging
```

To verify your configuration changes:

```
> arduino-cli config dump --verbose
```

---

## 3. Install Libraries

```
> arduino-cli lib install "LIBRARY NAME HERE"
```

Required libraries:

- Adafruit BMP3XX Library
- Adafruit BusIO
- Adafruit GC9A01A
- Adafruit GFX Library
- Adafruit GPS Library
- Adafruit LSM6DS
- Adafruit MMC56x3
- Adafruit SSD1306
- Adafruit Unified Sensor
- RadioHead

***Some libraries will install each other as dependencies. This is fine.***

---

## 4. Compile

From the root of the repository:

```
> arduino-cli compile --fqbn rp2040:rp2040:adafruit_feather_rfm .\hh_proto
```

If you encounter stale build cache errors, add `--clean`:

```
> arduino-cli compile --fqbn rp2040:rp2040:adafruit_feather_rfm --clean .\hh_proto
```

---

## 5. Upload

***This assumes you are plugged into the full Handheld Prototype with all modules wired up as described by the code.***

Plug in the Feather and confirm its COM port:

```
> arduino-cli board list
```

Then upload (replace `COM8` with your port if different):

```
> arduino-cli upload -p COM8 --fqbn rp2040:rp2040:adafruit_feather_rfm .\hh_proto
```

---

## 6. Serial Monitor

```
> arduino-cli monitor -p COM8
```

To specify a baud rate:

```
> arduino-cli monitor -p COM8 --config baudrate=9600
```

Exit the monitor with `Ctrl+C`.
