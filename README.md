# Raspberry Pico SSD1309 + GFX Library
![IMG_20210210_142918](https://user-images.githubusercontent.com/44072895/107516326-88498a80-6bac-11eb-9c38-2ffe16fbd6ac.jpg)

# Usage

## Hardware
Connect your SSD1309 oled display to i2c1 port (you can change it during initialization)
```
SDA->GPIO2
SCL->GPIO3
GND->GND
VCC->3V3(OUT)
CS-> (NOT CONNECTED)
RES->To any GPIO
```

`DC->GND` (I2C Adress will be: `0x3C`) or `DC->3V3` (I2C Adress will be: `0x3D`)

## Software
The library needs only two files to run `SSD1309.hpp` and `SSD1309.cpp`.
This gives you the ability to display a bitmap or array of pixels. 
You need the GFX library to make it easier to create images for your display. (`GFX.hpp` and `GFX.cpp`)

`logo.hpp` is an example showing how to create bitmaps.
`fonts.hpp` contains one font.

## Example
Repo provides an example of how a library can be used.

```
git clone https://github.com/mbober1/RPi-Pico-SSD1309-library.git
cd RPi-Pico-SSD1309-library/example/
mkdir build
cd build
```
Change path to pico-sdk in CMakeLists.txt (line 3)
```
cmake ..
make -j4
```

## Make your own logo
http://www.majer.ch/lcd/adf_bitmap.php
