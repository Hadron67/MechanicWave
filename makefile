XX=gcc
GTK=`pkg-config --cflags --libs gtk+-2.0 --libs gthread-2.0`
PYTHON_INCLUDE=-I/usr/include/python3.4
PYTHON_LINKS=-L/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu -lpython3.4m
LIBS=-lm -lgfortran
OBJECTS=main.o window1.o window_python.o mechanic_wave_c.o mechanic_wave_fortran.o mechanic_wave_python.o bundle.o messagebox.o
OBJ_PYTHON_C=mechanic_wave_python.c mechanic_wave_python.c mechanic_wave_python.h mechanic_wave.h bundle.h bundle.c
ARGS=-std=c99
DEBUG=-g -ggdb
wave:$(OBJECTS)
	$(XX) -o wave $(OBJECTS) $(LIBS) $(GTK) $(PYTHON_LINKS) $(DEBUG)
	rm $(OBJECTS)
main.o:main.c window1.h window1.c
	$(XX) -c -o main.o main.c $(GTK) $(ARGS) $(DEBUG)
window1.o:window1.c mechanic_wave.h mechanic_wave.c
	$(XX) -c -o window1.o window1.c $(GTK) $(ARGS) $(DEBUG)
window_python.o:window_python.c mechanic_wave_python.h mechanic_wave_python.c messagebox.c messagebox.h
	$(XX) -c -o window_python.o window_python.c $(GTK) $(ARGS) $(DEBUG)
mechanic_wave_c.o:mechanic_wave.c
	$(XX) -c -o mechanic_wave_c.o mechanic_wave.c $(ARGS) $(DEBUG) 
mechanic_wave_fortran.o:mechanic_wave.f
	$(XX) -c -o mechanic_wave_fortran.o mechanic_wave.f $(DEBUG)
mechanic_wave_python.o:$(OBJ_PYTHON_C)
	$(XX) -c -o mechanic_wave_python.o mechanic_wave_python.c $(PYTHON_INCLUDE) $(ARGS) $(DEBUG) $(GTK)
bundle.o:bundle.c
	$(XX) -c -o bundle.o bundle.c $(ARGS) $(DEBUG)
messagebox.o:messagebox.c
	$(XX) -c -o messagebox.o messagebox.c $(GTK) $(DEBUG)
debug:$(OBJECTS)
	$(DEBUG)=-g -ggdb
	$(XX) -o wave $(OBJECTS) $(LIBS) $(GTK) $(DEBUG)
	rm $(OBJECTS)
.PHONY:clean
soor:
	-rm wave
	-rm $(OBJECTS)
	
