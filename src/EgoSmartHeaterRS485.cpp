#ifndef __EGO_SH_RS485_H__
#define __EGO_SH_RS485_H__

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
#include "EgoSmartHeaterRS485.h"
#include <Arduino.h>
#include "ModbusMaster.h"

// I don't like this, but value is required in the call back functions
int ego_sh_dere_pin = D0;

//------------------------------------------------------------------------------
/*
 * If constructed with enabled manual control, default Pin D0 is used. If any other PIN should be utilized, please use the second constructor method.
 */
EgoSmartHeaterRS485::EgoSmartHeaterRS485(boolean manualDere)
{
    this->manualDere = manualDere;
}

/*
 * Using this constructor implicitly enables the manual DE/RE control. DE/RE is controlled by the PIN provided as a parameter.
 */
EgoSmartHeaterRS485::EgoSmartHeaterRS485(int dere_pin)
{
    this->manualDere = true;
	ego_sh_dere_pin = dere_pin;
}

/*
 * Call back function to intiate modbus transmission
 */
void preTransmission()
{
  digitalWrite(ego_sh_dere_pin, 1);
}

/*
 * Call back function to finalize modbus transmission
 */
void postTransmission()
{
  digitalWrite(ego_sh_dere_pin, 0);
}

/*
 * If lauched this way, the device is addressed by the EGO SmartHeater default ID as defined in EGO_SH_RS485_MODBUS_ADR. In case a different modbus address shall be used, launch the communication by the other begin function, which accepts a dedicated address.
 */
void EgoSmartHeaterRS485::begin(Stream &serial)
{
  this->begin(serial, EGO_SH_RS485_MODBUS_ADR);
}

/*
 * If lauched this way, the device is addressed by an individual ModBus ID. In case the standard EGO Smart Heater ModBus ID shall be used, launch the communication by the other begin function.
 */
void EgoSmartHeaterRS485::begin(Stream &serial, uint8_t slave)
{
  _node.begin(slave, serial);

  if(this->manualDere)
  {
    Serial.println("Manual Dere Active!");
    pinMode(ego_sh_dere_pin, OUTPUT);
    _node.postTransmission(postTransmission);
    _node.preTransmission(preTransmission);
  }
}


uint8_t EgoSmartHeaterRS485::getErrCode(bool _clear)
{
  uint8_t _tmp = _result;
  if (_clear == true)
    clearErrCode();
  return (_tmp);
}

void EgoSmartHeaterRS485::clearErrCode()
{
  _result = _node.ku8MBSuccess;
}

float EgoSmartHeaterRS485::getModbusFloat(uint16_t data[2])
{
  union u_data
  {
    byte b[4];
    uint16_t data[2];
  } source;

  union u_tag
  {
    byte b[4];
    float val;
  } dest;

  source.data[0] = data[0];
  source.data[1] = data[1];

  dest.b[2] = source.b[0];
  dest.b[3] = source.b[1];
  dest.b[0] = source.b[2];
  dest.b[1] = source.b[3];

  return dest.val;
}

uint32_t EgoSmartHeaterRS485::getModbusUint32(uint16_t data[2])
{
  union u_data
  {
    byte b[4];
    uint16_t data[2];
  } source;

  union u_tag
  {
    byte b[4];
    uint32_t val;
  } dest;

  source.data[0] = data[0];
  source.data[1] = data[1];

  dest.b[2] = source.b[0];
  dest.b[3] = source.b[1];
  dest.b[0] = source.b[2];
  dest.b[1] = source.b[3];

  return dest.val;
}

int32_t EgoSmartHeaterRS485::getModbusInt32(uint16_t data[2])
{
  union u_data
  {
    byte b[4];
    uint16_t data[2];
  } source;

  union u_tag
  {
    byte b[4];
    int32_t val;
  } dest;

  source.data[0] = data[0];
  source.data[1] = data[1];

  dest.b[2] = source.b[0];
  dest.b[3] = source.b[1];
  dest.b[0] = source.b[2];
  dest.b[1] = source.b[3];

  return dest.val;
}

