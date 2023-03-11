/**
 * @file
 * @author  Thomas Hock <th.hock@gmx.de>
 * @version 1.0
 *
 * @section under MIT LICENSE
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @section DESCRIPTION
 *
 * Library for controlling E.G.O. RS485 Smart Heaters.
 * Reading and writing via Hardware or Software Serial library & rs232<->rs485 converter
 */

//------------------------------------------------------------------------------
#ifndef EGO_SH_RS485_h
#define EGO_SH_RS485_h
//------------------------------------------------------------------------------
#include <Arduino.h>
#include <ModbusMaster.h>
//------------------------------------------------------------------------------
#define EGO_SH_RS485_SERIAL_BAUD 19200
#define EGO_SH_RS485_MODBUS_ADR 247     // Modbus address of EGO Smart Heaters

//------------------------------------------------------------------------------

/// \struct RelaisConfigurationData_t
/// setup of a specific relais
struct RelaisConfigurationData_t
{
  uint16_t ActualPower;
  uint32_t OperatingSeconds;
  uint32_t SwitchingCycles;
  uint16_t MinOnTime;
  uint16_t MinOffTime;
};

/// \struct ErrorData_t
/// record set of a single error entry
struct ErrorData_t
{
  uint32_t OperatingHour;
  uint16_t OperatingSecond;
  uint16_t ErrorCode;
};

/// \struct RelaisOperatingTime_t
/// Operating seconds value for all three relais
struct RelaisOperatingTime_t
{
  uint32_t OperatingSeconds1;
  uint32_t OperatingSeconds2;
  uint32_t OperatingSeconds3;
};

//------------------------------------------------------------------------------
/// \class EgoSmartHeaterRS485
/// E.G.O. Smart Heater control
/// Controls SmartHeater product code 29.65335.000 and RS-485 module for Arduino (MAX485) on multiple architectures
class EgoSmartHeaterRS485
{
public:
  /// @brief Constructor to setup the SmartHeater instance in automatic or manual DE/RE control.
  /// @param manualDere is a boolean to enable or disable manual DE/RE control (default: false).
  EgoSmartHeaterRS485(boolean manualDere = false);
  /// @brief Constructor to setup the SmartHeater instance with enabled manual DE/RE control.
  /// @param dere_pin is number of the PIN which shall be used to control the DE/RE input of the MAX485 board.
  EgoSmartHeaterRS485(int dere_pin);

  /// @brief Function to launch the SmartHeater communication using the default modbus ID.
  /// @param serial is a reference to the instance of serial interface, to which the MAX485 is connected. Might be HW-or SW serial.
  void begin(Stream &serial);
  /// @brief Function to launch the SmartHeater communication using the individual modbus ID.
  /// @param serial is a reference to the instance of serial interface, to which the MAX485 is connected. Might be HW-or SW serial.
  /// @param slave specifies the modbus address of the slave device to communicate with.
  void begin(Stream &serial, uint8_t slave);

  /// @brief Function to retreive the latest error code, which occured during the device communication
  /// @param _clear indicates if the error code shall be cleared implicitly after reading (default: false).
  uint8_t getErrCode(bool _clear = false);
  /// @brief Function to clear the error code store
  void clearErrCode();
  boolean manualDere;

  //Basic Device Information
  /// @brief Retrieve ManufacturerID (0x2000)
  /// @return For EGO SmartHeater always: 0x14ef
  uint16_t getManufacturerId();
  /// @brief Retrieve ProductID (0x2001)
  /// @return Indicates the E.G.O. product ID
  uint16_t getProductId();
  /// @brief Retrieve ProductVersion (0x2002)
  /// @return Indicates the E.G.O. variant ID
  uint16_t getProductVersion();
  /// @brief Retrieve FirmwareVersion (0x2003)
  /// @return Firmware-Revision (e.g. 0x64 = 100 = 1.00)
  uint16_t getFirmwareVersion();
  /// @brief Retrieve VendorName (0x2013)
  /// @return Vendor name as a string (Example: E.G.O.)
  String getVendorName();
  /// @brief Retrieve ProductName (0x2023)
  /// @return Device name as a string (Example: Smart Heater SM1000)
  String getProductName();
  /// @brief Retrieve SerialNumber (0x2033)
  /// @return Serial number as a string (Example: 30380912332211)
  String getSerialNumber();
  /// @brief Retrieve ProductionDate (0x2035)
  /// @return Date of when device was assembled. This field is BCD-encoded and thus can be interpreted as a string with a fixed length. (Example: 0x20140515)
  uint32_t getProductionDate();
  /// @brief Retrieve details for a particular relais (0x1000, 0x1020, 0x1040)
  /// @param Number of the relais to query (0: 500W, 1: 1000W, 2: 2000W)
  /// @return Structure which contains ActualPower, OperatingSeconds, SwitchingCycles, MinOnTime, MinOffTime
  RelaisConfigurationData_t getRelaisConfiguration(int r);
  /// @brief Retrieve RelaisCount (0x1204)
  /// @return Number of relais available in this product. Should be 3.
  uint16_t getRelaisCount();

