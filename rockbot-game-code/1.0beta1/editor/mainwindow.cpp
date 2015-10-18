#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"
#include "addwizard.h"
#include "dialognpcedit.h"
#include "dialogobjectedit.h"
#include "loadgamepicker.h"
#include "dialog_pick_color.h"
#include <QStandardItemModel>
#include <QListView>
#include <QList>
#include <QListWidgetItem>
#include "../defines.h"

#include "common.h"

extern Mediator *dataExchanger;
extern char EDITOR_FILEPATH[512];
extern std::string FILEPATH;

extern format_v_2_0_1::file_game game_data;
bool background_filled = false;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _npcedit_tab_selectednpc(0)
{
	QStandardItem* temp_game_item;
	QStandardItem* temp_map_item;
	QStandardItem* temp_sub_map_item;
	int n, n_game;
	char buffer[512];
	int i, j;

	ui->setupUi(this);
	dataExchanger->loadGame(1);
	fillStageList();
	fillMapList();
	fillNpcList();
	fillObjList();

	fill_background_list();

	ui->editArea->repaint();
	QObject::connect(ui->pallete, SIGNAL(signalPalleteChanged()), ui->editArea, SLOT(repaint()));
	// add maps to the list
	model = new QStandardItemModel();

	QStandardItem* games = new QStandardItem("GAMES");
/*
	sprintf(buffer, "%s/data/game/", EDITOR_FILEPATH);
	n = dataExchanger->freeSlot(buffer, "gme");
	for (i=1; i<n; i++) {
		dataExchanger->centNumberFormat(i);
		sprintf(buffer, "Game %s", dataExchanger->centNumber);
		temp_game_item = new QStandardItem(buffer);
		temp_game_item->setEditable(FALSE);
		games->appendRow(temp_game_item);


		sprintf(buffer, "%s/data/game/game_%s/", EDITOR_FILEPATH, dataExchanger->centNumber);
		if (!QDir(buffer).exists()) {
			//printf("WARNING: Creating '%s'' folder\n", buffer);
			QDir().mkdir(buffer);
		}
		n_game = dataExchanger->freeSlot(buffer, "map");
		temp_map_item = new QStandardItem("MAPS");
		temp_game_item->appendRow(temp_map_item);
		for (j=1; j<n_game; j++) {
			dataExchanger->centNumberFormat(j);
			sprintf(buffer, "%s.map", dataExchanger->centNumber);
			temp_sub_map_item = new QStandardItem(buffer);
			temp_map_item->appendRow(temp_sub_map_item);
			//QObject::connect(ui->pallete, SIGNAL(signalPalleteChanged()), ui->editArea, SLOT(repaint()));
		}

	}
	*/
	QStandardItem* npcs = new QStandardItem("NPCs");
	npcs->setEditable(FALSE);
	QStandardItem* npc1 = new QStandardItem("NPC 001");
	npc1->setEditable(FALSE);
	npcs->appendRow(npc1);

	model->appendRow(games);
	model->appendRow(npcs);
	//iStandardModel->appendRow(item2);
	model->setHeaderData(0, Qt::Horizontal, "Item List");
	//ui->mapList->setModel(model);
	//parent->restoreGeometry();
	currentProjectileIndex = 0;

	fillProjectileImagesList();
	fillTrajectoriesList();
	reloadProjectileListCombo();
	fill_player_graphics_list();

	//fill_stage_tab_data();
	fill_players_data();


	// insert NPC tab form
	npc_edit_tab = new npc_edit();
	ui->npc_tab->layout()->addWidget(npc_edit_tab);

	// insert AI tab form
	ai_edit_tab = new ai_tab();
	ui->ai_tab->layout()->addWidget(ai_edit_tab);

	// insert OBJECT tab form
	object_edit_tab = new object_tab();
	ui->object_tab->layout()->addWidget(object_edit_tab);


	// insert OBJECT tab form
	weapon_edit_tab = new weapon_edit();
	ui->weapon_edit_tab->layout()->addWidget(weapon_edit_tab);

	// insert STAGE tab form
	stage_edit_tab = new stage_edit();
	ui->stage_tab->layout()->addWidget(stage_edit_tab);
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::fillStageList() {
	char directory[512], buffer[512];
	bool fim = false;
	int stage_n = 1;

    /*
	printf("&&& fillStageList.START &&&\n");

	dataExchanger->centNumberFormat(1);
	sprintf(directory, "%s/data/game/game_%s/", EDITOR_FILEPATH, dataExchanger->centNumber);

	while (!fim) {
		dataExchanger->centNumberFormat(stage_n);
		sprintf(buffer, "%s/stage_%s", directory, dataExchanger->centNumber);
		if (!QDir(buffer).exists()) {
			return;
		}
		ui->stageListCombo->addItem (dataExchanger->centNumber);
		stage_n++;
	}
	printf("&&& fillStageList.END &&&\n");
    */

    for (int i=0; i<MAX_STAGES; i++) {
        if (game_data.stages[i].id != -1) {
            QString temp_name(QString("[")+QString::number(game_data.stages[i].id)+QString("] ")+QString(game_data.stages[i].name));
            ui->stageListCombo->addItem(temp_name);
        }
    }
}

void MainWindow::fillMapList() {
	char buffer[512];
	int i, max;

	max = ui->mapListCombo->count();
	for (i=0; i<max; i++) {
		ui->mapListCombo->removeItem(0);
	}
	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(buffer, "%s/data/game/game_%s/", EDITOR_FILEPATH, dataExchanger->centNumber);
	/*
	n_game = dataExchanger->freeSlot(buffer, "map");
	for (i=1; i<n_game; i++) {
		dataExchanger->centNumberFormat(i);
		sprintf(buffer, "%s.map", dataExchanger->centNumber);
		ui->mapListCombo->addItem(buffer);
	}
	*/
	for (i=0; i<3; i++) {
		dataExchanger->centNumberFormat(i);
		sprintf(buffer, "%s", dataExchanger->centNumber);
		ui->mapListCombo->addItem(buffer);
	}
}


void MainWindow::fillNpcList() {
	QListWidgetItem* item;
	QListWidgetItem* item_to_del;

	int max = ui->listWidget->count();
	// remove previous ones
	for (int i=0; i<max; i++) {
		//printf(">>> Removing item %d from list, count: %d\n", i, ui->listWidget->count());
		item_to_del = ui->listWidget->takeItem(0);
		delete item_to_del;
	}

	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.game_npcs[i].id != -1) {
			item = new QListWidgetItem;
			item->setText(game_data.game_npcs[i].name);
			std::string filename = FILEPATH + "/data/images/sprites/enemies/" + game_data.game_npcs[i].graphic_filename;
			QPixmap image(filename.c_str());
			image = image.copy(0, 0, game_data.game_npcs[i].frame_size.width, game_data.game_npcs[i].frame_size.heigth);
			image = image.scaled(32, 32);
			item->setIcon(image);
			ui->listWidget->addItem(item);
		}
	}

	ui->listWidget->update();
}


