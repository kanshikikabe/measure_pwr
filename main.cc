#include "utils.h"
#include "i2c.h"
#include "pwrmon.h"
#include <stdexcept>

int main() {
	SEND_CODE(MOVE_TO(1, 1));
	SEND_CODE(CLEARSCREEN);
	LOG("Compiled at: " __DATE__ " " __TIME__);

	try {
		I2C i2cWrapper;
		PwrMon monitor(&i2cWrapper);
		LOG("Vendor id is: %d", monitor.getVendorID());

	}
	catch (const std::exception& ex) {
		std::printf("Caught exception: %s\r\n", ex.what());
	}

	return 0;
}