  // Configuration Information
  /// @brief Retrieve TemperatureMinValue (0x1209).
  /// Below this temperature the heater will warm the boiler even if no solar power is available. This can be used by the consumer to ensure a minimum water temperature in the boiler. 0 = Off, otherwise value has to be at least 10K below the actual setting of the Potentiometer.
  /// @return Temperature in °C
  uint16_t getTemperatureMinValue();
  /// @brief Configure TemperatureMinValue (0x1209).
  /// Below this temperature the heater will warm the boiler even if no solar power is available. This can be used by the consumer to ensure a minimum water temperature in the boiler. 0 = Off, otherwise value has to be at least 10K below the actual setting of the Potentiometer.
  /// @param value is the Temperature in °C to be applied 
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setTemperatureMinValue(uint16_t value);
  /// @brief Retrieve TemperatureMaxValue (0x120A). The maximum specified allowed water temperature which will not exceeded by the smart heater even if the potentiometer is in the maximum position.
  /// @return Temperature in °C
  uint16_t getTemperatureMaxValue();
  /// @brief Configure TemperatureMaxValue (0x120A).
  /// The maximum specified allowed water temperature which will not exceeded by the smart heater even if the potentiometer is in the maximum position.
  /// @param value is the Temperature in °C to be applied 
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setTemperatureMaxValue(uint16_t value);
  /// @brief Retrieve TemperatureNominalValue (0x120B).
  /// This is the desired water temperature of the consumer. The special value zero means that the optional hardware potentiometer should be used by the regulator. The value of this potentiometer is available in register “UserTemperaturNominalValue”. The maximum value must not be higher than “UserTemperaturNominalValue”.
  /// @return Temperature in °C
  uint16_t getTemperatureNominalValue();
  /// @brief Configure TemperatureNominalValue (0x120B).
  /// This is the desired water temperature of the consumer. The special value zero means that the optional hardware potentiometer should be used by the regulator. The value of this potentiometer is available in register “UserTemperaturNominalValue”. The maximum value must not be higher than “UserTemperaturNominalValue”.
  /// @param value is the Temperature in °C to be applied 
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setTemperatureNominalValue(uint16_t value);
  /// @brief Retrieve PowerNominalValue (0x1300).
  /// This is the desired power value which the heater should use to heat the boiler. The special value -1 means, that the heater should use the HomeTotalPower value and use as much power as possible. When writing this value the heater will match the desired value itself to the available relais and constraints (minimum switch on times etc.). Therefore this register is threat on a best-effort basis.
  /// @return Power in Watts.
  int16_t getPowerNominalValue();
  /// @brief Configure PowerNominalValue (0x1300).
  ///  This is the desired power value which the heater should use to heat the boiler. The special value -1 means, that the heater should use the HomeTotalPower value and use as much power as possible. When writing this value the heater will match the desired value itself to the available relais and constraints (minimum switch on times etc.). Therefore this register is threat on a best-effort basis.
  /// @param value is the power in Watts 
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setPowerNominalValue(int16_t value);
  /// @brief Retrieve HomeTotalPower (0x1301). This register is written by the smart meter and contains the total power consumption/generation of the home/flat. When the value is negative then the home is feeding power back to the utilities, thus the heater should consume energy to heat up the boiler. When the value is positive then the home consumes energy from the utilities and the heater should stop heating.
  /// @return Power in Watts.
  int32_t getHomeTotalPower();
  /// @brief Retrieve HomeTotalPower (0x1301).
  ///  This register is written by the smart meter and contains the total power consumption/generation of the home/flat. When the value is negative then the home is feeding power back to the utilities, thus the heater should consume energy to heat up the boiler. When the value is positive then the home consumes energy from the utilities and the heater should stop heating.
  /// @param value is the power in Watts 
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setHomeTotalPower(int32_t value);
  /// @brief Configure relais MinOnTime for a specific relais (0x1005, 0x1025, 0x1045).
  /// This field defines the minimum time the relais remains switched on.
  /// @param Number of the relais to query (0: 500W, 1: 1000W, 2: 2000W)
  /// @param Minimum on-time in seconds.
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setRelaisMinOnTime(int r, uint16_t value);
  /// @brief Configure relais MinOffTime for a specific relais (0x1006, 0x1026, 0x1046).
  ///  This field defines the minimum time the relais remains switched off.
  /// @param Number of the relais to query (0: 500W, 1: 1000W, 2: 2000W)
  /// @param Minimum on-time in seconds.
  /// @return result code of the modbus write operation (see ModBus libary)
  uint8_t setRelaisMinOffTime(int r, uint16_t value);
  
