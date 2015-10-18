#include "editorarea.h"
#include <stdio.h>
#include <QBrush>
#include <QDir>
#include "defines.h"
#include <QResource>

#include <iostream>

extern format_v1_0::file_game game;
extern char EDITOR_FILEPATH[512];

#include "../file/format.h"
#include "../file/file_io.h"
extern format_v_2_0_1::file_game game_data;


// construtor
EditorArea::EditorArea(QWidget *parent) : QWidget(parent) {
	mouse_released = true;
    myParent = parent;
    temp = 0;
    editor_selectedTileX = 0;
    editor_selectedTileY = 0;
	tempX = -1;
	tempY = -1;
    my_pallete=NULL;
    this->show();
}

void EditorArea::changeTile() {
	printf("DEBUG.changeTile in editor area\n");
	temp = 1;
	repaint();
}

void EditorArea::setInfoPalette(EditorTilePallete *temp_pallete) {
   if (temp_pallete != NULL) {
      printf("DEBUG.EditorArea::setPallete - got pallete\n");
   } else {
      printf("DEBUG.EditorArea::setPallete - null pallete\n");
   }
   my_pallete = temp_pallete;
}

void EditorArea::paintEvent(QPaintEvent *) {
    int i, j, pos;
    QPainter painter(this);
    QLineF *line;
    QString *filename;

    filename = new QString("");
    filename->append(my_pallete->getPallete());
    QPixmap image(*filename);
    if (image.isNull()) {
        printf("ERROR: EditorArea::paintEvent - Could not load image file '%s'\n", qPrintable(*filename));
    }

	// draw background-color
	if (dataExchanger->show_background_color == true) {
		//painter.setPen(QColor(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.r, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.g, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.b, 255));
		painter.fillRect(QRectF(0.0, 0.0, MAP_W*TILESIZE, MAP_H*TILESIZE), QColor(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.r, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.g, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].background_color.b, 255));
	}
	// draw background1
	if (dataExchanger->show_bg1 == true && strlen(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename) > 0) {
		filename->clear();
		filename->append(QString(QString(EDITOR_FILEPATH)+"/data/images/map_backgrounds/"+QString(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename)));
		QPixmap bg1_image(*filename);
		if (bg1_image.isNull()) {
			printf("ERROR: EditorArea::paintEvent - Could not load bg1 image file '%s'\n", qPrintable(*filename));
		} else {
			for (int k=0; k<((MAP_W*16)/bg1_image.width())+1; k++) {
				QRectF pos_source(QPoint(0, 0), QSize(bg1_image.width(), bg1_image.height()));
				QRectF pos_dest(QPoint(k*bg1_image.width(), game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].adjust_y*dataExchanger->zoom), QSize(bg1_image.width()*dataExchanger->zoom, bg1_image.height()*dataExchanger->zoom));
				painter.drawPixmap(pos_dest, bg1_image, pos_source);
			}
		}
	}
	// draw background2
	if (dataExchanger->show_bg2 == true && strlen(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename) > 0) {
		filename->clear();
		filename->append(QString(QString(EDITOR_FILEPATH)+"/data/images/map_backgrounds/"+QString(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].filename)));
		QPixmap bg2_image(*filename);
		if (bg2_image.isNull()) {
			printf("ERROR: EditorArea::paintEvent - Could not load bg1 image file '%s'\n", qPrintable(*filename));
		} else {
			for (int k=0; k<((MAP_W*16)/bg2_image.width())+1; k++) {
				QRectF pos_source(QPoint(0, 0), QSize(bg2_image.width(), bg2_image.height()));
				QRectF pos_dest(QPoint(k*bg2_image.width(), game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[1].adjust_y*dataExchanger->zoom), QSize(bg2_image.width()*dataExchanger->zoom, bg2_image.height()*dataExchanger->zoom));
				painter.drawPixmap(pos_dest, bg2_image, pos_source);
			}
		}
	}



    // draw tiles
    for (i=0; i<MAP_W; i++) {
        for (j=0; j<MAP_H; j++) {
            // level one
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y != -1) {
                QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
				QRectF source(QPoint((game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x*16), (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y*16)), QSize(16, 16));
                painter.drawPixmap(target, image, source);
            }
            // level two
            if (dataExchanger->layerLevel == 2 || dataExchanger->layerLevel == 3) {
                    //printf(">> EditorArea::paintEvent - SHOW TILES LEVEL 2\n");
					if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y != -1) {
                        QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
						QRectF source(QPoint((game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x*16), (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y*16)), QSize(16, 16));
                        painter.drawPixmap(target, image, source);
                    }
            }
            if (dataExchanger->layerLevel == 3) {
					if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y != -1) {
                        QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
						QRectF source(QPoint((game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x*16), (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y*16)), QSize(16, 16));
                        painter.drawPixmap(target, image, source);
                    }
            }
			// locked areas, stairs, doors, etc
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked != 0 && dataExchanger->editTool == EDITMODE_LOCK) {
                    // transparent rectangle
					//painter.setBrush(QColor(255, 0, 0, 30));
					//painter.drawRect(i*16, j*16, 16, 16);
                    // red border
                    painter.setBrush(Qt::NoBrush);
                    painter.setPen(QColor(255, 0, 0, 255));
                    painter.drawRect(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom, 16*dataExchanger->zoom, 16*dataExchanger->zoom);
					// terrain type icon
					QString terrainIcon;
					QResource::registerResource("resources/icons/icons.qrc");
					if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 1) {
						terrainIcon = QString::fromUtf8(":/toolbar_icons/Lock"); // solid
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 2) {
						terrainIcon = QString(":/toolbar_icons/stairs.png"); // stairs
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 3) {
						terrainIcon = QString(":/toolbar_icons/object-order-lower.png"); // door
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 4) {
						terrainIcon = QString(":/toolbar_icons/edit-delete.png"); // spikes
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 5) {
						terrainIcon = QString(":/toolbar_icons/flag-blue.png"); // water
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 6) {
						terrainIcon = QString(":/toolbar_icons/flag-green.png"); // ice
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 7) {
						terrainIcon = QString(":/toolbar_icons/arrow-left.png"); // move left
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 8) {
						terrainIcon = QString(":/toolbar_icons/arrow-right.png"); // move right
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 9) {
						terrainIcon = QString(":/toolbar_icons/arrow-down.png"); // move right
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 10) {
						terrainIcon = QString(":/toolbar_icons/system-switch-user.png"); // move right
					} else if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].locked == 11) {
						terrainIcon = QString(":/toolbar_icons/Save"); // move right
					}
					QPixmap terrainImage(terrainIcon);
					if (terrainImage.isNull()) {
						printf("ERROR: EditorArea::paintEvent - terrainType - Could not load image file '%s'\n", qPrintable(terrainIcon));
					} else {
						terrainIcon.resize(16);
						painter.setOpacity(0.7);
						QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
						QRectF source(QPoint(0, 0), QSize(terrainImage.width (), terrainImage.height ()));
						painter.drawPixmap(target, terrainImage, source);
					}
					painter.setOpacity(1.0);
			}
        }
    }
    painter.setPen(QColor(120, 120, 120));
    for (i=1; i<MAP_W; i++) {
        pos = i*16*dataExchanger->zoom-1;
        //QLineF line(0, 800, 16, 800);
        // linhas horizontais
        line = new QLineF(pos, 0, pos, MAP_H*16*dataExchanger->zoom-1);
        painter.drawLine(*line);
    }
    for (i=1; i<MAP_H; i++) {
        pos = i*16*dataExchanger->zoom-1;
        //QLineF line(0, 800, 16, 800);
        // linhas verticais
        line = new QLineF(0, pos, MAP_W*16*dataExchanger->zoom-1, pos);
        painter.drawLine(*line);
    }
    // draw links
	//printf("editoMode: %d, EDITMODE_NORMAL: %d, editTool: %d, EDITMODE_LINK_DEST: %d\n", dataExchanger->editMode, EDITMODE_NORMAL, dataExchanger->editTool, EDITMODE_LINK_DEST);
    if (dataExchanger->editMode == EDITMODE_NORMAL && (dataExchanger->editTool == EDITMODE_LINK_DEST || dataExchanger->editTool == EDITMODE_LINK)) {


		for (int i=0; i<STAGE_MAX_LINKS; i++) {
			if (dataExchanger->currentMap ==  game_data.stages[dataExchanger->currentStage].links[i].id_map_origin) {
					// transparent blue rectangle
					painter.setBrush(QColor(0, 0, 255, 180));
					painter.drawRect(game_data.stages[dataExchanger->currentStage].links[i].pos_origin.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].links[i].pos_origin.y *16*dataExchanger->zoom, 16*dataExchanger->zoom*game_data.stages[dataExchanger->currentStage].links[i].size, 16*dataExchanger->zoom);
					// blue border
					painter.setBrush(Qt::NoBrush);
					painter.setPen(QColor(0, 0, 255, 255));
					painter.drawRect(game_data.stages[dataExchanger->currentStage].links[i].pos_origin.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].links[i].pos_origin.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*game_data.stages[dataExchanger->currentStage].links[i].size, 16*dataExchanger->zoom);
					painter.setPen(QColor(255, 255, 255, 255));
					painter.drawText(game_data.stages[dataExchanger->currentStage].links[i].pos_origin.x*16*dataExchanger->zoom, (game_data.stages[dataExchanger->currentStage].links[i].pos_origin.y+1)*16*dataExchanger->zoom, QString::number(i));
			} else if (dataExchanger->currentMap == game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny) {
					// transparent cyan rectangle
					painter.setBrush(QColor(0, 255, 255, 180));
					painter.drawRect(game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*game_data.stages[dataExchanger->currentStage].links[i].size, 16*dataExchanger->zoom);
					// cyan border
					painter.setBrush(Qt::NoBrush);
					painter.setPen(QColor(0, 255, 255, 255));
					painter.drawRect(game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*game_data.stages[dataExchanger->currentStage].links[i].size, 16*dataExchanger->zoom);
					painter.setPen(QColor(0, 0, 0, 255));
					painter.drawText(game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.x*16*dataExchanger->zoom, (game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.y+1)*16*dataExchanger->zoom, QString::number(i));
			}
		}
    }
	/// draw NPCs
	for (int i=0; i<MAX_MAP_NPC_N; i++) {
		if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc != -1) {
			std::string filename = FILEPATH + "/data/images/sprites/enemies/" + game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].graphic_filename;
			QPixmap temp_image(filename.c_str());
			if (temp_image.isNull()) {
				std::cout << "EditorArea::paintEvent - Could not load NPC image file '" << filename << std::endl;
			} else {
					QRectF target(QPoint(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y*16*dataExchanger->zoom), QSize(game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.width*dataExchanger->zoom, game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.heigth*dataExchanger->zoom));
					QRectF source;
					if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].direction != ANIM_DIRECTION_RIGHT) {
						source = QRectF(QPoint(0, 0), QSize(game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.width, game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.heigth));
					} else {
						source = QRectF(QPoint(0, game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.heigth), QSize(game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.width, game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].frame_size.heigth));
					}
                    if (game_data.stages[dataExchanger->currentStage].boss.id_npc == game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc) {
                        // transparent green rectangle
                        painter.setBrush(QColor(0, 255, 0, 180));
                        painter.drawRect(target);
                        painter.drawPixmap(target, temp_image, source);
                    } else {
                        painter.drawPixmap(target, temp_image, source);
                    }
			}

		}
	}



	/// draw objects
	for (int i=0; i<MAX_MAP_NPC_N; i++) {
		if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object != -1) {
			std::string filename = FILEPATH + "/data/images/sprites/objects/" + game_data.objects[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object].graphic_filename;
			QPixmap temp_image(filename.c_str());
			if (temp_image.isNull()) {
				//printf("EditorArea::paintEvent DEBUG.Editor - Could not load OBJECT image file '%s'\n", buffer);
				painter.setBrush(QColor(255, 255, 255, 180));
				painter.drawRect(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y*16*dataExchanger->zoom, 16*dataExchanger->zoom, 16*dataExchanger->zoom);
			} else {
				QRectF target(QPoint(game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x*16*dataExchanger->zoom, game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y*16*dataExchanger->zoom), QSize(game_data.objects[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object].size.width, game_data.objects[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object].size.heigth));
				QRectF source(QPoint(0, 0), QSize(game_data.objects[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object].size.width, game_data.objects[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object].size.heigth));
				painter.drawPixmap(target, temp_image, source);
			}
		}
	}


   QSize resizeMe(MAP_W*16*dataExchanger->zoom, MAP_H*16*dataExchanger->zoom);
   this->resize(resizeMe);
   myParent->adjustSize();
}

