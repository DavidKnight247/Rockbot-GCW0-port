#include <QtGui/QApplication>
#include <dirent.h>
#include "mediator.h"

int palleteX=0;
int palleteY=0;
Mediator *dataExchanger = new Mediator();

#include "format/file_structs.h"
#include "editortilepallete.h"
#include "editorarea.h"
#include "mainwindow.h"

struct file_map_v1 map;
struct file_game game;
char FILEPATH[512];
struct struct_object *obj_list = NULL;
struct struct_object *main_obj_list=NULL; // object list from .obj files


#undef main

int main(int argc, char *argv[])
{
	#ifndef WIN32
	strncpy (FILEPATH, argv[0], strlen(argv[0])-7);
	#else
		strncpy (FILEPATH, argv[0], strlen(argv[0])-11);
	#endif
	printf("*********** FILEPATH: '%s' **************\n", FILEPATH);

	dataExchanger->initGameVar();
    QApplication a(argc, argv);
    MainWindow w;
	w.resize(1024, 680);
    w.show();
    return a.exec();
}