String EgoSmartHeaterRS485::getModbusString32(uint16_t data[16])
{
  union u_data
  {
    byte b[32];
    uint16_t data[16];
  } source;

  String text = "";
  String chr = " ";
  
  for(int i=0; i<16; i++)
  {
    source.data[i] = data[i];
  }

  for(int i=0; (i<32 && source.b[i] != 0x0); i++)
  {
    chr.setCharAt(0, source.b[i]);
    text.concat(chr);
  }

  return text;
}

//------------------------------------------------------------------------------
// Basic Device Information
uint16_t EgoSmartHeaterRS485::getManufacturerId()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterManufacturerId, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint16_t EgoSmartHeaterRS485::getProductId()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterProductId, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;  
}

uint16_t EgoSmartHeaterRS485::getProductVersion()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterProductVersion, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint16_t EgoSmartHeaterRS485::getFirmwareVersion()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterFirmwareVersion, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

String EgoSmartHeaterRS485::getVendorName()
{
  uint8_t j;
  uint16_t data[16];
  String result = "";

  _result = _node.readHoldingRegisters(RegisterVendorName, 16);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 16; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusString32(data);
  }
  return result;
}

String EgoSmartHeaterRS485::getProductName()
{
  uint8_t j;
  uint16_t data[16];
  String result = "";

  _result = _node.readHoldingRegisters(RegisterProductName, 16);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 16; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusString32(data);
  }
  return result;
}

String EgoSmartHeaterRS485::getSerialNumber()
{
  uint8_t j;
  uint16_t data[16];
  String result = "";

  _result = _node.readHoldingRegisters(RegisterSerialNumber, 16);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 16; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusString32(data);
  }
  return result;
}


uint32_t EgoSmartHeaterRS485::getProductionDate()
{
  uint8_t j;
  uint16_t data[2];
  uint32_t result = 0;

  _result = _node.readHoldingRegisters(RegisterProductionDate, 2);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusUint32(data);
  }
  return result;
}

RelaisConfigurationData_t EgoSmartHeaterRS485::getRelaisConfiguration(int r)
{
  uint16_t data[2];
  RelaisConfigurationData_t result;

  _result = _node.readHoldingRegisters(RegisterRelaisConfiguration[r], 7);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result.ActualPower = _node.getResponseBuffer(0);
    data[0] = _node.getResponseBuffer(1);
    data[1] = _node.getResponseBuffer(2);
    result.OperatingSeconds = getModbusUint32(data);
    data[0] = _node.getResponseBuffer(3);
    data[1] = _node.getResponseBuffer(4);
    result.SwitchingCycles = getModbusUint32(data);
    result.MinOnTime = _node.getResponseBuffer(5);
    result.MinOffTime = _node.getResponseBuffer(6);
  }
  return result;
}

uint16_t EgoSmartHeaterRS485::getRelaisCount()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterRelaisCount, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}


//------------------------------------------------------------------------------
// Configuration Information
uint16_t EgoSmartHeaterRS485::getTemperatureMinValue()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterTemperatureMinValue, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint8_t EgoSmartHeaterRS485::setTemperatureMinValue(uint16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterTemperatureMinValue, 1);
  return _result;
}

uint16_t EgoSmartHeaterRS485::getTemperatureMaxValue()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterTemperatureMaxValue, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint8_t EgoSmartHeaterRS485::setTemperatureMaxValue(uint16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterTemperatureMaxValue, 1);
  return _result;
}

uint16_t EgoSmartHeaterRS485::getTemperatureNominalValue()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterTemperatureNominalValue, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint8_t EgoSmartHeaterRS485::setTemperatureNominalValue(uint16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterTemperatureNominalValue, 1);
  return _result;
}

int16_t EgoSmartHeaterRS485::getPowerNominalValue()
{
  int16_t result = -99;

  _result = _node.readHoldingRegisters(RegisterPowerNominalValue, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint8_t EgoSmartHeaterRS485::setPowerNominalValue(int16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterPowerNominalValue, 1);
  return _result;
}

int32_t EgoSmartHeaterRS485::getHomeTotalPower()
{
  uint8_t j;
  uint16_t data[2];
  int32_t result = 0;

  _result = _node.readHoldingRegisters(RegisterHomeTotalPower, 2);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusInt32(data);
  }
  return result;
}

