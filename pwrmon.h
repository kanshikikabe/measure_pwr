#pragma once

#include "i2c.h"

#include <optional>
#include <xil_types.h>

class PwrMon {
public:
	PwrMon(const I2C& i2cImpl);

public:
	 enum class Averaging : u8 {
		 AVERAGING_1    = 0,
		 AVERAGING_4    = 1,
		 AVERAGING_16   = 2,
		 AVERAGING_64   = 3,
		 AVERAGING_128  = 4,
		 AVERAGING_256  = 5,
		 AVERAGING_512  = 6,
		 AVERAGING_1024 = 7
	 };

	void setAveraging(Averaging avg);
	double getBusVoltage();    // in mV
	double getShuntVoltage();  // in mV
	double getCurrent();       // in mA
	double getPowerUsage() { return getBusVoltage() * getCurrent(); }
	u16 getVendorID();

private:
	// The register addresses also identify the operating mode
	enum class INA226_REG_ADDR : u8 {
		CONFIG 		  = 0x00,
		SHUNT_VOLTAGE = 0x01,
		BUS_VOLTAGE   = 0x02,
		POWER		  = 0x03,
		CURRENT 	  = 0x04,
		CALIBRATION   = 0x05,
		VENDOR_ID	  = 0xFE,
		DIE_ID		  = 0xFF
	};

	void setRegisterPtr(INA226_REG_ADDR mode);
	u16 getConfigReg();
	u16 getCalibrationReg();
	void send(INA226_REG_ADDR regptr, u16 data);

private:
	I2C _dev;
	std::optional<INA226_REG_ADDR> _currentRegPtr = std::nullopt;

	constexpr static u8 INA226_ADDR = 0x40;
	constexpr static u8 INA226_I2C_MPX_CHANNEL = 5;
	constexpr static double INA226_BUS_VOLTAGE_LSB = 1.25; // mV
	constexpr static double INA226_SHUNT_VOLTAGE_LSB= 2.5e-6; // 2.5 uV
	constexpr static double INA226_CURRENT_LSB = 5e-4; // 0.5 mA; should be enough
	constexpr static double INA226_SHUNT_RESISTOR = 0.01; // Ohms
};
