#include <stdexcept>

#include "xparameters.h"
#include "i2c.h"
#include "utils.h"
#include "sleep.h"

I2C::I2C() {
	XIicPs_Config* config = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
	if (NULL == config) {
		throw std::runtime_error("LookupConfig failed");
	}

	XIicPs_CfgInitialize(&_handle, config, config->BaseAddress);

	if (XST_SUCCESS != XIicPs_SelfTest(&_handle)) {
		throw std::runtime_error("Selftest failed");
	}

	if (XST_SUCCESS != XIicPs_SetSClk(&_handle, SCLK_RATE)) {
		throw std::runtime_error("setSclk failed");
	}
}

void I2C::send(u16 addr, u8* payload, u32 payloadSize) {
	DEBUG("Sending to addr: %d", addr);
	if (XST_SUCCESS != XIicPs_MasterSendPolled(&_handle, payload, sizeof(payload), addr)) {
		std::printf("%s shat itself\n", __FUNCTION__);
		throw std::runtime_error("Failed to send");
	}
}

void I2C::recv(u16 addr, u8* recvBuffer, u32 bufSize) {
	DEBUG("Receiving from addr: %d", addr);
	while(XIicPs_BusIsBusy(&_handle)) {
#ifdef DEBUG_MODE
		xil_printf(".");
	}
	xil_printf("\r\n");
#else
	}
#endif

	u32 status = XIicPs_MasterRecvPolled(&_handle, recvBuffer, bufSize, addr);
	if (status != XST_SUCCESS) {
		xil_printf("O shit\r\n");
		std::printf("%s shat itself\r\n", __FUNCTION__);
		throw std::runtime_error("MasterRecvPolled failed");
	}
}

void I2C::switchTo(u8 pos) {
	u8 controlReg = (1 << pos);
	send(TCA9548A_ADDR, &controlReg, sizeof(controlReg));
	usleep(1000);
}

void I2C::probeAddressSpace() {
	u16 status;
	u8 payload = 0;

	for (size_t i=0; i<(1<<7); ++i) {
		if (TCA9548A_ADDR == i) {
			continue;
		}
		//xil_printf("Probing address %d ..... ", i);
		usleep(50000);
		status = XIicPs_MasterSendPolled(&_handle, &payload, sizeof(payload), i);
		if (XST_SUCCESS == status) {
			xil_printf("Probing address %d ..... ", i);
			LOG("OK");
		}
		else {
			//LOG("FAIL");
		}
	}
}


