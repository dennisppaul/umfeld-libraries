| Physical Pin | BCM | Name / Function     |
|:------------ |:--- |:------------------- |
| 1            | —   | 3.3 V Power         |
| 2            | —   | 5 V Power           |
| 3            | 2   | SDA1 (I²C)          |
| 4            | —   | 5 V Power           |
| 5            | 3   | SCL1 (I²C)          |
| 6            | —   | Ground              |
| 7            | 4   | GPIO 4              |
| 8            | 14  | TXD0 (UART TX)      |
| 9            | —   | Ground              |
| 10           | 15  | RXD0 (UART RX)      |
| 11           | 17  | GPIO 17             |
| 12           | 18  | GPIO 18 / PWM0      |
| 13           | 27  | GPIO 27             |
| 14           | —   | Ground              |
| 15           | 22  | GPIO 22             |
| 16           | 23  | GPIO 23             |
| 17           | —   | 3.3 V Power         |
| 18           | 24  | GPIO 24             |
| 19           | 10  | SPI0 MOSI           |
| 20           | —   | Ground              |
| 21           | 9   | SPI0 MISO           |
| 22           | 25  | GPIO 25             |
| 23           | 11  | SPI0 SCLK           |
| 24           | 8   | SPI0 CE0 N          |
| 25           | —   | Ground              |
| 26           | 7   | SPI0 CE1 N          |
| 27           | 0   | ID_SD (EEPROM SDA)  |
| 28           | 1   | ID_SC (EEPROM SCL)  |
| 29           | 5   | GPIO 5              |
| 30           | —   | Ground              |
| 31           | 6   | GPIO 6              |
| 32           | 12  | GPIO 12 / PWM0      |
| 33           | 13  | GPIO 13 / PWM1      |
| 34           | —   | Ground              |
| 35           | 19  | GPIO 19 / SPI1 MISO |
| 36           | 16  | GPIO 16             |
| 37           | 26  | GPIO 26             |
| 38           | 20  | GPIO 20 / SPI1 MOSI |
| 39           | —   | Ground              |
| 40           | 21  | GPIO 21 / SPI1 SCLK |

## `#include "rpi_gpio_pins.h"`

```c
#pragma once

// Raspberry Pi GPIO pin definitions (BCM numbering)
// for use with libgpiod v2.x, offsets on /dev/gpiochip0

constexpr unsigned GPIO_2_SDA1          = 2;
constexpr unsigned GPIO_3_SCL1          = 3;
constexpr unsigned GPIO_4               = 4;
constexpr unsigned GPIO_5               = 5;
constexpr unsigned GPIO_6               = 6;
constexpr unsigned GPIO_7_SPI0_CE1_N    = 7;
constexpr unsigned GPIO_8_SPI0_CE0_N    = 8;
constexpr unsigned GPIO_9_SPI0_MISO     = 9;
constexpr unsigned GPIO_10_SPI0_MOSI    = 10;
constexpr unsigned GPIO_11_SPI0_SCLK    = 11;
constexpr unsigned GPIO_12_PWM0         = 12;
constexpr unsigned GPIO_13_PWM1         = 13;
constexpr unsigned GPIO_14_TXD0         = 14;
constexpr unsigned GPIO_15_RXD0         = 15;
constexpr unsigned GPIO_16              = 16;
constexpr unsigned GPIO_17              = 17;
constexpr unsigned GPIO_18_PWM0         = 18;
constexpr unsigned GPIO_19_SPI1_MISO    = 19;
constexpr unsigned GPIO_20_SPI1_MOSI    = 20;
constexpr unsigned GPIO_21_SPI1_SCLK    = 21;
constexpr unsigned GPIO_22              = 22;
constexpr unsigned GPIO_23              = 23;
constexpr unsigned GPIO_24              = 24;
constexpr unsigned GPIO_25              = 25;
constexpr unsigned GPIO_26              = 26;
constexpr unsigned GPIO_27              = 27;
constexpr unsigned GPIO_0_ID_SD         = 0;
constexpr unsigned GPIO_1_ID_SC         = 1;
```

## Adafruit Cyberdeck Pin Numbering

```
02 04 06 08 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 
01 03 05 07 09 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39
```

## RPI 400 + RPI 4 GPIO Pinout Diagram

![](Raspberry-pi-4-pinout.jpg)