void MainWindow::fillObjList() {

	char buffer[512], directory[512], filename[512];
	int i, n_game, max;
	QListWidgetItem* item;
	QListWidgetItem* item_to_del;
	FILE *fp;
	struct format_v1_0::st_object temp_object;

	max = ui->objectListWidget->count();
	// remove previous ones
	for (i=0; i<max; i++) {
		item_to_del = ui->objectListWidget->takeItem(0);
		delete item_to_del;
	}

	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.objects[i].id != -1) {
			item = new QListWidgetItem;
			item->setText(game_data.objects[i].name);
			std::string filename = FILEPATH + "/data/images/sprites/objects/" + game_data.objects[i].graphic_filename;
			QPixmap image(filename.c_str());
			image = image.copy(0, 0, game_data.objects[i].size.width, game_data.objects[i].size.heigth);
			image = image.scaled(32, 32);
			item->setIcon(image);
			ui->objectListWidget->addItem(item);
		}
	}


	ui->objectListWidget->update();
}


void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionSave_triggered()
{
    std::cout << "on_actionSave_triggered" << std::endl;
	ui->editArea->saveGame(dataExchanger->currentGame);
}

void MainWindow::on_actionOpen_triggered()
{
	QDialog *open = new loadGamePicker;
	open->show();
	//dataExchanger->loadGame();
	//ui->editArea->repaint();
}

void MainWindow::on_pallete_signalPalleteChanged()
{
	printf("DEBUG on_pallete_signalPalleteChanged\n");
}

/*
void MainWindow::on_addNpcButton_clicked()
{
	printf("DEBUG.MainWindow::on_addNpcButton_clicked - NpcAddNumber: '%d'\n", dataExchanger->NpcAddNumber);
	dataExchanger->editMode = EDITMODE_ADDNPC;
}
*/




void MainWindow::on_actionNew_triggered()
{
	dataExchanger->createGame();
	ui->editArea->repaint();
}

void MainWindow::reloadComboItems(void) {
	printf(">>>>> MainWindow::reloadComboItems\n");
	fillStageList();
	fillMapList();
	fillNpcList();
	fillObjList();
}

// ------------------- EDIT BUTTONS --------------------- //



void MainWindow::on_actionLock_Edit_triggered()
{
	if (ui->actionLock_Edit->isChecked()) {
		ui->actionNormal_Edit->setChecked(FALSE);
		ui->actionEraser->setChecked(FALSE);
		ui->actionFill->setChecked(FALSE);
		ui->actionLink->setChecked(FALSE);
		ui->actionStairs->setChecked(FALSE);
		ui->toolBox->setCurrentWidget(ui->Terrain);
		dataExchanger->editTool = EDITMODE_LOCK;
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionLock_Edit->setChecked(TRUE);
	}
	ui->editArea->repaint();
}

