#pragma once
#include "imports.h"
#define X_MIN  -20.0f
#define X_MAX  20.0f
#define Y_MIN  -60.0f
#define Y_MAX  60.0f
#define SC_WIDTH 2000
#define SC_HEIGHT 1000
#define RANGE 1000 +1
#define X_STEP (X_MAX - X_MIN) /(RANGE-1)

 const size_t BUF_SIZE = static_cast<int>(RANGE) * 20; //buffer size to get data from server, big numbner so it can get all cordinates in one  call


