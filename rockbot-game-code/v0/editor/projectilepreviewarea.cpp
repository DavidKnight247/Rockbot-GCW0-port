#include "projectilepreviewarea.h"

projectilePreviewArea::projectilePreviewArea(QWidget *parent) : QWidget(parent) {
    myParent = parent;
	editor_selectedTileX = 0;
	editor_selectedTileY = 0;
	parent->resize(QSize(999, 200));
	this->show();
}

void projectilePreviewArea::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QLineF *line;
	QRectF *target, *source;
	int i;
	//printf("DEBUG - EditorArea.paintEvent\n");
	// draw the picked tile in the tileset
	//printf("DEBUG.projectilePreviewArea::paintEvent - filename: %s\n", dataExchanger->addprojectileFilename);
	QPixmap image(dataExchanger->addProjectileFilename);
	if (image.isNull()) {
		//printf("projectilePreviewArea::paintEvent - Could not load image file '%s'\n", dataExchanger->addProjectileFilename);
		return;
	}
	image_w = image.width();
	image_h = image.height();
	image = image.scaled(image.width()*2, image.height()*2);
	this->resize(image.size());
	myParent->adjustSize();

	target = new QRectF(QPoint(0, 0), QSize(image.width(), image.height()));
	source = new QRectF(QPoint(0, 0), QSize(image.width(), image.height()));
	painter.drawPixmap(*target, image, *source);

	//printf("projectilePreviewArea::paintEvent - gSize: %d\n", dataExchanger->projectileGraphicSize_w);

   painter.setPen(QColor(0, 120, 0));
   for (i=1; i<=this->width(); i=i+dataExchanger->projectileGraphicSize_w*2) {
	  // linhas verticais
	  line = new QLineF(i, 0, i, this->height());
	  painter.drawLine(*line);
   }
   for (i=1; i<this->height(); i=i+dataExchanger->projectileGraphicSize_h*2) {
	  // linhas horizontais
	  line = new QLineF(0, i, this->width(), i);
	  painter.drawLine(*line);
   }

   int posX = editor_selectedTileX*dataExchanger->projectileGraphicSize_w*2;
   int posY = editor_selectedTileY*dataExchanger->projectileGraphicSize_h*2;
   // paint the selected frame
   painter.setPen(QColor(255, 0, 0));
   line = new QLineF(posX, posY, posX+dataExchanger->projectileGraphicSize_w*2, posY);
   painter.drawLine(*line);
   line = new QLineF(posX, posY, posX, posY+dataExchanger->projectileGraphicSize_h*2);
   painter.drawLine(*line);
   line = new QLineF(posX, posY+dataExchanger->projectileGraphicSize_h*2, posX+dataExchanger->projectileGraphicSize_w*2, posY+dataExchanger->projectileGraphicSize_h*2);
   painter.drawLine(*line);
   line = new QLineF(posX+dataExchanger->projectileGraphicSize_w*2, posY+dataExchanger->projectileGraphicSize_h*2, posX+dataExchanger->projectileGraphicSize_w*2, posY);
   painter.drawLine(*line);
}