void MainWindow::on_actionNormal_Edit_triggered()
{
	if (ui->actionNormal_Edit->isChecked()) {
		ui->actionLock_Edit->setChecked(FALSE);
		ui->actionEraser->setChecked(FALSE);
		ui->actionFill->setChecked(FALSE);
		ui->actionLink->setChecked(FALSE);
		ui->actionStairs->setChecked(FALSE);
		dataExchanger->editTool = EDITMODE_NORMAL;
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionNormal_Edit->setChecked(TRUE);
	}
	ui->editArea->repaint();
}

void MainWindow::on_actionEraser_triggered()
{
	if (ui->actionEraser->isChecked()) {
		ui->actionLock_Edit->setChecked(FALSE);
		ui->actionNormal_Edit->setChecked(FALSE);
		ui->actionFill->setChecked(FALSE);
		ui->actionLink->setChecked(FALSE);
		ui->actionStairs->setChecked(FALSE);
		dataExchanger->editTool = EDITMODE_ERASER;
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionEraser->setChecked(TRUE);
	}
}




void MainWindow::on_actionFill_triggered()
{
	if (ui->actionFill->isChecked()) {
		ui->actionLock_Edit->setChecked(FALSE);
		ui->actionNormal_Edit->setChecked(FALSE);
		ui->actionEraser->setChecked(FALSE);
		ui->actionLink->setChecked(FALSE);
		ui->actionStairs->setChecked(FALSE);
		dataExchanger->editTool = EDITMODE_FILL;
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionFill->setChecked(TRUE);
	}
}


void MainWindow::on_MainWindow_iconSizeChanged(QSize iconSize)
{
	Q_UNUSED (iconSize);
	saveGeometry();
}

void MainWindow::on_actionLink_triggered()
{
	ui->toolBox->setCurrentWidget(ui->page_Link);
	if (ui->actionLink->isChecked()) {
		ui->actionLock_Edit->setChecked(FALSE);
		ui->actionNormal_Edit->setChecked(FALSE);
		ui->actionEraser->setChecked(FALSE);
		ui->actionFill->setChecked(FALSE);
		ui->actionStairs->setChecked(FALSE);
		ui->actionAdd_Object->setChecked(FALSE);
		dataExchanger->editTool = EDITMODE_LINK;
		ui->editArea->repaint();
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionFill->setChecked(TRUE);
		ui->editArea->repaint();
	}
}


void MainWindow::on_actionStairs_triggered()
{
	if (ui->actionStairs->isChecked()) {
		ui->actionNormal_Edit->setChecked(FALSE);
		ui->actionEraser->setChecked(FALSE);
		ui->actionFill->setChecked(FALSE);
		ui->actionLock_Edit->setChecked(FALSE);
		ui->actionLink->setChecked(FALSE);
		ui->actionAdd_Object->setChecked(FALSE);
		dataExchanger->editTool = EDITMODE_STAIRS;
	// to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
	} else {
		ui->actionStairs->setChecked(TRUE);
	}
	ui->editArea->repaint();
}


void MainWindow::on_actionAdd_Object_triggered()
{
	ui->toolBox->setCurrentWidget(ui->page_objects);
	ui->actionEdit_Tileset->setChecked(FALSE);
	ui->actionEdit_NPC->setChecked(FALSE);
	ui->actionNormal_Edit->setChecked(TRUE);
	ui->actionEraser->setChecked(FALSE);
	ui->actionFill->setChecked(FALSE);
	ui->actionLock_Edit->setChecked(FALSE);
	ui->actionLink->setChecked(FALSE);
	ui->actionAdd_Object->setChecked(TRUE);
	dataExchanger->editMode = EDITMODE_OBJECT;
	dataExchanger->editTool = EDITMODE_NORMAL;
    // to make things simpler, we do not allow "uncheck" of the tool, you must pick another one to uncheck
    ui->editArea->repaint();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
	dataExchanger->layerLevel = index+1;
	ui->editArea->repaint();
}

void MainWindow::on_actionEdit_NPC_triggered()
{
	ui->toolBox->setCurrentWidget(ui->page_npc);
	ui->actionEdit_Tileset->setChecked(FALSE);
	ui->actionAdd_Object->setChecked(FALSE);
	ui->actionEdit_NPC->setChecked(TRUE);
	ui->actionFill->setDisabled(TRUE);
	ui->actionLink->setDisabled(TRUE);
	ui->actionLock_Edit->setDisabled(TRUE);
	ui->actionStairs->setDisabled(TRUE);
	dataExchanger->editMode = EDITMODE_NPC;
}

void MainWindow::on_actionEdit_Tileset_triggered()
{
	ui->actionEdit_NPC->setChecked(FALSE);
	ui->actionAdd_Object->setChecked(FALSE);
	ui->actionEdit_Tileset->setChecked(TRUE);
	ui->actionFill->setDisabled(FALSE);
	ui->actionLink->setDisabled(FALSE);
	ui->actionLock_Edit->setDisabled(FALSE);
	ui->actionStairs->setDisabled(FALSE);
	ui->toolBox->setCurrentWidget(ui->page_map);
	dataExchanger->editMode = EDITMODE_NORMAL;
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
	printf(">>>> MainWindow::on_listWidget_currentRowChanged, row: %d\n", currentRow);
	dataExchanger->selectedNPC = currentRow;
}

