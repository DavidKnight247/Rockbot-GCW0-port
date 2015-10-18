#include <QtGui/QApplication>
#include <dirent.h>
#include "mediator.h"

int palleteX=0;
int palleteY=0;
Mediator *dataExchanger = new Mediator();
std::string FILEPATH;

#include "../file/format.h"
#include "editortilepallete.h"
#include "editorarea.h"
#include "mainwindow.h"

struct format_v1_0::file_game game;
char EDITOR_FILEPATH[512];

#include "../file/file_io.h"
format_v_2_0_1::file_game game_data;


#undef main

int main(int argc, char *argv[])
{
	std::string EXEC_NAME;
	#ifndef WIN32
		strncpy (EDITOR_FILEPATH, argv[0], strlen(argv[0])-7);
		EXEC_NAME = "editor";
	#else
        strncpy (EDITOR_FILEPATH, argv[0], strlen(argv[0])-11);
		EXEC_NAME = "editor.exe";
	#endif
	printf("*********** FILEPATH: '%s' **************\n", EDITOR_FILEPATH);

	std::string argvString = std::string(argv[0]);
	FILEPATH = argvString.substr(0, argvString.size()-EXEC_NAME.size());

	format_v_2_0_1::file_io fio;
	fio.read_game(game_data);

	dataExchanger->initGameVar();
    QApplication a(argc, argv);
    MainWindow w;
	w.resize(1024, 680);
    w.show();
    return a.exec();
}
