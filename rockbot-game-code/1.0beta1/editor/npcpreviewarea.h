#ifndef NPCPREVIEWAREA_H
#define NPCPREVIEWAREA_H

#include <QWidget>
#include <QPainter>
#include <stdio.h>
#include <QMouseEvent>
#include "mediator.h"


extern Mediator *dataExchanger;

class npcPreviewArea : public QWidget
{
  Q_OBJECT

public:
  // methods
  npcPreviewArea(QWidget *parent = 0);
  void set_graphicfile(std::string filename);
  void set_grid_w(int new_w);
  void set_grid_h(int new_h);
  int get_cursor_pos();

  // variables
  //QWidget *myParent;
private:
  int editor_selectedTileX;
  int editor_selectedTileY;
  std::string graphic_filename;
  int img_grid_w;
  int img_grid_h;
  int image_w;
  int image_h;

protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent * event);

  QWidget *myParent;

signals:
	 void clickedIn();

public slots:
   //void changeTile();
};


#endif // NPCPREVIEWAREA_H
