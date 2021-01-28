#include "pwrmon.h"
#include "i2c.h"
#include "utils.h"

#include <stdexcept>

PwrMon::PwrMon(const I2C& i2cImpl) : _dev(i2cImpl) {
	_dev.switchTo(INA226_I2C_MPX_CHANNEL);
	u16 calibrationRegValue = static_cast<u16>((double) 0.00512 / (INA226_CURRENT_LSB * INA226_SHUNT_RESISTOR));
	DEBUG("Setting calibration register to %d", calibrationRegValue);
	send(INA226_REG_ADDR::CALIBRATION, calibrationRegValue);
}

void PwrMon::setRegisterPtr(INA226_REG_ADDR mode) {
	//if (mode != _currentRegPtr) {
		u8 regptr = static_cast<u8>(mode);
		//DEBUG("Sending regptr %d", regptr);
		_dev.send(INA226_ADDR, &regptr, sizeof(regptr));
		_currentRegPtr = mode;
	//}
//	else {
//		DEBUG("Not setting the register pointer");
//	}
}

void PwrMon::setAveraging(Averaging avg) {
	u8 numericAvg = static_cast<u8>(avg);
	u16 configReg = getConfigReg();
	configReg &= ~(static_cast<u16>(Averaging::AVERAGING_1024) << 9); // Clear the previous averaging
	configReg |= (numericAvg << 9);

	DEBUG("Setting the config register to 0x%04x", configReg);
	send(INA226_REG_ADDR::CONFIG, configReg);
}

void PwrMon::send(INA226_REG_ADDR regptr, u16 data) {
	unsigned char buf[3];

	_currentRegPtr = regptr;
	buf[0] = static_cast<u8>(regptr);
	buf[1] = (data >> 8); // high byte
	buf[2] = (data & 0xFF); // low byte

	_dev.send(INA226_ADDR, buf, sizeof(buf));
}

u16 PwrMon::getCalibrationReg() {
	u16 calibrationReg = 0;

	setRegisterPtr(INA226_REG_ADDR::CALIBRATION);
	_dev.recv(INA226_ADDR, (u8*) &calibrationReg, sizeof(calibrationReg));
	return SWAP_BYTES(calibrationReg);
}

u16 PwrMon::getConfigReg() {
	u16 configReg = 0;

	setRegisterPtr(INA226_REG_ADDR::CONFIG);
	_dev.recv(INA226_ADDR, (u8*) &configReg, sizeof(configReg));
	return SWAP_BYTES(configReg);
}

double PwrMon::getBusVoltage() {
	u16 voltage = 0;

	setRegisterPtr(INA226_REG_ADDR::BUS_VOLTAGE);
	_dev.recv(INA226_ADDR, (u8*) &voltage, sizeof(voltage));
	return INA226_BUS_VOLTAGE_LSB * SWAP_BYTES(voltage);
}

double PwrMon::getShuntVoltage() {
	s16 voltage = 0;

	setRegisterPtr(INA226_REG_ADDR::SHUNT_VOLTAGE);
	_dev.recv(INA226_ADDR, (u8*) &voltage, sizeof(voltage));
	//DEBUG("Raw shunt voltage readout: %d", SWAP_BYTES(voltage));
	return 1000 * INA226_SHUNT_VOLTAGE_LSB * SWAP_BYTES(voltage); // return voltage in mV
}

double PwrMon::getCurrent() {
//	u16 current = 0;

//	setRegisterPtr(INA226_REG_ADDR::CURRENT);
//	_dev.recv(INA226_ADDR, (u8*) &current, sizeof(current));
//	DEBUG("Raw current readout: %d", SWAP_BYTES(current));
//	return 1000 * INA226_CURRENT_LSB * SWAP_BYTES(current);
	return getShuntVoltage() / INA226_SHUNT_RESISTOR;
}

u16 PwrMon::getVendorID() {
	u16 vendorID = 0;

	setRegisterPtr(INA226_REG_ADDR::VENDOR_ID);
	_dev.recv(INA226_ADDR, (u8*) &vendorID, sizeof(vendorID));
	return SWAP_BYTES(vendorID);
}
