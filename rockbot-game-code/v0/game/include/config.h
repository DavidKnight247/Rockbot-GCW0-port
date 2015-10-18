#ifndef CONFIG_H
#define CONFIG_H

#define WPN_COLUMN1_X 42
#define WPN_COLUMN2_X 182
#define WPN_COLUMN_Y 33



SDL_Surface *config_img=NULL, *dialog_bg=NULL;
SDL_Surface *weapon_menu_surface = NULL;
short int menu_pos_x, menu_pos_y;
short int weapon_menu_pos_x, weapon_menu_pos_y;

#define WEAPON_SPACING 16

// position - 0 is top, 1 is bottom
void showDialog(int face_n, char *lines, int position) {
    char *temp_char;
    char temp_text[2];
	int posY;

    int text_x=0, text_y=0;

    if (dialog_bg == NULL) {
        createDialogBG();
    }

	if (position == 0) {
		posY = 9;
	} else {
		posY = RES_H - dialog_bg->h - 25;
	}

	copy_area(FACES_SURFACES[face_n], 0, 0, FACES_SURFACES[face_n]->w, FACES_SURFACES[face_n]->h, 8, 9, dialog_bg);

    temp_char = lines;
    // full-line = 27 character, x.ini=7
    // face-lines = 23 characters, x.ini=7+32
    while (*temp_char != '\0') {
            sprintf(temp_text, "%c", *temp_char);
			draw_text(text_x*9+43, text_y*11+9, temp_text, dialog_bg);
			copy_area(dialog_bg, 0, 0, dialog_bg->w, dialog_bg->h, (RES_W-dialog_bg->w)*0.5, posY, game_screen);
			updateScreen(game_screen);
            temp_char = temp_char+1;
            text_x++;
            if (*temp_char == '\n') {
                    text_x = 0;
                    text_y++;
                    temp_char = temp_char+1;
            }
			if (game_screen) {
				exec_colorcycle(game_screen);
				updateScreen(game_screen);
			}
			waitTime(5);
    }
    wait_player_key();
}


void setVideoMode(short int fullscreen) {
	SDL_Surface *temp_screen;

	// create a backup of the screen
	temp_screen = SDL_CreateRGBSurface(SDL_SWSURFACE , MAX_W, MAX_H, 8, 0, 0, 0, 0);
	SDL_SetColors(temp_screen, colormap, 0, COLOR_COUNT);
	copy_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, temp_screen);
	#ifdef PLAYSTATION
		game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	#else
		if (fullscreen) {
			#ifndef SCALE
				game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
			#else
				game_screen_scaled = SDL_SetVideoMode(RES_W*2, RES_H*2, 8, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
			#endif
		} else {
			#ifndef SCALE
				game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
			#else
				game_screen_scaled = SDL_SetVideoMode(RES_W*2, RES_H*2, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
			#endif
		}
	#endif
	// restore the screem backup
	copy_area(temp_screen, 0, 0, MAX_W, MAX_H, 0, 0, game_screen);
	SDL_FreeSurface(temp_screen);
}


void show_credits() {
	SDL_Surface *temp_screen;
	temp_screen = SDL_CreateRGBSurface(SDL_SWSURFACE , config_img->w-10, config_img->h+10, 8, 0, 0, 0, 0);
	SDL_SetColors(config_img, colormap, 0, COLOR_COUNT);
	draw_text(120+menu_pos_x, 10+menu_pos_y, "windowed", game_screen);
	SDL_FreeSurface(temp_screen);
}

void config_showMenu() {
	clean_input(1);
    char buffer_file[255];

	if (config_img == NULL) {
		sprintf(buffer_file, "%sdata/images/backgrounds/menu.png", FILEPATH);
		config_img = loadSpriteFile(buffer_file);
		SDL_SetColors(config_img, colormap, 0, COLOR_COUNT);
	}

	menu_pos_x = (RES_W-config_img->w)*0.5;
	menu_pos_y = (RES_H-config_img->h)*0.5;
    copy_area(config_img, 0, 0, config_img->w, config_img->h, menu_pos_x, menu_pos_y, game_screen);
}


void config_show_weapon_menu() {
	clean_input(1);
	char buffer_file[255];

	if (weapon_menu_surface == NULL) {
		sprintf(buffer_file, "%sdata/images/backgrounds/weapon_menu.png", FILEPATH);
		weapon_menu_surface = loadSpriteFile(buffer_file);
		if (!weapon_menu_surface) {
			printf("ERROR loading weapon menu background '%s' <<\n", buffer_file);
		}
		SDL_SetColors(weapon_menu_surface, colormap, 0, COLOR_COUNT);
	}

	weapon_menu_pos_x = (RES_W-weapon_menu_surface->w)*0.5;
	weapon_menu_pos_y = (RES_H-weapon_menu_surface->h)*0.5;
	copy_area(weapon_menu_surface, 0, 0, weapon_menu_surface->w, weapon_menu_surface->h, weapon_menu_pos_x, weapon_menu_pos_y, game_screen);
}

void subMenu(int pos) {
	short int optionPicked=0, fim=0;

	clear_area(game_screen, 120+menu_pos_x, 20+menu_pos_y, 120, 120, 0, 0, 0);
	// sound
	if (pos == 0) {
		draw_text(120+menu_pos_x, 10+menu_pos_y, "enabled", game_screen);
		draw_text(120+menu_pos_x, 25+menu_pos_y, "disabled", game_screen);
	// video
#ifndef PLAYSTATION
	} else if (pos == 1) {
		draw_text(120+menu_pos_x, 10+menu_pos_y, "windowed", game_screen);
		draw_text(120+menu_pos_x, 25+menu_pos_y, "fullscreen", game_screen);
	} else if (pos == 2) {
		show_credits();
	}
#else
	//PS2 video
	} else if (pos == 2) {
		draw_text(120+menu_pos_x, 10+menu_pos_y, "filtered", game_screen);
		draw_text(120+menu_pos_x, 25+menu_pos_y, "no-filter", game_screen);
	}
#endif
	clean_input(1);
	waitTime(200);
	draw_text(110+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);

	while (!fim) {
		printf("subMenu LOOP\n");
		read_input();
		if (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 110+menu_pos_x, (optionPicked*15)+10+menu_pos_y, 8, 10, 0, 0, 0);
			optionPicked--;
			if (optionPicked < 0) {
				optionPicked = 1;
			}
			draw_text(110+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 110+menu_pos_x, (optionPicked*15)+10+menu_pos_y, 8, 10, 0, 0, 0);
			optionPicked++;
			if (optionPicked > 1) {
				optionPicked = 0;
			}
			draw_text(110+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1 || keyState[0][KEYJUMP] == 1 || keyState[1][KEYJUMP] == 1) {
			if (pos == 0) {
				if (optionPicked == 0) {
					if (game_config.sound_enabled == 0) {
						load_music("bgmusic.mod");
						play_music();
					}
					game_config.sound_enabled = 1;
				} else {
					if (game_config.sound_enabled == 1) {
						stop_music();
					}
					game_config.sound_enabled = 0;
				}
			} else if (pos == 1) {
			#ifndef PLAYSTATION
				if (optionPicked == 0) {
					if (game_config.video_fullscreen == 1) {
						setVideoMode(0);
					}
					game_config.video_fullscreen = 0;
				} else {
					if (game_config.video_fullscreen == 0) {
						setVideoMode(1);
					}
					game_config.video_fullscreen = 1;
				}
			#endif
			}
			fim = 1;
		} else if (keyState[0][KEYATTACK] == 1 || keyState[1][KEYATTACK] == 1) {
			fim = 1;
		}
		clean_input(1);
		updateScreen(game_screen);
		waitTime(10);
	}
	clean_input(1);
	waitTime(300);
	clear_area(game_screen, 110+menu_pos_x, 10+menu_pos_y, 120, 120, 0, 0, 0);
	updateScreen(game_screen);

}

