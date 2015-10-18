#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDir>
#include <QModelIndex>
#include <QStandardItemModel>

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
	int currentProjectileIndex;

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
	void fillTrajectoriesList();



	void on_stageListCombo_currentIndexChanged(int index);

public slots:
	void reloadComboItems();


};


#endif // MAINWINDOW_H
