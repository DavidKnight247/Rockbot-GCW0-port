#ifndef OBJECT_TAB_H
#define OBJECT_TAB_H

#include <QWidget>

namespace Ui {
    class object_tab;
}

class object_tab : public QWidget
{
    Q_OBJECT

public:
    explicit object_tab(QWidget *parent = 0);
    ~object_tab();

private slots:
	void on_graphicfile_combo_currentIndexChanged(const QString &arg1);

	void on_objectlist_combo_currentIndexChanged(int index);

	void on_add_new_pushbutton_clicked();

	void on_name_textChanged(const QString &arg1);

	void on_type_combo_currentIndexChanged(int index);

	void on_limit_valueChanged(int arg1);

	void on_graphic_w_valueChanged(int arg1);

	void on_graphic_h_valueChanged(int arg1);

private:
    Ui::object_tab *ui;
	int _current_object;
	int _data_loaded;
};

#endif // OBJECT_TAB_H
