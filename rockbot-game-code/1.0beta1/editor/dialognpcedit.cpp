#include <stdio.h>
#include <QDir>
#include "dialognpcedit.h"
#include "ui_dialognpcedit.h"
#include "mediator.h"
#include "defines.h"
#include "../file/format.h".h"
//#include "shared_IA.h"

extern char EDITOR_FILEPATH[512];
extern std::string FILEPATH;
extern format_v_2_0_1::file_game game_data;

extern Mediator *dataExchanger;

DialogNPCEdit::DialogNPCEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNPCEdit)
{
	std::cout << "DialogNPCEdit::DialogNPCEdit::START" << std::endl;
	char* IA_TYPE_NAMES[IA_TYPES_COUNT] = { "IA_STAND", "IA_WAIT", "IA_FOLLOW", "IA_ZIGZAG", "IA_SIDETOSIDE", "IA_BAT", "IA_ROOF_SHOOTER", "IA_GROUND_SHOOTER", "IA_SHOOT_AND_GO", "IA_FLY_ZIG_ZAG", "IA_BUTTERFLY", "IA_GO_LEFT", "IA_GO_RIGHT", "IA_FIXED_JUMPER", "IA_SIDE_SHOOTER", "IA_GHOST", "IA_FISH", "IA_DOLPHIN" };

    ui->setupUi(this);

	for (int i=0; i<IA_TYPES_COUNT; i++) {
		ui->comboBoxIA->addItem(QString(IA_TYPE_NAMES[i]));
	}
	ui->comboBoxIA->update();

	fillNPCGraphicsList();
	//printf("%%%% DialogNPCEdit::DialogNPCEdit - dataExchanger->selectedNPC: %d, editMode: %d\n", dataExchanger->selectedNPC, dataExchanger->editModeNPC);
    if (dataExchanger->editModeNPC == 1) {
		loadNPCData(dataExchanger->selectedNPC);
		std::cout << "DialogObjectEdit::DialogObjectEdit - editing npc, id: " << dataExchanger->selectedNPC << std::endl;
	} else {
		std::cout << "DialogObjectEdit::DialogObjectEdit - adding new npc, id: " << dataExchanger->selectedNPC << std::endl;
	}


    QObject::connect(ui->npcPreviewAreaWidget, SIGNAL(clickedIn()), this, SLOT(setNPCFrame()));
	std::cout << "DialogNPCEdit::DialogNPCEdit::END" << std::endl;
}




void DialogNPCEdit::loadNPCData(int npc_n) {
	std::cout << "DialogNPCEdit::loadNPCData::START" << std::endl;
	ui->NpcName->setText(game_data.game_npcs[npc_n].name);
	ui->NpcHP->setValue(game_data.game_npcs[npc_n].hp.total);
	ui->npcGraphicSizeSpin_w->setValue(game_data.game_npcs[npc_n].frame_size.width);
	ui->npcGraphicSizeSpin_h->setValue(game_data.game_npcs[npc_n].frame_size.heigth);
	dataExchanger->npcGraphicSize_w = game_data.game_npcs[npc_n].frame_size.width;
	dataExchanger->npcGraphicSize_h = game_data.game_npcs[npc_n].frame_size.heigth;
	for (int i=0; i<ui->npcListCombobox->count(); i++) {
		if (ui->npcListCombobox->itemText(i) == QString(game_data.game_npcs[npc_n].graphic_filename)) {
			ui->npcListCombobox->setCurrentIndex(i);
			break;
		}
	}
	for (int i=0; i<NPC_FRAME_NUMBER; i++) {
		dataExchanger->frameset[i] = game_data.game_npcs[npc_n].frames[i].state;
		dataExchanger->frameset_time[i] = game_data.game_npcs[npc_n].frames[i].duration;
	}
	if (game_data.game_npcs[npc_n].can_shoot) {
		ui->checkBoxCanShoot->setChecked(true);
	}
	/// @TODO
	/*
	if (temp_npc.is_boss == 1) {
		ui->isBossCheckbox->setChecked(true);
	}
	*/
	ui->comboBoxShieldType->setCurrentIndex(game_data.game_npcs[npc_n].shield_type);

	ui->comboBoxIA->setCurrentIndex(game_data.game_npcs[npc_n].IA_type);

	ui->speed_spin->setValue(game_data.game_npcs[npc_n].speed);
	ui->range_spin->setValue(game_data.game_npcs[npc_n].walk_range);
	std::cout << "DialogNPCEdit::loadNPCData::END" << std::endl;
}