void MainWindow::on_editNPCButton_clicked()
{
	dataExchanger->editModeNPC = 1;
	QDialog *npc_editor = new DialogNPCEdit;
	npc_editor->show();
	QObject::connect(npc_editor, SIGNAL(finishedNPCEditor()), this, SLOT(reloadComboItems()));
}

void MainWindow::on_newNPCButton_clicked()
{
	// search for an empty slot
	int selected_obj = -1;
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.game_npcs[i].id == -1) {
			selected_obj = i;
			break;
		}
	}
	if (selected_obj == -1) {
		QMessageBox msgBox;
		msgBox.setText("There is no space left to create more NPCs, limit reached.");
		msgBox.exec();
		return;
	}
	dataExchanger->selectedNPC = selected_obj;
	dataExchanger->editModeNPC = 0;
	QDialog *npc_editor = new DialogNPCEdit;
	npc_editor->show();
	QObject::connect(npc_editor, SIGNAL(finishedNPCEditor()), this, SLOT(reloadComboItems()));
}

void MainWindow::on_actionOne_triggered()
{
    dataExchanger->layerLevel = 1;
    ui->editArea->repaint();
}

void MainWindow::on_actionTwo_triggered()
{
    dataExchanger->layerLevel = 2;
    ui->editArea->repaint();
}

void MainWindow::on_actionThree_triggered()
{
    dataExchanger->layerLevel = 3;
    ui->editArea->repaint();
}


void MainWindow::on_mapListCombo_currentIndexChanged(int index)
{
	if (index < 0) {
		return;
	}
	//dataExchanger->loadStageMap(dataExchanger->currentGame, dataExchanger->currentStage, index);
	dataExchanger->currentMap = index;
	fill_background_list();
	ui->editArea->repaint();
}

void MainWindow::on_editObjectButton_clicked()
{
	dataExchanger->editModeNPC = 1;
	QDialog *object_editor = new DialogObjectEdit ();
	object_editor->show();
	QObject::connect(object_editor, SIGNAL(finishedObjectEditor()), this, SLOT(reloadComboItems()));
}

void MainWindow::on_newObjectButton_clicked()
{
	// search for an empty slot
	int selected_obj = -1;
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.objects[i].id == -1) {
			selected_obj = i;
			break;
		}
	}
	if (selected_obj == -1) {
		QMessageBox msgBox;
		msgBox.setText("There is no space left to create more objects, limit reached.");
		msgBox.exec();
		return;
	}
	dataExchanger->selectedNPC = selected_obj;
	dataExchanger->editModeNPC = 0;
	QDialog *object_editor = new DialogObjectEdit ();
	object_editor->show();
	QObject::connect(object_editor, SIGNAL(finishedObjectEditor()), this, SLOT(reloadComboItems()));
}

void MainWindow::on_objectListWidget_currentRowChanged(int currentRow)
{
	dataExchanger->selectedNPC = currentRow;
	printf(">>>> MainWindow::on_objectListWidget_currentRowChanged, row: %d, dataExchanger->selectedNPC: %d\n", currentRow, dataExchanger->selectedNPC);
}

void MainWindow::on_toolBox_currentChanged(int index)
{
	Q_UNUSED (index);
	dataExchanger->selectedNPC = -1;
}

void MainWindow::on_pushButton_clicked()
{
	//dataExchanger->addMap();
	fillMapList();
}

void MainWindow::on_listWidget_2_currentRowChanged(int currentRow)
{
	dataExchanger->terrainType = currentRow+1;
}

void MainWindow::on_spinBox_valueChanged(int value)
{
        dataExchanger->zoom = value;
}

void MainWindow::on_link_orientation_combobox_currentIndexChanged(int index)
{
	dataExchanger->link_type = index;
}

void MainWindow::on_npc_direction_combo_currentIndexChanged(int index)
{
	dataExchanger->npc_direction = index;
}

void MainWindow::on_stageListCombo_currentIndexChanged(int index)
{
    dataExchanger->currentMap = 0;
    dataExchanger->currentStage = index;
	ui->mapListCombo->setCurrentIndex(0);
	fill_background_list();
	ui->editArea->repaint();
}


void MainWindow::fill_player_graphics_list()
{
	QDir *dir = new QDir(QString(EDITOR_FILEPATH)+"/data/images/sprites/");
	if (!dir->exists()) {
		printf(">> Directory does not exist. <<");
		return;
	}
	dir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	dir->setSorting(QDir::Size | QDir::Reversed);
	QFileInfoList list = dir->entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		//printf("addWizard::fillNPCGraphicsList - fileName: %s\n", qPrintable(fileInfo.fileName()));

		ui->player_graphics_combo->addItem(fileInfo.fileName());
	}
	//printf(">> dataExchanger::fillNPCGraphicsList->addNpcFilename: '%s'\n", dataExchanger->addNpcFilename);
	ui->player_graphics_combo->repaint();
}


