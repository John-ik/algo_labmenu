#pragma once

#define ANSI_BEGIN "\033["
#define ANSI_END   "m"
#define ANSI(attr) ANSI_BEGIN attr ANSI_END


#define ANSI_RESET "0"
#define ANSI_BOLD   "1"
#define ANSI_BLINK "5"
#define ANSI_COLOR_DEFAULT "39"
#define ANSI_COLOR_BACK_DEFAULT "49"

// ------------------------------------ FOREGROUND ----------------------------
#define ANSI_COLOR_BLACK    "30"
#define ANSI_COLOR_RED      "31"
#define ANSI_COLOR_GREEN    "32"
#define ANSI_COLOR_YELLOW   "33"
#define ANSI_COLOR_BLUE     "34"
#define ANSI_COLOR_MAGENTA  "35"
#define ANSI_COLOR_CYAN     "36"
#define ANSI_COLOR_WHITE    "37"

#define ANSI_COLOR_BRIGHT_BLACK    "90"
#define ANSI_COLOR_BRIGHT_RED      "91"
#define ANSI_COLOR_BRIGHT_GREEN    "92"
#define ANSI_COLOR_BRIGHT_YELLOW   "93"
#define ANSI_COLOR_BRIGHT_BLUE     "94"
#define ANSI_COLOR_BRIGHT_MAGENTA  "95"
#define ANSI_COLOR_BRIGHT_CYAN     "96"
#define ANSI_COLOR_BRIGHT_WHITE    "97"

// ------------------------------------ BACKGROUND ----------------------------
#define ANSI_COLOR_BACK_BLACK    "40"
#define ANSI_COLOR_BACK_RED      "41"
#define ANSI_COLOR_BACK_GREEN    "42"
#define ANSI_COLOR_BACK_YELLOW   "43"
#define ANSI_COLOR_BACK_BLUE     "44"
#define ANSI_COLOR_BACK_MAGENTA  "45"
#define ANSI_COLOR_BACK_CYAN     "46"
#define ANSI_COLOR_BACK_WHITE    "47"

#define ANSI_COLOR_BACK_BRIGHT_BLACK    "100"
#define ANSI_COLOR_BACK_BRIGHT_RED      "101"
#define ANSI_COLOR_BACK_BRIGHT_GREEN    "102"
#define ANSI_COLOR_BACK_BRIGHT_YELLOW   "103"
#define ANSI_COLOR_BACK_BRIGHT_BLUE     "104"
#define ANSI_COLOR_BACK_BRIGHT_MAGENTA  "105"
#define ANSI_COLOR_BACK_BRIGHT_CYAN     "106"
#define ANSI_COLOR_BACK_BRIGHT_WHITE    "107"