#include "npcpreviewarea.h"

npcPreviewArea::npcPreviewArea(QWidget *parent) : QWidget(parent), graphic_filename(""), img_grid_w(16), img_grid_h(16) {
    myParent = parent;
	editor_selectedTileX = 0;
	editor_selectedTileY = 0;
	parent->resize(QSize(999, 200));
	this->show();
}

void npcPreviewArea::set_graphicfile(std::string filename)
{
	graphic_filename = filename;
}

void npcPreviewArea::set_grid_w(int new_w)
{
	img_grid_w = new_w;
	repaint();
}

void npcPreviewArea::set_grid_h(int new_h)
{
	img_grid_h = new_h;
	repaint();
}


void npcPreviewArea::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QLineF *line;
	QRectF *target, *source;
	int i;

	if (graphic_filename.size() < 1) {
		return;
	}

	//printf("DEBUG - EditorArea.paintEvent\n");
	// draw the picked tile in the tileset
	QPixmap image(graphic_filename.c_str());
	if (image.isNull()) {
		return;
	}
	image_w = image.width();
	image_h = image.height();
	image = image.scaled(image.width()*2, image.height()*2);
	this->resize(QSize(image.width()+1, image.height()+1));
	myParent->adjustSize();

	target = new QRectF(QPoint(0, 0), QSize(image.width(), image.height()));
	source = new QRectF(QPoint(0, 0), QSize(image.width(), image.height()));
	painter.drawPixmap(*target, image, *source);

	painter.setPen(QColor(0, 120, 0));
	for (i=0; i<=this->width(); i=i+img_grid_w*2) {
	  // linhas verticais
	  line = new QLineF(i, 0, i, this->height());
	  painter.drawLine(*line);
	}
	for (i=0; i<this->height(); i=i+img_grid_h*2) {
	  // linhas horizontais
	  line = new QLineF(0, i, this->width(), i);
	  painter.drawLine(*line);
	}

	int posX = editor_selectedTileX*img_grid_w*2;
	int posY = editor_selectedTileY*img_grid_h*2;
	// paint the selected frame
	painter.setPen(QColor(255, 0, 0));
	line = new QLineF(posX, posY, posX+img_grid_w*2, posY);
	painter.drawLine(*line);
	line = new QLineF(posX, posY, posX, posY+img_grid_h*2);
	painter.drawLine(*line);
	line = new QLineF(posX, posY+img_grid_h*2, posX+img_grid_w*2, posY+img_grid_h*2);
	painter.drawLine(*line);
	line = new QLineF(posX+img_grid_w*2, posY+img_grid_h*2, posX+img_grid_w*2, posY);
	painter.drawLine(*line);
}


void npcPreviewArea::mousePressEvent(QMouseEvent *event) {
	printf(">> INSIDE npcPreviewArea::mousePressEvent, editMode: %d, editTool: %d\n", dataExchanger->editMode, dataExchanger->editTool);

	QPoint pnt = event->pos();
	// checks if click was inside the image
	if (pnt.x() <= image_w*2 && pnt.y() <= image_h*2) {
		editor_selectedTileX = pnt.x()/img_grid_w/2;
		editor_selectedTileY = pnt.y()/img_grid_h/2;
		printf(">> npcPreviewArea::mousePressEvent - x: %d, y: %d\n", editor_selectedTileX, editor_selectedTileY);
		dataExchanger->setPalleteX(editor_selectedTileX);
		dataExchanger->setPalleteY(editor_selectedTileY);
		emit clickedIn();
		repaint();
	}
}

int npcPreviewArea::get_cursor_pos()
{
	return editor_selectedTileX;
}