void MainWindow::fillProjectileImagesList()
{
	QDir *dir = new QDir(QString(EDITOR_FILEPATH)+"/data/images/projectiles/");
	if (!dir->exists()) {
		printf(">> Directory does not exist. <<");
		return;
	}
	dir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	dir->setSorting(QDir::Size | QDir::Reversed);
	QFileInfoList list = dir->entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		//printf("addWizard::fillNPCGraphicsList - fileName: %s\n", qPrintable(fileInfo.fileName()));

		ui->projectile_imagefile_combo->addItem(fileInfo.fileName());
	}
	//printf(">> dataExchanger::fillNPCGraphicsList->addNpcFilename: '%s'\n", dataExchanger->addNpcFilename);
	ui->projectile_imagefile_combo->repaint();
}

void MainWindow::fillTrajectoriesList()
{
	char* PROJECTILE_TRAJECTORIES_NAMES[PROJECTILE_TRAJECTORIES_COUNT] = { "TRAJECTORY_LINEAR", "TRAJECTORY_ARC", "TRAJECTORY_SIN", "TRAJECTORY_CHAIN", "TRAJECTORY_QUAKE", "TRAJECTORY_BOMB", "TRAJECTORY_FOLLOW" };

	for (int i=0; i<PROJECTILE_TRAJECTORIES_COUNT; i++) {
		ui->projectile_trajectorytype_combo->addItem(QString(PROJECTILE_TRAJECTORIES_NAMES[i]));
	}
}


void MainWindow::on_pushButton_5_clicked()
{
	createProjectile();
}

void MainWindow::createProjectile()
{
	FILE *fp;
	char filename[255];
	struct format_v1_0::st_file_projectile new_proj;
	int total_items = 0;

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", EDITOR_FILEPATH);

	fp = fopen(filename, "rb");
	if (!fp) {
		printf(">> WARNING::createProjectile Could not open file '%s' for reading item number, will create a new file. <<\n", filename);
	} else {
		fseek (fp, 0, SEEK_END);
		int byte_size = ftell (fp);
		total_items = byte_size / sizeof(format_v1_0::st_file_projectile);
		printf(">> WARNING: byte_size: %d, total_items: %d <<\n", byte_size, total_items);
		fclose(fp);
	}

	fp = fopen(filename, "ab+");
	if (!fp) {
		printf(">> ERROR: Could not open file '%s' for writing. <<\n", filename);
		exit(-1);
	}
	fseek (fp, 0, SEEK_END);
	new_proj.frame_w = dataExchanger->projectileGraphicSize_w;
	new_proj.frame_h = dataExchanger->projectileGraphicSize_h;
	new_proj.is_destructible = 0;
	new_proj.trajectory_type = 0;
	new_proj.HP = 1;
	sprintf(new_proj.img_filename, "%s", "");
	sprintf(new_proj.name, "New Proj %d", total_items);
	printf(">> WARNING: New.name: '%s' <<\n", new_proj.name);
	fwrite(&new_proj, sizeof(struct format_v1_0::st_file_projectile), 1, fp);
	fclose(fp);
	reloadProjectileListCombo();
}


void MainWindow::on_projectile_imagefile_combo_currentIndexChanged(QString value)
{
	sprintf(dataExchanger->addProjectileFilename, "%s/data/images/projectiles/%s", EDITOR_FILEPATH, qPrintable(value));
	// calc and set the max value for width and height spinners
	QPixmap image(dataExchanger->addProjectileFilename);
	if (image.isNull()) {
		//printf("projectilePreviewArea::paintEvent - Could not load image file '%s'\n", dataExchanger->addProjectileFilename);
		return;
	}
	int image_w = image.width();
	int image_h = image.height();
	ui->projectileWidthSpinBox->setMaximum(image_w);
	ui->projectileHeightSpinBox->setMaximum(image_h);
	ui->projectileWidthSpinBox->setValue(image_w);
	ui->projectileHeightSpinBox->setValue(image_h);
	dataExchanger->projectileGraphicSize_w = image_w;
	dataExchanger->projectileGraphicSize_h = image_h;

	ui->projectilePreviewAreaWidget->repaint();
}

void MainWindow::on_projectileWidthSpinBox_valueChanged(int value)
{
	dataExchanger->projectileGraphicSize_w = value;
	ui->projectilePreviewAreaWidget->repaint();
}

void MainWindow::on_projectileHeightSpinBox_valueChanged(int value)
{
	dataExchanger->projectileGraphicSize_h = value;
	ui->projectilePreviewAreaWidget->repaint();

}

