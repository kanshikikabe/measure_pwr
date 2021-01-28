#include "utils.h"
#include <xil_printf.h>

void padString(std::string& str) {
	if (str.size() > TERMWIDTH) {
		LOG("String too long, truncating");
		str = str.substr(0, TERMWIDTH);
	}

	str.reserve(TERMWIDTH + 1);
	for (size_t i=str.size(); i<TERMWIDTH; ++i) {
		str[i] = ' ';
	}
	str[TERMWIDTH] = '\0';
}

u32 generateEnableValue(u8 input) {
	u32 res;

	for (int i=0; i<input; ++i) {
		res |= (1 << i);
	}

	return res;
}