// sound -> enabled, disabled
// video -> full screen, windowed
// ps2 video -> filter 1, filter 2, no-filter
void config_showMainConfig() {
	short int optionPicked=0, fim=0;

	config_showMenu();
	game_config.sound_enabled=1;
	game_config.video_fullscreen=0;
	game_config.video_filter=0;

	draw_text(20+menu_pos_x, 10+menu_pos_y, "Sound", game_screen);
	draw_text(20+menu_pos_x, 25+menu_pos_y, "Video", game_screen);
	draw_text(20+menu_pos_x, 40+menu_pos_y, "Credits", game_screen);
	clean_input(1);
	waitTime(200);
	draw_text(10+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);

	while (!fim) {
		read_input();
		if (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2) {
			printf("config_showMainConfig -> UP\n");
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 10+menu_pos_x, (optionPicked*15)+10+menu_pos_y, 8, 10, 0, 0, 0);
			optionPicked--;
			if (optionPicked < 0) {
				optionPicked = 1;
			}
			draw_text(10+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1) {
			printf("config_showMainConfig -> DOWN\n");
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 10+menu_pos_x, (optionPicked*15)+10+menu_pos_y, 8, 10, 0, 0, 0);
			optionPicked++;
			if (optionPicked > 1) {
				optionPicked = 0;
			}
			draw_text(10+menu_pos_x, (optionPicked*15)+10+menu_pos_y, ">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1 || keyState[0][KEYJUMP] == 1 || keyState[1][KEYJUMP] == 1) {
			subMenu(optionPicked);
		} else if (keyState[0][KEYATTACK] == 1 || keyState[1][KEYATTACK] == 1) {
			fim = 1;
		}
		clean_input(1);
		updateScreen(game_screen);
		waitTime(10);
	}
	clean_input(1);
	updateScreen(game_screen);
	waitTime(200);
	clean_input(1);
}

void mainMenu() {
	SDL_Surface* screenCopy;
	short int optionPicked=0, fim=0;

	screenCopy = SDL_CreateRGBSurface(SDL_SWSURFACE , RES_W, RES_H, 8, 0, 255, 0, 0);
	SDL_SetColors(screenCopy, colormap, 0, COLOR_COUNT);
	copy_area(game_screen, 0, 0, RES_W, RES_H, 0, 0, screenCopy);
	draw_text(40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12),">", game_screen);

	while (!fim) {
		read_input();
		if (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12), 8, 10, 0, 0, 0);
			optionPicked--;
			if (optionPicked < 0) {
				optionPicked = 3;
			}
			draw_text(40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12),">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12), 8, 10, 0, 0, 0);
			optionPicked++;
			if (optionPicked > 3) {
				optionPicked = 0;
			}
			draw_text(40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12),">", game_screen);
			waitTime(300);
		} else if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1 || keyState[0][KEYJUMP] == 1 || keyState[1][KEYJUMP] == 1) {
			if (optionPicked == 1) {
				config_showMainConfig();
				copy_area(screenCopy, 0, 0, RES_W, RES_H, 0, 0, game_screen);
				draw_text(40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12),">", game_screen);
			} else if (optionPicked == 2) {
				showCredits();
				copy_area(screenCopy, 0, 0, RES_W, RES_H, 0, 0, game_screen);
				draw_text(40-RES_DIFF_W-8, (MAX_H*0.5)-RES_DIFF_H+(optionPicked*12),">", game_screen);
			} else if (optionPicked == 3) {
				leave_game();
			} else {
				fim = 1;
			}
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
		}
		clean_input(1);
		updateScreen(game_screen);
		waitTime(1);
	}
	SDL_FreeSurface(screenCopy);
}

