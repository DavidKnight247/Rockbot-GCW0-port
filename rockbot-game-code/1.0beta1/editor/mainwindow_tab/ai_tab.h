#ifndef AI_TAB_H
#define AI_TAB_H

#include <QWidget>

namespace Ui {
    class ai_tab;
}

class ai_tab : public QWidget
{
    Q_OBJECT

public:
    explicit ai_tab(QWidget *parent = 0);
    ~ai_tab();

private slots:
	void on_pushButton_10_clicked();

	void on_remove_pushButton_clicked();

	void on_ai_name_edit_textChanged(const QString &arg1);

private:
    Ui::ai_tab *ui;
	QStringList _item_list;
};

#endif // AI_TAB_H
