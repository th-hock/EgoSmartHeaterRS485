# EgoSmartHeaterRS485

[![GitHub release](https://img.shields.io/github/release/th-hock/EgoSmartHeaterRS485.svg)][GitHub release]
[![license](https://img.shields.io/github/license/th-hock/EgoSmartHeaterRS485.svg)][license]


[GitHub release]:   https://github.com/th-hock/EgoSmartHeaterRS485
[license]:          LICENSE

## Overview

Library for controlling E.G.O. RS485 Smart Heaters.

Description from users manual:  
The EGO Smart Heater is a screwin heater controlled by a communication interface to heat up water and aqueous solutions (e.g. hot water storage).
The EGO Smart Heater is suitable to optimise the own power consumption by converting electrical energy, for example from the photovoltaic system, into thermal energy.
The power of the EGO Smart Heater can be adjusted within the range 0W to 3500W in 500W steps.

## Features

Please refer to the [Assembly und User manual](https://github.com/th-hock/EgoSmartHeaterRS485/blob/main/extras/smart_heater.pdf) to learn about the device features, possible fields of applications and boundary conditions.  
The [protocol description](https://github.com/th-hock/EgoSmartHeaterRS485/blob/main/extras/Protocol%2090.60034.744_001_1.pdf) document contains a description of all available modbus registers. The library provides appropriate reading and - if possible - writing functions for all registers.

Some important library functions:
- **setPowerNominalValue**: Activate the heater in manual mode, to switch it to a specific power consumption.
- **setHomeTotalPower**: Activate the heater in automatic mode. Provide the current metering value of a two-way meter. Negative values will cause the heater to be turned on.
- **getRelaisStatus**: Get current power consumption. Multiply the Relais-Status value by 500 to get the current power consumption of the heater.

Renew the activation at least every 60 seconds. Otherwise the heater is automatically turned off.

## Installation

### Manual

Refer to Arduino Tutorials > Libraries [Manual Installation](https://www.arduino.cc/en/Guide/Libraries#toc5).


## Hardware

This library has been tested with an Arduino [NodeMCU ESP8266](https://components101.com/development-boards/nodemcu-esp8266-pinout-features-and-datasheet) controller, connected via RS485 using a MAX485 [MAX485](https://microcontrollerslab.com/rs485-serial-communication-esp32-esp8266-tutorial/) transceiver. The transceiver is connected via software serial library.

### Wiring

The following image shows the wiring schema of the ESP8266 example. Other PINs might be used, but not all of the ESP I/O PINs will be usable for all purposes.
![esp8266_max485](https://user-images.githubusercontent.com/81650966/224537809-a8d09a4f-675d-4624-9469-216af9e032ad.png)
Please be aware, that a shielded cable is recommended to connect to the RS485 interface. Shielding must be unilaterally contacted (either on the EGO Smart Heater or on the control end only).

## Example

The library contains a sketch that demonstrates how to use the library with an ESP8266 controller and a MAX485 transceiver. You can find it in the [examples](https://github.com/th-hock/EgoSmartHeaterRS485/tree/main/examples/) folder.


## Documentation

The library is documented by a [Doxygen documentation](https://htmlpreview.github.io/?https://github.com/th-hock/EgoSmartHeaterRS485/blob/main/doc/). Please download the library and view the /html/index.html file.


## Support

Please [submit an issue](https://github.com/th-hock/EgoSmartHeaterRS485/issues) for all questions, bug reports, and feature requests. Email requests will be politely redirected to the issue tracker so others may contribute to the discussion and requestors get a more timely response.