void draw_horizontal_hp_bar(short int y_adjust, short int right, short int hp, SDL_Surface *screen) {
	short int x, y, i;
	short int r, g, b;
	// armas - coluna1: 42, 26, 182, coluna2: 26

	if (right == 0) {
		x = WPN_COLUMN1_X+18;
		r = 127;
		g = 11;
		b = 0;
	} else if (right == 1) {
		x = WPN_COLUMN2_X+18;
		r = 255;
		g = 102;
		b = 0;
	} else if (right == 2) {
		x = WPN_COLUMN1_X+18;
		r = 0;
		g = 0;
		b = 0;
	} else {
		x = WPN_COLUMN2_X+18;
		r = 0;
		g = 0;
		b = 0;
	}
	// border
	/*
	clear_area(weapon_menu_surface, x+1, y_adjust, 61, 2, 188, 188, 188);
	clear_area(weapon_menu_surface, x+1, y_adjust+7, 61, 2, 188, 188, 188);

	clear_area(weapon_menu_surface, x+1, y_adjust, 2, 9, 188, 188, 188);
	clear_area(weapon_menu_surface, x+60, y_adjust, 2, 9, 188, 188, 188);
	*/


	if (right < 2) {
		clear_area(screen, x+1, 1+y_adjust, 61, 9, r, g, b);
	}
	for (i=0; i<hp; i++) {
		y = ((28-i)*2+1);
		clear_area(screen, x+y+1, 2+y_adjust, 1, 5, 255, 255, 255);
	}
}


void openInGameMenu(int player_n) {
	SDL_Surface *energy_img;

	clean_input(1);
	if (weapon_menu_surface != NULL) {
		SDL_FreeSurface(weapon_menu_surface);
		weapon_menu_surface = NULL;
	}
	// armas - coluna1: 42, 26, 182, coluna2: 26
	// player: 25, 150
	// player-energy: 33, 175
	// items, y-icon: 161, y-value: 177, x: 121, 145, 169, 193
	config_show_weapon_menu();
	char buffer_file[255];
	sprintf(buffer_file, "%sdata/images/items.png", FILEPATH);
	energy_img = loadSpriteFile(buffer_file);
	SDL_SetColors(energy_img, colormap, 0, COLOR_COUNT);
	copy_area(energy_img, 0, 0, TILE_SIZE, TILE_SIZE, 121, 161, weapon_menu_surface); // energy tnak
	copy_area(energy_img, 48, 0, TILE_SIZE, TILE_SIZE, 145, 161, weapon_menu_surface); // weapons tank
	// show player standing in menu
	if (player_n == 0) {
		copy_area(p1Obj->sprite->sprite_frames[ANIM_LEFT][ANIM_STAND][0].frame, 0, 0, p1Obj->sprite->w, p1Obj->sprite->h, 22, 153, weapon_menu_surface);  // player1 face
	} else {
		copy_area(p1Obj->sprite->sprite_frames[ANIM_LEFT][ANIM_STAND][0].frame, 0, 0, p1Obj->sprite->w, p1Obj->sprite->h, 22, 153, weapon_menu_surface);  // player1 face
	}
	SDL_FreeSurface(energy_img);
	waitTime(50);
	updateScreen(game_screen);
	inGameMenu(player_n);
}

void drawInGameMenuSelection(struct pos point, int player_n) {
	int x, y;
	int r, g, b;
	int w, h;

	if (player_n == 0) {
		r = 127;
		g = 11;
		b = 0;
	} else if (player_n == 1) {
		r = 255;
		g = 102;
		b = 0;
	} else {
		r = 0;
		g = 0;
		b = 0;
	}

	if (point.x == 0) {
		x = WPN_COLUMN1_X + 19;
	} else {
		x = WPN_COLUMN2_X + 19;
	}

	w = 61;
	h = 7;

	// items, y-icon: 161, y-value: 177, x: 121, 145, 169, 193
	if (point.y == 0) {
		y = WPN_COLUMN_Y;
	} else if (point.y == 6 && point.x == 0) {
		y = 159;
		x = 119;
		w = 20;
		h = 27;
	} else if (point.y == 6 && point.x == 1) {
		y = 159;
		x = 143;
		w = 20;
		h = 27;
	} else {
		y = WEAPON_SPACING*point.y + 33;
	}


	clear_area(weapon_menu_surface, x, y, w, 2, r, g, b);
	clear_area(weapon_menu_surface, x, y+h, w, 2, r, g, b);

	clear_area(weapon_menu_surface, x, y, 2, h+2, r, g, b);
	clear_area(weapon_menu_surface, x+w-2, y, 2, h+2, r, g, b);
}


// converts a coordinate of menu weapon to boss number
int selectedWeaponNumber (struct pos point) {
	if (point.y == 1 && point.x == 0) { // apebot
		return APEBOT;
	} else if (point.y == 2 && point.x == 0) { // daisie
		return DAISIEBOT;
	} else if (point.y == 3 && point.x == 0) { // dynamite
		return DYNAMITEBOT;
	} else if (point.y == 4 && point.x == 0) { // mummy
		return MUMMYBOT;
	} else if (point.y == 5 && point.x == 0) { // spike
		return SPIKEBOT;
	} else if (point.y == 1 && point.x == 1) { // techno
		return TECHNOBOT;
	} else if (point.y == 2 && point.x == 1) { // mage
		return MAGEBOT;
	} else if (point.y == 3 && point.x == 1) { // seahorse
		return SEAHORSEBOT;
	} else if (point.y == 4 && point.x == 1) { // item coil
		return ITEM_COIL;
	} else if (point.y == 5 && point.x == 1) { // item jet
		return ITEM_JET;
	} else {
		return -1;
	}
}

