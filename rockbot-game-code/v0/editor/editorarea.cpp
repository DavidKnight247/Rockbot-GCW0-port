#include "editorarea.h"
#include <stdio.h>
#include <QBrush>
#include <QDir>
#include "defines.h"
#include <QResource>



extern file_game game;
extern file_map_v1 map;
extern char FILEPATH[512];

extern struct struct_object *obj_list;
struct struct_object *temp_obj_list=NULL;
extern struct struct_object *main_obj_list;



// construtor
EditorArea::EditorArea(QWidget *parent) : QWidget(parent) {
    myParent = parent;
    int i, j;
	strcpy(map.filename, "data/images/tilesets/default.png");
    for (i=0; i<MAP_W; i++) {
        for (j=0; j<MAP_H; j++) {
            map.tiles[i][j].tile1.x = -1;
            map.tiles[i][j].tile1.y = -1;
        }
    }
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
    //printf("DEBUG - EditorArea.paintEvent\n");
    // draw the picked tile in the tileset
    //QLine *line;
    //QLineF line(10.0, 80.0, 90.0, 20.0);
    QRectF target(QPoint(10, 20), QSize(30, 30));
    QRectF source(QPoint(100, 100), QSize(30, 30));
    char buffer[512];


    filename = new QString("");
    filename->append(my_pallete->getPallete());
    QPixmap image(*filename);
    if (image.isNull()) {
        printf("ERROR: EditorArea::paintEvent - Could not load image file '%s'\n", qPrintable(*filename));
    }
    // draw tiles
    for (i=0; i<MAP_W; i++) {
        for (j=0; j<MAP_H; j++) {
            // level one
            if (map.tiles[i][j].tile1.x != -1 && map.tiles[i][j].tile1.y != -1) {
                QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
                QRectF source(QPoint((map.tiles[i][j].tile1.x*16), (map.tiles[i][j].tile1.y*16)), QSize(16, 16));
                painter.drawPixmap(target, image, source);
            }
            // level two
            if (dataExchanger->layerLevel == 2 || dataExchanger->layerLevel == 3) {
                    //printf(">> EditorArea::paintEvent - SHOW TILES LEVEL 2\n");
                    if (map.tiles[i][j].tile2.x != -1 && map.tiles[i][j].tile2.y != -1) {
                        QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
                        QRectF source(QPoint((map.tiles[i][j].tile2.x*16), (map.tiles[i][j].tile2.y*16)), QSize(16, 16));
                        painter.drawPixmap(target, image, source);
                    }
            }
            if (dataExchanger->layerLevel == 3) {
                    if (map.tiles[i][j].tile3.x != -1 && map.tiles[i][j].tile3.y != -1) {
                        QRectF target(QPoint(i*16*dataExchanger->zoom, j*16*dataExchanger->zoom), QSize(16*dataExchanger->zoom, 16*dataExchanger->zoom));
                        QRectF source(QPoint((map.tiles[i][j].tile3.x*16), (map.tiles[i][j].tile3.y*16)), QSize(16, 16));
                        painter.drawPixmap(target, image, source);
                    }
            }
			// locked areas, stairs, doors, etc
			if (map.tiles[i][j].locked != 0 && dataExchanger->editTool == EDITMODE_LOCK) {
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
					if (map.tiles[i][j].locked == 1) {
						terrainIcon = QString::fromUtf8(":/toolbar_icons/Lock"); // solid
					} else if (map.tiles[i][j].locked == 2) {
						terrainIcon = QString(":/toolbar_icons/stairs.png"); // stairs
					} else if (map.tiles[i][j].locked == 3) {
						terrainIcon = QString(":/toolbar_icons/object-order-lower.png"); // door
					} else if (map.tiles[i][j].locked == 4) {
						terrainIcon = QString(":/toolbar_icons/edit-delete.png"); // spikes
					} else if (map.tiles[i][j].locked == 5) {
						terrainIcon = QString(":/toolbar_icons/flag-blue.png"); // water
					} else if (map.tiles[i][j].locked == 6) {
						terrainIcon = QString(":/toolbar_icons/flag-green.png"); // ice
					} else if (map.tiles[i][j].locked == 7) {
						terrainIcon = QString(":/toolbar_icons/arrow-left.png"); // move left
					} else if (map.tiles[i][j].locked == 8) {
						terrainIcon = QString(":/toolbar_icons/arrow-right.png"); // move right
					} else if (map.tiles[i][j].locked == 9) {
						terrainIcon = QString(":/toolbar_icons/arrow-down.png"); // move right
					} else if (map.tiles[i][j].locked == 10) {
						terrainIcon = QString(":/toolbar_icons/system-switch-user.png"); // move right
					} else if (map.tiles[i][j].locked == 11) {
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
		struct links_list *temp_links = NULL;
		temp_links = dataExchanger->map_links;
		i = 1;
		while (temp_links) {
			if (dataExchanger->currentStage == temp_links->link.stage) {
				//printf(">> searching map_links, currentMap: %d map_origin: %d, map_dest: %d <<\n", dataExchanger->currentMap, temp_links->link.map_origin, temp_links->link.map_dest);
				if (dataExchanger->currentMap ==  temp_links->link.map_origin && temp_links->link.link_pos.x != -1 && temp_links->link.link_pos.y != -1) {
						// transparent blue rectangle
						painter.setBrush(QColor(0, 0, 255, 180));
						painter.drawRect(temp_links->link.link_pos.x*16*dataExchanger->zoom, temp_links->link.link_pos.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*temp_links->link.link_size, 16*dataExchanger->zoom);
						// blue border
						painter.setBrush(Qt::NoBrush);
						painter.setPen(QColor(0, 0, 255, 255));
						painter.drawRect(temp_links->link.link_pos.x*16*dataExchanger->zoom, temp_links->link.link_pos.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*temp_links->link.link_size, 16*dataExchanger->zoom);
						painter.setPen(QColor(255, 255, 255, 255));
						painter.drawText(temp_links->link.link_pos.x*16*dataExchanger->zoom, (temp_links->link.link_pos.y+1)*16*dataExchanger->zoom, QString::number(i));
				} else if (dataExchanger->currentMap == temp_links->link.map_dest && temp_links->link.link_dest.x != -1 && temp_links->link.link_dest.y != -1) {
						// transparent cyan rectangle
						painter.setBrush(QColor(0, 255, 255, 180));
						painter.drawRect(temp_links->link.link_dest.x*16*dataExchanger->zoom, temp_links->link.link_dest.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*temp_links->link.link_size, 16*dataExchanger->zoom);
						// cyan border
						painter.setBrush(Qt::NoBrush);
						painter.setPen(QColor(0, 255, 255, 255));
						painter.drawRect(temp_links->link.link_dest.x*16*dataExchanger->zoom, temp_links->link.link_dest.y*16*dataExchanger->zoom, 16*dataExchanger->zoom*temp_links->link.link_size, 16*dataExchanger->zoom);
						painter.setPen(QColor(0, 0, 0, 255));
						painter.drawText(temp_links->link.link_dest.x*16*dataExchanger->zoom, (temp_links->link.link_dest.y+1)*16*dataExchanger->zoom, QString::number(i));
				}
			}
			i++;
			temp_links = temp_links->next;
		}

    }
    // draw NPCs
	struct list_map_npc *temp_list;
	temp_list = dataExchanger->npc_map_list;


	// TODO - this must be stored into a struct that holds all necessary information in order to not reload each paint-event
	while (temp_list) {
		struct npc_static_data_list *temp_main_npc = NULL;
		//printf("--- searching NPC id %d in main list ---\n", temp_list->id);
		temp_main_npc = dataExchanger->get_main_npc (temp_list->id);
		//printf("++++++++++++++ filename: %s <<\n", temp_main_npc->npc_data.filename);
		sprintf(buffer, "%s/data/images/sprites/enemies/%s", FILEPATH, temp_main_npc->npc_data.filename);
		QPixmap temp_image(buffer);
		if (temp_image.isNull()) {
			printf("EditorArea::paintEvent - Could not load NPC image file '%s', npc.hp: %d\n", buffer, temp_main_npc->npc_data.hp.total);
		} else {
				QRectF target(QPoint(temp_list->start_point.x*16*dataExchanger->zoom, temp_list->start_point.y*16*dataExchanger->zoom), QSize(temp_main_npc->npc_data.framesize_w*dataExchanger->zoom, temp_main_npc->npc_data.framesize_h*dataExchanger->zoom));
				//printf(">> map.npc_list[j].direction: %d <<\n", temp_list->direction);
				QRectF source;
				//source = QRectF(QPoint(0, 0), QSize(map.npc_list[j].framesize_w, map.npc_list[j].framesize_h));
				if (temp_list->direction != 1) {
					source = QRectF(QPoint(0, 0), QSize(temp_main_npc->npc_data.framesize_w, temp_main_npc->npc_data.framesize_h));
				} else {
					source = QRectF(QPoint(0, temp_main_npc->npc_data.framesize_h), QSize(temp_main_npc->npc_data.framesize_w, temp_main_npc->npc_data.framesize_h));
				}
				painter.drawPixmap(target, temp_image, source);
		}
		temp_list = temp_list->next;
	}


	/*
    //if (dataExchanger->editMode == EDITMODE_ADDNPC) {
    for (j=0; j<STAGE_NPC_LIMIT; j++) {
            //printf("DEBUG - looking for npcs - hp.totoal: %d\n", map.npc_list[j].hp.total);
            if (map.npc_list[j].hp.total >= 0) {
					//printf("DEBUG.EditorArea::paintEvent - must draw NPC %d\n", j);
					sprintf(buffer, "%s/data/images/sprites/enemies/%s", FILEPATH, map.npc_list[j].filename);
                    QPixmap temp_image(buffer);
                    if (temp_image.isNull()) {
						printf("EditorArea::paintEvent - Could not load NPC image file '%s', npc.hp: %d\n", buffer, map.npc_list[j].hp.total);
					} else {
                            QRectF target(QPoint(map.npc_list[j].start_point.x*16*dataExchanger->zoom, map.npc_list[j].start_point.y*16*dataExchanger->zoom), QSize(map.npc_list[j].framesize_w*dataExchanger->zoom, map.npc_list[j].framesize_h*dataExchanger->zoom));
							printf(">> map.npc_list[j].direction: %d <<\n", map.npc_list[j].direction);
							QRectF source;
							//source = QRectF(QPoint(0, 0), QSize(map.npc_list[j].framesize_w, map.npc_list[j].framesize_h));
							if (map.npc_list[j].direction != 1) {
								source = QRectF(QPoint(0, 0), QSize(map.npc_list[j].framesize_w, map.npc_list[j].framesize_h));
								printf(">> direction LEFT <<\n");
							} else {
								source = QRectF(QPoint(0, map.npc_list[j].framesize_h), QSize(map.npc_list[j].framesize_w, map.npc_list[j].framesize_h));
								printf(">> direction RIGHT <<\n");
							}
                            painter.drawPixmap(target, temp_image, source);
                    }
            }
    }
	*/
    //}

	temp_obj_list = obj_list;
	while (temp_obj_list) {
		if (temp_obj_list->type >= 0) {
			sprintf(buffer, "%s/data/images/sprites/objects/%s", FILEPATH, temp_obj_list->filename);
			QPixmap temp_image(buffer);
			if (temp_image.isNull()) {
					//printf("EditorArea::paintEvent DEBUG.Editor - Could not load OBJECT image file '%s'\n", buffer);
					painter.setBrush(QColor(255, 255, 255, 180));
                                        painter.drawRect(temp_obj_list->start_point.x*16*dataExchanger->zoom, temp_obj_list->start_point.y*16*dataExchanger->zoom, 16*dataExchanger->zoom, 16*dataExchanger->zoom);
			} else {
                                        QRectF target(QPoint(temp_obj_list->start_point.x*16*dataExchanger->zoom, temp_obj_list->start_point.y*16*dataExchanger->zoom), QSize(temp_obj_list->framesize_w, temp_obj_list->framesize_h));
					QRectF source(QPoint(0, 0), QSize(temp_obj_list->framesize_w, temp_obj_list->framesize_h));
					painter.drawPixmap(target, temp_image, source);
			}
		}
		temp_obj_list = temp_obj_list->next;
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
    int i;
    int foundNPC = -1;

    QPoint pnt = event->pos();
    editor_selectedTileX = pnt.x()/(16*dataExchanger->zoom);
    editor_selectedTileY = pnt.y()/(16*dataExchanger->zoom);

	if (dataExchanger->editMode == EDITMODE_NORMAL) {
		if (dataExchanger->editTool == EDITMODE_NORMAL) {
			if (dataExchanger->layerLevel == 1) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.x = dataExchanger->getPalleteX();
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.y = dataExchanger->getPalleteY();
			} else if (dataExchanger->layerLevel == 2) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.x = dataExchanger->getPalleteX();
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.y = dataExchanger->getPalleteY();
			} else if (dataExchanger->layerLevel == 3) {
				printf(">> EditorArea::mousePressEvent - placing tile 3\n");
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.x = dataExchanger->getPalleteX();
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.y = dataExchanger->getPalleteY();
			}
		} else if (dataExchanger->editTool == EDITMODE_ERASER) {
			if (dataExchanger->layerLevel == 1) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.x = -1;
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.y = -1;
			} else if (dataExchanger->layerLevel == 2) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.x = -1;
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.y = -1;
			} else if (dataExchanger->layerLevel == 3) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.x = -1;
				map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.y = -1;
			}
		} else if (dataExchanger->editTool == EDITMODE_LOCK) {
			if (map.tiles[editor_selectedTileX][editor_selectedTileY].locked == 0) {
				map.tiles[editor_selectedTileX][editor_selectedTileY].locked = dataExchanger->terrainType;
			} else {
				map.tiles[editor_selectedTileX][editor_selectedTileY].locked = 0;
			}

		} else if (dataExchanger->editTool == EDITMODE_FILL) {
			fill_area();
		} else if (dataExchanger->editTool == EDITMODE_LINK && tempX == -1) {
			// checks if a link in this position already exits to remove it
			struct links_list *temp_links = NULL;
			temp_links = dataExchanger->map_links;
			i = 1;
			while (temp_links) {
				if (dataExchanger->currentStage == temp_links->link.stage) {
					if ((temp_links->link.map_origin == dataExchanger->currentMap && temp_links->link.link_pos.x == editor_selectedTileX && temp_links->link.link_pos.y == editor_selectedTileY) || (temp_links->link.map_dest == dataExchanger->currentMap && temp_links->link.link_dest.x == editor_selectedTileX && temp_links->link.link_dest.y == editor_selectedTileY)) {
						dataExchanger->remove_map_link(temp_links);
						repaint();
						return;
					}
				}
				temp_links = temp_links->next;
			}
			tempX = editor_selectedTileX;
			tempY = editor_selectedTileY;
		} else if (dataExchanger->editTool == EDITMODE_LINK_DEST) {
			struct links_list *new_link = dataExchanger->add_map_link();
			// set origin point
			new_link->link.map_origin = link_map_origin;
			new_link->link.link_pos.x = link_pos_x;
			new_link->link.link_pos.y = link_pos_y;
			new_link->link.link_size = link_size;
			// set destiny point
			new_link->link.link_dest.x = editor_selectedTileX;
			new_link->link.link_dest.y = editor_selectedTileY;
			new_link->link.map_dest = dataExchanger->currentMap;
			new_link->link.link_type = dataExchanger->link_type;			// can be TELEPORTER, LINK_VERTICAL OR LINK_HORIZONTAL

			printf(">>> WARNING::add_map_link.type: %d <<<\n", dataExchanger->link_type);
			new_link->link.link_bidi = 1;							// indicates if it is bidirecional
			new_link->link.is_door = 0;							// if it is a door, it is not bidiand willmake door sound and show animation of door opening/closing
			new_link->link.stage = dataExchanger->currentStage;

			printf(">>>>>>>> created link in map: %d, pos: (%d, %d) with size: %d <<<<<<<<<<\n", link_map_origin, link_pos_x, link_pos_y, link_size);
			dataExchanger->editTool = EDITMODE_LINK;
			repaint();
			return;
		}
	} else if (dataExchanger->editMode == EDITMODE_NPC) {
		printf(">> EditorArea::mousePressEvent - EDITMODE_NPC\n");
		struct list_map_npc *temp_list;
		temp_list = dataExchanger->npc_map_list;
		while (temp_list) {
			if (temp_list->start_point.x == editor_selectedTileX && temp_list->start_point.y == editor_selectedTileY) {
				foundNPC = 1;
			}
			temp_list = temp_list->next;
		}
		/*
		for (i=0; i<STAGE_NPC_LIMIT; i++) {
			// removing npc from the map
			if (map.npc_list[i].hp.total != -1 && map.npc_list[i].start_point.x == editor_selectedTileX &&  map.npc_list[i].start_point.y == editor_selectedTileY) {
				foundNPC=i;
				break;
			}
			// must check is one npc is selected in the list to add it
		}
		*/
		if (dataExchanger->editTool == EDITMODE_ERASER && foundNPC != -1) {
			removeNpcFromMap(editor_selectedTileX, editor_selectedTileY);
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && foundNPC == -1 && dataExchanger->selectedNPC != -1) {
			addNpcToMap(editor_selectedTileX, editor_selectedTileY);
		} else if (dataExchanger->editTool == EDITMODE_NORMAL && foundNPC != -1 && dataExchanger->selectedNPC != -1) {
			printf(">> EditorArea::mousePressEvent - Adding NPC - place already taken\n");
		}
	} else if (dataExchanger->editMode == EDITMODE_OBJECT) {
		printf(">> EditorArea::mousePressEvent - EDITMODE_OBJECT\n");
		struct struct_object *oldObj=NULL;
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
	}
    temp = 1;
    repaint();
}


