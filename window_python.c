#include "mechanic_wave_python.h"
#include "bundle.h"
#include "messagebox.h"
typedef GtkWidget window_python;
void hidewindow(GtkWidget* widget,GtkWidget* data){
	gtk_widget_hide_all(widget);
	printf("hide\n");
}
void btn_run_onclick(GtkWidget* button,Bundle* bundle){
	char *text2;
	GtkTextIter start,end;
   	GtkTextBuffer* buffer=(GtkTextBuffer*)bundle->objects[0];
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
	const GtkTextIter s=start,e=end;
	text2 = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),&s,&e,FALSE);
	char* errcode;
	if(run_script((MechanicWave*)bundle->objects[1],text2,&errcode)){
		//msgbox("Python Runtime Error",errcode);
		msgbox_error((GtkWidget*)bundle->objects[2],"Python Runtime Error",errcode);
	}
	free(text2);
}
window_python* window_python_new(MechanicWave* wave){
	GtkWidget* window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkTextBuffer* buffer_text1=gtk_text_buffer_new(NULL);
	GtkWidget* text1=gtk_text_view_new_with_buffer(buffer_text1);
	GtkWidget* btn_run=gtk_button_new_with_label("run");
	GtkWidget* vbox=gtk_vbox_new(FALSE,10);
	gtk_widget_set_usize((text1),400,300);
	gtk_widget_set_usize((window),400,400);
	gtk_container_add(GTK_CONTAINER(vbox),text1);
	gtk_container_add(GTK_CONTAINER(vbox),btn_run);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_window_set_title(GTK_WINDOW(window),"python console");
	gtk_widget_set_uposition(window,400,400);
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(hidewindow),NULL);
	Bundle* bundle=bundle_new(3);
	bundle->objects[0]=buffer_text1;
	bundle->objects[1]=wave;
	bundle->objects[2]=window;
	g_signal_connect(GTK_OBJECT(btn_run),"clicked",GTK_SIGNAL_FUNC(btn_run_onclick),bundle);
	return window;
}
