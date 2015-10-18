#include "ai_tab.h"
#include "ui_ai_tab.h"
//#include "defines.h"
#include "common.h"


#include <QStringListModel>

ai_tab::ai_tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ai_tab)
{
    ui->setupUi(this);
	for (int i=0; i<MAX_AI_TYPES; i++) {
		QString temp_str = QString("[") + QString::number(i) + QString("] - ") + QString(game_data.ai_types[i].name);
		ui->ai_select_combo->insertItem(i, temp_str);
	}
	ui->ai_actions_list->setModel(new QStringListModel(_item_list));
}

ai_tab::~ai_tab()
{
    delete ui;
}

void ai_tab::on_pushButton_10_clicked()
{
	if (ui->action_radio_jump->isChecked()) {
		game_data.ai_types[ui->ai_select_combo->currentIndex()].states[_item_list.size()].state = ui->ai_action_jump_combo->currentIndex();
		switch (ui->ai_action_jump_combo->currentIndex()) {
		case 0:
			_item_list.insert(_item_list.size(), "JUMP-LEFT");
			break;
		case 1:
			_item_list.insert(_item_list.size(), "JUMP-CENTER");
			break;
		case 2:
			_item_list.insert(_item_list.size(), "JUMP-RIGHT");
			break;
		case 3:
			_item_list.insert(_item_list.size(), "JUMP TO TOP");
			break;
		case 4:
			_item_list.insert(_item_list.size(), "JUMP TO GROUND");
			break;
		case 5:
			_item_list.insert(_item_list.size(), "JUMP TO PLAYER");
			break;
		default:
			break;
		}
	} else if (ui->action_radio_attack->isChecked()) {

	} else if (ui->action_radio_move->isChecked()) {
		switch (ui->ai_action_move_combo->currentIndex()) {
		case 0:
			_item_list.insert(_item_list.size(), "MOVE LEFT");
			break;
		case 1:
			_item_list.insert(_item_list.size(), "MOVE RIGHT");
			break;
		case 2:
			_item_list.insert(_item_list.size(), "MOVE CROSS-SCREEN");
			break;
		case 3:
			_item_list.insert(_item_list.size(), "MOVE FOLLOW-PLAYER");
			break;
		case 4:
			_item_list.insert(_item_list.size(), "MOVE RETURN");
			break;
		case 5:
			_item_list.insert(_item_list.size(), "MOVE PLAYER-DIRECTION");
			break;
		default:
			break;
		}
	} else if (ui->action_radio_wait->isChecked()) {
		QString temp_str = QString("WAIT ") + QString::number(ui->ai_action_wait_spinbox->value());
		_item_list.insert(_item_list.size(), temp_str);
	}
	ui->ai_actions_list->setModel(new QStringListModel(_item_list));
	ui->ai_actions_list->repaint();
}

void ai_tab::on_remove_pushButton_clicked()
{
	_item_list.removeAt(ui->ai_actions_list->currentIndex().row());
	ui->ai_actions_list->setModel(new QStringListModel(_item_list));
	ui->ai_actions_list->repaint();
}

void ai_tab::on_ai_name_edit_textChanged(const QString &arg1)
{
	sprintf(game_data.ai_types[ui->ai_select_combo->currentIndex()].name, "%s", arg1.toStdString().c_str());
	QString temp_str = QString("[") + QString::number(ui->ai_select_combo->currentIndex()) + QString("] - ") + QString(game_data.ai_types[ui->ai_select_combo->currentIndex()].name);

	ui->ai_select_combo->setItemText(ui->ai_select_combo->currentIndex(), temp_str);
}
