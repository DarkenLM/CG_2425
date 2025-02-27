#pragma once
#include <iostream>

#define ESCAPE_STR(x) #x
#define STR(x) ESCAPE_STR(x)

#define LOG_HEADER "[" __FILE__ ":" STR(__LINE__) "] "

#define ERROR std::cerr << LOG_HEADER

/* It's 3 in the fucking morning, don't ask. */
#define fuckAround try
#define findOut catch
#define yeet throw