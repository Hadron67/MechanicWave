#ifndef WINDOW1_C
#define WINDOW1_C
#include "window1.h"
#include<gdk/gdk.h>
#include "mechanic_wave.h"
#include "window_python.h"
#include "bundle.h"
typedef GtkWidget window1;
int emulationswitch=1;
void closeApp(GtkWidget* window,gpointer data){
	emulationswitch=0;
	gtk_main_quit();
}

void gdk_color_set_hsv(GdkColor* color,float h,float s,float v){
	float rgb[3];
	for(int offset=240,i=0;i<3;i++,offset-=120) {
		float y=h+offset;
		while(y>=360) y-=360;
		float x=abs(y-240.0);
		if(x<=60) rgb[i]=0xffff;
		else if(60<x && x<120) rgb[i]=((1-(x-60)/60)*0xffff);
		else rgb[i]=0;
	}
	for(int i=0;i<3;i++) rgb[i]+=(0xffff-rgb[i])*(1-s);
	for(int i=0;i<3;i++) rgb[i]*=v;
	color->red=rgb[0];
	color->green=rgb[1];
	color->blue=rgb[2];
}
void set_colormap(GdkColor* color,float w){
	//XXX:need a better choice
	color->red=0xffff;
	color->green=(atan(w)/1.6+1)/2*0xffff;
	color->blue=0xffff;
}
void draw_mechanic_wave(GtkWidget* widget,MechanicWave* w){
	gdk_threads_enter();
	GdkGC* gc=gdk_gc_new(widget->window);
	GdkColor color;
	GdkPixmap* map;
	map=gdk_pixmap_new(widget->window,widget->allocation.width,widget->allocation.height,-1);
	gdk_draw_rectangle(map,widget->style->white_gc,TRUE,0,0,widget->allocation.width,widget->allocation.height);
	int index;
	for(int i=0;i<w->height;i++)
		for(int j=0;j<w->width;j++){
			index=i*w->width+j;
			set_colormap(&color,w->wave[index]);
			gdk_gc_set_rgb_fg_color(gc,&color);
			gdk_draw_point(map,gc,i,j);
		}
	gdk_draw_drawable(widget->window,gc,map,0,0,0,0,widget->allocation.width,widget->allocation.height);
	g_object_unref(map);
	g_object_unref(gc);
	gdk_threads_leave();
}
void onClick(GtkWidget* button,MechanicWave* d){
	emulationswitch=!emulationswitch;
	printf("you clicked the button!\n");
}
void btn_reset_onclick(GtkWidget* button,MechanicWave* d){
	mechanic_wave_reset(d);
}
void btn_python_onclick(GtkWidget* button,MechanicWave* wave){
	window_python* w=window_python_new(wave);
	gtk_widget_show_all(w);
}
static void time_handler(Bundle* bundle){
	MechanicWave* w=(MechanicWave*)bundle->objects[1];
	GtkWidget* draw=(GtkWidget*)bundle->objects[0];
	bundle_free(bundle);
	while(1){
		if(emulationswitch){
			mechanic_wave_react(w,0.1);
			mechanic_wave_react(w,0.1);
			mechanic_wave_react(w,0.1);
			mechanic_wave_react(w,0.1);
			draw_mechanic_wave(draw,w);
			
			g_usleep(10);
		}
	}
}
void initwave(MechanicWave* w){
	w->boundaryType=1;
	mechanic_wave_add_source(w,mechanic_wave_source_new(200,150,400,2,0));
	mechanic_wave_add_source(w,mechanic_wave_source_new(200,250,400,2,3.1415926));
	//for(int i=50;i<350;i++) mechanic_wave_add_source(w,mechanic_wave_source_new(i,100,60,2,0));
	//for(int i=50;i<350;i++) mechanic_wave_add_source(w,mechanic_wave_source_new(i,300,60,2,3.1415926));
	/*for(int i=1;i<400;i++){
		if(abs(i-100)>5&&abs(i-300)>5) mechanic_wave_add_source(w,mechanic_wave_source_new(i,200,0,0,0));
	}*/
}
window1* window1_new(){
	GtkWidget* window;
	GtkWidget* button1;
	GtkWidget* vbox;
	GtkWidget* draw;
	GtkWidget* fixed;
	GtkWidget* btn_reset=gtk_button_new_with_label("reset");
	GtkWidget* btn_python=gtk_button_new_with_label("script");
	MechanicWave* wave;
	wave=mechanic_wave_new(400,400,0.4,1.2);
//	initwave(wave);
	fixed=gtk_fixed_new();
	draw=gtk_drawing_area_new();
	gtk_drawing_area_size(GTK_DRAWING_AREA(draw),400,400);
	vbox=gtk_vbox_new(FALSE,40);
	button1=gtk_button_new_with_label("start/stop");
	gtk_widget_set_usize((button1),100,40);
	gtk_widget_set_usize((btn_reset),100,40);
	gtk_widget_set_usize((btn_python),120,40);
	char title[]="Mechanic Wave Emulation";
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(closeApp),NULL);
	g_signal_connect(GTK_OBJECT(button1),"clicked",GTK_SIGNAL_FUNC(onClick),wave);
	g_signal_connect(GTK_OBJECT(btn_reset),"clicked",GTK_SIGNAL_FUNC(btn_reset_onclick),wave);
	g_signal_connect(GTK_OBJECT(btn_python),"clicked",G_CALLBACK(btn_python_onclick),wave);
	gtk_container_add(GTK_CONTAINER(window),fixed);
	gtk_fixed_put(GTK_FIXED(fixed),draw,0,0);
	gtk_fixed_put(GTK_FIXED(fixed),button1,0,400);
	gtk_fixed_put(GTK_FIXED(fixed),btn_reset,100,400);
	gtk_fixed_put(GTK_FIXED(fixed),btn_python,200,400);
	gtk_window_set_title(GTK_WINDOW(window),title);
	gtk_widget_set_usize((window),400,440);
	gtk_widget_set_uposition((window),200,200);
	//send wave and drawing area to a new thread.
	Bundle* bundle=bundle_new(3);
	bundle->objects[0]=draw;
	bundle->objects[1]=wave;
	g_thread_new("draw_wave",(GThreadFunc)time_handler,bundle);
	return window;
}
#endif
