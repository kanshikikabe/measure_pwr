#pragma once

#include <xil_printf.h>
#include <string>
#include <cstring>

// Uncomment to enable debug messages
#define DEBUG_MODE

#define TERMWIDTH 80
#define TERMHEIGHT 25

#define SAVECURSOR "7"
#define RESTORECURSOR "8"
#define CLEARSCREEN "[2J"
#define ASCII_ESC 27
#define MOVE_TO(X, Y) "[" #X ";" #Y "H"
#define SEND_CODE(code) xil_printf("%c" code, ASCII_ESC)

#define COLOR_RESET       "\033[0m"
#define FGCOLOR_RED       "\033[31;1m"
#define FGCOLOR_GREEN     "\033[32;1m"
#define FGCOLOR_YELLOW    "\033[33;1m"
#define FGCOLOR_BLUE      "\033[34;1m"
#define FGCOLOR_MAGENTA   "\033[35;1m"
#define FGCOLOR_CYAN      "\033[36;1m"
#define FGCOLOR_WHITE     "\033[37;1m"

#define BGCOLOR_BLUE      "\033[104;1m"

#define LOGGER_USE_NATIVE_PRINTF 1
#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#ifdef LOGGER_USE_NATIVE_PRINTF
#include <cstdio>
#define LOG(fmt, ...) std::printf("[%s:%d] " fmt "\r\n",  __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#error "Not using the glorious std::printf" // xil_printf() is shit
#define LOG(fmt, ...) xil_printf(fmt "\r\n", ##__VA_ARGS__)
#endif

#define ERROR(msg, ...) LOG(FGCOLOR_RED "[ERROR] " msg COLOR_RESET, ##__VA_ARGS__)
#define FATAL(msg, ...) LOG(FGCOLOR_RED "[FATAL] " msg COLOR_RESET, ##__VA_ARGS__)
#ifdef DEBUG_MODE
#define DEBUG(msg, ...) LOG("[DEBUG] " msg, ##__VA_ARGS__)
#else
#define DEBUG(msg, ...)
#endif

#define MEM32(ADDR) (*(volatile unsigned long *) (ADDR))

inline u16 SWAP_BYTES(u16 x) {
	return ((x << 8) | (x >> 8));
}

void padString(std::string& str);
u32 generateEnableValue(u8 input);