struct pos selectedWeaponPos (int number) {
	struct pos pos;
	if (number == APEBOT) {
		pos.x = 0;
		pos.y = 1;
	} else if (number == DAISIEBOT) {
		pos.x = 0;
		pos.y = 2;
	} else if (number == DYNAMITEBOT) {
		pos.x = 0;
		pos.y = 3;
	} else if (number == MUMMYBOT) {
		pos.x = 0;
		pos.y = 4;
	} else if (number == SPIKEBOT) {
		pos.x = 0;
		pos.y = 5;
	} else if (number == TECHNOBOT) {
		pos.x = 1;
		pos.y = 1;
	} else if (number == MAGEBOT) {
		pos.x = 1;
		pos.y = 2;
	} else if (number == SEAHORSEBOT) {
		pos.x = 1;
		pos.y = 3;
	} else if (number == ITEM_COIL) {
		pos.x = 1;
		pos.y = 4;
	} else if (number == ITEM_JET) {
		pos.x = 1;
		pos.y = 5;
	} else {
		pos.x = 0;
		pos.y = 0;
	}
	return pos;
}

void setPlayerColor(int pos, int player_n) {
	struct st_color p1_color1, p1_color2, p2_color1, p2_color2, p2_color3;

	printf(">> pos: %d <<\n", pos);

	if (pos == APEBOT) { // apebot
		p1_color1.r = 139;
		p1_color1.g = 115;
		p1_color1.b = 0;
		p1_color2.r = 196;
		p1_color2.g = 93;
		p1_color2.b = 0;
		// player 2
		p2_color1.r = 139; // laranja claro
		p2_color1.g = 115;
		p2_color1.b = 0;
		p2_color2.r = 67; // laranja escuro
		p2_color2.g = 47;
		p2_color2.b = 0;
		p2_color3.r = 255; // amarelo
		p2_color3.g = 231;
		p2_color3.b = 163;
	} else if (pos == DAISIEBOT) { // daisie
		p1_color1.r = 79;
		p1_color1.g = 223;
		p1_color1.b = 75;
		p1_color2.r = 255;
		p1_color2.g = 119;
		p1_color2.b = 183;
		// player 2
		p2_color1.r = 79; // laranja claro
		p2_color1.g = 233;
		p2_color1.b = 75;
		p2_color2.r = 0; // laranja escuro
		p2_color2.g = 147;
		p2_color2.b = 59;
		p2_color3.r = 255; // amarelo
		p2_color3.g = 119;
		p2_color3.b = 183;
	} else if (pos == DYNAMITEBOT) { // dynamite
		p1_color1.r = 188;
		p1_color1.g = 188;
		p1_color1.b = 188;
		p1_color2.r = 0;
		p1_color2.g = 171;
		p1_color2.b = 0;
		// player 2
		p2_color1.r = 188; // laranja claro
		p2_color1.g = 188;
		p2_color1.b = 188;
		p2_color2.r = 81; // laranja escuro
		p2_color2.g = 81;
		p2_color2.b = 81;
		p2_color3.r = 0; // amarelo
		p2_color3.g = 171;
		p2_color3.b = 0;
	} else if (pos == MUMMYBOT) { // mummy
		p1_color1.r = 255;
		p1_color1.g = 255;
		p1_color1.b = 255;
		p1_color2.r = 0;
		p1_color2.g = 115;
		p1_color2.b = 239;
		// player 2
		p2_color1.r = 255; // laranja claro
		p2_color1.g = 255;
		p2_color1.b = 255;
		p2_color2.r = 63; // laranja escuro
		p2_color2.g = 191;
		p2_color2.b = 255;
		p2_color3.r = 171; // amarelo
		p2_color3.g = 231;
		p2_color3.b = 255;
	} else if (pos == SPIKEBOT) { // spike
		p1_color1.r = 81;
		p1_color1.g = 81;
		p1_color1.b = 81;
		p1_color2.r = 0;
		p1_color2.g = 235;
		p1_color2.b = 219;
		// player 2
		p2_color1.r = 167; // laranja claro
		p2_color1.g = 139;
		p2_color1.b = 253;
		p2_color2.r = 131; // laranja escuro
		p2_color2.g = 0;
		p2_color2.b = 243;
		p2_color3.r = 0; // amarelo
		p2_color3.g = 235;
		p2_color3.b = 219;
	} else if (pos == TECHNOBOT) { // techno
		p1_color1.r = 219;
		p1_color1.g = 43;
		p1_color1.b = 0;
		p1_color2.r = 243;
		p1_color2.g = 191;
		p1_color2.b = 63;
		// player 2
		p2_color1.r = 167; // laranja claro
		p2_color1.g = 0;
		p2_color1.b = 0;
		p2_color2.r = 127; // laranja escuro
		p2_color2.g = 11;
		p2_color2.b = 0;
		p2_color3.r = 255; // amarelo
		p2_color3.g = 102;
		p2_color3.b = 0;
	} else if (pos == MAGEBOT) { // mage
		p1_color1.r = 191;
		p1_color1.g = 0;
		p1_color1.b = 191;
		p1_color2.r = 255;
		p1_color2.g = 192;
		p1_color2.b = 0;
		// player 2
		p2_color1.r = 191; // laranja claro
		p2_color1.g = 0;
		p2_color1.b = 191;
		p2_color2.r = 143; // laranja escuro
		p2_color2.g = 0;
		p2_color2.b = 119;
		p2_color3.r = 255; // amarelo
		p2_color3.g = 192;
		p2_color3.b = 0;
	} else if (pos == SEAHORSEBOT) { // seahorse
		p1_color1.r = 0;
		p1_color1.g = 131;
		p1_color1.b = 139;
		p1_color2.r = 219;
		p1_color2.g = 43;
		p1_color2.b = 0;
		// player 2
		p2_color1.r = 171; // laranja claro
		p2_color1.g = 243;
		p2_color1.b = 191;
		p2_color2.r = 0; // laranja escuro
		p2_color2.g = 131;
		p2_color2.b = 139;
		p2_color3.r = 219; // amarelo
		p2_color3.g = 43;
		p2_color3.b = 0;
	} else if (pos == ITEM_COIL) {
		p1_color1.r = 255;
		p1_color1.g = 51;
		p1_color1.b = 0;
		p1_color2.r = 235;
		p1_color2.g = 235;
		p1_color2.b = 235;
		// player 2
		p2_color1.r = 255; // laranja claro
		p2_color1.g = 51;
		p2_color1.b = 0;
		p2_color2.r = 219; // laranja escuro
		p2_color2.g = 43;
		p2_color2.b = 0;
		p2_color3.r = 235; // amarelo
		p2_color3.g = 235;
		p2_color3.b = 235;
	} else if (pos == ITEM_JET) {
		p1_color1.r = 235;
		p1_color1.g = 235;
		p1_color1.b = 235;
		p1_color2.r = 255;
		p1_color2.g = 51;
		p1_color2.b = 0;
		// player 2
		p2_color1.r = 235; // laranja claro
		p2_color1.g = 235;
		p2_color1.b = 235;
		p2_color2.r = 188; // laranja escuro
		p2_color2.g = 188;
		p2_color2.b = 188;
		p2_color3.r = 255; // amarelo
		p2_color3.g = 51;
		p2_color3.b = 0;
	} else {
		// player 1
		p1_color1.r = 143;
		p1_color1.g = 0;
		p1_color1.b = 119;
		p1_color2.r = 0;
		p1_color2.g = 115;
		p1_color2.b = 239;
		// player 2
		p2_color1.r = 255; // laranja claro
		p2_color1.g = 102;
		p2_color1.b = 0;
		p2_color2.r = 255; // laranja escuro
		p2_color2.g = 51;
		p2_color2.b = 0;
		p2_color3.r = 230; // amarelo
		p2_color3.g = 255;
		p2_color3.b = 0;
	}

	if (player_n == 0) {
		if ((game_config.two_players == 1 && game_config.selected_player == 1) || game_config.two_players == 2) {
			change_player_colormap(p1Obj, 4, p1_color1.r, p1_color1.g, p1_color1.b);
			change_player_colormap(p1Obj, 17, p1_color2.r, p1_color2.g, p1_color2.b);
		} else {
			change_player_colormap(p1Obj, 29, p2_color1.r, p2_color1.g, p2_color1.b);
			change_player_colormap(p1Obj, 30, p2_color2.r, p2_color2.g, p2_color2.b);
			change_player_colormap(p1Obj, 48, p2_color3.r, p2_color3.g, p2_color3.b);
		}
		p1Obj->selected_weapon = pos;
	} else {
		change_player_colormap(p2Obj, 29, p2_color1.r, p2_color1.g, p2_color1.b);
		change_player_colormap(p2Obj, 30, p2_color2.r, p2_color2.g, p2_color2.b);
		change_player_colormap(p2Obj, 48, p2_color3.r, p2_color3.g, p2_color3.b);
		p2Obj->selected_weapon = pos;
	}
}