void EditorArea::mouseMoveEvent(QMouseEvent *event) {
	QPoint pnt = event->pos();
        if (editor_selectedTileX != pnt.x()/(16*dataExchanger->zoom) || editor_selectedTileY != pnt.y()/(16*dataExchanger->zoom)) {
		mousePressEvent(event);
	}
}

void EditorArea::mousePressEvent(QMouseEvent *event) {
    if (mouse_released == false && (dataExchanger->editTool == EDITMODE_LINK || dataExchanger->editTool == EDITMODE_LINK_DEST || dataExchanger->editMode == EDITMODE_NPC || dataExchanger->editMode == EDITMODE_OBJECT || dataExchanger->editMode == EDITMODE_SET_BOSS)) {
		return;
	}

    QPoint pnt = event->pos();
    editor_selectedTileX = pnt.x()/(16*dataExchanger->zoom);
    editor_selectedTileY = pnt.y()/(16*dataExchanger->zoom);

	if (dataExchanger->editMode == EDITMODE_NORMAL) {
		if (dataExchanger->editTool == EDITMODE_NORMAL) {
			if (dataExchanger->layerLevel == 1) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.x = dataExchanger->getPalleteX();
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.y = dataExchanger->getPalleteY();
			} else if (dataExchanger->layerLevel == 2) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.x = dataExchanger->getPalleteX();
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.y = dataExchanger->getPalleteY();
			} else if (dataExchanger->layerLevel == 3) {
				printf(">> EditorArea::mousePressEvent - placing tile 3\n");
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.x = dataExchanger->getPalleteX();
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.y = dataExchanger->getPalleteY();
			}
		} else if (dataExchanger->editTool == EDITMODE_ERASER) {
			if (dataExchanger->layerLevel == 1) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.x = -1;
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.y = -1;
			} else if (dataExchanger->layerLevel == 2) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.x = -1;
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.y = -1;
			} else if (dataExchanger->layerLevel == 3) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.x = -1;
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.y = -1;
			}
		} else if (dataExchanger->editTool == EDITMODE_LOCK) {
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].locked == 0) {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].locked = dataExchanger->terrainType;
			} else {
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].locked = 0;
			}

		} else if (dataExchanger->editTool == EDITMODE_FILL) {
			fill_area();








		// first click on origin link
		} else if (dataExchanger->editTool == EDITMODE_LINK && tempX == -1) {
			// checks if a link in this position already exits to remove it
			bool removed_link = false;
			for (int i=0; i<STAGE_MAX_LINKS; i++) {
				if (game_data.stages[dataExchanger->currentStage].links[i].id_map_origin == dataExchanger->currentMap) {
					if (game_data.stages[dataExchanger->currentStage].links[i].pos_origin.x == editor_selectedTileX && game_data.stages[dataExchanger->currentStage].links[i].pos_origin.y == editor_selectedTileY) {
						game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny = -1;
						game_data.stages[dataExchanger->currentStage].links[i].id_map_origin = -1;
						std::cout << "######## -> editorArea::mousePress - removed link 1" << std::endl;
						removed_link = true;
						break;
					}
				} else if (game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny == dataExchanger->currentMap) {
					if (game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.x == editor_selectedTileX && game_data.stages[dataExchanger->currentStage].links[i].pos_destiny.y == editor_selectedTileY) {
						game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny = -1;
						game_data.stages[dataExchanger->currentStage].links[i].id_map_origin = -1;
						std::cout << "######## -> editorArea::mousePress - removed link 2" << std::endl;
						removed_link = true;
						break;
					}
				}
			}
			if (removed_link == false) {
				// check if there is a slot free
				int link_n = -1;
				for (int i=0; i<STAGE_MAX_LINKS; i++) {
					std::cout << ">> DEBUG - links[" << i << "].id_map_destiny: " << game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny << ", id_map_origin: " << game_data.stages[dataExchanger->currentStage].links[i].id_map_origin << std::endl;
					if (game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny == -1 && game_data.stages[dataExchanger->currentStage].links[i].id_map_origin == -1) {
						link_n = i;
						break;
					}
				}
				if (link_n == -1) {
					QMessageBox msgBox;
					msgBox.setText("########### -> Error: there is no slot free to add a new link.");
					msgBox.exec();
					return;
				}
				std::cout << "########### -> editorArea::mousePress - adding link ORIGIN at slot[" << link_n << "] - PART 1" << std::endl;
				tempX = editor_selectedTileX;
				tempY = editor_selectedTileY;
			}
		} else if (dataExchanger->editTool == EDITMODE_LINK_DEST) {
			std::cout << "########### -> editorArea::mousePress - adding link - PART 3" << std::endl;
			int link_n = -1;
			for (int i=0; i<STAGE_MAX_LINKS; i++) {
				if (game_data.stages[dataExchanger->currentStage].links[i].id_map_destiny == -1 && game_data.stages[dataExchanger->currentStage].links[i].id_map_origin == -1) {
					link_n = i;
					break;
				}
			}
			if (link_n == -1) {
				QMessageBox msgBox;
				msgBox.setText("Error: there is no slot free to add a new link.");
				msgBox.exec();
				return;
			}
			std::cout << "########### -> editorArea::mousePress - adding link DESTINY at slot[" << link_n << "] - PART 3" << std::endl;
			game_data.stages[dataExchanger->currentStage].links[link_n].id_map_origin = link_map_origin;
			game_data.stages[dataExchanger->currentStage].links[link_n].pos_origin.x = link_pos_x;
			game_data.stages[dataExchanger->currentStage].links[link_n].pos_origin.y = link_pos_y;
			game_data.stages[dataExchanger->currentStage].links[link_n].size = link_size;
			game_data.stages[dataExchanger->currentStage].links[link_n].id_map_destiny = dataExchanger->currentMap;
			game_data.stages[dataExchanger->currentStage].links[link_n].pos_destiny.x = editor_selectedTileX;
			game_data.stages[dataExchanger->currentStage].links[link_n].pos_destiny.y = editor_selectedTileY;
			/// @TODO: this must come from editor properties
			game_data.stages[dataExchanger->currentStage].links[link_n].bidirecional = dataExchanger->link_bidi;
			game_data.stages[dataExchanger->currentStage].links[link_n].is_door = dataExchanger->link_is_door;
			game_data.stages[dataExchanger->currentStage].links[link_n].type = dataExchanger->link_type;

			dataExchanger->editTool = EDITMODE_LINK;
			repaint();
			return;
		}



    } else if (dataExchanger->editMode == EDITMODE_SET_BOSS) {
        std::cout << ">> EditorArea::mousePressEvent - EDITMODE_NPC" << std::endl;
        int found_npc = -1;
        // search if there is an existing NPC in ths position, and if yes, remove it
        for (int i=0; i<MAX_MAP_NPC_N; i++) {
            if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x == editor_selectedTileX && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y == editor_selectedTileY) {
                game_data.stages[dataExchanger->currentStage].boss.id_npc = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc;
                sprintf(game_data.stages[dataExchanger->currentStage].boss.name, "%s", game_data.game_npcs[game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc].name);
                found_npc = i;
                break;
            }
        }




