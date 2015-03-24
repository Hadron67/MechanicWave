#include<gtk/gtk.h>
#include<gdk/gdk.h>
#include<stdio.h>
#include "window1.h"
void* globals[10];
int main(int agv,char* ags[]){
	if(!g_thread_supported()) g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();
	gtk_init(&agv,&ags);
	window1* window;
	window=window1_new();
	gtk_widget_show_all(window);
	gtk_main();
	gdk_threads_leave();
	return 0;
}
