#include "weapon_edit.h"
#include "ui_weapon_edit.h"

#include "common.h"

weapon_edit::weapon_edit(QWidget *parent) :
QWidget(parent),
ui(new Ui::weapon_edit), _selected_weapon(0), _loaded(false)
{
	ui->setupUi(this);
	reload_weapon_list();
	for (int i=0; i<MAX_FILE_PROJECTILES; i++) {
		ui->weapon_projectile_type->addItem(QString("[")+QString::number(i)+QString("] - ")+QString(game_data.projectiles[i].name));
	}
	_loaded = true;
}

weapon_edit::~weapon_edit()
{
	delete ui;
}

void weapon_edit::reload_weapon_list()
{
	ui->weapon_select_combo->clear();
	for (int i=0; i<WEAPON_COUNT; i++) {
		ui->weapon_select_combo->addItem(QString("[")+QString::number(i)+QString("] - ")+QString(game_data.weapons[i].name));
	}
}

void weapon_edit::on_weapon_select_combo_currentIndexChanged(int index)
{
	_selected_weapon = index;
	ui->weapon_name->setText(QString(game_data.weapons[_selected_weapon].name));
	ui->weapon_damage->setValue(game_data.weapons[_selected_weapon].damage);
	ui->weapon_projectile_type->setCurrentIndex(game_data.weapons[_selected_weapon].id_projectile);
}

void weapon_edit::on_weapon_name_textChanged(const QString &arg1)
{
	sprintf(game_data.weapons[_selected_weapon].name, "%s", arg1.toStdString().c_str());
	//reload_weapon_list();
}

void weapon_edit::on_weapon_projectile_type_currentIndexChanged(int index)
{
	game_data.weapons[_selected_weapon].id_projectile = index;
}

void weapon_edit::on_weapon_damage_valueChanged(int arg1)
{
	game_data.weapons[_selected_weapon].damage = arg1;
}

void weapon_edit::on_weapon_name_lostFocus()
{
	reload_weapon_list();
}
