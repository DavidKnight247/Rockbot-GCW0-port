#ifndef STAGE_EDIT_H
#define STAGE_EDIT_H

#include <QWidget>
#include <QComboBox>
#include <QListWidgetItem>

namespace Ui {
class stage_edit;
}

class stage_edit : public QWidget
{
	Q_OBJECT
	
public:
	explicit stage_edit(QWidget *parent = 0);
	~stage_edit();
	void update_stage_data();

	
private slots:
	void on_stages_tab_stage_combo_currentIndexChanged(int index);
	void on_stages_tab_bossfaces_view_itemClicked(QListWidgetItem *item);
	void on_dialogs_line1_face_itemClicked(QListWidgetItem *item);
	void on_dialogs_line1_text1_textChanged(const QString &arg1);
	void on_dialogs_line1_text2_textChanged(const QString &arg1);
	void on_dialogs_line1_text3_textChanged(const QString &arg1);
	void on_dialogs_line2_text1_textChanged(const QString &arg1);
	void on_dialogs_line2_text2_textChanged(const QString &arg1);
	void on_dialogs_line2_text3_textChanged(const QString &arg1);
	void on_dialogs_answer1_text1_textChanged(const QString &arg1);
	void on_dialogs_answer1_text2_textChanged(const QString &arg1);
	void on_dialogs_answer1_text3_textChanged(const QString &arg1);
	void on_dialogs_answer2_text1_textChanged(const QString &arg1);
	void on_dialogs_answer2_text2_textChanged(const QString &arg1);
	void on_dialogs_answer2_text3_textChanged(const QString &arg1);
	void on_dialogs_answer1_player_currentIndexChanged(int index);

	void on_boss_dialog_text1_line1_textChanged(const QString &arg1);

	void on_boss_dialog_text1_line2_textChanged(const QString &arg1);

	void on_boss_dialog_text1_line3_textChanged(const QString &arg1);

	void on_boss_dialog_answer1_line1_textChanged(const QString &arg1);

	void on_boss_dialog_answer1_line2_textChanged(const QString &arg1);

	void on_boss_dialog_answer1_line3_textChanged(const QString &arg1);

private:
	void fill_players_combo(QComboBox* combo);
	void fill_stage_tab_data();

private:
	Ui::stage_edit *ui;
};

#endif // STAGE_EDIT_H