void useEnergyTank(struct player_struct* pObj) {
	int n = 0;
	while (pObj->HP < INITIAL_HP) {
		pObj->HP++;
		if (pObj == p1Obj) {
			draw_horizontal_hp_bar(22, 2, pObj->HP, weapon_menu_surface);
		} else {
			draw_horizontal_hp_bar(22, 4, pObj->HP, weapon_menu_surface);
		}
		copy_area(weapon_menu_surface, 0, 0, weapon_menu_surface->w, weapon_menu_surface->h, menu_pos_x, menu_pos_y, game_screen);
		updateScreen(game_screen);
		waitTime(50);
		if (n == 0 || n % 6 == 0) {
			play_sfx(SFX_GOT_ENERGY);
		}
	}
}

void useWeaponsTank(struct player_struct* pObj) {
	int i, n;
	useEnergyTank(pObj);


	for (i=0; i<WEAPON_COUNT; i++) {
		n = 0;
		if (save_001.stages[i]) {
			while (p1Obj->items.weapons[i] < INITIAL_HP) {
				p1Obj->items.weapons[i]++;
				if (i < 4) {
					draw_horizontal_hp_bar(52+i*WEAPON_SPACING, 2, p1Obj->items.weapons[i], weapon_menu_surface);
				} else {
					draw_horizontal_hp_bar(52+(i-3)*WEAPON_SPACING, 3, p1Obj->items.weapons[i], weapon_menu_surface);
				}
				copy_area(weapon_menu_surface, 0, 0, weapon_menu_surface->w, weapon_menu_surface->h, weapon_menu_pos_x, weapon_menu_pos_y, game_screen);
				updateScreen(game_screen);
				waitTime(50);
				if (n == 0 || n % 6 == 0) {
					play_sfx(SFX_GOT_ENERGY);
				}
			}
		}
	}
}

