#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"
#include "addwizard.h"
#include "dialognpcedit.h"
#include "dialogobjectedit.h"
#include "loadgamepicker.h"
#include <QStandardItemModel>
#include <QListView>
#include <QList>
#include <QListWidgetItem>
#include "shared_trajectories.h"

extern Mediator *dataExchanger;
extern char FILEPATH[512];



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
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
	ui->editArea->repaint();
	QObject::connect(ui->pallete, SIGNAL(signalPalleteChanged()), ui->editArea, SLOT(repaint()));
	// add maps to the list
	model = new QStandardItemModel();

	QStandardItem* games = new QStandardItem("GAMES");

	sprintf(buffer, "%s/data/game/", FILEPATH);
	n = dataExchanger->freeSlot(buffer, "gme");
	for (i=1; i<n; i++) {
		dataExchanger->centNumberFormat(i);
		sprintf(buffer, "Game %s", dataExchanger->centNumber);
		temp_game_item = new QStandardItem(buffer);
		temp_game_item->setEditable(FALSE);
		games->appendRow(temp_game_item);


		sprintf(buffer, "%s/data/game/game_%s/", FILEPATH, dataExchanger->centNumber);
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
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::fillStageList() {
	char directory[512], buffer[512];
	bool fim = false;
	int stage_n = 1;

	printf("&&& fillStageList.START &&&\n");

	dataExchanger->centNumberFormat(1);
	sprintf(directory, "%s/data/game/game_%s/", FILEPATH, dataExchanger->centNumber);

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
}

void MainWindow::fillMapList() {
	char buffer[512];
	int i, max;

	max = ui->mapListCombo->count();
	for (i=0; i<max; i++) {
		ui->mapListCombo->removeItem(0);
	}
	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(buffer, "%s/data/game/game_%s/", FILEPATH, dataExchanger->centNumber);
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
	char buffer[512], directory[512], filename[512];
	int i, n_game, max;
	QListWidgetItem* item;
	QListWidgetItem* item_to_del;
	FILE *fp;
	struct file_npc temp_npc;

	max = ui->listWidget->count();
	// remove previous ones
	for (i=0; i<max; i++) {
		//printf(">>> Removing item %d from list, count: %d\n", i, ui->listWidget->count());
		item_to_del = ui->listWidget->takeItem(0);
		delete item_to_del;
	}

	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(directory, "%s/data/game/game_%s", FILEPATH, dataExchanger->centNumber);
	n_game = dataExchanger->freeSlot(directory, "npc");


	sprintf(buffer, "%s/main_list.npc", directory);
	fp = fopen(buffer, "rb");
	if (fp) {
		while (!feof(fp)) {
			if (fread(&temp_npc, sizeof(struct file_npc), 1, fp) != 1) {
				//printf("MainWindow::fillNpcList - Could not load npc from file -> interrupt.\n");
				break;
			}
			item = new QListWidgetItem;
			item->setText(temp_npc.name);
			sprintf(filename, "%s/data/images/sprites/enemies/%s", FILEPATH, temp_npc.filename);
			QPixmap image(filename);
			image = image.copy(0, 0, temp_npc.framesize_w, temp_npc.framesize_h);
			image = image.scaled(32, 32);
			item->setIcon(image);
			ui->listWidget->addItem(item);
		}
		fclose(fp);
	}

	ui->listWidget->update();
}


void MainWindow::fillObjList() {

        char buffer[512], directory[512], filename[512];
	int i, n_game, max;
	QListWidgetItem* item;
	QListWidgetItem* item_to_del;
	FILE *fp;
	struct struct_object temp_object;

	max = ui->objectListWidget->count();
	// remove previous ones
	for (i=0; i<max; i++) {
		item_to_del = ui->objectListWidget->takeItem(0);
		delete item_to_del;
	}

	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(directory, "%s/data/game/game_%s", FILEPATH, dataExchanger->centNumber);
	n_game = dataExchanger->freeSlot(directory, "obj");

	//printf("######### MainWindow::fillObjList - buffer: %s, n_game: %d\n", directory, dataExchanger->currentGame);
	for (i=1; i<n_game; i++) {
		dataExchanger->centNumberFormat(i);
		sprintf(buffer, "%s/%s.obj", directory, dataExchanger->centNumber);
		//printf("######### MainWindow::fillObjList - adding item: %s, count: %d\n", buffer, ui->objectListWidget->count());

		fp = fopen(buffer, "r");
		if (fp) {
			fread(&temp_object, sizeof(struct_object), 1, fp);
			fclose(fp);
		} else {
			printf("DEBUG - no object file, ignoring load.\n");
		}
		//printf("DEBUG.MainWindow::fillNpcList - adding %s\n", buffer);
		//ui->npcListCombo->addItem(buffer);
		item = new QListWidgetItem;
		item->setText(temp_object.name);
		sprintf(filename, "%s/data/images/sprites/objects/%s", FILEPATH, temp_object.filename);
		//printf(">> MainWindow::fillObjectList - name: '%s', filename: '%s'\n", temp_object.name, filename);
		QPixmap image(filename);
		if (!image) {
			printf("Error loading file '%s' into a QPixmap\n", filename);
		}
		image = image.copy(0, 0, temp_object.framesize_w, temp_object.framesize_h);
		image = image.scaled(32, 32);
		item->setIcon(image);
		ui->objectListWidget->addItem(item);
	}
	ui->objectListWidget->update();
}


void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionSave_triggered()
{
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
	ui->editArea->saveMap(dataExchanger->currentGame, dataExchanger->currentMap);
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
	dataExchanger->selectedNPC = currentRow+1;
	ui->editNPCButton->setEnabled(TRUE);
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
	printf(">> loadStageMap::CALL 1 - map: %d <<\n", index);
	dataExchanger->loadStageMap(dataExchanger->currentGame, dataExchanger->currentStage, index);
	dataExchanger->currentMap = index;
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
	dataExchanger->editModeNPC = 0;
	QDialog *object_editor = new DialogObjectEdit ();
	object_editor->show();
	QObject::connect(object_editor, SIGNAL(finishedObjectEditor()), this, SLOT(reloadComboItems()));
}

void MainWindow::on_objectListWidget_currentRowChanged(int currentRow)
{
	dataExchanger->selectedNPC = currentRow+1;
	printf(">>>> MainWindow::on_objectListWidget_currentRowChanged, row: %d, dataExchanger->selectedNPC: %d\n", currentRow, dataExchanger->selectedNPC);
	ui->editObjectButton->setEnabled(TRUE);
}

void MainWindow::on_toolBox_currentChanged(int index)
{
	Q_UNUSED (index);
	dataExchanger->selectedNPC = -1;
}

void MainWindow::on_pushButton_clicked()
{
	dataExchanger->addMap();
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
	dataExchanger->loadStage(dataExchanger->currentGame, index+1);
	ui->stage_bossName->setText(dataExchanger->stage.boss_name);
	printf(">> loadStageMap::CALL 2 - map: %d <<\n", 0);
	dataExchanger->loadStageMap(dataExchanger->currentGame, index+1, 0);
	dataExchanger->currentMap = 1;
	dataExchanger->currentStage = index+1;
	printf(">>> 1.currentStage; %d <<<\n", dataExchanger->currentStage);
	ui->mapListCombo->setCurrentIndex(0);
	ui->editArea->repaint();
}


void MainWindow::fillProjectileImagesList()
{
	QDir *dir = new QDir(QString(FILEPATH)+"/data/images/projectiles/");
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
	printf(">> dataExchanger::fillNPCGraphicsList->addNpcFilename: '%s'\n", dataExchanger->addNpcFilename);
	ui->projectile_imagefile_combo->repaint();
}

void MainWindow::fillTrajectoriesList()
{
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
	struct file_projectile new_proj;
	int total_items = 0;

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", FILEPATH);

	fp = fopen(filename, "rb");
	if (!fp) {
		printf(">> WARNING::createProjectile Could not open file '%s' for reading item number, will create a new file. <<\n", filename);
	} else {
		fseek (fp, 0, SEEK_END);
		int byte_size = ftell (fp);
		total_items = byte_size / sizeof(file_projectile);
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
	fwrite(&new_proj, sizeof(struct file_projectile), 1, fp);
	fclose(fp);
	reloadProjectileListCombo();
}


void MainWindow::on_projectile_imagefile_combo_currentIndexChanged(QString value)
{
	sprintf(dataExchanger->addProjectileFilename, "%s/data/images/projectiles/%s", FILEPATH, qPrintable(value));
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
	struct file_projectile new_proj;


	ui->projectileList_combo->clear();
	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", FILEPATH);
	fp = fopen(filename, "rb");
	if (fp) {
		while (!feof(fp)) {
			if (fread(&new_proj, sizeof(struct file_projectile), 1, fp) == 1) {
				printf(">> DEBUG: adding item '%s' into projectile combo list <<\n", new_proj.name);
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
	struct file_projectile new_proj;
	FILE *fp;

	printf(">> DEBUG on_projectileList_combo_currentIndexChanged <<\n");

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", FILEPATH);
	fp = fopen(filename, "rb");
	if (!fp) {
		printf(">> ERROR: Could not open file '%s' for loading. <<", filename);
		return;
	}
	if (fseek(fp, (index)*sizeof(file_projectile), SEEK_SET) != 0) {
		printf("ERROR: could not set projectile list file position (%d).\n", index);
		return;
	}
	if (!fread(&new_proj, sizeof(file_projectile), 1, fp)) {
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
	struct file_projectile new_proj;
	FILE *fp;

	printf(">> DEBUG on_projectileList_combo_currentIndexChanged <<\n");

	sprintf(filename, "%s/data/game/game_001/projectile_list.dat", FILEPATH);
	fp = fopen(filename, "wb");
	if (!fp) {
		printf(">> ERROR: Could not open file '%s' for reading. <<", filename);
		exit(-1);
	}
	if (fseek(fp, (index)*sizeof(file_projectile), SEEK_SET) != 0) {
		printf("ERROR: could not set projectile list file position (%d).\n", index);
		return;
	}


	sprintf(new_proj.name, "%s", ui->projectileName_lineEdit->text());
	sprintf(new_proj.img_filename, "%s", ui->projectile_imagefile_combo->itemData(ui->projectile_imagefile_combo->currentIndex()));
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