void DialogNPCEdit::saveNPCData(int npc_n) {
	if (game_data.game_npcs[npc_n].id == -1) {
		game_data.game_npcs[npc_n].id = npc_n;
	}
	sprintf(game_data.game_npcs[npc_n].name, "%s", ui->NpcName->text().toStdString().c_str());
	game_data.game_npcs[npc_n].hp.total = ui->NpcHP->value();
	game_data.game_npcs[npc_n].frame_size.width = ui->npcGraphicSizeSpin_w->value();
	game_data.game_npcs[npc_n].frame_size.heigth = ui->npcGraphicSizeSpin_h->value();
	sprintf(game_data.game_npcs[npc_n].graphic_filename, ui->npcListCombobox->currentText().toStdString().c_str());

	if (ui->checkBoxCanShoot->isChecked()) {
		game_data.game_npcs[npc_n].can_shoot=1;
	} else {
		game_data.game_npcs[npc_n].can_shoot=0;
	}

	/*
	if (ui->isBossCheckbox->isChecked()) {
		temp_npc.is_boss = 1;
	} else {
		temp_npc.is_boss = 0;
	}
	*/
	game_data.game_npcs[npc_n].shield_type = ui->comboBoxShieldType->currentIndex();

	for (int i=0; i<NPC_FRAME_NUMBER; i++) {
		game_data.game_npcs[npc_n].frames[i].state = dataExchanger->frameset[i];
		game_data.game_npcs[npc_n].frames[i].duration = dataExchanger->frameset_time[i];
	}
	game_data.game_npcs[npc_n].IA_type = ui->comboBoxIA->currentIndex();
	game_data.game_npcs[npc_n].speed = ui->speed_spin->value();
	game_data.game_npcs[npc_n].walk_range = ui->range_spin->value();



	/*
	strcpy (temp_npc.name, ui->NpcName->text().toLatin1());

	temp_npc.hp.total = ui->NpcHP->value();
	temp_npc.framesize_w = ui->npcGraphicSizeSpin_w->value();
	temp_npc.framesize_h = ui->npcGraphicSizeSpin_h->value();
	strcpy (temp_npc.graphic_filename, ui->npcListCombobox->currentText().toLatin1());

	//printf(">> DialogNPCEdit::saveNPCData - name: '%s', filename: '%s'\n", temp_npc.name, temp_npc.filename);

	//printf(">> DialogNPCEdit::saveNPCData - Trying to save npc '%d' data\n", npc_n);
	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(directory, "%s/data/game/game_%s", EDITOR_FILEPATH, dataExchanger->centNumber);
	dataExchanger->centNumberFormat(npc_n);
	sprintf(buffer, "%s/%s.npc", directory, dataExchanger->centNumber);
	//printf(">> DialogNPCEdit::saveNPCData - buffer: %s\n", buffer);

	if (ui->checkBoxCanShoot->isChecked()) {
		temp_npc.can_shoot=1;
	} else {
		temp_npc.can_shoot=0;
	}
	if (ui->isBossCheckbox->isChecked()) {
		temp_npc.is_boss = 1;
	} else {
		temp_npc.is_boss = 0;
	}
	temp_npc.shield_type = ui->comboBoxShieldType->currentIndex();

	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		temp_npc.frameset[i] = dataExchanger->frameset[i];
		temp_npc.frameset_time[i] = dataExchanger->frameset_time[i];
	}

	temp_npc.IA_type = ui->comboBoxIA->currentIndex();


	temp_npc.speed = ui->speed_spin->value();
	temp_npc.walk_range = ui->range_spin->value();

	dataExchanger->centNumberFormat(dataExchanger->currentGame);
	sprintf(directory, "%s/data/game/game_%s", EDITOR_FILEPATH, dataExchanger->centNumber);


	sprintf(buffer, "%s/main_list.npc", directory);
	fp = fopen(buffer, "rb+");
	if (fp) {
		if (fseek (fp, (npc_n-1)*sizeof(format_v1_0::st_file_npc), SEEK_SET) == 0) {
			printf(">> saving NPC in position %d <<\n", npc_n);
			fwrite(&temp_npc, sizeof(format_v1_0::st_file_npc), 1, fp);
		// if can't position, go to the end
		} else {
			printf(">> adding a new NPC <<\n");
			fseek (fp, sizeof(format_v1_0::st_file_npc), SEEK_END);
			fwrite(&temp_npc, sizeof(format_v1_0::st_file_npc), 1, fp);
		}
		fclose(fp);
	}
	*/
}



