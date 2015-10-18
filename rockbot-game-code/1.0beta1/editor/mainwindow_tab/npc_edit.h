#ifndef NPC_EDIT_H
#define NPC_EDIT_H

#include <QWidget>

namespace Ui {
    class npc_edit;
}

class npc_edit : public QWidget
{
    Q_OBJECT

public:
    explicit npc_edit(QWidget *parent = 0);
    ~npc_edit();

private slots:
	void on_npc_edit_tab_selectnpccombo_currentIndexChanged(int index);

	void on_npc_edit_tab_graphiccombo_currentIndexChanged(const QString &arg1);

	void on_npc_edit_tab_graphicwidth_valueChanged(int arg1);

	void on_npc_edit_tab_graphicheight_valueChanged(int arg1);

	void on_npc_edit_tab_NpcName_textChanged(const QString &arg1);

	void on_npc_edit_tab_NpcHP_valueChanged(int arg1);

	void on_npc_edit_tab_canshoot_toggled(bool checked);

	void on_npc_edit_tab_shieldtype_currentIndexChanged(int index);

	void on_npc_edit_tab_iatype_currentIndexChanged(int index);

	void on_npc_edit_tab_movespeed_valueChanged(int arg1);

	void on_npc_edit_tab_range_valueChanged(int arg1);

	void on_pushButton_11_clicked();

	void on_npc_edit_tab_frametype_currentIndexChanged(int index);

	void on_npc_edit_tab_frameduration_valueChanged(int arg1);

private:
	void fill_data();

public slots:
	void set_npc_frame();


private:
    Ui::npc_edit *ui;
	int _npcedit_tab_selectednpc;
	bool _data_loaded; // prevents "on_change" ahppening while data is being filled in
};

#endif // NPC_EDIT_H
