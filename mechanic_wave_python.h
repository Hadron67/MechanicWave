#ifndef MECHANIC_WAVE_PYTHON_H
#define MECHANIC_WAVE_PYTHON_H
#define SEAL(a) __mechanic_wave_py_sealed__ ## a
#include "mechanic_wave.h"
int run_script(MechanicWave* target,char* script,char** errcode);
#endif
