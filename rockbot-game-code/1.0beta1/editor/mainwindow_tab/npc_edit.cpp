#include "npc_edit.h"
#include "ui_npc_edit.h"

#include "common.h"

npc_edit::npc_edit(QWidget *parent) : _data_loaded(false), QWidget(parent), _npcedit_tab_selectednpc(0), ui(new Ui::npc_edit)
{
	ui->setupUi(this);
	QObject::connect(ui->npc_edit_tab_previewarea, SIGNAL(clickedIn()), this, SLOT(set_npc_frame()));
    fill_data();
	_data_loaded = true;
	on_npc_edit_tab_selectnpccombo_currentIndexChanged(0);
}

npc_edit::~npc_edit()
{
    delete ui;
}



void npc_edit::fill_data()
{
    common::fill_files_combo("data/images/sprites/enemies", ui->npc_edit_tab_graphiccombo);
	ui->npc_edit_tab_graphiccombo->setCurrentIndex(ui->npc_edit_tab_graphiccombo->findText(QString(game_data.game_npcs[0].graphic_filename)));
	ui->npc_edit_tab_previewarea->set_graphicfile(FILEPATH+std::string("/data/images/sprites/enemies/")+std::string(game_data.game_npcs[0].graphic_filename));
	ui->npc_edit_tab_previewarea->repaint();

	common::fill_iatypes_combo(ui->npc_edit_tab_iatype);
    common::fill_npc_combo(ui->npc_edit_tab_selectnpccombo);
}

/*


void npc_edit::on_pushButton_11_clicked()
{
    for (int i=0; i<GAME_MAX_OBJS; i++) {
        if (game_data.game_npcs[i].id != -1) {
            game_data.game_npcs[i].id = i;
            ui->npc_edit_tab_selectnpccombo->setCurrentIndex(i);
        }
    }
}
*/


void npc_edit::on_npc_edit_tab_selectnpccombo_currentIndexChanged(int index)
{
	if (_data_loaded == false) {
		return;
	}
	_npcedit_tab_selectednpc = index;
	ui->npc_edit_tab_canshoot->setChecked(game_data.game_npcs[index].can_shoot);
	ui->npc_edit_tab_graphiccombo->setCurrentIndex(ui->npc_edit_tab_graphiccombo->findText(QString(game_data.game_npcs[index].graphic_filename)));
	ui->npc_edit_tab_graphicheight->setValue(game_data.game_npcs[index].frame_size.heigth);
	ui->npc_edit_tab_graphicwidth->setValue(game_data.game_npcs[index].frame_size.width);
	dataExchanger->npcGraphicSize_h = game_data.game_npcs[index].frame_size.heigth;
	dataExchanger->npcGraphicSize_w = game_data.game_npcs[index].frame_size.width;
	ui->npc_edit_tab_iatype->setCurrentIndex(game_data.game_npcs[index].IA_type);
	ui->npc_edit_tab_movespeed->setValue(game_data.game_npcs[index].speed);
	ui->npc_edit_tab_NpcHP->setValue(game_data.game_npcs[index].hp.total);
	ui->npc_edit_tab_NpcName->setText(game_data.game_npcs[index].name);
	ui->npc_edit_tab_range->setValue(game_data.game_npcs[index].walk_range);
	ui->npc_edit_tab_shieldtype->setCurrentIndex(game_data.game_npcs[index].shield_type);
	//ui->npc_edit_tab_ = game_data.game_npcs[index].;
}

void npc_edit::on_npc_edit_tab_graphiccombo_currentIndexChanged(const QString &arg1)
{
	if (_data_loaded == false) {
		return;
	}
	sprintf(game_data.game_npcs[_npcedit_tab_selectednpc].graphic_filename, "%s", arg1.toStdString().c_str());
	std::cout << "npc[" << _npcedit_tab_selectednpc << "].graphic_filename: " << game_data.game_npcs[_npcedit_tab_selectednpc].graphic_filename << std::endl;
	ui->npc_edit_tab_previewarea->set_graphicfile(FILEPATH+std::string("/data/images/sprites/enemies/")+arg1.toStdString());
	ui->npc_edit_tab_previewarea->repaint();
}

