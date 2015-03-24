// Library on Mechanic waves
// cfy's Physics Project
#include<stdlib.h>
#include<math.h>
#include "mechanic_wave.h"
//header start
extern void fortranfunc_mechanic_wave_react_(float*,float*,int*,int*,float*,float*,int*,float*);
extern void fortranfunc_mechanic_wave_init_(float*,float*,int*,int*,float*,float*);
extern float fortranfunc_mechanic_wave_getmax_(float*,int*);
//header end
Wavesource* mechanic_wave_source_new(float x,float y,float a,float f,float p){
	Wavesource* source=(Wavesource*)malloc(sizeof(Wavesource));
	source->x=x;
	source->y=y;
	source->amplitude=a;
	source->frequency=f;
	source->phase=p;
	source->state=1;
	source->vx=source->vy=0;
	return source;
}
MechanicWave* mechanic_wave_new(int height,int width,float dx,float v){
	MechanicWave* w;
	w=(MechanicWave*)malloc(sizeof(MechanicWave));
	w->wave=(float*)malloc(sizeof(float)*height*width);
	w->wavev=(float*)malloc(sizeof(float)*height*width);
	w->v=(float*)malloc(sizeof(float)*height*width);
	w->sources=NULL;
	w->sourcecount=0;
	w->dx=dx;
	w->boundaryType=0;
	w->height=height;
	w->width=width;
	fortranfunc_mechanic_wave_init_(w->wave,w->wavev,&w->height,&w->width,w->v,&v);
	w->time=0;
	return w;
}
int mechanic_wave_add_source(MechanicWave* target,Wavesource* newsource){
	if(newsource!=NULL){
		if(target->sources!=NULL){
			target->sources=(Wavesource**)realloc(target->sources,sizeof(void*)*++target->sourcecount);
			target->sources[target->sourcecount-1]=newsource;
		}
		else{
			target->sources=(Wavesource**)malloc(sizeof(void*));
			target->sources[0]=newsource;
			target->sourcecount=1;
		}
	}
	else return -1;
}
void mechanic_wave_remove_source(MechanicWave* target){
	for(int i=0;i<target->sourcecount;i++) free(target->sources[i]);
	free(target->sources);
	target->sources=NULL;
	target->sourcecount=0;
}
void mechanic_wave_free(MechanicWave* w){
	mechanic_wave_remove_source(w);
	free(w->wave);
	free(w->wavev);
	free(w);
};
void mechanic_wave_react(MechanicWave* w,float dt){
	for(int i=0;i<w->sourcecount;i++) {
		w->sources[i]->x+=w->sources[i]->vx*dt/w->dx;
		w->sources[i]->y+=w->sources[i]->vy*dt/w->dx;
		int x=(int)(w->sources[i]->x/w->dx);
		int y=(int)(w->sources[i]->y/w->dx);
		if(x>=w->height-1) w->sources[i]->x=w->dx;
		else if(x<=0) w->sources[i]->x=(w->height-2)*w->dx;
		if(y>=w->width-1) w->sources[i]->y=w->dx;
		else if(y<=0) w->sources[i]->y=(w->width-2)*w->dx;
		x=(int)(w->sources[i]->x/w->dx);
		y=(int)(w->sources[i]->y/w->dx);
		if(w->sources[i]->state) w->wave[w->width*x+y]=w->sources[i]->amplitude*sin(w->sources[i]->frequency*w->time+w->sources[i]->phase);
	}
	fortranfunc_mechanic_wave_react_(w->wave,w->wavev,&w->height,&w->width,&w->dx,&dt,&w->boundaryType,w->v);
	w->time+=dt;
}
void mechanic_wave_reset(MechanicWave* w){
	for(int i=0;i<w->width*w->height;i++) w->wave[i]=w->wavev[i]=0;
}
void mechanic_wave_set_boundarytype(MechanicWave* _this,int type){
	_this->boundaryType=type;
	int index;
	for(int i=0;i<_this->width;i++){
		index=i+_this->width*(_this->height-1);
		_this->wave[i]=_this->wavev[i]=0;
		_this->wave[index]=_this->wavev[index]=0;
	}
	for(int i=0;i<_this->height;i++){
		index=i*_this->width;
		_this->wave[index]=_this->wavev[index]=0;
		_this->wave[index+_this->width-1]=_this->wavev[index+_this->width-1]=0;
	}
}
float mechanic_wave_get_maxvalue(MechanicWave* _this){
	int len=_this->width*_this->height;
	return fortranfunc_mechanic_wave_getmax_(_this->wave,&len);
}
float mechanic_wave_get_maxvelocity(MechanicWave* _this){
	int len=_this->width*_this->height;
	return fortranfunc_mechanic_wave_getmax_(_this->wavev,&len);
}