// search for the first item in the given column (y)
int weapon_first_row(struct pos *player_point, int column) {
	int i;

	printf(">> weapon_first_row - column: %d <<\n", column);

	for (i=player_point->y+1; i<5; i++) {
		if (save_001.stages[weapon_matrix[column][i]]) {
			player_point->x = column;
			player_point->y = i+1;
			printf(">> weapon_first_row x: %d, y: %d <<\n", player_point->x, player_point->y);
			return 1;
		}
	}
	if (column == 1 && player_point->y < 6) {
		player_point->y = 6;
		return 1;
	}
	for (i=0; i<player_point->y+1; i++) {
		if (save_001.stages[weapon_matrix[column][i]]) {
			player_point->x = column;
			player_point->y = i+1;
			printf(">> weapon_first_row x: %d, y: %d <<\n", player_point->x, player_point->y);
			return 1;
		}
	}
	if (column == 0 && (player_point->x != 0 || player_point->y != 0)) {
		player_point->x = 0;
		player_point->y = 0;
		return 1;
	}
	return 0;
}


int weapon_last_row(struct pos *player_point, int column) {
	int i;

	printf(">> weapon_LAST_row - column: %d, player_point->y: %d <<\n", column, player_point->y);

	for (i=player_point->y-2; i>=0; i--) {
		if (save_001.stages[weapon_matrix[column][i]]) {
			player_point->x = column;
			player_point->y = i+1;
			printf(">> 1. weapon_LAST_row i: %d, x: %d, y: %d <<\n", i, player_point->x, player_point->y);
			return 1;
		}
	}
	if (column == 0 && player_point->y == 0) {
		player_point->y = 6;
		return 1;
	}

	for (i=5; i<player_point->y-1; i--) {
		if (save_001.stages[weapon_matrix[column][i]]) {
			player_point->x = column;
			player_point->y = i+1;
			printf(">> 2. weapon_LAST_row x: %d, y: %d <<\n", player_point->x, player_point->y);
			return 1;
		}
	}
	if (column == 0 && (player_point->x != 0 || player_point->y != 0)) {
		player_point->x = 0;
		player_point->y = 0;
		return 1;
	}
	return 0;
}


// column1: normal, ape,    daisie, dynamite, mummy,     spike
// column2:         techno, mage,   seahorse, item coil, item jet
// column3: e-tank, w-tank *(must be changed, as currently are two rows)
void weaponmenu_move_cursor(struct pos *player_point, struct player_struct *playerObj, int x_inc, int y_inc) {
	// left/right: if position exists, just move. if not then go to first item in nexct column or stays in place
	int point2, point1;

	printf(">> weaponmenu_move_cursor - x_inc: %d <<\n", x_inc);

	if (x_inc != 0) {
		if (player_point->y == 6) {
			player_point->x = !player_point->x;
			play_sfx(SFX_CURSOR);
			return;
		}

		if (player_point->x == 0) {
			point2 = 1;
		} else {
			point2 = 0;
		}
		if (player_point->y == 0) {
			point1 = 1;
		} else {
			point1 = player_point->y;
		}
		printf(">> weaponmenu_move_cursor - change x (%d) to: %d <<\n", player_point->x, point2);
		if (save_001.stages[weapon_matrix[point2][point1]] == 1) {
			player_point->x = point2;
			player_point->y = point1+1;
			play_sfx(SFX_CURSOR);
		} else {
			if (weapon_first_row(player_point, point2)) {
				play_sfx(SFX_CURSOR);
			} else if (weapon_first_row(player_point, !point2)) {
				play_sfx(SFX_CURSOR);
			}
		}
	}

	if (y_inc != 0) {
		if (player_point->y+y_inc < 0) {
			point2 = 6;
		} else if (player_point->y+y_inc > 6) {
			player_point->x = 0;
			player_point->y = 0;
			play_sfx(SFX_CURSOR);
			return;
		} else {
			point2 = player_point->y-1+y_inc;
		}
		printf(">> weaponmenu_move_cursor - x: %d, change y: (%d) to: %d, y_inc: %d <<\n", player_point->x, player_point->y, point2, y_inc);
		if (save_001.stages[weapon_matrix[player_point->x][point2]] == 1) {
			player_point->y = point2+1;
			play_sfx(SFX_CURSOR);
		} else if (y_inc > 0) {
			if (weapon_first_row(player_point, player_point->x)) {
				play_sfx(SFX_CURSOR);
			} else if (weapon_first_row(player_point, !player_point->x)) {
				play_sfx(SFX_CURSOR);
			}
		} else {
			if (weapon_last_row(player_point, player_point->x)) {
				printf(">> last.move 1 <<\n");
				play_sfx(SFX_CURSOR);
			} else if (weapon_last_row(player_point, !player_point->x)) {
				printf(">> last.move 2 <<\n");
				play_sfx(SFX_CURSOR);
			}
		}
	}

}


