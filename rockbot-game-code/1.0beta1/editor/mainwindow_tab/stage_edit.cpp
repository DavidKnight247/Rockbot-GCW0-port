#include "stage_edit.h"
#include "ui_stage_edit.h"

//#include "defines.h"
//#include "../defines.h"
#include "common.h"

#include "../mediator.h"
extern Mediator *dataExchanger;
extern char EDITOR_FILEPATH[512];
extern std::string FILEPATH;


stage_edit::stage_edit(QWidget *parent) :
QWidget(parent),
ui(new Ui::stage_edit)
{
	ui->setupUi(this);
	ui->dialogs_line1_text1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_line1_text2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_line1_text3->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer1_text1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer1_text2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer1_text3->setMaxLength(DIALOG_LINE_LIMIT);

	ui->dialogs_line2_text1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_line2_text2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_line2_text3->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer2_text1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer2_text2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->dialogs_answer2_text3->setMaxLength(DIALOG_LINE_LIMIT);

	ui->boss_dialog_text1_line1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_text1_line2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_text1_line3->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer1_line1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer1_line2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer1_line3->setMaxLength(DIALOG_LINE_LIMIT);

	ui->boss_dialog_text2_line1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_text2_line2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_text2_line3->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer2_line1->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer2_line2->setMaxLength(DIALOG_LINE_LIMIT);
	ui->boss_dialog_answer2_line3->setMaxLength(DIALOG_LINE_LIMIT);
	fill_stage_tab_data();
}

stage_edit::~stage_edit()
{
	delete ui;
}

void stage_edit::fill_players_combo(QComboBox* combo)
{
	for (int i=0; i<MAX_FILE_PLAYERS; i++) {
		combo->addItem(QString::number(i+1)+QString(" [")+QString(game_data.players[i].name)+QString("]"));
	}
}

void stage_edit::fill_stage_tab_data()
{
	QListWidgetItem* item_empty = new QListWidgetItem;
	item_empty->setText("");
	std::string filename = FILEPATH + "/data/images/empty.png";
	QPixmap image(filename.c_str());
	image = image.copy(0, 0, image.width(), image.height());
	image = image.scaled(32, 32);
	item_empty->setIcon(image);

	dataExchanger->current_player = 0;
	fill_players_combo(ui->dialogs_answer1_player);

	// FILL ITEMS
	for (int i=0; i<MAX_STAGES; i++) {
		ui->stages_tab_stage_combo->addItem(QString::number(i)+QString("[")+QString(game_data.stages[i].name)+QString("]"));
	}
	// fill bg music combo
	common::fill_files_combo(std::string("/data/music"), ui->stages_tab_bgmusic_combo);
	// fill boss faces
	ui->stages_tab_bossfaces_view->addItem(item_empty);
	common::fill_graphicfiles_listwidget(std::string("data/images/faces"), ui->stages_tab_bossfaces_view);
	// fill digalogs faces
	common::fill_graphicfiles_listwidget(std::string("data/images/faces"), ui->dialogs_line1_face);

	QString temp_name(game_data.stages[0].boss.face_graphics_filename);
	for (int i=0; i<ui->stages_tab_bossfaces_view->count(); i++) {
		if (ui->stages_tab_bossfaces_view->item(i)->text() == temp_name) {
			ui->stages_tab_bossfaces_view->item(i)->setSelected(true);
			ui->stages_tab_bossfaces_view->setCurrentRow(i);
		} else {
			ui->stages_tab_bossfaces_view->item(i)->setSelected(false);
		}
	}

	for (int i=0; i<WEAPON_COUNT; i++) {
		ui->stage_boss_weapon_combo->addItem(QString(game_data.weapons[i].name));
	}
	ui->stage_boss_weapon_combo->setCurrentIndex(game_data.stages[dataExchanger->currentStage].boss.id_weapon);

	update_stage_data();
}


