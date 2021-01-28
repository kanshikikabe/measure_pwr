#pragma once

#include "xiicps.h"
#include "sleep.h"

class I2C {
public:
	I2C();
	void send(u16 addr, u8* payload, u32 payloadSize);
	void recv(u16 addr, u8* recvBuffer, u32 bufSize);
	void switchTo(u8 pos);
	void probeAddressSpace();

private:
	constexpr static u8 TCA9548A_ADDR = 0x75;
	constexpr static u32 SCLK_RATE = 1e5; // 100 KHz
	XIicPs _handle;
};
