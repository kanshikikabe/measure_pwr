#include "utils.h"
#include "i2c.h"
#include "pwrmon.h"
#include <stdexcept>

constexpr static int MAINLOOP_SLEEP_SEC = 5;
unsigned int nLFSREnabled = 0;
unsigned int nMultipliersEnabled = 0;

//void drawHeader() {
//	SEND_CODE(MOVE_TO(1, 1));
//	std::string date("Compiled at: " __DATE__ " " __TIME__);
//	padString(date);
//	printf(BGCOLOR_BLUE FGCOLOR_WHITE "%s\r\n", date.c_str());
//
//	for (int i=0; i<2; ++i) {
//		for (int j=0; j<TERMWIDTH; ++j) {
//			printf(" ");
//		}
//		printf("\r\n");
//	}
//	printf(COLOR_RESET);
//}
//
//void displayStats(double voltage, double current, double power) {
//	std::string stats;
//	SEND_CODE(SAVECURSOR);
//	SEND_CODE(MOVE_TO(2, 1));
//
//	stats = "# LFSRs: " + std::to_string(nLFSREnabled) + " | "
//			"# Multipliers: " + std::to_string(nMultipliersEnabled);
//	padString(stats);
//	printf(BGCOLOR_BLUE FGCOLOR_MAGENTA "%s\r\n" COLOR_RESET, stats.c_str());
//
//	SEND_CODE(MOVE_TO(3, 1));
//
//	stats = "Voltage: " + std::to_string(voltage) + " mV | "
//			"Current: " + std::to_string(current) + " mA | "
//			"Power: "   + std::to_string(power)   + " mW";
//
//	padString(stats);
//	printf(BGCOLOR_BLUE FGCOLOR_RED "%s\r\n" COLOR_RESET, stats.c_str());
//	SEND_CODE(RESTORECURSOR);
//}

void mainLoop(PwrMon& monitor) {
	double busVoltage = 0;
	double current = 0;
	double power = 0;

	for(;;) {
		busVoltage = monitor.getBusVoltage();
		usleep(50);
		current = monitor.getCurrent();
		power = (busVoltage / 1000) * current; // mW
		//displayStats(busVoltage, current, power);
		LOG("Voltage: %.2lf mV | Current: %.2lf mA | Power: %.2lf mW", busVoltage, current, power);
		//LOG("Current should be: %lf", monitor.getShuntVoltage() / 0.01);
		sleep(MAINLOOP_SLEEP_SEC);
	}
}

void readUserInput() {
	printf("Enter number of LFSRs to be enabled: ");
	if (1 != scanf("%u", &nLFSREnabled)) {
		throw new std::invalid_argument("Failed to read # of LFSRs");
	}
	printf("\nEnter number of multipliers to be enabled: ");
	if (1 != scanf("%u", &nMultipliersEnabled)) {
		throw new std::invalid_argument("Failed to read # of multipliers");
	}
	SEND_CODE(MOVE_TO(1, 1));
	SEND_CODE(CLEARSCREEN);
}

int main() {
	SEND_CODE(MOVE_TO(1, 1));
	SEND_CODE(CLEARSCREEN);
	LOG("Compiled at: " __DATE__ " " __TIME__);
	//readUserInput();
	//drawHeader();

	try {
		I2C i2cWrapper;
		PwrMon monitor(i2cWrapper);
		monitor.setAveraging(PwrMon::Averaging::AVERAGING_128);
		mainLoop(monitor);
	}
	catch (const std::exception& ex) {
		FATAL("Caught exception: %s", ex.what());
	}

	return 0;
}

