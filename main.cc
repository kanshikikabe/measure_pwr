#include <xil_printf.h>

#include "xparameters.h"
#include "xiicps.h"

#define CLEARSCREEN "[2J"
#define ASCII_ESC 27
#define IIC_DEVICE_ID		XPAR_XIICPS_0_DEVICE_ID

#define LOG(fmt, ...) xil_printf(fmt "\r\n", ##__VA_ARGS__)
#define ERR(msg, ...) LOG("[ERROR] " msg)
#define SEND_CODE(code) xil_printf("%c" code, ASCII_ESC)

void i2cTest() {
	//constexpr u16 INA226_ADDR = 0x40;
	constexpr u16 TCA9548A_ADDR = 0x72;
	constexpr unsigned int SCLK_RATE = 4e5; // 400 KHz

	XIicPs Iic;
	int status;
	XIicPs_Config* config;

	config = XIicPs_LookupConfig(IIC_DEVICE_ID);
	if (NULL == config) {
		ERR("LookupConfig failed");
		return;
	}
	else {
		LOG("Lookupconfig success");
	}

	status = XIicPs_CfgInitialize(&Iic, config, config->BaseAddress);
	if (XST_SUCCESS != status) {
		ERR("CfgInitialize failed");
		return;
	}
	else {
		LOG("cfginitialize success");
	}

	status = XIicPs_SelfTest(&Iic);
	if (XST_SUCCESS != status) {
		ERR("Selftest failed");
		return;
	}
	else {
		LOG("selftest success");
	}

	if (XST_SUCCESS != XIicPs_SetSClk(&Iic, SCLK_RATE)) {
		LOG("setSclk failed");
	}
	else {
		LOG("setSclk success");
	}

	u8 controlReg = (1 << 6); // Select channel 5
	status = XIicPs_MasterSendPolled(&Iic, &controlReg, sizeof(controlReg),
					TCA9548A_ADDR);
	if (status != XST_SUCCESS) {
		ERR("MasterSendPolled failed");
		LOG("Returned status: %d", status);
		return;
	}
	else {
		LOG("MasterSendPolled success");
	}
}

int main() {
	SEND_CODE("[1;1H");
	SEND_CODE(CLEARSCREEN);
	LOG("Compiled at: " __DATE__ " " __TIME__);
	i2cTest();
	return 0;
}