void MainWindow::reloadProjectileListCombo()
{
	FILE *fp;
	char filename[255];
	struct format_v1_0::st_file_projectile new_proj;


	ui->projectileList_combo->clear();
	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", EDITOR_FILEPATH);
	fp = fopen(filename, "rb");
	if (fp) {
		while (!feof(fp)) {
			if (fread(&new_proj, sizeof(struct format_v1_0::st_file_projectile), 1, fp) == 1) {
				//printf(">> DEBUG: adding item '%s' into projectile combo list <<\n", new_proj.name);
				ui->projectileList_combo->addItem(QString(new_proj.name));
			}
		}
		fclose(fp);
	}
}


//###############################################################################//
// load an projectile item                                                       //
//###############################################################################//
void MainWindow::on_projectileList_combo_currentIndexChanged(int index)
{
	char filename[255];
	struct format_v1_0::st_file_projectile new_proj;
	FILE *fp;

	//printf(">> DEBUG on_projectileList_combo_currentIndexChanged <<\n");

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", EDITOR_FILEPATH);
	fp = fopen(filename, "rb");
	if (!fp) {
		printf(">> ERROR: Could not open file '%s' for loading. <<", filename);
		return;
	}
	if (fseek(fp, (index)*sizeof(format_v1_0::st_file_projectile), SEEK_SET) != 0) {
		printf("ERROR: could not set projectile list file position (%d).\n", index);
		return;
	}
	if (!fread(&new_proj, sizeof(format_v1_0::st_file_projectile), 1, fp)) {
		printf("ERROR: could not load projectile data (%d).\n", index);
	}
	ui->projectileName_lineEdit->setText(QString(new_proj.name));
	//TODO: create a generic function to set combobox position according to value
	ui->projectile_imagefile_combo->setCurrentIndex(ui->projectile_imagefile_combo->findData(QString(new_proj.img_filename)));
	ui->projectileWidthSpinBox->setValue(new_proj.frame_w);
	ui->projectileHeightSpinBox->setValue(new_proj.frame_h);
	ui->projectile_trajectorytype_combo->setCurrentIndex(new_proj.trajectory_type);
	ui->projectileDestructibleCheckBox->setChecked(new_proj.is_destructible);
	ui->projectileHitPointsSpinBox->setValue(new_proj.HP);
	fclose(fp);
}

void MainWindow::saveProjectile(int index) {
	char filename[255];
	struct format_v1_0::st_file_projectile new_proj;
	FILE *fp;

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", EDITOR_FILEPATH);
	fp = fopen(filename, "wb");
	if (!fp) {
		printf(">> ERROR: Could not open file '%s' for reading. <<", filename);
		exit(-1);
	}
	if (fseek(fp, (index)*sizeof(format_v1_0::st_file_projectile), SEEK_SET) != 0) {
		printf("ERROR: could not set projectile list file position (%d).\n", index);
		return;
	}


	sprintf(new_proj.name, "%s", (const char*)ui->projectileName_lineEdit->text().toLocal8Bit());
	//sprintf(new_proj.img_filename, "%s", ui->projectile_imagefile_combo->itemData(ui->projectile_imagefile_combo->currentIndex()));
	new_proj.frame_w = ui->projectileWidthSpinBox->value();
	new_proj.frame_h = ui->projectileHeightSpinBox->value();
	new_proj.trajectory_type = ui->projectile_trajectorytype_combo->currentIndex();
	if (ui->projectileDestructibleCheckBox->isChecked()) {
		new_proj.is_destructible = 1;
	} else {
		new_proj.is_destructible = 0;
	}
	new_proj.HP = ui->projectileHitPointsSpinBox->value();
	fclose(fp);
	printf(">> saveProjectile::END <<\n");
}

void MainWindow::on_link_bidi_checkbox_toggled(bool checked)
{
	dataExchanger->link_bidi = checked;
}

void MainWindow::on_link_isdoor_checkbox_toggled(bool checked)
{
	dataExchanger->link_is_door = checked;
}

void MainWindow::on_pushButton_2_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_color1()));
}

