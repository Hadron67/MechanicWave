#include "messagebox.h"
#include<stdlib.h>
typedef GtkWidget MessageBox;
void on_window_destroy(GtkWidget* window,char* s){
	//free(s);
}
MessageBox* messagebox_new(char* title,char* content){
	GtkWidget* window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),title);
	GtkWidget* label=gtk_label_new(content);
	gtk_container_add(GTK_CONTAINER(window),label);
	gtk_widget_set_usize((window),400,300);
	gtk_widget_set_uposition(window,400,400);
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(on_window_destroy),content);
	return window;
}
void msgbox_error(GtkWidget* parent,char* title,char* content){
	GtkWidget* dialogue;
	dialogue=gtk_message_dialog_new(GTK_WINDOW(parent),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s",content);
	gtk_window_set_title(GTK_WINDOW(dialogue),title);
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