DialogNPCEdit::~DialogNPCEdit()
{
    delete ui;
}

void DialogNPCEdit::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void DialogNPCEdit::fillNPCGraphicsList() {
	printf("DEBUG - fillNPCGraphicsList - START\n");
	qDebug(QString("dir: "+QString(EDITOR_FILEPATH)+"/data/images/sprites/enemies/").toAscii());
	QDir *dir = new QDir(QString(EDITOR_FILEPATH)+"/data/images/sprites/enemies/");
	dir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	dir->setSorting(QDir::Size | QDir::Reversed);
	QFileInfoList list = dir->entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		//printf("DialogNPCEdit::fillNPCGraphicsList - fileName: %s\n", qPrintable(fileInfo.fileName()));
		if (i == 0) {
			sprintf(dataExchanger->addNpcFilename, "%s/data/images/sprites/enemies/%s", EDITOR_FILEPATH, qPrintable(fileInfo.fileName()));
			printf(">> SET > dataExchanger::fillNPCGraphicsList->addNpcFilename: '%s'\n", dataExchanger->addNpcFilename);
		}
		//dataExchanger->NpcPreviewFile = new QString(fileInfo.fileName());
		//sprintf(dataExchanger->addNpcFilename, "%s", qPrintable(fileInfo.fileName()));
		ui->npcListCombobox->addItem(fileInfo.fileName());
	}
	//printf(">> dataExchanger::fillNPCGraphicsList->addNpcFilename: '%s'\n", dataExchanger->addNpcFilename);
	ui->npcPreviewAreaWidget->repaint();
}




void DialogNPCEdit::on_npcGraphicSizeSpin_w_valueChanged(int value)
{
	dataExchanger->npcGraphicSize_w = value;
	ui->npcPreviewAreaWidget->repaint();
}

void DialogNPCEdit::on_npcGraphicSizeSpin_h_valueChanged(int value)
{
	dataExchanger->npcGraphicSize_h = value;
	ui->npcPreviewAreaWidget->repaint();
}

void DialogNPCEdit::on_npcListCombobox_currentIndexChanged(QString item)
{
	sprintf(dataExchanger->addNpcFilename, "%s/data/images/sprites/enemies/%s", EDITOR_FILEPATH, qPrintable(item));
	ui->npcPreviewAreaWidget->repaint();
}

void DialogNPCEdit::on_buttonBox_accepted()
{
	printf(">> DialogNPCEdit::on_buttonBox_accepted\n");
	saveNPCData(dataExchanger->selectedNPC);
	emit finishedNPCEditor();
}

void DialogNPCEdit::setNPCFrame() {
	printf(">> DialogNPCEdit::setNPCFrame - CALLED\n");
	ui->frameTypeCombo->setCurrentIndex(dataExchanger->frameset[dataExchanger->getPalleteX()]+1);
	ui->spinBox->setValue(dataExchanger->frameset_time[dataExchanger->getPalleteX()]);
}

void DialogNPCEdit::on_frameTypeCombo_currentIndexChanged(int index)
{
	dataExchanger->frameset[dataExchanger->getPalleteX()] = index-1;
}

void DialogNPCEdit::on_spinBox_valueChanged(int value)
{
	dataExchanger->frameset_time[dataExchanger->getPalleteX()] = value;
}