void npc_edit::on_npc_edit_tab_graphicwidth_valueChanged(int arg1)
{
	ui->npc_edit_tab_previewarea->set_grid_w(arg1);
	game_data.game_npcs[_npcedit_tab_selectednpc].frame_size.width = arg1;
}

void npc_edit::on_npc_edit_tab_graphicheight_valueChanged(int arg1)
{
	ui->npc_edit_tab_previewarea->set_grid_h(arg1);
	game_data.game_npcs[_npcedit_tab_selectednpc].frame_size.heigth = arg1;
}

void npc_edit::on_npc_edit_tab_NpcName_textChanged(const QString &arg1)
{
	sprintf(game_data.game_npcs[_npcedit_tab_selectednpc].name, "%s", arg1.toStdString().c_str());
	ui->npc_edit_tab_selectnpccombo->setItemText(_npcedit_tab_selectednpc, QString(game_data.game_npcs[_npcedit_tab_selectednpc].name));
}

void npc_edit::on_npc_edit_tab_NpcHP_valueChanged(int arg1)
{
	game_data.game_npcs[_npcedit_tab_selectednpc].hp.total = arg1;
}

void npc_edit::on_npc_edit_tab_canshoot_toggled(bool checked)
{
	game_data.game_npcs[_npcedit_tab_selectednpc].can_shoot = checked;
}

void npc_edit::on_npc_edit_tab_shieldtype_currentIndexChanged(int index)
{
	if (_data_loaded == false) {
		return;
	}
	game_data.game_npcs[_npcedit_tab_selectednpc].shield_type = index;
}

void npc_edit::on_npc_edit_tab_iatype_currentIndexChanged(int index)
{
	if (_data_loaded == false) {
		return;
	}
	game_data.game_npcs[_npcedit_tab_selectednpc].IA_type = index;
}

void npc_edit::on_npc_edit_tab_movespeed_valueChanged(int arg1)
{
	game_data.game_npcs[_npcedit_tab_selectednpc].speed = arg1;
}

void npc_edit::on_npc_edit_tab_range_valueChanged(int arg1)
{
	game_data.game_npcs[_npcedit_tab_selectednpc].walk_range = arg1;
}

void npc_edit::on_pushButton_11_clicked()
{
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.game_npcs[i].id == -1) {
			game_data.game_npcs[i].id = i;
			QString temp_str = QString("[") + QString::number(i) + QString("]") + QString("New NPC");
			ui->npc_edit_tab_selectnpccombo->addItem(temp_str);
			ui->npc_edit_tab_selectnpccombo->setCurrentIndex(i);
			return;
		}
	}
}

void npc_edit::on_npc_edit_tab_frametype_currentIndexChanged(int index)
{
	if (_data_loaded == false) {
		return;
	}
	game_data.game_npcs[_npcedit_tab_selectednpc].frames[dataExchanger->getPalleteX()].state = index-1;
	//dataExchanger->frameset[dataExchanger->getPalleteX()] = index-1;
}

void npc_edit::on_npc_edit_tab_frameduration_valueChanged(int arg1)
{
	game_data.game_npcs[_npcedit_tab_selectednpc].frames[dataExchanger->getPalleteX()].duration = arg1;
}

void npc_edit::set_npc_frame() {
	printf(">> DialogNPCEdit::setNPCFrame - CALLED\n");
	ui->npc_edit_tab_frameduration->setValue(game_data.game_npcs[_npcedit_tab_selectednpc].frames[dataExchanger->getPalleteX()].duration);
	ui->npc_edit_tab_frametype->setCurrentIndex(game_data.game_npcs[_npcedit_tab_selectednpc].frames[dataExchanger->getPalleteX()].state+1);
}
