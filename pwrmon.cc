#include "pwrmon.h"
#include "i2c.h"
#include "utils.h"

PwrMon::PwrMon(I2C* i2cImpl) : _dev(i2cImpl) {
	DEBUG("Switching multiplexer to INA226");
	_dev->switchTo(INA226_I2C_MPX_POS);
	sleep(1);
}

u32 PwrMon::readVoltage() {
	throw std::runtime_error("Not implemented");
	return 0;
}

u16 PwrMon::getVendorID() {
	u16 vendorID = 0;
	u8 buf = static_cast<u8>(INA226_REG_ADDR::VENDOR_ID);

	DEBUG("Getting vendor ID");
	_dev->send(INA226_ADDR, &buf, sizeof(buf));
	_dev->recv(INA226_ADDR, (u8*) &vendorID, sizeof(vendorID));

	return vendorID;
}
