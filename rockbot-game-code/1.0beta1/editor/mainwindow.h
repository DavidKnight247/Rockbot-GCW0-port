#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDir>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QComboBox>
#include <QListWidget>

#include "mainwindow_tab/npc_edit.h"
#include "mainwindow_tab/ai_tab.h"
#include "mainwindow_tab/object_tab.h"
#include "mainwindow_tab/weapon_edit.h"
#include "mainwindow_tab/stage_edit.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	QStandardItemModel* model;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void fillGraphicsList();
	void fillMapList();
	void fillNpcList();
	void fillObjList();
	void fillStageList();
	void createProjectile();
	void reloadProjectileListCombo();
	void saveProjectile(int index);


private:
    Ui::MainWindow *ui;
	npc_edit *npc_edit_tab;
	ai_tab *ai_edit_tab;
	object_tab *object_edit_tab;
	weapon_edit *weapon_edit_tab;
	stage_edit *stage_edit_tab;
	int currentProjectileIndex;
	void fill_players_data();
	void fill_players_combo(QComboBox* combo);
	void fill_NPC_edit_tab();
	void fill_background_list();

private slots:

	void on_projectileList_combo_currentIndexChanged(int index);
 void on_projectileHeightSpinBox_valueChanged(int );
 void on_projectileWidthSpinBox_valueChanged(int );
 void on_projectile_imagefile_combo_currentIndexChanged(QString );
 void on_pushButton_5_clicked();
 void on_npc_direction_combo_currentIndexChanged(int index);
	void on_link_orientation_combobox_currentIndexChanged(int index);
	void on_spinBox_valueChanged(int );
	void on_listWidget_2_currentRowChanged(int currentRow);
	void on_pushButton_clicked();
	void on_toolBox_currentChanged(int index);
	void on_objectListWidget_currentRowChanged(int currentRow);
	void on_editObjectButton_clicked();
	void on_mapListCombo_currentIndexChanged(int index);
	void on_newObjectButton_clicked();
    void on_actionAdd_Object_triggered();
    void on_actionThree_triggered();
    void on_actionTwo_triggered();
    void on_actionOne_triggered();
    void on_newNPCButton_clicked();
    void on_editNPCButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_actionEdit_Tileset_triggered();
    void on_actionEdit_NPC_triggered();
    void on_comboBox_currentIndexChanged(int index);
    void on_actionStairs_triggered();
    void on_actionLink_triggered();
    void on_MainWindow_iconSizeChanged(QSize iconSize);
    void on_actionNew_triggered();
    void on_actionFill_triggered();
    void on_actionEraser_triggered();
    void on_actionNormal_Edit_triggered();
    void on_actionLock_Edit_triggered();
    void on_pallete_signalPalleteChanged();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionQuit_triggered();

	void fillProjectileImagesList();
	void fill_player_graphics_list();
	void fillTrajectoriesList();



	void on_stageListCombo_currentIndexChanged(int index);

	void on_link_bidi_checkbox_toggled(bool checked);

	void on_link_isdoor_checkbox_toggled(bool checked);

	void on_pushButton_2_clicked();

	void on_player_sprite_h_valueChanged(int arg1);

	void on_player_hitarea_x_valueChanged(int arg1);

	void on_player_hitarea_y_valueChanged(int arg1);

	void on_player_hitarea_w_valueChanged(int arg1);

	void on_player_hitarea_h_valueChanged(int arg1);

	void on_player_sprite_w_valueChanged(int arg1);

	void on_player_graphics_combo_currentIndexChanged(const QString &arg1);

	void on_pushButton_7_clicked();

	void on_pushButton_8_clicked();

	void on_pushButton_9_clicked();

	void on_comboBox_7_currentIndexChanged(int index);

	void on_comboBox_6_currentIndexChanged(int index);

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_players_tab_list_combo_currentIndexChanged(int index);


	void on_tabWidget_currentChanged(int index);

    void on_actionSet_Boss_triggered(bool checked);

	void on_bg1_filename_currentIndexChanged(const QString &arg1);

	void on_checkBox_clicked(bool checked);

	void on_bg1_y_pos_valueChanged(int arg1);

	void on_bg2_filename_currentIndexChanged(const QString &arg1);

	void on_bg2_y_pos_valueChanged(int arg1);

	void on_bg1_speed_valueChanged(int arg1);

	void on_bg2_speed_valueChanged(int arg1);

	void on_bg_color_pick_clicked();

	void on_checkBox_2_clicked(bool checked);

	void on_checkBox_3_clicked(bool checked);

	void on_stage_boss_weapon_combo_currentIndexChanged(int index);

	void on_bg1_speed_valueChanged(double arg1);

	void on_bg2_speed_valueChanged(double arg1);

	void on_actionReset_Map_triggered();

public slots:
	void reloadComboItems();
	void pick_player_keycolor1();
	void pick_player_keycolor2();
	void pick_player_keycolor3();
	void pick_player_color1();
	void pick_player_color2();
	void pick_player_color3();
	void pick_bg_color();


private:
	// USED IN NPC TAB
	std::string _npcedit_tab_graphic;
	int _npcedit_tab_selectednpc;

};


#endif // MAINWINDOW_H