void stage_edit::update_stage_data()
{
	// SET ITEMS
	ui->stages_tab_stage_name_lineedit->setText(game_data.stages[dataExchanger->currentStage].name);
	ui->stages_tab_bossname_lineedit->setText(game_data.stages[dataExchanger->currentStage].boss.name);
	ui->stages_tab_bgmusic_combo->setCurrentIndex(ui->stages_tab_bgmusic_combo->findText(QString(game_data.stages[dataExchanger->currentStage].bgmusic_filename)));
	//ui->dialogs_line1_face->setCurrentIndex(ui->dialogs_line1_face->findText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.face_graphics_filename)));

	std::string search_text = std::string(game_data.stages[dataExchanger->currentStage].intro_dialog.face_graphics_filename);
	for (int i=0; i<ui->dialogs_line1_face->count(); i++) {
		std::string item_text = ui->dialogs_line1_face->item(i)->text().toStdString();
		if (item_text == search_text) {
			ui->dialogs_line1_face->item(i)->setSelected(true);
			ui->dialogs_line1_face->setCurrentRow(i);
			break;
		}
	}
	//ui->dialogs_line1_face->item(0)->setSelected(true);

	QString temp_name(game_data.stages[dataExchanger->currentStage].boss.face_graphics_filename);
	for (int i=0; i<ui->stages_tab_bossfaces_view->count(); i++) {
		if (ui->stages_tab_bossfaces_view->item(i)->text() == temp_name) {
			ui->stages_tab_bossfaces_view->item(i)->setSelected(true);
			ui->stages_tab_bossfaces_view->setCurrentRow(i);
		} else {
			ui->stages_tab_bossfaces_view->item(i)->setSelected(false);
		}
	}

	//std::cout << "stage: " << dataExchanger->currentStage << ", boss.id_weapon: " << game_data.stages[dataExchanger->currentStage].boss.id_weapon << std::endl;
	ui->stage_boss_weapon_combo->setCurrentIndex(game_data.stages[dataExchanger->currentStage].boss.id_weapon);

	ui->dialogs_line1_text1->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[0]));
	ui->dialogs_line1_text2->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[1]));
	ui->dialogs_line1_text3->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[2]));

	ui->dialogs_line2_text1->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[0]));
	ui->dialogs_line2_text2->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[1]));
	ui->dialogs_line2_text3->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[2]));

	ui->dialogs_answer1_text1->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][0]));
	ui->dialogs_answer1_text2->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][1]));
	ui->dialogs_answer1_text3->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][2]));

	ui->dialogs_answer2_text1->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][0]));
	ui->dialogs_answer2_text2->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][1]));
	ui->dialogs_answer2_text3->setText(QString(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][2]));



	ui->boss_dialog_text1_line1->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[0]));
	ui->boss_dialog_text1_line2->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[1]));
	ui->boss_dialog_text1_line3->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[2]));

	ui->boss_dialog_text2_line1->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line2[0]));
	ui->boss_dialog_text2_line2->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line2[1]));
	ui->boss_dialog_text2_line3->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.line2[2]));

	ui->boss_dialog_answer1_line1->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][0]));
	ui->boss_dialog_answer1_line2->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][1]));
	ui->boss_dialog_answer1_line3->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][2]));

	ui->boss_dialog_answer2_line1->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer2[dataExchanger->current_player][0]));
	ui->boss_dialog_answer2_line2->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer2[dataExchanger->current_player][1]));
	ui->boss_dialog_answer2_line3->setText(QString(game_data.stages[dataExchanger->currentStage].boss_dialog.answer2[dataExchanger->current_player][2]));


}

void stage_edit::on_stages_tab_stage_combo_currentIndexChanged(int index)
{
	dataExchanger->currentStage = index;
	update_stage_data();
}

void stage_edit::on_stages_tab_bossfaces_view_itemClicked(QListWidgetItem *item)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss.face_graphics_filename, "%s", item->text().toStdString().c_str());
}

void stage_edit::on_dialogs_line1_face_itemClicked(QListWidgetItem *item)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.face_graphics_filename, "%s", item->text().toStdString().c_str());
}



void stage_edit::on_dialogs_line1_text1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_line1_text2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_line1_text3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line1[2], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_line2_text1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_line2_text2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_line2_text3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.line2[2], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer1_text1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer1_text2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer1_text3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer1[dataExchanger->current_player][2], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer2_text1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer2_text2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer2_text3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].intro_dialog.answer2[dataExchanger->current_player][2], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_dialogs_answer1_player_currentIndexChanged(int index)
{
	dataExchanger->current_player = index;
	update_stage_data();
}

void stage_edit::on_boss_dialog_text1_line1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_boss_dialog_text1_line2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_boss_dialog_text1_line3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.line1[2], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_boss_dialog_answer1_line1_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][0], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_boss_dialog_answer1_line2_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][1], "%s", arg1.toStdString().c_str());
}

void stage_edit::on_boss_dialog_answer1_line3_textChanged(const QString &arg1)
{
	sprintf(game_data.stages[dataExchanger->currentStage].boss_dialog.answer1[dataExchanger->current_player][2], "%s", arg1.toStdString().c_str());
}