void MainWindow::on_player_sprite_h_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.frame_size.heigth = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_hitarea_x_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.hit_area.x = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_hitarea_y_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.hit_area.y = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_hitarea_w_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.hit_area.w = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_hitarea_h_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.hit_area.h = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_sprite_w_valueChanged(int arg1)
{
	dataExchanger->player_graphics_data.frame_size.width = arg1;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_player_graphics_combo_currentIndexChanged(const QString &arg1)
{
	dataExchanger->player_graphics_data.graphics_filename = arg1.toStdString();
	ui->player_preview_widget->repaint();
}


void MainWindow::pick_player_keycolor1()
{
	std::cout << "MainWindow::pick_player_keycolor1 - setting colorkey #0 as " << dataExchanger->picked_color_n << std::endl;
	game_data.players[dataExchanger->current_player].color_keys[0].r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].color_keys[0].g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].color_keys[0].b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_player_keycolor2()
{
	game_data.players[dataExchanger->current_player].color_keys[1].r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].color_keys[1].g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].color_keys[1].b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_player_keycolor3()
{
	game_data.players[dataExchanger->current_player].color_keys[2].r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].color_keys[2].g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].color_keys[2].b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_player_color1()
{
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color1.r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color1.g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color1.b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_player_color2()
{
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color2.r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color2.g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color2.b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_player_color3()
{
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color3.r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color3.g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.players[dataExchanger->current_player].weapon_colors[dataExchanger->current_weapon].color3.b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	ui->player_preview_widget->repaint();
}

void MainWindow::pick_bg_color()
{
	//std::cout << "********* MainWindow::pick_bg_color, stage: " << dataExchanger->currentStage << ", map_n: " << dataExchanger->currentMap << ", r: " << dataExchanger->colormap[dataExchanger->picked_color_n].r << ", g: " << dataExchanger->colormap[dataExchanger->picked_color_n].g << ", b: " << dataExchanger->colormap[dataExchanger->picked_color_n].b << std::endl;
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.r = dataExchanger->colormap[dataExchanger->picked_color_n].r;
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.g = dataExchanger->colormap[dataExchanger->picked_color_n].g;
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.b = dataExchanger->colormap[dataExchanger->picked_color_n].b;
	fill_background_list();
	ui->editArea->repaint();
}


void MainWindow::on_pushButton_7_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_keycolor1()));
}

void MainWindow::on_pushButton_8_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_keycolor2()));
}

void MainWindow::on_pushButton_9_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_keycolor3()));
}

void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{
	dataExchanger->current_weapon = index;
	ui->player_preview_widget->repaint();
}

void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
	dataExchanger->current_player = index;
}

void MainWindow::on_pushButton_3_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_color2()));
}

void MainWindow::on_pushButton_4_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_player_color3()));
}



void MainWindow::fill_players_combo(QComboBox* combo)
{
	for (int i=0; i<MAX_FILE_PLAYERS; i++) {
		combo->addItem(QString::number(i+1)+QString(" [")+QString(game_data.players[i].name)+QString("]"));
	}
}


void MainWindow::fill_players_data()
{
	ui->players_tab_name->setText(QString(game_data.players[0].name));
	ui->players_tab_hp->setValue(game_data.players[0].HP);
	ui->players_tab_hasshield->setChecked(game_data.players[0].have_shield);
	ui->players_tab_maxshots->setValue(game_data.players[0].max_shots);
	ui->players_tab_fallgravity->setValue(game_data.players[0].fall_gravity);
	ui->players_tab_jumpgravity->setValue(game_data.players[0].jump_gravity);
	ui->players_tab_jumpspeed->setValue(game_data.players[0].jump_initial_speed);
	ui->players_tab_movespeed->setValue(game_data.players[0].move_speed);
	ui->player_graphics_combo->setCurrentIndex(ui->player_graphics_combo->findText(QString(game_data.players[0].graphic_filename)));
	ui->player_sprite_w->setValue(game_data.players[0].sprite_size.width);
	ui->player_sprite_h->setValue(game_data.players[0].sprite_size.heigth);

	ui->player_hitarea_x->setValue(game_data.players[0].sprite_hit_area.x);
	ui->player_hitarea_y->setValue(game_data.players[0].sprite_hit_area.y);
	ui->player_hitarea_w->setValue(game_data.players[0].sprite_hit_area.w);
	ui->player_hitarea_h->setValue(game_data.players[0].sprite_hit_area.h);
}

void MainWindow::on_players_tab_list_combo_currentIndexChanged(int index)
{
	ui->players_tab_name->setText(QString(game_data.players[index].name));
	ui->players_tab_hp->setValue(game_data.players[index].HP);
	ui->players_tab_hasshield->setChecked(game_data.players[index].have_shield);
	ui->players_tab_maxshots->setValue(game_data.players[index].max_shots);
	ui->players_tab_fallgravity->setValue(game_data.players[index].fall_gravity);
	ui->players_tab_jumpgravity->setValue(game_data.players[index].jump_gravity);
	ui->players_tab_jumpspeed->setValue(game_data.players[index].jump_initial_speed);
	ui->players_tab_movespeed->setValue(game_data.players[index].move_speed);
	ui->player_graphics_combo->setCurrentIndex(ui->player_graphics_combo->findText(QString(game_data.players[index].graphic_filename)));
	ui->player_sprite_w->setValue(game_data.players[index].sprite_size.width);
	ui->player_sprite_h->setValue(game_data.players[index].sprite_size.heigth);

	ui->player_hitarea_x->setValue(game_data.players[index].sprite_hit_area.x);
	ui->player_hitarea_y->setValue(game_data.players[index].sprite_hit_area.y);
	ui->player_hitarea_w->setValue(game_data.players[index].sprite_hit_area.w);
	ui->player_hitarea_h->setValue(game_data.players[index].sprite_hit_area.h);
	dataExchanger->current_player = index;
}