/// @TODO: convert to new file-system ///

	} else if (dataExchanger->editMode == EDITMODE_NPC) {
		printf(">> EditorArea::mousePressEvent - EDITMODE_NPC\n");

		int found_npc = -1;
		// search if there is an existing NPC in ths position, and if yes, remove it
		for (int i=0; i<MAX_MAP_NPC_N; i++) {
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x == editor_selectedTileX && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y == editor_selectedTileY) {
				found_npc = i;
				break;
			}
		}
		// clean old-format trash
		for (int i=0; i<MAX_MAP_NPC_N; i++) {
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x == -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y == -1) {
				std::cout << "reseting free-slot - i: " << i << ", id: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc << ", x: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x << ", y: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y << std::endl;
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc = -1;
			}
		}





		if (dataExchanger->editTool == EDITMODE_ERASER && found_npc != -1) {
			std::cout << "remove npc - slot: " << found_npc << std::endl;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[found_npc].id_npc = -1;
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && found_npc == -1 && dataExchanger->selectedNPC != -1) {
			// search for free slot
			for (int i=0; i<MAX_MAP_NPC_N; i++) {
				std::cout << "searching for free-slot - i: " << i << ", id: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc << ", x: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x << ", y: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y << std::endl;
				if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc == -1) {
					std::cout << "add npc - slot: " << i << ", id: " << dataExchanger->selectedNPC << std::endl;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].id_npc = dataExchanger->selectedNPC;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.x = editor_selectedTileX;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].start_point.y = editor_selectedTileY;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_npcs[i].direction = dataExchanger->npc_direction;
					repaint();
					return;
				}
			}
			QMessageBox msgBox;
			msgBox.setText("No free slot to place NPC in map.");
			msgBox.exec();
			return;
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && found_npc != -1 && dataExchanger->selectedNPC != -1) {
			printf(">> EditorArea::mousePressEvent - Adding NPC - place already taken\n");
		}



	} else if (dataExchanger->editMode == EDITMODE_OBJECT) {


		printf(">> EditorArea::mousePressEvent - EDITMODE_OBJECT");

		int found_object = -1;
		// search if there is an existing object in ths position, and if yes, remove it
		for (int i=0; i<MAX_MAP_NPC_N; i++) {
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object != -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x == editor_selectedTileX && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y == editor_selectedTileY) {
				found_object = i;
				break;
			}
		}
		// clean old-format trash
		for (int i=0; i<MAX_MAP_NPC_N; i++) {
			if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x == -1 && game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y == -1) {
				std::cout << "reseting free-slot - i: " << i << ", id: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object << ", x: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x << ", y: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y << std::endl;
				game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object = -1;
			}
		}





		if (dataExchanger->editTool == EDITMODE_ERASER && found_object != -1) {
			std::cout << "remove object - slot: " << found_object << std::endl;
			game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[found_object].id_object = -1;
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && found_object == -1 && dataExchanger->selectedNPC != -1) {
			// search for free slot
			for (int i=0; i<MAX_MAP_NPC_N; i++) {
				std::cout << "searching for free-slot - i: " << i << ", id: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object << ", x: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x << ", y: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y << std::endl;
				if (game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object == -1) {
					std::cout << "add object - slot: " << i << ", id: " << dataExchanger->selectedNPC << std::endl;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].id_object = dataExchanger->selectedNPC;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.x = editor_selectedTileX;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].start_point.y = editor_selectedTileY;
					game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].map_objects[i].direction = dataExchanger->npc_direction;
					repaint();
					return;
				}
			}
			QMessageBox msgBox;
			msgBox.setText("No free slot to place object in map.");
			msgBox.exec();
			return;
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && found_object != -1 && dataExchanger->selectedNPC != -1) {
			printf(">> EditorArea::mousePressEvent - Adding object - place already taken\n");
		}





		printf(">> EditorArea::mousePressEvent - EDITMODE_OBJECT\n");
		struct format_v1_0::st_object *oldObj=NULL;
		/// @TODO: convert
		/*
		temp_obj_list = obj_list;
		bool foundObj = false;

		while (temp_obj_list) {
			if (temp_obj_list->type != -1 && temp_obj_list->start_point.x == editor_selectedTileX &&  temp_obj_list->start_point.y == editor_selectedTileY) {


				printf(">> EditorArea::mousePressEvent - Removing Object\n");
				if (temp_obj_list == obj_list) { // first item in the list
					printf(">> EditorArea::mousePressEvent - Removing Object From List START\n");
					obj_list = obj_list->next;
					free (temp_obj_list);
				} else {
					printf(">> EditorArea::mousePressEvent - Removing Object From List MIDDLE\n");
					oldObj->next = temp_obj_list->next;
					free(temp_obj_list);
				}
				foundObj = true;
				break;


			}
			oldObj = temp_obj_list;
			temp_obj_list = temp_obj_list->next;
		}

		//printf(">> EDITMODE_OBJECT, foundObject: %p, dataExchanger->selectedNPC: %d\n", foundObj, dataExchanger->selectedNPC);
		if (foundObj == false) {
			printf("foundObj is NULL\n");
			if (dataExchanger->selectedNPC != -1) {
				printf(">> EditorArea::mousePressEvent - Adding Object\n");
				addObjectToMap(editor_selectedTileX, editor_selectedTileY);
			} else {
				printf(">> EditorArea::mousePressEvent - No object selected to be placed\n");
			}
		}
  */
	}
    temp = 1;
    repaint();
}


