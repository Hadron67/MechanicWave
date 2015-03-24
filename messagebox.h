#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
#include<gtk/gtk.h>
#define msgbox(title,content) gtk_widget_show_all(messagebox_new(title,content))
typedef GtkWidget MessageBox;
MessageBox* messagebox_new(char* title,char* content);
void msgbox_error(GtkWidget* parent,char* title,char* content);
#endif
