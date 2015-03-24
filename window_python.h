#ifndef WINDOW_PYTHON_H
#define WINDOW_PYTHON_H
#include<gtk/gtk.h>
#include "mechanic_wave.h"
typedef GtkWidget window_python;
window_python* window_python_new(MechanicWave* wave);
#endif
