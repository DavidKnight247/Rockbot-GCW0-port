#include "common.h"

common::common()
{
}

common::~common()
{
}


void common::fill_files_combo(std::string directory, QComboBox* combo, bool show_none)
{
    std::string str_filepath(FILEPATH+directory);
    QString filepath(str_filepath.c_str());
    QDir *dir = new QDir(filepath);
    if (!dir->exists()) {
        std::cout << ">> MainWindow::fill_files_combo ERROR: Directory '" << str_filepath << " does not exist. <<" << std::endl;
        exit(-1);
    }
    dir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir->setSorting(QDir::Size | QDir::Reversed);
	if (show_none == true) {
		combo->addItem(QString("None"));
	}
    QFileInfoList list = dir->entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
		combo->addItem(QString(fileInfo.fileName()));
    }
    combo->repaint();
}

void common::fill_graphicfiles_listwidget(std::string directory, QListWidget* listWidget)
{
    QListWidgetItem* item;

    std::string str_filepath(FILEPATH+directory);
    QString filepath(str_filepath.c_str());
    QDir *dir = new QDir(filepath);
    if (!dir->exists()) {
		//std::cout << ">> MainWindow::fill_graphicfiles_listwidget ERROR: Directory '" << str_filepath << " does not exist. <<" << std::endl;
        exit(-1);
    }
    dir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir->setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir->entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        item = new QListWidgetItem;
        item->setText(fileInfo.fileName());
        std::string filename = FILEPATH + directory + "/" + fileInfo.fileName().toStdString();
		//std::cout << ">> MainWindow::fill_graphicfiles_listwidget DEBUG: filename: '" << filename << std::endl;
        QPixmap image(filename.c_str());
        image = image.copy(0, 0, image.width(), image.height());
        image = image.scaled(32, 32);
        item->setIcon(image);
        listWidget->addItem(item);
    }
    listWidget->repaint();
}


void common::fill_iatypes_combo(QComboBox* combo)
{
    char* IA_TYPE_NAMES[IA_TYPES_COUNT] = { "IA_STAND", "IA_WAIT", "IA_FOLLOW", "IA_ZIGZAG", "IA_SIDETOSIDE", "IA_BAT", "IA_ROOF_SHOOTER", "IA_GROUND_SHOOTER", "IA_SHOOT_AND_GO", "IA_FLY_ZIG_ZAG", "IA_BUTTERFLY", "IA_GO_LEFT", "IA_GO_RIGHT", "IA_FIXED_JUMPER", "IA_SIDE_SHOOTER", "IA_GHOST", "IA_FISH", "IA_DOLPHIN" };

    for (int i=0; i<IA_TYPES_COUNT; i++) {
        combo->addItem(QString(IA_TYPE_NAMES[i]));
    }
    combo->update();
}

void common::fill_npc_combo(QComboBox* combo)
{
    for (int i=0; i<GAME_MAX_OBJS; i++) {
        if (game_data.game_npcs[i].id != -1) {
            QString temp_str = QString("[") + QString::number(game_data.game_npcs[i].id) + QString("]") + QString(game_data.game_npcs[i].name);
            combo->addItem(temp_str);
        }
    }
}

void common::fill_object_combo(QComboBox* combo)
{
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.objects[i].id != -1) {
			QString temp_str = QString("[") + QString::number(game_data.objects[i].id) + QString("]") + QString(game_data.objects[i].name);
			combo->addItem(temp_str);
		}
	}
}