void inGameMenu(int player_n) {
	char tempChr[10];
	short int fim=0;
	struct pos p1Sel;
	int i;
	SDL_Surface *icons;
	char buffer[255];
	struct player_struct *playerObj;
	if (player_n == 0) {
		playerObj = p1Obj;
	} else {
		playerObj = p2Obj;
	}


	// armas - coluna1: 42, 26, 182, coluna2: 26
	// player: 25, 150
	// player-energy: 33, 175
	// items, y-icon: 161, y-value: 177, x: 121, 145, 169, 193


	p1Sel = selectedWeaponPos(playerObj->selected_weapon);

	if (playerObj->items.energy_tanks > 10) {
		playerObj->items.energy_tanks = 9;
	}
	sprintf(tempChr, "0%d", playerObj->items.energy_tanks);
	draw_text(121, 177, tempChr, weapon_menu_surface); // energy thank p1

	if (playerObj->items.weapon_tanks > 10) {
		playerObj->items.weapon_tanks = 9;
	}
	sprintf(tempChr, "0%d", playerObj->items.weapon_tanks);
	draw_text(145, 177, tempChr, weapon_menu_surface); // weapons tank p1

	if (playerObj->items.lifes > 10) {
		playerObj->items.lifes = 9;
	}
	sprintf(tempChr, "0%d", playerObj->items.lifes);
	draw_text(65, 177, tempChr, weapon_menu_surface); // lifes p1

	// BOLTS text(257, 178)
	if (playerObj->items.bolts > 10) {
		playerObj->items.bolts = 9;
	}
	if (playerObj->items.bolts < 10) {
		sprintf(tempChr, "00%d", playerObj->items.bolts);
	} else if (playerObj->items.bolts < 100) {
		sprintf(tempChr, "0%d", playerObj->items.bolts);
	} else {
		sprintf(tempChr, "%d", playerObj->items.bolts);
	}
	draw_text(257, 178, tempChr, weapon_menu_surface); // lifes p1


	draw_horizontal_hp_bar(WPN_COLUMN_Y, 2, playerObj->HP, weapon_menu_surface);


	// weapons
	for (i=1; i<=5; i++) {
		if (save_001.stages[i]) {
			draw_horizontal_hp_bar(WPN_COLUMN_Y+i*WEAPON_SPACING, 2, playerObj->items.weapons[i], weapon_menu_surface);
		}
	}


	// place weapons icons
	sprintf(buffer, "%sdata/images/icons.png", FILEPATH);
	icons = surface_from_image(buffer);

	copy_area(icons, 0,  0, 14, 14, WPN_COLUMN1_X, 30, weapon_menu_surface); // normal weapon P1
	int y_pos = 46;

	if (save_001.stages[APEBOT]) {
		copy_area(icons, 14, 0, 14, 14, WPN_COLUMN1_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+1*WEAPON_SPACING, 2, playerObj->items.weapons[APEBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[DAISIEBOT]) {
		copy_area(icons, 28, 0, 14, 14, WPN_COLUMN1_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+2*WEAPON_SPACING, 2, playerObj->items.weapons[DAISIEBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[DYNAMITEBOT]) {
		copy_area(icons, 42, 0, 14, 14, WPN_COLUMN1_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+3*WEAPON_SPACING, 2, playerObj->items.weapons[DYNAMITEBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[MUMMYBOT]) {
		copy_area(icons, 56, 0, 14, 14, WPN_COLUMN1_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+4*WEAPON_SPACING, 2, playerObj->items.weapons[MUMMYBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[SPIKEBOT]) {
		copy_area(icons, 70, 0, 14, 14, WPN_COLUMN1_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+5*WEAPON_SPACING, 2, playerObj->items.weapons[SPIKEBOT], weapon_menu_surface);
	}

	y_pos = 46;

	if (save_001.stages[TECHNOBOT]) {
		copy_area(icons, 84, 0, 14, 14, WPN_COLUMN2_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+1*WEAPON_SPACING, 3, playerObj->items.weapons[TECHNOBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[MAGEBOT]) {
		copy_area(icons, 98, 0, 14, 14, WPN_COLUMN2_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+2*WEAPON_SPACING, 3, playerObj->items.weapons[MAGEBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	if (save_001.stages[SEAHORSEBOT]) {
		copy_area(icons, 112, 0, 14, 14, WPN_COLUMN2_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+3*WEAPON_SPACING, 3, playerObj->items.weapons[SEAHORSEBOT], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	// COIL
	if (save_001.stages[APEBOT]) {
		copy_area(icons, 126, 0, 14, 14, WPN_COLUMN2_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+4*WEAPON_SPACING, 3, playerObj->items.weapons[9], weapon_menu_surface);
	}
	y_pos += WEAPON_SPACING;

	// JET
	if (save_001.stages[MAGEBOT]) {
		copy_area(icons, 140, 0, 14, 14, WPN_COLUMN2_X, y_pos, weapon_menu_surface);
		draw_horizontal_hp_bar(WPN_COLUMN_Y+5*WEAPON_SPACING, 3, playerObj->items.weapons[10], weapon_menu_surface);
	}

	SDL_FreeSurface(icons);

	drawInGameMenuSelection(p1Sel, player_n);
	copy_area(weapon_menu_surface, 0, 0, weapon_menu_surface->w, weapon_menu_surface->h, menu_pos_x, menu_pos_y, game_screen);

	updateScreen(game_screen);
	waitTime(500);

	while (!fim) {
		read_input();
		if (keyState[player_n][KEYJUMP] == 1) {
			fim = 1;
		} else if (keyState[player_n][KEYY] == 1) {
			// player 1 down
			drawInGameMenuSelection(p1Sel, 3);
			weaponmenu_move_cursor(&p1Sel, playerObj, 0, 1);
			drawInGameMenuSelection(p1Sel, player_n);
			clean_input(1);
			waitTime(120);
			updateScreen(game_screen);
		} else if (keyState[player_n][KEYY] == 2) {
			// player 1 up
			drawInGameMenuSelection(p1Sel, 3);
			weaponmenu_move_cursor(&p1Sel, playerObj, 0, -1);
			drawInGameMenuSelection(p1Sel, player_n);
			clean_input(1);
			waitTime(120);
			updateScreen(game_screen);
		} else if (keyState[player_n][KEYX] == 1) {
			// player 1 right
			drawInGameMenuSelection(p1Sel, 3);
			weaponmenu_move_cursor(&p1Sel, playerObj, 1, 0);
			drawInGameMenuSelection(p1Sel, player_n);
			clean_input(1);
			waitTime(120);
			updateScreen(game_screen);
		} else if (keyState[player_n][KEYX] == 2) {
			// player 1 left
			drawInGameMenuSelection(p1Sel, 3);
			weaponmenu_move_cursor(&p1Sel, playerObj, -1, 0);
			drawInGameMenuSelection(p1Sel, player_n);
			clean_input(1);
			waitTime(120);
			updateScreen(game_screen);
		} else if (keyState[player_n][KEYSTART] == 1) {
			if (p1Sel.y <= 5) {
				fim = 1;
			} else if (p1Sel.y == 6) {
				useEnergyTank(playerObj);
			} else if (p1Sel.y == 7) {
				useWeaponsTank(playerObj);
			}
		}
		waitTime(80);
		clean_input(1);
		copy_area(weapon_menu_surface, 0, 0, weapon_menu_surface->w, weapon_menu_surface->h, menu_pos_x, menu_pos_y, game_screen);
		updateScreen(game_screen);
	}

	int select_n = selectedWeaponNumber(p1Sel);

	// remove item if changed item
	if (playerObj->platform != NULL && playerObj->platform->type == OBJ_ITEM_FLY && select_n != ITEM_JET) {
		playerObj->platform->type = -1;
		playerObj->platform = NULL;
	}
	if (playerObj->platform != NULL && playerObj->platform->type == OBJ_ITEM_JUMP && select_n != ITEM_COIL) {
		playerObj->platform->type = -1;
		playerObj->platform = NULL;
	}

	setPlayerColor(select_n, player_n);
	waitTime(200);
	updateScreen(game_screen);
	PAUSED = 0;
	clean_input(1);
	waitTime(250);
}

void createDialogBG() {
	SDL_Surface *temp_img;
	char buffer_file[255];

	sprintf(buffer_file, "%sdata/images/backgrounds/menu.png", FILEPATH);
	temp_img = loadSpriteFile(buffer_file);
	SDL_SetColors(temp_img, colormap, 0, COLOR_COUNT);
	dialog_bg = SDL_CreateRGBSurface(SDL_SWSURFACE , temp_img->w, 50, 8, 0, 0, 0, 0);
	SDL_SetColors(dialog_bg, colormap, 0, COLOR_COUNT);
	copy_area(temp_img, 0, 0, temp_img->w, 25, 0, 0, dialog_bg);
	copy_area(temp_img, 0, temp_img->h-25, temp_img->w, 25, 0, 25, dialog_bg);
	SDL_FreeSurface(temp_img);
}


void resetDialog() {
	clear_area(dialog_bg, 5, 5, dialog_bg->w-10, dialog_bg->h-10, 0, 0, 0);
}

void removeDialog() {

}

void showCredits() {
    config_showMenu();
    draw_text(40, 30, "Rockbot 0.1", game_screen);
    draw_text(40, 40, "Code Licensed under GPL v2", game_screen);
    draw_text(40, 50, "Images C. Commons Share Alike", game_screen);
    draw_text(40, 60, "Music under several licenses", game_screen);
    draw_text(40, 80, "Lead Game Developer:", game_screen);
    draw_text(42, 90, "Iuri Fiedoruk", game_screen);
    draw_text(40, 100, "Lead Design Developer:", game_screen);
    draw_text(42, 110, "Arismeire K. S. Fiedoruk", game_screen);
    draw_text(40, 120, "Helper Coders:", game_screen);
    draw_text(42, 130, "Demetrio Neto", game_screen);
    draw_text(40, 140, "Helper Artists:", game_screen);
	draw_text(42, 150, "Rodrigo Hahn, Akira Faganello", game_screen);
	clean_input(1);
    waitTime(300);
    while (!fim) {
		read_input();
		if (keyState[0][KEYJUMP] == 1 || keyState[1][KEYJUMP] == 1) {
                fim = 1;
		} else if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) {
				fim = 1;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
				fim = 1;
        }
    }
	clean_input(1);
    waitTime(400);
}


void draw_lights_select_player(SDL_Surface* lights, int mode, int adjustX, int adjustY) {
	int posX, invPosX;

	posX = 6;
	invPosX = 0;

	//printf(">>draw_lights_select_player - game_config.selected_player: %d, adjustX: %d, adjustY: %d\n", game_config.selected_player, adjustX, adjustY);

	if (game_config.selected_player == 2) {
		// light for p1
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+2, adjustY+2, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+2, adjustY+88, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+248, adjustY+2, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+248, adjustY+88, game_screen);
		// black for p2
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+2, adjustY+98, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+2, adjustY+184, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+248, adjustY+98, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+248, adjustY+184, game_screen);
	} else {
		// light for p2
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+2, adjustY+98, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+2, adjustY+184, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+248, adjustY+98, game_screen);
		copy_area(lights, posX, 0, lights->h, lights->h, adjustX+248, adjustY+184, game_screen);
		// black for p1
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+2, adjustY+2, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+2, adjustY+88, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+248, adjustY+2, game_screen);
		copy_area(lights, invPosX, 0, lights->h, lights->h, adjustX+248, adjustY+88, game_screen);
	}

	updateScreen(game_screen);
}

void select_player() {
	int adjustX, adjustY;


	blank_screen(game_screen);
	adjustX = (RES_W-INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->w)*0.5;
	adjustY = (RES_H-INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->h)*0.5;
	copy_area(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG], 0, 0, INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->w, INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->h, adjustX, adjustY, game_screen);
	updateScreen(game_screen);

	clean_input(1);
	waitTime(200);
	while (keyState[0][KEYSTART] != 1 && keyState[1][KEYSTART] != 1) {
		read_input();
		if (game_config.selected_player == 2 && (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1)) {
			play_sfx(SFX_CURSOR);
			game_config.selected_player = 1;
		} else if (game_config.selected_player == 1 && (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2)) {
			play_sfx(SFX_CURSOR);
			game_config.selected_player = 2;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
		}
		draw_lights_select_player(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_LIGHTS], 0, adjustX, adjustY);
	}
	printf(">> select_player.END, n: %d\n", game_config.selected_player);
}


#endif // CONFIG_H


