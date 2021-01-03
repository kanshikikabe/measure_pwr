#pragma once

#include <xil_printf.h>

// Uncomment to enable debug messages
#define DEBUG_MODE

#define CLEARSCREEN 	"[2J"
#define ASCII_ESC 		27
#define MOVE_TO(X, Y) "[" #X ";" #Y "H"
#define SEND_CODE(code) xil_printf("%c" code, ASCII_ESC)

#define LOGGER_USE_NATIVE_PRINTF 1

#ifdef LOGGER_USE_NATIVE_PRINTF
#include <cstdio>
#define LOG(fmt, ...) std::printf(fmt "\r\n", ##__VA_ARGS__)
#else
#error "Not using the glorious std::printf" // xil_printf() is shit
#define LOG(fmt, ...) xil_printf(fmt "\r\n", ##__VA_ARGS__)
#endif

#define ERR(msg, ...) LOG("[ERROR] " msg)
#define FATAL(msg, ...) LOG("[FATAL] " msg)
#ifdef DEBUG_MODE
#define DEBUG(msg, ...) LOG("[DEBUG] " msg)
#else
#define DEBUG(msg, ...)
#endif