void EditorArea::mouseReleaseEvent(QMouseEvent *event) {
	QPoint pnt = event->pos();
	editor_selectedTileX = pnt.x()/(16*dataExchanger->zoom);
	editor_selectedTileY = pnt.y()/(16*dataExchanger->zoom);

	if (dataExchanger->editTool == EDITMODE_LINK && tempX != -1) {
		std::cout << "########### -> editorArea::mousePress - adding link - PART 2" << std::endl;
		// TODO: add link
		dataExchanger->editTool = EDITMODE_LINK_DEST;

		link_map_origin = dataExchanger->currentMap;
		link_pos_x = tempX;
		link_pos_y = tempY;
		int tempSize = abs(editor_selectedTileX - tempX);
		if (tempSize < 1 || tempSize > 20) {
			link_size = 1;
		} else {
			link_size = abs(editor_selectedTileX - tempX) + 1;
		}
		printf(">>>>>>>> added link in map: %d, pos: (%d, %d) with size: %d <<<<<<<<<<\n", link_map_origin, link_pos_x, link_pos_y, link_size);

		//printf("editorArea::mouseReleaseEvent - tempX: %d, tempY: %d, p.X: %d, p.y: %d, LINK-SIZE: %d\n", tempX, tempY, editor_selectedTileX, editor_selectedTileY, game.map_links[i].link_size);
		tempX = -1;
		tempY = -1;
		repaint();
	}
	mouse_released = true;
}