  // Operating Information
  /// @brief Retrieve RestartCounter (0x1202)
  /// This is mainly used during development.
  /// @return Number of the restarts of the smart heater’s internal controller.
  uint32_t getRestartCounter();
  /// @brief Retrieve ActualTemperaturPCB (0x1205)
  /// This is the actual temperature of the heater’s control PCB. If the PCB temperature exceed this max. value the heater switches off. 10K below this temperature the heater swichtes on.
  /// @return Temperature in °C
  int16_t getActualTemperaturePCB();
  /// @brief Retrieve TotalOperatingSeconds (0x1400).
  /// @return Total operating seconds of the smart heater.
  uint32_t getTotalOperatingSeconds();
  /// @brief Retrieve ErrorCounter (0x1402)
  /// @return Number of errors
  uint32_t getErrorCounter();
  /// @brief Retrieve ActualTemperaturBoiler (0x1404)
  /// @return Actual water temperature in the boiler in °C
  int16_t getActualTemperatureBoiler();
  /// @brief Retrieve ActualTemperaturExternalSensor1 (0x1405)
  /// This is the actual temperature of an (optional) first external temperature sensor. Special values:
  /// 0x8000 – no sensor can be attached to this heater model
  /// 0x8001 – no sensor attached
  /// 0x8002 – sensor present but malfunctioning
  /// @return Temperature in °C
  int16_t getActualTemperatureExternalSensor1();
  /// @brief Retrieve ActualTemperaturExternalSensor2 (0x1406)
  /// This is the actual temperature of an (optional) second external temperature sensor. Special values:
  /// 0x8000 – no sensor can be attached to this heater model
  /// 0x8001 – no sensor attached
  /// 0x8002 – sensor present but malfunctioning
  /// @return Temperature in °C
  int16_t getActualTemperatureExternalSensor2();
  /// @brief Retrieve UserTemperaturNominalValue (0x1407)
  /// This value corresponds to the position of an (optional) potentiometer where the consumer can select a desired boiler temperature.
  /// @return Temperature in °C
  int16_t getUserTemperatureNominal();
  /// @brief Retrieve RelaisStatus (0x1408)
  /// This bitfield reflects the switching state of the heater’s internal relais:
  /// 0x0000: all relais are switched off
  /// 0x0001: only relais 1 is switched on
  /// 0x0002: only relais 2 is switched on
  /// 0x0004: only relais 3 is switched on
  /// 0x0005: relais 1 + 3 is switched on
  /// @return Relais Status
  uint16_t getRelaisStatus();
  /// @brief Retrieve the operating times of all relais (0x1409, 0x140B, 0x140D)
  /// @return Counter of operating seconds for the three relais.
  RelaisOperatingTime_t getRelaisOperatingTime();
  /// @brief Retrieve error struct (0x1500 - 0x1526)
  /// @param i is the number of error message (0 - 9)
  /// @return struct which contains OperatingHour, OperatingSecond and ErrorCode
  ErrorData_t getError(int i);

protected:
  // instantiate ModbusMaster object
  ModbusMaster _node;
  uint8_t _result = _node.ku8MBSuccess; // Value: 0=Success, 2=Illegal Address, 3=Illegal Value

  float getModbusFloat(uint16_t data[2]);
  uint32_t getModbusUint32(uint16_t data[2]);
  int32_t getModbusInt32(uint16_t data[2]);
  String getModbusString32(uint16_t data[16]);

  //Basic Device Information
  static const uint16_t RegisterManufacturerId = 0x2000;
  static const uint16_t RegisterProductId = 0x2001;
  static const uint16_t RegisterProductVersion = 0x2002;
  static const uint16_t RegisterFirmwareVersion = 0x2003;
  static const uint16_t RegisterVendorName = 0x2004;
  static const uint16_t RegisterProductName = 0x2014;
  static const uint16_t RegisterSerialNumber = 0x2024;
  static const uint16_t RegisterProductionDate = 0x2034;
  static constexpr uint16_t RegisterRelaisConfiguration[3] = {0x1000,0x1020,0x1040};
  static const uint16_t RegisterRelaisCount = 0x1204;

  // Configuration Information
  static const uint16_t RegisterTemperatureMinValue = 0x1209;
  static const uint16_t RegisterTemperatureMaxValue = 0x120A;
  static const uint16_t RegisterTemperatureNominalValue = 0x120B;
  static const uint16_t RegisterPowerNominalValue = 0x1300;
  static const uint16_t RegisterHomeTotalPower = 0x1301;
  static const uint16_t RegisterUserTemperatureNominal = 0x1407;

  //Operating Information
  static const uint16_t RegisterRestartCounter = 0x1202;
  static const uint16_t RegisterActualTemperaturePCB = 0x1205;
  static const uint16_t RegisterTotalOperatingSeconds = 0x1400;
  static const uint16_t RegisterErrorCounter = 0x1402;
  static const uint16_t RegisterActualTemperatureBoiler = 0x1404;
  static const uint16_t RegisterActualTemperaturExternalSensor1 = 0x1405;
  static const uint16_t RegisterActualTemperaturExternalSensor2 = 0x1406;
  static const uint16_t RegisterRelaisStatus = 0x1408;
  static constexpr uint16_t RegisterRelaisOperatingTime[3] = {0x1409,0x140B,0x140D};
  static constexpr uint16_t RegisterErrorData[10] = {0x1500,0x1504,0x1508,0x150C,0x1510,0x1514,0x1518,0x151C,0x1520,0x1524};
};

#endif //EGO_SH_RS485_h