void MainWindow::on_tabWidget_currentChanged(int index)
{
	dataExchanger->currentMap = 0;
	dataExchanger->currentGame = 0;
	dataExchanger->currentStage = 0;
	dataExchanger->current_player = 0;
	dataExchanger->current_weapon = 0;
	fillMapList();
	fillNpcList();
	fill_background_list();
}






void MainWindow::on_actionSet_Boss_triggered(bool checked)
{
    if (checked == true) {
        ui->toolBox->setCurrentWidget(ui->page_npc);
        ui->actionEdit_Tileset->setChecked(FALSE);
        ui->actionAdd_Object->setChecked(FALSE);
        ui->actionEdit_NPC->setChecked(FALSE);
        ui->actionFill->setDisabled(TRUE);
        ui->actionLink->setDisabled(TRUE);
        ui->actionLock_Edit->setDisabled(TRUE);
        ui->actionStairs->setDisabled(TRUE);
        dataExchanger->editMode = EDITMODE_SET_BOSS;
    }
}



void MainWindow::fill_background_list()
{
	QString bg1_filename(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename);
	QString bg2_filename(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename);
	ui->bg1_filename->clear();
	ui->bg2_filename->clear();
	common::fill_files_combo("data/images/map_backgrounds", ui->bg1_filename);
	common::fill_files_combo("data/images/map_backgrounds", ui->bg2_filename);

	// @TODO: set values from game_data
	//std::cout << "******** backgrounds[0].filename: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename << std::endl;
	ui->bg1_filename->setCurrentIndex(ui->bg1_filename->findText(bg1_filename));
	ui->bg2_filename->setCurrentIndex(ui->bg2_filename->findText(bg2_filename));
	ui->bg1_y_pos->setValue(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].adjust_y);
	ui->bg2_y_pos->setValue(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].adjust_y);
	float bg1_speed = (float)game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed/10;
	//std::cout << "*** fill_background_list - setvalue: " << bg1_speed << ", bg1.speed: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed << std::endl;
	ui->bg1_speed->setValue(bg1_speed);
	ui->bg2_speed->setValue((float)game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].speed/10);
	std::stringstream ss;
	ss.str(std::string());
	ss << "background-color: rgb(" << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.r << ", " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.g << ", " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.b << ")";
	ui->bg_color_pick->setStyleSheet(ss.str().c_str());
	background_filled = true;
}

void MainWindow::on_bg1_filename_currentIndexChanged(const QString &arg1)
{
	if (arg1.toStdString() == std::string("None")) {
		game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename[0] = '\0';
	} else {
		sprintf(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename, "%s", arg1.toStdString().c_str());
	}
	ui->editArea->repaint();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
	dataExchanger->show_background_color = checked;
	ui->editArea->repaint();
}

void MainWindow::on_bg1_y_pos_valueChanged(int arg1)
{
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].adjust_y = arg1;
	ui->editArea->repaint();
}

void MainWindow::on_bg1_speed_valueChanged(int arg1)
{
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed = arg1*10;
	std::cout << "*** on_bg1_speed_valueChanged - setvalue: " << arg1 << ", bg1.speed: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed << std::endl;
	ui->editArea->repaint();
}


void MainWindow::on_bg2_filename_currentIndexChanged(const QString &arg1)
{
	if (arg1.toStdString() == std::string("None")) {
		game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename[0] = '\0';
	} else {
		sprintf(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename, "%s", arg1.toStdString().c_str());
	}
}

void MainWindow::on_bg2_y_pos_valueChanged(int arg1)
{
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].adjust_y = arg1;
	ui->editArea->repaint();
}


void MainWindow::on_bg2_speed_valueChanged(int arg1)
{
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].speed = arg1*10;
	ui->editArea->repaint();
}

void MainWindow::on_bg_color_pick_clicked()
{
	QDialog *color_pick = new dialog_pick_color;
	color_pick->show();
	QObject::connect(color_pick, SIGNAL(accepted()), this, SLOT(pick_bg_color()));
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
	dataExchanger->show_bg1 = checked;
	ui->editArea->repaint();
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
	dataExchanger->show_bg2 = checked;
	ui->editArea->repaint();
}

void MainWindow::on_stage_boss_weapon_combo_currentIndexChanged(int index)
{
	game_data.stages[dataExchanger->currentStage].boss.id_weapon = index;
}

void MainWindow::on_bg1_speed_valueChanged(double arg1)
{
	if (background_filled == false) {
		return;
	}
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed = arg1*10;
	std::cout << "*** on_bg1_speed_valueChanged - setvalue: " << arg1 << ", bg1.speed: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].speed << std::endl;
	ui->editArea->repaint();
}

void MainWindow::on_bg2_speed_valueChanged(double arg1)
{
	if (background_filled == false) {
		return;
	}
	game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].speed = arg1*10;
	ui->editArea->repaint();
}

void MainWindow::on_actionReset_Map_triggered()
{
	for (int i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked = 0;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y = -1;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename[0] = '\0';
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename[0] = '\0';
		}
	}
}