void EditorArea::fill_area() {
	int i, j;
	int filled1=0;
	int filled2=0;
	int originalX, originalY;
	short int *searchX, *searchY;

	if (dataExchanger->layerLevel == 1) {
		originalX = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.x;
		originalY = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile1.y;
	} else if (dataExchanger->layerLevel == 2) {
		originalX = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.x;
		originalY = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile2.y;
	} else if (dataExchanger->layerLevel == 3) {
		originalX = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.x;
		originalY = game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[editor_selectedTileX][editor_selectedTileY].tile3.y;
	}
	// área a esquerda
	for (i=editor_selectedTileX; i>=0; i--) {
		// área acima
		filled1 = 0;
		filled2 = 0;
		for (j=editor_selectedTileY; j>=0; j--) {
			if (dataExchanger->layerLevel == 1) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y;
			}
			//printf("DEBUG.EditorArea::i: %d, j: %d, fill_area - tile.x: %d, tile;y: %d, originalX: %d, originalY: %d\n", i, j, map.tiles[i][j].tile1.x, map.tiles[i][j].tile1.y, originalX, originalY);
			if (*searchX == originalX && *searchY == originalY) {
				*searchX = dataExchanger->getPalleteX();
				*searchY = dataExchanger->getPalleteY();
				filled1 = 1;
			} else {
				printf("1.not filling\n");
				break;
			}
		}
		// área abaixo
		for (j=editor_selectedTileY+1; j<MAP_H; j++) {
			if (dataExchanger->layerLevel == 1) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y;
			}
			if (*searchX == originalX && *searchY == originalY) {
				*searchX = dataExchanger->getPalleteX();
				*searchY = dataExchanger->getPalleteY();
				filled2 = 1;
			} else {
				printf("2.not filling\n");
				break;
			}
		}
		if (filled1 == 0 && filled2 == 0) {
			break;
		}

	}
	// area a direita
	for (i=editor_selectedTileX+1; i<MAP_W; i++) {
		filled1 = 0;
		filled2 = 0;
		for (j=editor_selectedTileY; j>=0; j--) {
			//printf("DEBUG.EditorArea::i: %d, j: %d, fill_area - tile.x: %d, tile;y: %d, originalX: %d, originalY: %d\n", i, j, map.tiles[i][j].tile1.x, map.tiles[i][j].tile1.y, originalX, originalY);
			if (dataExchanger->layerLevel == 1) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y;
			}
			if (*searchX == originalX && *searchY == originalY) {
				*searchX = dataExchanger->getPalleteX();
				*searchY = dataExchanger->getPalleteY();
				filled1 = 1;
			} else {
				printf("3.not filling\n");
				break;
			}
		}
		// área abaixo
		for (j=editor_selectedTileY+1; j<MAP_H; j++) {
			if (dataExchanger->layerLevel == 1) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.x;
				searchY = &game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].tiles[i][j].tile3.y;
			}
			if (*searchX == originalX && *searchY == originalY) {
				*searchX = dataExchanger->getPalleteX();
				*searchY = dataExchanger->getPalleteY();
				filled2 = 1;
			} else {
				printf("4.not filling\n");
				break;
			}
		}
		if (filled1 == 0 || filled2 == 0) {
			break;
		}
	}
}