uint8_t EgoSmartHeaterRS485::setHomeTotalPower(int32_t value)
{
  _node.setTransmitBuffer(0, highWord(value));
  _node.setTransmitBuffer(1, lowWord(value));

  _result = _node.writeMultipleRegisters(RegisterHomeTotalPower, 2);
  return _result;
}

uint8_t EgoSmartHeaterRS485::setRelaisMinOnTime(int r, uint16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterRelaisConfiguration[r]+5, 1);
  return _result;
}

uint8_t EgoSmartHeaterRS485::setRelaisMinOffTime(int r, uint16_t value)
{
  _node.setTransmitBuffer(0, value);

  _result = _node.writeMultipleRegisters(RegisterRelaisConfiguration[r]+6, 1);
  return _result;
}


//------------------------------------------------------------------------------
// Operating Information
uint32_t EgoSmartHeaterRS485::getRestartCounter()
{
  uint8_t j;
  uint16_t data[2];
  uint32_t result = 0;

  _result = _node.readHoldingRegisters(RegisterRestartCounter, 2);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusUint32(data);
  }
  return result;
}

int16_t EgoSmartHeaterRS485::getActualTemperaturePCB()
{
  int16_t result = -99;
  _result = _node.readHoldingRegisters(RegisterActualTemperaturePCB, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint32_t EgoSmartHeaterRS485::getTotalOperatingSeconds()
{
  uint8_t j;
  uint16_t data[2];
  uint32_t result = 0;

  _result = _node.readHoldingRegisters(RegisterTotalOperatingSeconds, 2);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusUint32(data);
  }
  return result;
}

uint32_t EgoSmartHeaterRS485::getErrorCounter()
{
  uint8_t j;
  uint16_t data[2];
  uint32_t result = 0;

  _result = _node.readHoldingRegisters(RegisterErrorCounter, 2);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = _node.getResponseBuffer(j);
    }
    result = getModbusUint32(data);
  }
  return result;
}

int16_t EgoSmartHeaterRS485::getActualTemperatureBoiler()
{
  int16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterActualTemperatureBoiler, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

int16_t EgoSmartHeaterRS485::getActualTemperatureExternalSensor1()
{
  int16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterActualTemperaturExternalSensor1, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

int16_t EgoSmartHeaterRS485::getActualTemperatureExternalSensor2()
{
  int16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterActualTemperaturExternalSensor2, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

int16_t EgoSmartHeaterRS485::getUserTemperatureNominal()
{
  int16_t result = -99;
  _result = _node.readHoldingRegisters(RegisterUserTemperatureNominal, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

uint16_t EgoSmartHeaterRS485::getRelaisStatus()
{
  uint16_t result = -1;

  _result = _node.readHoldingRegisters(RegisterRelaisStatus, 1);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    result = _node.getResponseBuffer(0);
  }
  return result;
}

RelaisOperatingTime_t EgoSmartHeaterRS485::getRelaisOperatingTime()
{
  uint8_t j;
  uint16_t data[2];
  uint32_t result = 0;
  RelaisOperatingTime_t rot;

  for(int i=0; i<3; i++) {
    _result = _node.readHoldingRegisters(RegisterRelaisOperatingTime[i], 2);
  
    // do something with data if read is successful
    if (_result == _node.ku8MBSuccess)
    {
      for (j = 0; j < 2; j++)
      {
        data[j] = _node.getResponseBuffer(j);
      }
      switch(i){
        case 0:
          rot.OperatingSeconds1 = getModbusUint32(data);
          break;
        case 1:
          rot.OperatingSeconds2 = getModbusUint32(data);
          break;
        case 2:
          rot.OperatingSeconds3 = getModbusUint32(data);
          break;
      }
    }
  }
  return rot;
}

ErrorData_t EgoSmartHeaterRS485::getError(int i)
{
  uint16_t data[2];
  ErrorData_t result;

  _result = _node.readHoldingRegisters(RegisterErrorData[i], 4);

  // do something with data if read is successful
  if (_result == _node.ku8MBSuccess)
  {
    data[0] = _node.getResponseBuffer(0);
    data[1] = _node.getResponseBuffer(1);
    result.OperatingHour = getModbusUint32(data);
    result.OperatingSecond = _node.getResponseBuffer(2);
    result.ErrorCode = _node.getResponseBuffer(3);
  }
  return result;  
}


#endif //__EGO_SH_RS485_H__