void EditorArea::mouseReleaseEvent(QMouseEvent *event) {
	QPoint pnt = event->pos();
	editor_selectedTileX = pnt.x()/(16*dataExchanger->zoom);
	editor_selectedTileY = pnt.y()/(16*dataExchanger->zoom);

	if (dataExchanger->editTool == EDITMODE_LINK && tempX != -1) {
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
}

void EditorArea::fill_area() {
	int i, j;
	int filled1=0;
	int filled2=0;
	int originalX, originalY;
	short int *searchX, *searchY;

	if (dataExchanger->layerLevel == 1) {
		originalX = map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.x;
		originalY = map.tiles[editor_selectedTileX][editor_selectedTileY].tile1.y;
	} else if (dataExchanger->layerLevel == 2) {
		originalX = map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.x;
		originalY = map.tiles[editor_selectedTileX][editor_selectedTileY].tile2.y;
	} else if (dataExchanger->layerLevel == 3) {
		originalX = map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.x;
		originalY = map.tiles[editor_selectedTileX][editor_selectedTileY].tile3.y;
	}
	// área a esquerda
	for (i=editor_selectedTileX; i>=0; i--) {
		// área acima
		filled1 = 0;
		filled2 = 0;
		for (j=editor_selectedTileY; j>=0; j--) {
			if (dataExchanger->layerLevel == 1) {
				searchX = &map.tiles[i][j].tile1.x;
				searchY = &map.tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &map.tiles[i][j].tile2.x;
				searchY = &map.tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &map.tiles[i][j].tile3.x;
				searchY = &map.tiles[i][j].tile3.y;
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
				searchX = &map.tiles[i][j].tile1.x;
				searchY = &map.tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &map.tiles[i][j].tile2.x;
				searchY = &map.tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &map.tiles[i][j].tile3.x;
				searchY = &map.tiles[i][j].tile3.y;
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
				searchX = &map.tiles[i][j].tile1.x;
				searchY = &map.tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &map.tiles[i][j].tile2.x;
				searchY = &map.tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &map.tiles[i][j].tile3.x;
				searchY = &map.tiles[i][j].tile3.y;
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
				searchX = &map.tiles[i][j].tile1.x;
				searchY = &map.tiles[i][j].tile1.y;
			} else if (dataExchanger->layerLevel == 2) {
				searchX = &map.tiles[i][j].tile2.x;
				searchY = &map.tiles[i][j].tile2.y;
			} else if (dataExchanger->layerLevel == 3) {
				searchX = &map.tiles[i][j].tile3.x;
				searchY = &map.tiles[i][j].tile3.y;
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




void EditorArea::saveMap(int game_n, int map_n) {
	char filename[512], buffer[512];
	char temp_cent_number[4];
	char map_cent_number[4];

	printf("****************** saveMap game_n: %d, map_n: %d ****************** \n", game_n, map_n);

	dataExchanger->centNumberFormat(game_n);
	sprintf(temp_cent_number, "%s", dataExchanger->centNumber);
	dataExchanger->centNumberFormat(map_n);
	sprintf(map_cent_number, "%s", dataExchanger->centNumber);

	sprintf(buffer, "%s/data/game/game_%s/", FILEPATH, temp_cent_number);
	if (!QDir(buffer).exists()) {
		QDir().mkdir(buffer);
	}

	//sprintf(filename, "%s/data/game/game_%s/%s.map", FILEPATH, temp_cent_number, dataExchanger->centNumber);
	//printf("DEBUG.EditorArea::saveMap - filename: %s\n", filename);
	//fp = fopen(filename, "wb");
	//fwrite(&map, sizeof(map), 1, fp);
	//fclose(fp);

	// ------------------- EXPORT MAP TO NEW FORMAT AND LOCATION ---------------------- //
	//int stage_n = dataExchanger->get_stage_n(map_n);
	int stage_n = dataExchanger->currentStage;

	char directory[255];

	//printf(">>> 2.currentStage; %d <<<\n", dataExchanger->currentStage);
	dataExchanger->centNumberFormat(stage_n);
	sprintf(temp_cent_number, "%s", dataExchanger->centNumber);
	sprintf(directory, "%s/stage_%s", buffer, dataExchanger->centNumber);
	if (!QDir(directory).exists()) {
		QDir().mkdir(directory);
	}
	sprintf(filename, "%s/%s_map.dat", directory, map_cent_number);
	//printf("DEBUG.EditorArea::saveMap.export_maps - filename: %s\n", filename);

	// copy data from old struct to the newer one
	struct file_map_v1 map_exp;
	sprintf(map_exp.filename, "%s", map.filename);
	for (int i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			map_exp.tiles[i][j].locked = map.tiles[i][j].locked;
			map_exp.tiles[i][j].tile1.x = map.tiles[i][j].tile1.x;
			map_exp.tiles[i][j].tile1.y = map.tiles[i][j].tile1.y;
			map_exp.tiles[i][j].tile2.x = map.tiles[i][j].tile2.x;
			map_exp.tiles[i][j].tile2.y = map.tiles[i][j].tile2.y;
			map_exp.tiles[i][j].tile3.x = map.tiles[i][j].tile3.x;
			map_exp.tiles[i][j].tile3.y = map.tiles[i][j].tile3.y;
		}
	}


	FILE *fp_exp = fopen(filename, "wb");
	if (fp_exp) {
		fwrite(&map_exp, sizeof(struct file_map_v1), 1, fp_exp);
		fclose(fp_exp);
	}
	// ------------------- EXPORT MAP TO NEW FORMAT AND LOCATION ---------------------- //

	saveMapNpcs(game_n, map_n);
	saveMapObjects(game_n, map_n);
	dataExchanger->saveMapLinks();
}


void EditorArea::saveMapNpcs(int game_n, int map_n) {
	char filename[255];
	char directory[255];
	char game_cent_number[4], map_cent_number[4], stage_cent_number[4];

	dataExchanger->centNumberFormat(game_n);
	sprintf(game_cent_number, "%s", dataExchanger->centNumber);
	dataExchanger->centNumberFormat(map_n);
	sprintf(map_cent_number, "%s", dataExchanger->centNumber);
	dataExchanger->centNumberFormat(dataExchanger->currentStage);
	sprintf(stage_cent_number, "%s", dataExchanger->centNumber);

	sprintf(directory, "%s/data/game/game_%s/stage_%s", FILEPATH, game_cent_number, stage_cent_number);

	printf(">>> WARNING::EditorArea::saveMapNpcs - directory: '%s' <<<\n", directory);

	// save map npc list
	if (dataExchanger->static_npc_list == NULL) {
		dataExchanger->loadGameNpcs (game_n);
	}
	if (dataExchanger->npc_map_list == NULL) {
		dataExchanger->loadMapNpcs (game_n, dataExchanger->currentStage, dataExchanger->currentMap);
	}
	if (dataExchanger->npc_map_list != NULL) {
		FILE *fp_map_npc = NULL;
		struct list_map_npc *temp_map_npc_list = NULL;
		struct file_map_npc file_map_npc;

		sprintf(filename, "%s/map_%s_npc_list.dat", directory, map_cent_number);
		printf(">>>>>>>>>>>>> DEBUG.EditorArea::saveMap.exportNPCList - filename: %s\n", filename);
		fp_map_npc = fopen(filename, "wb");

		if (fp_map_npc) {
			temp_map_npc_list = dataExchanger->npc_map_list;
			while (temp_map_npc_list) {
				//printf("*** saveMap::saveMapNPCs - id: %d ***\n", temp_map_npc_list->id);
				file_map_npc.direction = temp_map_npc_list->direction;
				file_map_npc.id = temp_map_npc_list->id;
				file_map_npc.start_point.x = temp_map_npc_list->start_point.x;
				file_map_npc.start_point.y = temp_map_npc_list->start_point.y;
				temp_map_npc_list = temp_map_npc_list->next;
				if (fwrite(&file_map_npc, sizeof(struct file_map_npc), 1, fp_map_npc) != 1) {
					printf(">>>>>>>>>>>>>>>> ERROR Writing NPC to file <<<<<<<<<<<<<<<\n");
				}
			}
			fclose(fp_map_npc);
		}
	}
}

void EditorArea::saveMapObjects(int game_n, int map_n)  {
	FILE *fp;
	char filename[255], directory[255];
	char game_cent_number[4], map_cent_number[4], stage_cent_number[4];

	dataExchanger->centNumberFormat(game_n);
	sprintf(game_cent_number, "%s", dataExchanger->centNumber);
	dataExchanger->centNumberFormat(map_n);
	sprintf(map_cent_number, "%s", dataExchanger->centNumber);
	dataExchanger->centNumberFormat(dataExchanger->currentStage);
	sprintf(stage_cent_number, "%s", dataExchanger->centNumber);

	sprintf(directory, "%s/data/game/game_%s/stage_%s", FILEPATH, game_cent_number, stage_cent_number);
	printf(">>> WARNING::EditorArea::saveMapObjects - directory: '%s' <<<\n", directory);
	return;



	sprintf(filename, "%s/%s.map.obj", directory, map_cent_number);
	fp = fopen(filename, "wb");
	temp_obj_list = obj_list;
	struct stFileObject tempFileObj;
	struct struct_object *temp_main_obj_list, *found_main_obj;

	if (fp) {
		while (temp_obj_list) {
			// find the equivalent object from main list
			temp_main_obj_list = main_obj_list;
			found_main_obj = NULL;
			while (temp_main_obj_list) {
				if (strstr(temp_main_obj_list->id, temp_obj_list->id)) {
					found_main_obj = temp_main_obj_list;
					break;
				}
				temp_main_obj_list = temp_main_obj_list->next;
			}
			if (found_main_obj == NULL) {
				//printf("EditorArea::saveMap - ERROR: Could not find object with id '%s' in the main object list\n", temp_obj_list->id);
				break;
			}

			// copy information for the file list format
			sprintf(tempFileObj.id, "%s", temp_obj_list->id);
			tempFileObj.start_point.x = temp_obj_list->start_point.x;
			tempFileObj.start_point.y = temp_obj_list->start_point.y;
			fwrite(&tempFileObj, sizeof(struct stFileObject), 1, fp);
			temp_obj_list = temp_obj_list->next;
		}
		fclose(fp);
	}

}

void EditorArea::saveStages(int game_n) {
	char filename[255];
	struct st_file_stage stage;
	FILE *fp;
	int i, j;

	dataExchanger->centNumberFormat(game_n);

	sprintf(filename, "%s/data/game/game_%s/stages.dat", FILEPATH, dataExchanger->centNumber);
	fp = fopen(filename, "wb");

	if (!fp) {
		printf(">> ERROR.saveStages - could not open '%s' <<\n", filename);
		return;
	} else {
		printf(">> opened file '%s' for writing <<\n", filename);
	}

	// reset colorcycle
	for (i=0; i<3; i++) {
		stage.colorcycle[i].original.r = -1;
		stage.colorcycle[i].original.g = -1;
		stage.colorcycle[i].original.b = -1;
		for (j=0; j<COLORCYCLE_MAX_STEPS; j++) {
			stage.colorcycle[i].current_color[j].duration = 0;
			stage.colorcycle[i].current_color[j].duration = 0;
			stage.colorcycle[i].current_color[j].r = -1;
			stage.colorcycle[i].current_color[j].g = -1;
			stage.colorcycle[i].current_color[j].b = -1;
		}
	}


	sprintf(stage.stage_name, "%s", "Intro Stage");
	sprintf(stage.boss_name, "%s", "Dummy Boss");
	sprintf(stage.boss_sprites_filename, "%s", "canotus.png");
	sprintf(stage.boss_face_filename, "%s", "canotus.png");
	sprintf(stage.bg_music_filename, "%s", "castle.mod");

	// colorcycle - color 1
	stage.colorcycle[0].original.r = 55; // 1.original
	stage.colorcycle[0].original.g = 255;
	stage.colorcycle[0].original.b = 0;
	stage.colorcycle[0].current_color[0].duration = 300; // 1.cl.1
	stage.colorcycle[0].current_color[0].r = 255;
	stage.colorcycle[0].current_color[0].g = 155;
	stage.colorcycle[0].current_color[0].b = 59;
	stage.colorcycle[0].current_color[1].duration = 300; // 1.cl.2
	stage.colorcycle[0].current_color[1].r = 196;
	stage.colorcycle[0].current_color[1].g = 93;
	stage.colorcycle[0].current_color[1].b = 0;
	stage.colorcycle[0].current_color[2].duration = 300; // 1.cl.3
	stage.colorcycle[0].current_color[2].r = 255;
	stage.colorcycle[0].current_color[2].g = 219;
	stage.colorcycle[0].current_color[2].b = 171;
	// colorcycle - color 2
	stage.colorcycle[1].original.r = 55; // 2.original
	stage.colorcycle[1].original.g = 255;
	stage.colorcycle[1].original.b = 0;
	stage.colorcycle[1].current_color[0].duration = 300; // 2.cl.1
	stage.colorcycle[1].current_color[0].r = 255;
	stage.colorcycle[1].current_color[0].g = 155;
	stage.colorcycle[1].current_color[0].b = 59;
	stage.colorcycle[1].current_color[1].duration = 300; // 2.cl.2
	stage.colorcycle[1].current_color[1].r = 196;
	stage.colorcycle[1].current_color[1].g = 93;
	stage.colorcycle[1].current_color[1].b = 0;
	stage.colorcycle[1].current_color[2].duration = 300; // 2.cl.3
	stage.colorcycle[1].current_color[2].r = 255;
	stage.colorcycle[1].current_color[2].g = 219;
	stage.colorcycle[1].current_color[2].b = 171;
	// colorcycle - color 3
	stage.colorcycle[2].original.r = 55; // 3.original
	stage.colorcycle[2].original.g = 255;
	stage.colorcycle[2].original.b = 0;
	stage.colorcycle[2].current_color[0].duration = 300; // 3.cl.1
	stage.colorcycle[2].current_color[0].r = 255;
	stage.colorcycle[2].current_color[0].g = 155;
	stage.colorcycle[2].current_color[0].b = 59;
	stage.colorcycle[2].current_color[1].duration = 300; // 3.cl.2
	stage.colorcycle[2].current_color[1].r = 196;
	stage.colorcycle[2].current_color[1].g = 93;
	stage.colorcycle[2].current_color[1].b = 0;
	stage.colorcycle[2].current_color[2].duration = 300; // 3.cl.3
	stage.colorcycle[2].current_color[2].r = 255;
	stage.colorcycle[2].current_color[2].g = 219;
	stage.colorcycle[2].current_color[2].b = 171;


	printf(">> WARNING.fwrite BEFORE <<\n");
	// INTRO
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	printf(">> WARNING.fwrite AFTER <<\n");

	sprintf(stage.boss_name, "%s", "African Savana");
	sprintf(stage.boss_name, "%s", "Ape Bot");
	sprintf(stage.boss_sprites_filename, "%s", "apebot.png");
	sprintf(stage.boss_face_filename, "%s", "apebot.png");
	sprintf(stage.bg_music_filename, "%s", "ape.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Flower Field");
	sprintf(stage.boss_name, "%s", "Daisie Bot");
	sprintf(stage.boss_sprites_filename, "%s", "daisiebot.png");
	sprintf(stage.boss_face_filename, "%s", "daisiebot.png");
	sprintf(stage.bg_music_filename, "%s", "daisie.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Undersea Lab.");
	sprintf(stage.boss_name, "%s", "Seahorse Bot");
	sprintf(stage.boss_sprites_filename, "%s", "seahorsebot.png");
	sprintf(stage.boss_face_filename, "%s", "seahorsebot.png");
	sprintf(stage.bg_music_filename, "%s", "seahorse.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Piramid Maze");
	sprintf(stage.boss_name, "%s", "Mummy Bot");
	sprintf(stage.boss_sprites_filename, "%s", "mummybot.png");
	sprintf(stage.boss_face_filename, "%s", "mummybot.png");
	sprintf(stage.bg_music_filename, "%s", "mummy.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Destrin Castle - Part1");
	sprintf(stage.boss_name, "%s", "Dr. Destrin");
	sprintf(stage.boss_sprites_filename, "%s", "dr_destrin.png");
	sprintf(stage.boss_face_filename, "%s", "destrin_capsule_small.png.png");
	sprintf(stage.bg_music_filename, "%s", "castle.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Magic Tower");
	sprintf(stage.boss_name, "%s", "Mage Bot");
	sprintf(stage.boss_sprites_filename, "%s", "magebot.png");
	sprintf(stage.boss_face_filename, "%s", "magebot.png");
	sprintf(stage.bg_music_filename, "%s", "mage.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Dark Mines");
	sprintf(stage.boss_name, "%s", "Dynamite Bot");
	sprintf(stage.boss_sprites_filename, "%s", "dynamitebot.png");
	sprintf(stage.boss_face_filename, "%s", "dynamitebot.png");
	sprintf(stage.bg_music_filename, "%s", "dynamite.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Danger Dungeon");
	sprintf(stage.boss_name, "%s", "Spike Bot");
	sprintf(stage.boss_sprites_filename, "%s", "spikebot.png");
	sprintf(stage.boss_face_filename, "%s", "spikebot.png");
	sprintf(stage.bg_music_filename, "%s", "spike.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Computer Factory");
	sprintf(stage.boss_name, "%s", "Techno Bot");
	sprintf(stage.boss_sprites_filename, "%s", "technobot.png");
	sprintf(stage.boss_face_filename, "%s", "technobot.png");
	sprintf(stage.bg_music_filename, "%s", "techno.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Destrin Castle - Part2");
	sprintf(stage.boss_name, "%s", "Dr. Destrin");
	sprintf(stage.boss_sprites_filename, "%s", "dr_destrin.png");
	sprintf(stage.boss_face_filename, "%s", "destrin_capsule_small.png.png");
	sprintf(stage.bg_music_filename, "%s", "castle.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Destrin Castle - Part3");
	sprintf(stage.boss_name, "%s", "Dr. Destrin");
	sprintf(stage.boss_sprites_filename, "%s", "dr_destrin.png");
	sprintf(stage.boss_face_filename, "%s", "destrin_capsule_small.png.png");
	sprintf(stage.bg_music_filename, "%s", "castle.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	sprintf(stage.boss_name, "%s", "Destrin Castle - Part4");
	sprintf(stage.boss_name, "%s", "Dr. Destrin");
	sprintf(stage.boss_sprites_filename, "%s", "dr_destrin.png");
	sprintf(stage.boss_face_filename, "%s", "destrin_capsule_small.png.png");
	sprintf(stage.bg_music_filename, "%s", "castle.mod");
	fwrite(&stage, sizeof(struct st_file_stage), 1, fp);

	fclose(fp);
	//printf("--- saveStages.DONE - game_n: %d ---\n", game_n);
}

void EditorArea::saveGame(int game_n) {
	char filename[512];
	if (game_n == 0) {
		game_n++;
	}

	printf("****************** saveGame ****************** \n");

	// ******************* HARDCODED VARIABLES ******************* //
	sprintf(game.zones[0].bg_filename, "%s", "stage_select.png");
	game.zones[0].stages[0].initial_map_number = 1;
	game.zones[0].stages[0].initial_point.x = 0;
	game.zones[0].stages[0].initial_point.y = 0;
	// ******************* HARDCODED VARIABLES ******************* //

	dataExchanger->centNumberFormat(game_n);
	sprintf(filename, "%s/data/game/%s.gme", FILEPATH, dataExchanger->centNumber);

	FILE *fp = fopen(filename, "wb");
	fwrite(&game, sizeof(game), 1, fp);
	fclose(fp);

	saveStages(game_n);

	saveMap(game_n, dataExchanger->currentMap);
}

void EditorArea::addNpcToMap(int posx, int posy) {
	int i = 0;
	// checks if there is a npc in this position
	struct list_map_npc *temp_map_npc_list = dataExchanger->npc_map_list;
	while (temp_map_npc_list) {
		if (temp_map_npc_list->start_point.x == posx && temp_map_npc_list->start_point.y == posy) {
			QMessageBox msgBox;
			msgBox.setText("There is a NPC in this point already");
			msgBox.exec();
			return;
		}
		temp_map_npc_list = temp_map_npc_list->next;
	}
	dataExchanger->placeNpc(posx, posy, i);
}

void EditorArea::removeNpcFromMap(int posx, int posy) {
	struct list_map_npc *temp_list, *item_ant=NULL, *item_aux=NULL;

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
	struct struct_object *new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
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

}