void EditorArea::saveGame(int game_n) {
	format_v_2_0_1::file_io fio;
	//std::cout << "EditorArea::saveGame ******** backgrounds[0].filename: " << game_data.stages[dataExchanger->currentStage].maps[dataExchanger->currentMap].backgrounds[0].filename << std::endl;
	fio.write_game(game_data);
}


void EditorArea::removeNpcFromMap(int posx, int posy) {
	struct format_v1_0::list_map_npc *temp_list, *item_ant=NULL, *item_aux=NULL;

	// empty npc list
	temp_list = dataExchanger->npc_map_list;
	while (temp_list) {
		if (temp_list->start_point.x == posx && temp_list->start_point.y == posy) {
			item_aux = temp_list->next;
			free(temp_list);
			temp_list = NULL;
			temp_list = item_aux;
			if (item_ant != NULL) {
				item_ant->next = temp_list;
			} else {
				dataExchanger->npc_map_list = temp_list;
			}
		} else {
			item_ant = temp_list;
			temp_list = temp_list->next;
		}
	}
}

void EditorArea::addObjectToMap(int posx, int posy) {
	// checks if there is a npc in this position
	/// @TODO: rework
	/*
	temp_obj_list = obj_list;
	while (temp_obj_list) {
		if (temp_obj_list->type != -1 && temp_obj_list->start_point.x == posx && temp_obj_list->start_point.y == posy) {
			QMessageBox msgBox;
			msgBox.setText("addObjectToMap: There is a Object in this point already");
			msgBox.exec();
			return;
		}
		temp_obj_list = temp_obj_list->next;
	}
	struct format_v1_0::st_object *new_obj = (struct format_v1_0::st_object *)malloc(sizeof(struct format_v1_0::st_object));
	new_obj->direction = 0;
	new_obj->distance = 0;
	sprintf(new_obj->filename, "%s", "");
	new_obj->frame = 0;
	new_obj->framesize_h = 0;
	new_obj->framesize_w = 0;
	new_obj->limit = 0;
	sprintf(new_obj->name, "%s", "");
	new_obj->next = NULL;
	new_obj->position.x = 0;
	new_obj->position.y = 0;
	new_obj->speed = 0;
	new_obj->start_point.x = posx;
	new_obj->start_point.y = posy;
	//printf("EditorArea::addObjectToMap - x: %d, y: %d\n", new_obj->start_point.x, new_obj->start_point.y);
	new_obj->timer = 0;
	new_obj->type = 0;
	if (obj_list == NULL) {
		//printf (">>addObjectToMap - adding to the BEGIN of obj_list\n");
		obj_list = new_obj;
		if (obj_list == NULL) {
			printf (">>addObjectToMap - adding to the BEGIN of obj_list - FAIL!\n");
		} else {
			//printf (">>addObjectToMap - adding to the BEGIN of obj_list - OK!\n");
		}
	} else {
		temp_obj_list = obj_list;
		while (temp_obj_list->next != NULL) {
			temp_obj_list = temp_obj_list->next;
		}
		//printf (">>addObjectToMap - adding to the END of obj_list\n");
		temp_obj_list->next = new_obj;
	}
	dataExchanger->placeObject(posx, posy, new_obj);
	// go to the end of the list to add
	*/

}


