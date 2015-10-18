#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <QDir>
#include <QListWidget>
#include <QComboBox>
#include "../file/format.h"

extern std::string FILEPATH;
extern format_v_2_0_1::file_game game_data;


class common
{

public:
    common();
    ~common();
	static void fill_files_combo(std::string directory, QComboBox* combo, bool show_none=false);
    static void fill_graphicfiles_listwidget(std::string directory, QListWidget* listWidget);
    static void fill_iatypes_combo(QComboBox* combo);
    static void fill_npc_combo(QComboBox* combo);
	static void fill_object_combo(QComboBox* combo);

private:


};


#endif // COMMON_H
