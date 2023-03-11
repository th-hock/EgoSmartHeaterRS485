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
The output of the EGO Smart Heater can be adjusted within the range 0W to 3500W in 500W steps.

## Features
Please refer to the assembly und user manual. The protocol description document contains a description of all the registers
Some important functions:
 - setPowerNominalValue: Activate the heater in manual mode, to switch ich to a specific power consumption.
 - setHomeTotalPower: Activate the heater in automatic mode. Provide the current metering value of a two-way meter. Negative values will cause the heater to be turned on.
 - getRelaisStatus: Get current power consumption. Multiply the Relais-Status value by 500 to get the current power consumption of the heater.

Renew the activation at least every 60 seconds. Otherwise the heater is automatically turned off.
