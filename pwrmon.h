#pragma once

#include <optional>
#include <xil_types.h>

class I2C;

class PwrMon {
public:
	PwrMon(I2C* i2cImpl);
	u32 readVoltage();
	u16 getVendorID();

private:
	// The register addresses also identify the operating mode
	enum class INA226_REG_ADDR : u8 {
		CONFIG 		= 0x00,
		SHUNT  		= 0x01,
		BUS_VOLTAGE = 0x02,
		POWER		= 0x03,
		CURRENT 	= 0x04,
		CALIBRATION = 0x05,
		VENDOR_ID	= 0xFE
	};
	//void writeMode(INA226_REG_ADDR mode);

private:
	std::optional<INA226_REG_ADDR> _currentRegPtr = std::nullopt;
	constexpr static u8 INA226_ADDR = 0x40;
	constexpr static u8 INA226_I2C_MPX_POS = 5;
	I2C* _dev;
};
