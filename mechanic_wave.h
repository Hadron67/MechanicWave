#ifndef MECHANIC_WAVE_H
#define MECHANIC_WAVE_H
#include<stdlib.h>
#include<math.h>
typedef struct _WaveSource{
	float x;
	float y;
	float amplitude;
	float frequency;
	float phase;
	int state;
	float vx;
	float vy;
}Wavesource;
typedef struct _MechanicWave {
	float* wave;
	float* wavev;
	float* v;
	Wavesource** sources;
	int sourcecount;
	int height;
	int width;
	float time;
	int boundaryType;
	float dx;
}MechanicWave;
Wavesource* mechanic_wave_source_new(float x,float y,float a,float f,float p);
MechanicWave* mechanic_wave_new(int height,int width,float dx,float v);
int mechanic_wave_add_source(MechanicWave* target,Wavesource* newsource);
void mechanic_wave_remove_source(MechanicWave* target);
void mechanic_wave_free(MechanicWave* w);
void mechanic_wave_react(MechanicWave* w,float dt);
void mechanic_wave_reset(MechanicWave* w);
void mechanic_wave_set_boundarytype(MechanicWave* _this,int type);
float mechanic_wave_get_maxvalue(MechanicWave* _this);
float mechanic_wave_get_maxvelocity(MechanicWave* _this);
#endif
