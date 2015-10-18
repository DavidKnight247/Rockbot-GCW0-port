struct pos pos;
struct pos star_list[INTRO_STARS_NUMBER], small_star_list[INTRO_STARS_NUMBER];
int run_stars_animation=1;
int timer_intro;

void stop_music();




// TODO - convert text into an array to simplfy cut
void main_intro() {
	int cut=0;
	struct struct_colorcycle_list *cl_item;

	change_colormap(game_screen, 0, 0, 0, 0);



	blank_screen(game_screen);



	// do not cut here because we need to load the music
	draw_progressive_text(MAX_W*0.5-45-RES_DIFF_W, MAX_H*0.5-15,"20XX AD.", game_screen, 0);
	waitScapeTime(2000);


	play_music();



	//add colorcycling
	// green
	colorcycle_n = SDL_MapRGB(game_screen->format, 55, 255, 0);
	cl_item = add_colorcycle_list(colorcycle_n);
	add_colorcycle(cl_item, 100, 219, 43, 0); // red-clear
	add_colorcycle(cl_item, 4400, 219, 43, 0); // red-clear
	add_colorcycle(cl_item, 4400, 167, 0, 0); // red
	add_colorcycle(cl_item, 4400, 127, 11, 0); // red-dark
	add_colorcycle(cl_item, 4400, 27, 63, 95); // blue
	add_colorcycle(cl_item, 9999, 0, 0, 0); // black

	// cyan
	colorcycle_n = SDL_MapRGB(game_screen->format, 255, 0, 255);
	cl_item = add_colorcycle_list(colorcycle_n);
	add_colorcycle(cl_item, 100, 255, 119, 99); // light-red
	add_colorcycle(cl_item, 4400, 255, 119, 99); // light-red
	add_colorcycle(cl_item, 4400, 219, 43, 0); // red-clear
	add_colorcycle(cl_item, 4400, 167, 0, 0); // red
	add_colorcycle(cl_item, 4400, 127, 11, 0); // red-dark
	add_colorcycle(cl_item, 9999, 27, 63, 95); // blue

	// cyan
	colorcycle_n = SDL_MapRGB(game_screen->format, 0, 255, 255);
	cl_item = add_colorcycle_list(colorcycle_n);
	add_colorcycle(cl_item, 100, 243, 191, 63); // light-red
	add_colorcycle(cl_item, 4400, 243, 191, 63); // light-red
	add_colorcycle(cl_item, 4400, 255, 155, 59); // red-clear
	add_colorcycle(cl_item, 4400, 255, 102, 0); // red
	add_colorcycle(cl_item, 4400, 203, 79, 15); // red-dark
	add_colorcycle(cl_item, 9999, 81, 81, 81); // blue


	blank_screen(game_screen);
	copy_area(INTRO_SURFACES[INTRO_SURFACES_CITY_BG], 0, 0, INTRO_SURFACES[INTRO_SURFACES_CITY_BG]->w, INTRO_SURFACES[INTRO_SURFACES_CITY_BG]->h, 10-RES_DIFF_W, 10-RES_DIFF_H, game_screen);


	cut = draw_progressive_text(10, 120,"AFTER T.LIGHT CREATED THE FIRST", game_screen, 1);

	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 135,"HUMANOID ROBOT, ARTIFICIAL BEINGS", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 150,"STARTED TO TAKE WORK FROM HUMANS.", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(500);
	if (cut) { return; }

	blank_area(game_screen, 0, 120, MAX_W, 100);

	cut = draw_progressive_text(10, 120,"USING THE CHAOS, A ENEMY CALLED", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 135,"WILY ATTACKED TOKYO.", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	blank_area(game_screen, 0, 120, MAX_W, 100);

	cut = draw_progressive_text(10, 120,"HE CREATED AN ARMY OF ROBOTS,", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 135,"AND WAS DEFEATED, SEVERAL TIMES", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 150,"BY A ROBOT CALLED ROCKMAN.", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	blank_area(game_screen, 0, 120, MAX_W, 100);

	freeColorCycling();


	blank_screen(game_screen);


	copy_area(INTRO_SURFACES[INTRO_SURFACES_LAB_BG], 0, 0, INTRO_SURFACES[INTRO_SURFACES_LAB_BG]->w, INTRO_SURFACES[INTRO_SURFACES_LAB_BG]->h, 10-RES_DIFF_W, 10, game_screen);

	copy_area(INTRO_SURFACES[INTRO_SURFACES_KANOTUS], 0, 0, 21, 24, 226-RES_DIFF_W, 82, game_screen);
	updateScreen(game_screen);

	//add colorcycling
	colorcycle_n = SDL_MapRGB(game_screen->format, 55, 255, 0);

	cl_item = add_colorcycle_list(colorcycle_n);

	add_colorcycle(cl_item, 1300, 0, 151, 0); // verde
	add_colorcycle(cl_item, 1600, 219, 43, 0); // vermelho
	add_colorcycle(cl_item, 1400, 0, 0, 171); // azul
	// second
	colorcycle_n = SDL_MapRGB(game_screen->format, 255, 0, 255);
	cl_item = add_colorcycle_list(colorcycle_n);
	add_colorcycle(cl_item, 1800, 143, 0, 119); // roxo
	add_colorcycle(cl_item, 1200, 255, 102, 0); // laranja
	add_colorcycle(cl_item, 1600, 230, 255, 0); // amarelo
	// third
	colorcycle_n = SDL_MapRGB(game_screen->format, 0, 255, 255);
	cl_item = add_colorcycle_list(colorcycle_n);
	add_colorcycle(cl_item, 1200, 255, 219, 171); // bege
	add_colorcycle(cl_item, 1600, 255, 155, 59); // laranja
	add_colorcycle(cl_item, 1800, 219, 43, 0); // marrom

	cut = draw_progressive_text(10, 120,"A YOUNG SCIENTIST IN KYOTO,", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 135,"CALLED KANOTUS, ALSO CREATED", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 150,"TWO ROBOT-HEROES TO FIGHT EVIL,", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(500);
	if (cut) { return; }

	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_KANOTUS], colormap, 0, COLOR_COUNT);
	copy_area(INTRO_SURFACES[INTRO_SURFACES_KANOTUS], 43, 0, 21, INTRO_SURFACES[INTRO_SURFACES_KANOTUS]->h, 226-RES_DIFF_W, 82, game_screen);

	cut = draw_progressive_text(10, 165,"RIGHT BEFORE AN HORDE OF ROBOTS,", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 180,"ATTACKED, DEFEATING CITY FORCES.", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(500);
	if (cut) { return; }
	blank_area(game_screen, 0, 120, MAX_W, 100);

	copy_area(INTRO_SURFACES[INTRO_SURFACES_BETABOT], 88, 9, 19,24, 158-RES_DIFF_W, 83, game_screen);

	cut = draw_progressive_text(10, 120,"NOW ROCKBOT AND BETABOT ARE THE", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	cut = draw_progressive_text(10, 135,"ONLY HOPE TO PROTECT THE CITY", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(100);
	if (cut) { return; }

	copy_area(INTRO_SURFACES[INTRO_SURFACES_ROCKBOT], 88, 9, 19, 24, 34-RES_DIFF_W, 83, game_screen);

	cut = draw_progressive_text(10, 150,"FROM THE RAGE OF ROBOTS!", game_screen, 1);
	if (cut) { return; }
	cut = waitScapeTime(1000);
	if (cut) { return; }
}

void select_player_number() {
	if (game_config.two_players == 1) {
		draw_text(23, MAX_H*0.5,">", game_screen);
	} else if (game_config.two_players == 2) {
		draw_text(23, (MAX_H*0.5)+13,">", game_screen);
	}
	updateScreen(game_screen);
	clean_input(1);
	waitTime(200);

	while (keyState[0][KEYSTART] != 1 && keyState[1][KEYSTART] != 1) {
		clean_input(1);
		read_input();
		if (game_config.two_players == 1 && (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1)) {
			//printf(">> intro.h::select_player_number - cursor DOWN - y[1]: %d, y[2]: %d\n", keyState[0][KEYY], keyState[1][KEYY]);
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 23, MAX_H*0.5, 8, 10, 0, 0, 0);
			draw_text(23, (MAX_H*0.5)+13,">", game_screen);
			game_config.two_players = 2;
			clean_input(1);
			waitTime(50);
			updateScreen(game_screen);
		} else if (game_config.two_players == 2 && (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2)) {
			//printf(">> intro.h::select_player_number - cursor UP - y[1]: %d, y[2]: %d\n", keyState[0][KEYY], keyState[1][KEYY]);
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 23, (MAX_H*0.5)+13, 8, 10, 0, 0, 0);
			draw_text(23, MAX_H*0.5,">", game_screen);
			game_config.two_players = 1;
			clean_input(1);
			waitTime(50);
			updateScreen(game_screen);
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
		}
	}
}


void select_load_game() {
	FILE* fp;
	int option;
	char tempBuffer[150];

	sprintf(tempBuffer, "%sdata/001.sav", FILEPATH);
	fp = fopen(tempBuffer, "rb");

	if (fp) {
		option = 2;
	} else {
		option = 1;
	}

	//printf(">> select_load_game::START\n");
	clear_area(game_screen, 30, MAX_H*0.5, 100, 46, 0, 0, 0);
	draw_text(30-RES_DIFF_W, (MAX_H*0.5)-RES_DIFF_H,"New Game", game_screen);
	draw_text(30-RES_DIFF_W, (MAX_H*0.5)+13-RES_DIFF_H,"Load Game", game_screen);
	if (option == 1) {
		draw_text(23, MAX_H*0.5,">", game_screen);
	} else {
		draw_text(23, (MAX_H*0.5)+13,">", game_screen);
	}


	updateScreen(game_screen);
	clean_input(1);
	waitTime(500);


	while (keyState[0][KEYSTART] != 1 && keyState[1][KEYSTART] != 1 && keyState[0][KEYJUMP] != 1 && keyState[1][KEYJUMP] != 1) {
		clean_input(1);
		read_input();
		waitTime(20);
		if (option == 1 && (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1)) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 23, MAX_H*0.5, 8, 10, 0, 0, 0);
			draw_text(23, (MAX_H*0.5)+13,">", game_screen);
			option = 2;
		} else if (option == 2 && (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2)) {
			play_sfx(SFX_CURSOR);
			clear_area(game_screen, 23, (MAX_H*0.5)+13, 8, 10, 0, 0, 0);
			draw_text(23, MAX_H*0.5,">", game_screen);
			option = 1;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			if (fp) {
				fclose(fp);
			}
			leave_game();
		}
		updateScreen(game_screen);
	}
	clean_input(1);
	read_input(); // to check released buttons

	clean_input(1);
	waitTime(200);
	if (option == 2) {
		fread (&save_001, sizeof(struct st_save), 1, fp);
	}
	if (fp) {
		fclose(fp);
	}
}


void move_highlight(int x_inc, int y_inc, SDL_Surface *s_light, SDL_Surface *s_dark, struct pos *highlight_pos) {
	copy_area(s_dark, 0, 0, s_dark->w, s_dark->h, highlight_pos->x, highlight_pos->y, game_screen);
	highlight_pos->x += x_inc;
	highlight_pos->y += y_inc;
	copy_area(s_light, 0, 0, s_light->w, s_light->h, highlight_pos->x, highlight_pos->y, game_screen);
	updateScreen(game_screen);
}




void place_face(int face_n, char botname[20], short int posx, short int posy) {
    // TODO - calculate the text position according to it's strlen
    int txtPosX, txtPosY;

	if (face_n != -1) {
		copy_area(FACES_SURFACES[face_n], 0, 0, FACES_SURFACES[face_n]->w, FACES_SURFACES[face_n]->h, posx*80+63-RES_DIFF_W, posy*60+26-RES_DIFF_H, game_screen);
	}
    txtPosX = posx*80+76-RES_DIFF_W-(strlen(botname)*7)*0.5;
    txtPosY = posy*60+66-RES_DIFF_H;
    draw_text(txtPosX, txtPosY, botname, game_screen);
}


int animate_highlight(SDL_Surface *white, SDL_Surface *current) {
    if (getTimer() > timer_intro) {
        timer_intro = getTimer()+200;
        if (current == white) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (current == white) {
            return 0;
        } else {
            return 1;
        }
    }
}

void draw_eyes(int x, int y, SDL_Surface *eyes) {
    int posX, posY;
    if (x == 1) {
        posX = 11;
    } else if (x == 0) {
        posX = 9;
    } else {
        posX = 13;
    }
    if (y == 1) {
		posY = 18;
    } else if (y == 0) {
		posY = 16;
    } else {
		posY = 20;
    }
	if (game_config.selected_player == 2) {
		posY -= 3;
	}
    posX = posX+80+63-RES_DIFF_W;
    posY = posY+60+26-RES_DIFF_H;
    copy_area(eyes, 0, 0, eyes->w, eyes->h, posX, posY, game_screen);
}

struct pos stage_select() {
	char buffer [255];
	SDL_Surface *spriteCopy, *s_light=NULL, *s_dark, *eyes_surface, *eyes_bg, *light_mode;
	struct pos spacing, highlight_pos;
	int selection_end=0;

	load_music("menu.mod");
	play_music();
	clean_input(1);
	waitTime(300);

	pos.x = 1;
	pos.y = 1;
	spacing.x = 80;
	spacing.y = 60;
	highlight_pos.x = 135-RES_DIFF_W;
	highlight_pos.y = 78-RES_DIFF_H;
	blank_screen(game_screen);
	light_mode = s_light;


	sprintf(buffer, "%sdata/images/backgrounds/stage_select.png", FILEPATH);
	spriteCopy = loadSpriteFile(buffer);
	copy_area(spriteCopy, 0, 0, MAX_W, spriteCopy->w, -RES_DIFF_W, -RES_DIFF_H, game_screen);


	if (save_001.stages[APEBOT] == 0) {
		place_face(FACES_APEBOT, "APE", 0, 0);
	} else {
		place_face(-1, "APE", 0, 0);
	}

	if (save_001.stages[DAISIEBOT] == 0) {
		place_face(FACES_DAISIEBOT, "DAISIE", 1, 0);
	} else {
		place_face(-1, "DAISIE", 1, 0);
	}

	if (save_001.stages[SEAHORSEBOT] == 0) {
		place_face(FACES_SEAHORSEBOT, "SEAHORSE", 2, 0);
	} else {
		place_face(-1, "SEAHORSE", 2, 0);
	}

	if (save_001.stages[MUMMYBOT] == 0) {
		place_face(FACES_MUMMYBOT, "MUMMY", 0, 1);
	} else {
		place_face(-1, "MUMMY", 0, 1);
	}

	if (save_001.stages[MAGEBOT] == 0) {
		place_face(FACES_MAGEBOT, "MAGE", 2, 1);
	} else {
		place_face(-1, "MAGE", 2, 1);
	}

	if (save_001.stages[DYNAMITEBOT] == 0) {
		place_face(FACES_DYNAMITEBOT, "DYNAMITE", 0, 2);
	} else {
		place_face(-1, "DYNAMITE", 0, 2);
	}

	if (save_001.stages[SPIKEBOT] == 0) {
		place_face(FACES_SPIKEBOT, "SPIKE", 1, 2);
	} else {
		place_face(-1, "SPIKE", 1, 2);
	}

	if (save_001.stages[TECHNOBOT] == 0) {
		place_face(FACES_TECHNOBOT, "TECHNO", 2, 2);
	} else {
		place_face(-1, "TECHNO", 2, 2);
	}

	if (save_001.finished_stages < 8) {
		if (game_config.selected_player == 1) {
			place_face(FACES_ROCKBOT_NOEYES, "", 1, 1);
		} else {
			place_face(FACES_BETABOT_NOEYES, "", 1, 1);
		}
	} else {
		place_face(FACES_DR_DESTRIN, "Dr. D.", 1, 1);
	}

	sprintf(buffer, "%sdata/images/backgrounds/stage_select_highlighted.png", FILEPATH);
	s_light = loadSpriteFile(buffer);
	SDL_SetColors(s_light, colormap, 0, COLOR_COUNT);
	sprintf(buffer, "%sdata/images/backgrounds/stage_select_darkned.png", FILEPATH);
	s_dark = loadSpriteFile(buffer);
	SDL_SetColors(s_dark, colormap, 0, COLOR_COUNT);
	light_mode = s_light;

	if (game_config.selected_player == 1) {
		sprintf(buffer, "%sdata/images/faces/rockbot_eyes.png", FILEPATH);
	} else {
		sprintf(buffer, "%sdata/images/faces/betabot_eyes.png", FILEPATH);
	}
	eyes_surface = loadSpriteFile(buffer);
	eyes_bg = SDL_CreateRGBSurface(SDL_SWSURFACE , 32, 32, 8, 0, 0, 0, 0);
	SDL_SetColors(eyes_bg, colormap, 0, COLOR_COUNT);
	copy_area(game_screen, (80+63-RES_DIFF_W), (60+26-RES_DIFF_H), 32, 32, 0, 0, eyes_bg);

	copy_area(s_light, 0, 0, s_light->w, s_light->h, highlight_pos.x, highlight_pos.y, game_screen);
	updateScreen(game_screen);
	draw_text(MAX_W*0.5-45-RES_DIFF_W, 5-RES_DIFF_H,"PRESS START", game_screen);

	clean_input(1);
	printf(">>> save_001.finished_stages: %d\n", save_001.finished_stages);
	if (save_001.finished_stages < 8) {
		draw_eyes(pos.x, pos.y, eyes_surface);
	}
	waitTime(200);

	while (selection_end == 0) {
		read_input();
		//printf(">> intro.h::stage_select - pos.x: %d, pos.y: %d\n", pos.x, pos.y);

		if (pos.y < 2 && (keyState[0][KEYY] == 1 || keyState[1][KEYY] == 1)) {
					copy_area(eyes_bg, 0, 0, 32, 32, 80+63-RES_DIFF_W, 60+26-RES_DIFF_H, game_screen);
					pos.y++;
					if (save_001.finished_stages < 8) {
						draw_eyes(pos.x, pos.y, eyes_surface);
					}
					play_sfx(SFX_CURSOR);
					move_highlight(0, spacing.y, s_light, s_dark, &highlight_pos);
					waitTime(200);
					clean_input(1);
		} else if (pos.y > 0 && (keyState[0][KEYY] == 2 || keyState[1][KEYY] == 2)) {
					copy_area(eyes_bg, 0, 0, 32, 32, 80+63-RES_DIFF_W, 60+26-RES_DIFF_H, game_screen);
					pos.y--;
					if (save_001.finished_stages < 8) {
						draw_eyes(pos.x, pos.y, eyes_surface);
					}
					play_sfx(SFX_CURSOR);
					move_highlight(0, -spacing.y, s_light, s_dark, &highlight_pos);
					waitTime(200);
					clean_input(1);
		} else if (pos.x > 0 && (keyState[0][KEYX] == 2 || keyState[1][KEYX] == 2)) {
					copy_area(eyes_bg, 0, 0, 32, 32, 80+63-RES_DIFF_W, 60+26-RES_DIFF_H, game_screen);
					pos.x--;
					if (save_001.finished_stages < 8) {
						draw_eyes(pos.x, pos.y, eyes_surface);
					}
					play_sfx(SFX_CURSOR);
					move_highlight(-spacing.x, 0, s_light, s_dark, &highlight_pos);
					waitTime(200);
					clean_input(1);
		} else if (pos.x < 2 && (keyState[0][KEYX] == 1 || keyState[1][KEYX] == 1)) {
					copy_area(eyes_bg, 0, 0, 32, 32, 80+63-RES_DIFF_W, 60+26-RES_DIFF_H, game_screen);
					pos.x++;
					if (save_001.finished_stages < 8) {
						draw_eyes(pos.x, pos.y, eyes_surface);
					}
					play_sfx(SFX_CURSOR);
					move_highlight(spacing.x, 0, s_light, s_dark, &highlight_pos);
					waitTime(200);
					clean_input(1);
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
		} else if ((keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) && save_001.finished_stages < 8 && (pos.x != 1 || pos.y != 1)) {
			selection_end = 1;
		} else if ((keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) && save_001.finished_stages >= 8) {
			selection_end = 1;
		}
		if (animate_highlight(s_light, light_mode) == 0) {
			light_mode = s_light;
		} else {
			light_mode = s_dark;
		}
		move_highlight(0, 0, light_mode, s_dark, &highlight_pos);
	}
	SDL_FreeSurface(s_light);
	SDL_FreeSurface(s_dark);
	SDL_FreeSurface(eyes_surface);
	SDL_FreeSurface(eyes_bg);
	light_mode = NULL;
	// do fade

	blink_screen(255, 255, 255);
	SDL_FreeSurface(spriteCopy);
	printf(">> stage_select - pos;x: %d, pos.y: %d\n", pos.x, pos.y);
	return pos;
}

void drawStar(short int x, short int y, short int is_big) {
	if (is_big) {
		if (surface_star_big == NULL) {
			surface_star_big = SDL_CreateRGBSurface(SDL_SWSURFACE, 3, 3, 8, 188, 0, 188, 0);
			SDL_SetColors(surface_star_big, colormap, 0, COLOR_COUNT);
			SDL_FillRect(surface_star_big, 0, SDL_MapRGB(game_screen->format, 188, 188, 188));
		}
		copy_area(surface_star_big, 0, 0, surface_star_big->w, surface_star_big->h, x, y, game_screen);
	} else {
		if (surface_star_small == NULL) {
			surface_star_small = SDL_CreateRGBSurface(SDL_SWSURFACE, 2, 2, 8, 0, 235, 235, 0);
			SDL_SetColors(surface_star_small, colormap, 0, COLOR_COUNT);
			SDL_FillRect(surface_star_small, 0, SDL_MapRGB(game_screen->format, 235, 235, 235));
		}
		copy_area(surface_star_small, 0, 0, surface_star_small->w, surface_star_small->h, x, y, game_screen);
	}
}

void eraseStar(short int x, short int y, short int is_big) {
	if (is_big) {
		if (!surface_star_bg_big) {
			surface_star_bg_big = SDL_CreateRGBSurface(SDL_SWSURFACE, 3, 3, 8, 255, 0, 0, 0);
			SDL_SetColors(surface_star_bg_big, colormap, 0, COLOR_COUNT);
			SDL_FillRect(surface_star_bg_big, 0, SDL_MapRGB(game_screen->format, 0, 0, 0));
		}
		copy_area(surface_star_bg_big, 0, 0, 3, 3, x, y, game_screen);
	} else {
		if (!surface_star_bg_small) {
			surface_star_bg_small = SDL_CreateRGBSurface(SDL_SWSURFACE, 2, 2, 8, 0, 255, 0, 0);
			SDL_SetColors(surface_star_bg_small, colormap, 0, COLOR_COUNT);
			SDL_FillRect(surface_star_bg_small, 0, SDL_MapRGB(game_screen->format, 0, 0, 0));
		}
		copy_area(surface_star_bg_small, 0, 0, 2, 2, x, y, game_screen);
	}
}


// random a position for each star
void initStars() {
	int i;
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		star_list[i].x = rand() % 320;
		if (i % 2 == 0) {
			star_list[i].y = rand() % 46;
		} else {
			star_list[i].y = rand() % 53 + 145;
		}
	}
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		small_star_list[i].x = rand() % 320;
		if (i % 2 == 0) {
			small_star_list[i].y = rand() % 46;
		} else {
			small_star_list[i].y = rand() % 53 + 145;
		}
	}
}



void waitTimeWithStars(int wait_period) {
	int now_time=0;
	now_time = getTimer();
	wait_period = now_time + wait_period;
	while (now_time < wait_period) {
		now_time = getTimer();
		#ifdef PC
			waitTime(1);
		#endif
		animStars();
	}
}


void animStars() {
	int i;
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		eraseStar(star_list[i].x, star_list[i].y, 1);
		eraseStar(small_star_list[i].x, small_star_list[i].y, 0);
		star_list[i].x -= ANIM_STARS_STEP;
		if (star_list[i].x <= 0) {
			star_list[i].x = 320+ANIM_STARS_STEP;
			if (i % 2 == 0) {
				star_list[i].y = rand() % 46;
			} else {
				star_list[i].y = rand() % 53 + 145;
			}
		}
		small_star_list[i].x -= ANIM_STARS_SMALL_STEP;
		if (small_star_list[i].x <= 0) {
			small_star_list[i].x = 320+ANIM_STARS_SMALL_STEP;
			if (i % 2 == 0) {
				small_star_list[i].y = rand() % 46;
			} else {
				small_star_list[i].y = rand() % 53 + 145;
			}
		}
	}
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		drawStar(star_list[i].x, star_list[i].y, 1);
		drawStar(small_star_list[i].x, small_star_list[i].y, 0);
	}
	updateScreen(game_screen);
}

void boss_intro(struct pos pos) {
	char buffer [255], letter[2], botname[50];
	SDL_Surface *spriteCopy, *boss_bg, *bgCopy;
	struct pos boss_pos, sprite_size;
	int intro_frames_n = 1, intro_frames_rollback=0;
	boss_pos.x = 20;
	boss_pos.y = -37;
	int text_x = MAX_W*0.5 - 60;
	int i;
	int ship_x;
	int dest_x, dest_y, dest_w, dest_h;

	int pos_n = (pos.x + pos.y*3)+1;
	printf("*** boss_intro - START - pos.x: %d, pos.y: %d, pos: %d ***\n", pos.x, pos.y, pos_n);

	if (pos_n == SKULLCASTLE1) {
		// show dr. destrin scaping in the capsule
		blank_screen(game_screen);
		sprintf(buffer, "%sdata/images/sprites/enemies/destrin_capsule_small.png", FILEPATH);
		spriteCopy = loadSpriteFile(buffer);
		ship_x = RES_W + TILE_SIZE;

		play_timed_sfx(SFX_DESTRIN_SHIP, 8000);

		dest_w = spriteCopy->w/3;
		dest_h = spriteCopy->h;
		dest_y = RES_H*0.5;
		bgCopy = SDL_CreateRGBSurface(SDL_SWSURFACE , dest_w, dest_h, 8, 0, 0, 0, 0);
		SDL_SetColors(bgCopy, colormap, 0, COLOR_COUNT);
		copy_area(game_screen, 0, 0, dest_w, dest_h, 0, 0, bgCopy);

		// part 1 - shi moves from right to screen center
		while (ship_x > RES_W*0.5 - spriteCopy->h/2) {
			dest_x = ship_x + MOVE_STEP;
			copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
			copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(10);
			ship_x -= MOVE_STEP;
		}
		// part 2 - show ship without cover
		waitTime(10);
		dest_x = ship_x += MOVE_STEP;
		copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
		copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
		updateScreen(game_screen);
		waitTime(800);
		copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
		// part 3 - show eyebrowns animation
		for (i=0; i<3; i++) {
			copy_area(spriteCopy, (spriteCopy->w/3)*2, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(200);
			copy_area(spriteCopy, spriteCopy->w/3, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(200);
		}
		updateScreen(game_screen);
		waitTime(1200);
		// part 4 - move ship from center to left
		while (ship_x > -spriteCopy->w) {
			copy_area(bgCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			ship_x -= MOVE_STEP;
			copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(10);
		}
		SDL_FreeSurface(spriteCopy);
		SDL_FreeSurface(bgCopy);


		sprintf(buffer, "%sdata/images/backgrounds/skull_castle.png", FILEPATH);
		spriteCopy = loadSpriteFile(buffer);
		copy_area(spriteCopy, 0, 0, spriteCopy->w, spriteCopy->h, 0, 0, game_screen);
		SDL_FreeSurface(spriteCopy);
		updateScreen(game_screen);
		play_sfx(SFX_SKULL_CASTLE_INTRO);
		waitTime(7550);
		return;
	}

	play_sfx(SFX_STAGE_SELECTED);
	blank_screen(game_screen);
	sprintf(buffer, "%sdata/images/backgrounds/stage_boss_intro.png", FILEPATH);
	spriteCopy = loadSpriteFile(buffer);
	copy_area(spriteCopy, 0, 0, spriteCopy->w, spriteCopy->h, 0, 0, game_screen);
	SDL_FreeSurface(spriteCopy);

	bgCopy = SDL_CreateRGBSurface(SDL_SWSURFACE , MAX_W, MAX_H, 8, 0, 0, 0, 0);
	SDL_SetColors(bgCopy, colormap, 0, COLOR_COUNT);
	copy_area(game_screen, 0, 0, game_screen->w, game_screen->h, 0, 0, bgCopy);
	updateScreen(game_screen);



	if (pos_n == SPIKEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/spikebot.png", FILEPATH);
		sprite_size.x = 38;
		sprite_size.y = 34;
		intro_frames_n = 3;
		sprintf(botname, "SPIKE BOT");
	} else if (pos_n == DAISIEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/daisiebot.png", FILEPATH);
		sprite_size.x = 36;
		sprite_size.y = 31;
		intro_frames_n = 3;
		sprintf(botname, "DAISIE BOT");
	} else if (pos_n == DYNAMITEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/dynamitebot.png", FILEPATH);
		sprite_size.x = 44;
		sprite_size.y = 48;
		intro_frames_n = 3;
		intro_frames_rollback = 1;
		sprintf(botname, "DYNAMITE BOT");
	} else if (pos_n == TECHNOBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/technobot.png", FILEPATH);
		sprite_size.x = 51;
		sprite_size.y = 47;
		intro_frames_n = 3;
		sprintf(botname, "TECHNO BOT");
		intro_frames_rollback = 1;
	} else if (pos_n == MUMMYBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/mummybot.png", FILEPATH);
		sprite_size.x = 33;
		sprite_size.y = 33;
		intro_frames_n = 3;
		sprintf(botname, "MUMMY BOT");
	} else if (pos_n == APEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/apebot.png", FILEPATH);
		sprite_size.x = 40;
		sprite_size.y = 39;
		intro_frames_n = 5;
		sprintf(botname, "APE BOT");
	} else if (pos_n == SEAHORSEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/seahorsebot.png", FILEPATH);
		sprite_size.x = 31;
		sprite_size.y = 38;
		intro_frames_n = 1;
		sprintf(botname, "SEAHORSE BOT");
	} else if (pos_n == MAGEBOT && save_001.stages[pos_n] == 0) {
		sprintf(buffer, "%sdata/images/sprites/enemies/magebot.png", FILEPATH);
		sprite_size.x = 26;
		sprite_size.y = 33;
		intro_frames_n = 4;
		sprintf(botname, "MAGE BOT");

	}

	spriteCopy = loadSpriteFile(buffer);
	boss_bg = SDL_CreateRGBSurface(SDL_SWSURFACE , sprite_size.x, sprite_size.y, 8, 0, 0, 0, 0);
	SDL_SetColors(boss_bg, colormap, 0, COLOR_COUNT);
	copy_area(bgCopy, boss_pos.x, 80, sprite_size.x, sprite_size.y, 0, 0, boss_bg);
	SDL_SetColors(boss_bg, colormap, 0, COLOR_COUNT);


	initStars();

	while (boss_pos.y < 80) {
		copy_area(boss_bg, 0, 0, sprite_size.x, sprite_size.y, boss_pos.x, boss_pos.y, game_screen);
		boss_pos.y += 4;
		copy_area(bgCopy, boss_pos.x, boss_pos.y, sprite_size.x, sprite_size.y, 0, 0, boss_bg);
		copy_area(spriteCopy, 0, 0, sprite_size.x, sprite_size.y, boss_pos.x, boss_pos.y, game_screen);
		updateScreen(game_screen);
		waitTimeWithStars(5);
	}
	waitTimeWithStars(500);
	if (intro_frames_n > 1) {
		for (i=0; i<intro_frames_n; i++) {
			copy_area(boss_bg, 0, 0, spriteCopy->w, spriteCopy->h, boss_pos.x, boss_pos.y, game_screen);
			copy_area(spriteCopy, i*sprite_size.x, 0, sprite_size.x, sprite_size.y, boss_pos.x, boss_pos.y, game_screen);
			waitTimeWithStars(200);
		}
		if (intro_frames_rollback == 1) {
			for (i=intro_frames_n-1; i>=0; i--) {
				copy_area(boss_bg, 0, 0, spriteCopy->w, spriteCopy->h, boss_pos.x, boss_pos.y, game_screen);
				copy_area(spriteCopy, i*sprite_size.x, 0, sprite_size.x, sprite_size.y, boss_pos.x, boss_pos.y, game_screen);
				waitTimeWithStars(200);
			}
		}
	}
	for (i=0; i<strlen(botname); i++) {
		sprintf(letter, "%c", botname[i]);
		waitTimeWithStars(100);
		draw_text(text_x, 118, letter, game_screen);
		text_x += 8;
	}
	waitTimeWithStars(2500);
	SDL_FreeSurface(bgCopy);
	SDL_FreeSurface(boss_bg);
	SDL_FreeSurface(spriteCopy);
	run_stars_animation=0;
}




void intro_presents() {
    char tempBuffer[255];

	clear_area(game_screen, 0, 0, RES_W, RES_H, 0, 0, 0);
	updateScreen(game_screen);

	copy_area(INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS], 0, 0, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->w, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->h, RES_W*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->w*0.5, RES_H*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->h*0.5, game_screen);
	updateScreen(game_screen);
	updateScreen(game_screen);
	waitScapeTime(1600);


	blank_screen(game_screen);
}


// mostra tela de introdução, até alguém apertar start/enter
// a partir daí, mostra tela de seleção de personagem
struct pos intro_screen() {
	char buffer [255];
	SDL_Surface *spriteCopy;


	blank_screen(game_screen);
	draw_text(MAX_W*0.5-45-RES_DIFF_W, MAX_H*0.5-15,"LOADING...", game_screen);
	updateScreen(game_screen);
	preload_intro();

	waitTime(200);

	intro_presents();

	waitTime(100);
	main_intro();
	freeColorCycling();

	waitTime(200);
	clean_input(1);

	blank_screen(game_screen);

	// PARTE 1 - TITLE SCREEN
	copy_area(INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN], 0, 0, MAX_W, INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN]->w, -RES_DIFF_W, -RES_DIFF_H, game_screen);

	draw_text(8, 50,"KYOTO EVOLUTION!!", game_screen);

	draw_text(40-RES_DIFF_W, (MAX_H*0.5)-RES_DIFF_H,"Game Start", game_screen);
	draw_text(40-RES_DIFF_W, (MAX_H*0.5+12)-RES_DIFF_H,"Options", game_screen);
	draw_text(40-RES_DIFF_W, (MAX_H*0.5+24)-RES_DIFF_H,"Credits", game_screen);
	draw_text(40-RES_DIFF_W, (MAX_H*0.5+36)-RES_DIFF_H,"Quit", game_screen);

	draw_text(40-RES_DIFF_W, (RES_H-30),"(C) 2009/2010 UPPERLAND STUDIO", game_screen);
	draw_text(40-RES_DIFF_W, (RES_H-20),"DESIGN GREENNEKO WEB&DESIGN", game_screen);
	draw_text(40-RES_DIFF_W, (RES_H-10),"AND OTHERS", game_screen);


	mainMenu();
	updateScreen(game_screen);


	select_load_game();


	#ifdef PLAYSTAION
		if (joystick2) {
			clear_area(game_screen, 20, MAX_H*0.5, 100, 46, 0, 0, 0);
			draw_text(30-RES_DIFF_W, (MAX_H*0.5)-RES_DIFF_H,"ONE PLAYER", game_screen);
			draw_text(30-RES_DIFF_W, (MAX_H*0.5)+13-RES_DIFF_H,"TWO PLAYERS", game_screen);
			updateScreen(game_screen);
			//printf("DEBUG.intro_screen - game_config.two_players: %d\n", game_config.two_players);
			select_player_number();
		}
	#else
		if (joystick1 || joystick2) {
			clear_area(game_screen, 20, MAX_H*0.5, 100, 46, 0, 0, 0);
			draw_text(30-RES_DIFF_W, (MAX_H*0.5)-RES_DIFF_H,"ONE PLAYER", game_screen);
			draw_text(30-RES_DIFF_W, (MAX_H*0.5)+13-RES_DIFF_H,"TWO PLAYERS", game_screen);
			//printf("DEBUG.intro_screen - game_config.two_players: %d\n", game_config.two_players);
			updateScreen(game_screen);
			select_player_number();
		}
	#endif


	if (game_config.two_players == 1) {
		printf(">> intro_screen.select_player START\n");
		select_player();
		printf(">> intro_screen.select_player END\n");
	}

	return pick_stage();
}

struct pos pick_stage() {
	//printf(">> pick_stage::START\n");

	blank_screen(game_screen);
	struct pos pos = stage_select();
	int pos_n = pos.x + 1 + pos.y*3;
	//printf(">> pick_stage - pos.x: %d, pos.y: %d, pos_n: %d, save_001.stages[pos_n]: %d\n", pos.x, pos.y, pos_n, save_001.stages[pos_n]);
	if (pos_n == SKULLCASTLE1 || save_001.stages[pos_n] == 0) {
		stop_music();
		boss_intro(pos);
	}
	clean_input(1);
	waitTime(100);
	return pos;
}

/*
struct text_list {
	char text[32];
	int posY;
	struct text_list *next;
};
*/

void add_credit_line(char line[32]) {
	struct text_list *new_text_list;
	struct text_list *temp_text_list;
	int posY = 12;

	new_text_list = (struct text_list*)malloc(sizeof(struct text_list));
	sprintf(new_text_list->text, "%s", line);
	new_text_list->next = NULL;


	if (credits_list == NULL) {
		credits_list = new_text_list;
	} else {
		temp_text_list = credits_list;
		while (temp_text_list->next) {
			temp_text_list = temp_text_list->next;
			posY += 12;
		}
		temp_text_list->next = new_text_list;
	}
	new_text_list->posY = posY;
}

void create_credits_list() {
	add_credit_line("- LEAD GAME DESIGNER -");
	add_credit_line("IURI FIEDORUK");
	add_credit_line(" ");

	add_credit_line("- CONCEPT & MARKETING ART -");
	add_credit_line("ARISMEIRE KUMMER SILVA FIEDORUK");
	add_credit_line(" ");

	add_credit_line("- PIXEL ART -");
	add_credit_line("GIOVANI AKIRA FAGANELLO");
	add_credit_line("RODRIGO M. HAHN");
	add_credit_line(" ");

	add_credit_line("- PROGRAMMING -");
	add_credit_line("IURI FIEDORUK");
	add_credit_line(" ");

	add_credit_line("- ASSISTING DEVELOPMENT -");
	add_credit_line("DEMETRIO NETO");
	add_credit_line(" ");

	add_credit_line("- PS2 PORTING -");
	add_credit_line("RAGNAROK2040");
	add_credit_line("WOON-YUNG LIU");
	add_credit_line(" ");

	add_credit_line("- DEVELOPMENT LIBRARIES -");
	add_credit_line("LIBSDL.ORG");
	add_credit_line("NOKIA Qt4");
	add_credit_line("PS2DEV.ORG");
	add_credit_line("DSDEV.ORG");
	add_credit_line(" ");

	add_credit_line("- DEVELOPMENT TOOLS -");
	add_credit_line("LINUX");
	add_credit_line("GIMP");
	add_credit_line("QTCREATOR");
	add_credit_line("INKSCAPE");
	add_credit_line(" ");

	add_credit_line("- EXTRA THANKS TO -");
	add_credit_line("SDL LIBRARY PORTERS");
	add_credit_line(" ");

	add_credit_line("- PRESENTED BY -");
	add_credit_line("UPPERLAND STUDIOS");
	add_credit_line("GREENNEKO WEB & DESIGN");
	add_credit_line(" ");
}


void free_credits_list() {
	struct text_list *temp_text_list;
	struct text_list *old_text_list;

	temp_text_list = credits_list;
	if (temp_text_list != NULL) {
		while (temp_text_list != NULL) {
			old_text_list = temp_text_list;
			temp_text_list = temp_text_list->next;
			free(old_text_list);
		}
	}
}


void draw_credits_text(SDL_Surface *credits_surface, int initial_line) {
	struct text_list *temp_text_list;
	temp_text_list = credits_list;
	int line_n=0;

	// position cursor in the desired line
	while (line_n < initial_line && temp_text_list) {
		temp_text_list = temp_text_list->next;
		line_n++;
	}

	line_n=0;

	while (line_n <= 17 && temp_text_list) {
		draw_text(10-RES_DIFF_W, line_n*12, temp_text_list->text, credits_surface);
		line_n++;
		temp_text_list = temp_text_list->next;
	}
}


void draw_credit_line(SDL_Surface *credits_surface, int initial_line) {
	struct text_list *temp_text_list;
	temp_text_list = credits_list;
	int line_n=0;

	// position cursor in the desired line
	while (line_n < initial_line && temp_text_list) {
		temp_text_list = temp_text_list->next;
		line_n++;
	}

	if (temp_text_list) {
		draw_text(10-RES_DIFF_W, 17*12, temp_text_list->text, credits_surface);
	}
}

void ending_credits() {
	SDL_Rect dest;
	SDL_Surface *credits_surface = NULL;
	struct text_list *temp_text_list;
	int line_n=0, scrolled=0;
	int posY = -RES_H;
	int total_lines=0;

	stop_music();
	load_music("ending.mod");
	play_music();


	create_credits_list();
	credits_surface = SDL_CreateRGBSurface(SDL_SWSURFACE , RES_W, RES_H+12, 8, 0, 0, 0, 0);
	if (!credits_surface) {
		printf("ERROR creating credits_surface\n");
		leave_game();
	}
	SDL_FillRect(credits_surface, NULL, SDL_MapRGB(game_screen->format, 0, 0, 0));
	SDL_SetColors(credits_surface, colormap, 0, COLOR_COUNT);

	// add the initial lines to screen
	draw_credits_text(credits_surface, line_n);

	// check the total number of lines
	temp_text_list = credits_list;
	while (temp_text_list) {
		total_lines++;
		temp_text_list = temp_text_list->next;
	}
	blank_screen(game_screen);

	// scroll the lines
	while (scrolled < (total_lines*12)+RES_H+12) {
		copy_area(credits_surface, 0, posY, RES_W, RES_H, 0, 0, game_screen);
		updateScreen(game_screen);
		posY++;
		scrolled++;
		if (posY > 12) {
			copy_area(credits_surface, 0, posY, RES_W, RES_H, 0, 0, credits_surface);
			// scroll the lines
			dest.x = 0;
			dest.y = RES_H;
			dest.w = RES_W;
			dest.h = 12;
			SDL_FillRect(credits_surface, &dest, SDL_MapRGB(game_screen->format, 0, 0, 0));
			draw_credit_line(credits_surface, line_n+18);
			posY = 0;
			line_n++;
		} else {
			waitTime(50);
		}
	}
	blank_screen(game_screen);
	draw_text(10-RES_DIFF_W, RES_H/2,"PRESS START", game_screen);
	wait_player_start();
}


void ending_lab() {
	SDL_Surface *background = NULL;
	SDL_Surface *sprite_canotus = NULL;
	SDL_Surface *sprite_candy = NULL;
	char buffer[512];

	load_colorcycle(TECHNOBOT);

	blank_screen(game_screen);
	sprintf(buffer, "%sdata/images/scenes/lab_ending.png", FILEPATH);
	background = loadSpriteFile(buffer);
	SDL_SetColors(background, colormap, 0, COLOR_COUNT);

	copy_area(background, 0, 0, background->w, background->h, 10-RES_DIFF_W, 10, game_screen);
	sprintf(buffer, "%sdata/images/sprites/canotus.png", FILEPATH);
	sprite_canotus = loadSpriteFile(buffer);
	SDL_SetColors(sprite_canotus, colormap, 0, COLOR_COUNT);

	sprintf(buffer, "%sdata/images/sprites/sprites_p3.png", FILEPATH);
	sprite_candy = loadSpriteFile(buffer);
	SDL_SetColors(sprite_candy, colormap, 0, COLOR_COUNT);
	SDL_SetColors(game_screen, colormap, 0, COLOR_COUNT);


	p1Obj->sprite->x = RES_W+p1Obj->sprite->w;
	p1Obj->sprite->y = 74;
	p1Obj->sprite->direction = ANIM_LEFT;
	p1Obj->sprite->anim_type = ANIM_WALK;


	while (p1Obj->sprite->x > RES_W-100) {
		p1Obj->sprite->x -= MOVE_STEP;
		SDL_SetColors(background, colormap, 0, COLOR_COUNT);
		copy_area(background, 0, 0, background->w+32, background->h, 0, 10, game_screen);
		copy_area(sprite_canotus, 62, 0, 21, sprite_canotus->h, 60, 82, game_screen);
		copy_area(sprite_candy, 83, 14, 16, 25, 120, 81, game_screen);
		show_sprite(p1Obj->sprite, game_screen);
		exec_colorcycle(game_screen);
		updateScreen(game_screen);
		waitTime(20);
	}


	copy_area(background, 0, 0, background->w+32, background->h, 0, 10, game_screen);
	copy_area(sprite_canotus, 62, 0, 21, sprite_canotus->h, 60, 82, game_screen);
	copy_area(sprite_candy, 83, 14, 16, 25, 120, 81, game_screen);
	p1Obj->sprite->anim_type = ANIM_STAND;
	show_sprite(p1Obj->sprite, game_screen);
	updateScreen(game_screen);
	waitTime(200);

	SDL_FreeSurface(background);
	SDL_FreeSurface(sprite_candy);
	SDL_FreeSurface(sprite_canotus);


	if (game_config.two_players == 2 || game_config.selected_player == 1) {
		showDialog(FACES_CANDY, "ROCK, CONGRATULATIONS!\nYOU ARE THE CITY HERO,\nAND PEOPLE LOVE YOU.", 1);
		resetDialog();
		showDialog(FACES_ROCKBOT, "I DREAMED ABOUT THIS.\nBUT NOW THAT HAPPENED,\nI AM SAD.", 1);
		resetDialog();
		showDialog(FACES_CANDY, "WHY?\nAND WHERE IS BETA?", 1);
		resetDialog();
		showDialog(FACES_ROCKBOT, "HE...HE...\nHE DYIED AS A HERO,\nPROTECTING THE CITY.", 1);
		resetDialog();
		showDialog(FACES_KANOTUS, "WE WILL HONROR HIM\nAND NEVER FORGET.", 1);
		resetDialog();
	} else {
		showDialog(FACES_CANDY, "BETA, WHERE IS ROCK?", 1);
		resetDialog();
		showDialog(FACES_BETABOT, "NOT FUNCTIONAL.", 1);
		resetDialog();
		showDialog(FACES_KANOTUS, "WHAT HAPPENED?", 1);
		resetDialog();
		showDialog(FACES_BETABOT, "HE SIDED WITH ENEMY.", 1);
		resetDialog();
		showDialog(FACES_KANOTUS, "HOW CAN IT BE?", 1);
		resetDialog();
		showDialog(FACES_BETABOT, "HE WAS WEAK AND SOFT.\nCOULD NOT UNDERTAND.", 1);
		resetDialog();
		showDialog(FACES_BETABOT, "HERE. HIS CYBERBRAIN.\nYOU CAN TRY FIX HIM.", 1);
		resetDialog();
		showDialog(FACES_KANOTUS, "WHERE ARE YOU GOING?", 1);
		resetDialog();
		showDialog(FACES_BETABOT, "FIGHT.\nWHILE EVIL EXISTS,\nI FIGHT... ALONE.", 1);
		resetDialog();


	}
	removeDialog();
	waitTime(2000);
}


void ending_train() {
	int end_timer;
	int pos_layer_bg=0;
	int pos_layer_fence=0;
	int check_timer;
	int train_move=0;
	int k;
	SDL_Surface *train_surface = NULL;
	SDL_Surface *bg_surface = NULL;
	SDL_Surface *fence_surface = NULL;
	SDL_Surface *bushes_surface = NULL;
	struct player_struct *playerObj;
	char buffer[512];
	int played_sfx = 0;
	int posx_adjust = 0;
	int pos_bushes_1 = 0;
	int pos_bushes_2 = 0;

	scrollX = 0;
	map_pos_x = 0;
	map_pos_y = 0;
	restore_colormap();

	stop_music();
	load_music("train.mod");
	play_music();

	sprintf(buffer, "%sdata/images/backgrounds/ending_layer_train.png", FILEPATH);
	train_surface = surface_from_image(buffer);
	if (!train_surface) {
		printf(">> ERROR loading train layer '%s' <<\n", buffer);
	}
	sprintf(buffer, "%sdata/images/backgrounds/ending_layer_background.png", FILEPATH);
	bg_surface = surface_from_image(buffer);
	if (!bg_surface) {
		printf(">> ERROR loading train layer '%s' <<\n", buffer);
	}
	sprintf(buffer, "%sdata/images/backgrounds/ending_layer_fence.png", FILEPATH);
	fence_surface = surface_from_image(buffer);
	if (!fence_surface) {
		printf(">> ERROR loading train layer '%s' <<\n", buffer);
	}

	sprintf(buffer, "%sdata/images/backgrounds/ending_layer_bushes.png", FILEPATH);
	bushes_surface = surface_from_image(buffer);
	if (!bushes_surface) {
		printf(">> ERROR loading train layer '%s' <<\n", buffer);
	}
	pos_bushes_1 = RES_W + bushes_surface->w;
	pos_bushes_2 = RES_W*1.5 + bushes_surface->w;


	copy_area(bg_surface, 0, 0, RES_W, bg_surface->h, 0, 0, game_screen);
	copy_area(fence_surface, 0, 0, RES_W, fence_surface->h, 0, bg_surface->h, game_screen);
	copy_area(train_surface, 0, 0, RES_W, train_surface->h, 0, bg_surface->h+fence_surface->h, game_screen);
	updateScreen(game_screen);

	end_timer = getTimer()+10000;
	check_timer = getTimer()+2000;
	blank_screen(game_screen);
	while (getTimer() < end_timer) {
		// move background
		pos_layer_bg++;
		if (pos_layer_bg >= bg_surface->w-RES_W) {
			pos_layer_bg = 0;
		}
		// move fence
		pos_layer_fence += 4;
		if (pos_layer_fence >= fence_surface->w-RES_W) {
			pos_layer_fence = 0;
		}

		pos_bushes_1 -= 12;
		if (pos_bushes_1 < -bushes_surface->w) {
			pos_bushes_1 = RES_W + bushes_surface->w;
		}

		pos_bushes_2 -= 12;
		if (pos_bushes_2 < -bushes_surface->w) {
			pos_bushes_2 = RES_W + bushes_surface->w;
		}


		copy_area(bg_surface, pos_layer_bg, 0, RES_W, bg_surface->h, 0, 0, game_screen);
		copy_area(fence_surface, pos_layer_fence, 0, RES_W, fence_surface->h, 0, bg_surface->h, game_screen);
		if (getTimer() > check_timer) {
			if (train_move == 0) {
				train_move = 1;
				check_timer = getTimer()+100;
			} else {
				train_move = 0;
				check_timer = getTimer()+2000;
			}
		}
		if (train_move == 0) {
			posx_adjust = 0;
			played_sfx = 0;
		} else {
			if (!played_sfx) {
				play_sfx(SFX_TRAIN_CHUNK);
			}
			played_sfx = 1;
			posx_adjust = 2;
		}
		copy_area(train_surface, posx_adjust, 0, RES_W, train_surface->h, 0, bg_surface->h+fence_surface->h, game_screen);
		for (k=0; k<game_config.two_players; k++) {
			playerObj = getPlayerN(k);
			playerObj->sprite->x = RES_W/2 + (40*k) - posx_adjust;
			playerObj->sprite->y = bg_surface->h+fence_surface->h - playerObj->sprite->h;
			//scrollX = 0;
			//map_pos_x = 0;
			playerObj->sprite->anim_type = ANIM_STAND;
			show_sprite(playerObj->sprite, game_screen);
		}
		copy_area(bushes_surface, 0, 0, bushes_surface->w, bushes_surface->h, pos_bushes_1, RES_H-bushes_surface->h, game_screen);
		copy_area(bushes_surface, 0, 0, bushes_surface->w, bushes_surface->h, pos_bushes_2, RES_H-bushes_surface->h, game_screen);

		updateScreen(game_screen);

		waitTime(30);
	}
	// jump up
	int jump_counter = 0;
	int jump_speed = 0;
	int JUMP_LIMIT = 11;
	int jump_step = 0.65;
	playerObj->sprite->anim_type = ANIM_JUMP;
	int jump_up = 1;
	while (playerObj->sprite->y+playerObj->sprite->h < RES_H) {
		jump_speed = JUMP_LIMIT - jump_counter*jump_step;
		if (jump_up == 1) {
			playerObj->sprite->y -= jump_speed;
		} else {
			playerObj->sprite->y += jump_speed;
		}
		jump_counter += jump_speed;

		copy_area(bg_surface, pos_layer_bg, 0, RES_W, bg_surface->h, 0, 0, game_screen);
		copy_area(fence_surface, pos_layer_fence, 0, RES_W, fence_surface->h, 0, bg_surface->h, game_screen);
		copy_area(train_surface, 0, 0, RES_W, train_surface->h, 0, bg_surface->h+fence_surface->h, game_screen);
		show_sprite(playerObj->sprite, game_screen);
		copy_area(bushes_surface, 0, 0, bushes_surface->w, bushes_surface->h, pos_bushes_1, RES_H-bushes_surface->h, game_screen);
		copy_area(bushes_surface, 0, 0, bushes_surface->w, bushes_surface->h, pos_bushes_2, RES_H-bushes_surface->h, game_screen);
		updateScreen(game_screen);

		if (jump_up == 1 && jump_counter > TILE_SIZE*3) {
			jump_up = 0;
		}

		waitTime(50);
	}
	SDL_FreeSurface(bg_surface);
	SDL_FreeSurface(fence_surface);
	SDL_FreeSurface(train_surface);
	SDL_FreeSurface(bushes_surface);

	ending_lab();
}

void game_ending() {
	ending_train();
	ending_credits();
	leave_game();
	exit(-1);
}

void drawExplosion(short int centerX, short int centerY, const short int showPlayer) {
	unsigned int timerInit;
	char tempBuffer[100];
	int distance=0, mode=0;
	SDL_Surface *tempImg;
	int accel=1;

	timerInit = getTimer();

	sprintf(tempBuffer, "%sdata/images/tilesets/explosion_boss.png", FILEPATH);
	tempImg = surfaceFromFile(tempBuffer);
	SDL_SetColors(tempImg, colormap, 0, COLOR_COUNT);
	p1Obj->sprite->anim_type = ANIM_STAND;
	updateScreen(game_screen);
	while (getTimer() < timerInit+2000) {
		move_screen(game_screen, 0, 0);
		if (showPlayer == 1) {
			show_sprite(p1Obj->sprite, game_screen);
		}
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY-distance, game_screen);
		distance += 3;
		if (distance % 6 == 0) {
			if (mode+1 <= 2) {
				mode++;
			} else {
				mode = 0;
			}
			if (accel +1 <= 5) {
				accel++;
			}
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(10);
	}
	SDL_FreeSurface(tempImg);
}

void drawImplosion(short int centerX, short int centerY) {
	unsigned int timerInit;
	char tempBuffer[100];
	int distance=RES_W*0.5, mode=0;
	SDL_Surface *tempImg;
	int accel=1;

	play_timed_sfx(SFX_IMPLOSION, 1200);
	waitTime(200);

	timerInit = getTimer();

	sprintf(tempBuffer, "%sdata/images/tilesets/explosion_boss.png", FILEPATH);
	tempImg = surfaceFromFile(tempBuffer);
	SDL_SetColors(tempImg, colormap, 0, COLOR_COUNT);
	updateScreen(game_screen);
	centerX -= tempImg->w/6;
	centerY -= tempImg->h/2;

	while (distance > 0) {
		move_screen(game_screen, 0, 0);
		show_sprite(p1Obj->sprite, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY-distance, game_screen);
		distance -= 3;
		if (abs(distance) % 4 == 0) {
			if (mode+1 <= 2) {
				mode++;
			} else {
				mode = 0;
			}
			if (accel +1 <= 5) {
				accel++;
			}
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(5);
	}
	SDL_FreeSurface(tempImg);
}


void return_to_stage_select() {
	struct pos pos;

	setPlayerColor(-1, 0);
	pos = pick_stage();

	p1Obj->sprite->x = 80;
	p1Obj->sprite->y = TILE_SIZE;
	p1Obj->x = 80;
	p1Obj->y = TILE_SIZE;
	p1Obj->HP = INITIAL_HP;
	p1Obj->items.lifes = 3;

	if (p2Obj) {
		p2Obj->sprite->x = 100;
		p2Obj->sprite->y = TILE_SIZE;
		p2Obj->x = 100;
		p2Obj->y = TILE_SIZE;
		p2Obj->HP = INITIAL_HP;
		p2Obj->items.lifes = 3;
	}

	scrollX = 0;
	map_pos_x = 0;
	map_pos_y = 0;
	scrollY = 0;

	blank_screen(game_screen);
	draw_text(MAX_W*0.5-45-RES_DIFF_W, MAX_H*0.5-15,"LOADING...", game_screen);
	updateScreen(game_screen);

	STAGE_N = pos.y*3 + pos.x + 1;

	checkpoint.map = 0;
	checkpoint.x = p1Obj->sprite->x;
	checkpoint.y = -1;
	checkpoint.map_pos_x = 0;


	remove_all_projectiles();
	stage_start = 1;
	loadStage(1, STAGE_N);
	loadMap(1, 0);
	printf(">> load_stage_music.STAGE_N: %d <<\n", STAGE_N);
	load_stage_music(STAGE_N);
	map_pos_x = 0;
	map_pos_y = 0;
	move_screen(game_screen, 0, 0);
	play_music();
}

// TODO - remove all projectiles
void got_weapon(char boss_name[50]) {
	int fim=0;
	int player_n = 0;
	int i, j, beat_all=1;
	char tempBuffer[150];

	if (strstr(boss_name, "Destrin Tank")) {
		game_ending();
		return;
	}

	// check if the player must be teleported back to a capsule
	for (i=0; i<8; i++) {
		if (skull_castle_bosses[i].active == 1) {
			remove_all_projectiles();
			skull_castle_bosses[i].finished = 1;
			skull_castle_bosses[i].active = 0;
			teleportPlayersUp();
			loadMap(currentGame, game.map_links[skull_castle_bosses[i].teleporter_n].map_origin);
			// check if beat all the bosses
			for (j=0; j<8; j++) {
				if (skull_castle_bosses[j].finished == 0) {
					beat_all = 0;
					break;
				}
			}
			if (beat_all == 0) {
				map_pos_x = skull_castle_bosses[i].old_map_pos_x;
				p1Obj->sprite->x = skull_castle_bosses[i].old_player_pos.x;
				p1Obj->sprite->y = skull_castle_bosses[i].old_player_pos.y;
				if (p2Obj) {
					p2Obj->sprite->x = skull_castle_bosses[i].old_player_pos.x;
					p2Obj->sprite->y = skull_castle_bosses[i].old_player_pos.y;
				}
			} else {
				map_pos_x = skull_castle_bosses[i].old_map_pos_x - TILE_SIZE*20;
				p1Obj->sprite->x = skull_castle_bosses[i].old_player_pos.x + TILE_SIZE*20;
				p1Obj->sprite->y = skull_castle_bosses[i].old_player_pos.y;
				if (p2Obj) {
					p2Obj->sprite->x = skull_castle_bosses[i].old_player_pos.x + TILE_SIZE*20;
					p2Obj->sprite->y = skull_castle_bosses[i].old_player_pos.y;
				}
				checkpoint.x = p1Obj->sprite->x;
				checkpoint.y = p1Obj->sprite->y;
				checkpoint.map = currentMap;
				checkpoint.map_pos_x = map_pos_x;
			}
			move_screen(game_screen, 0, 0);
			updateScreen(game_screen);
			teleportPlayersStand();
			return;
		}
	}


	if (save_001.stages[STAGE_N] != 1) {
		save_001.finished_stages++;
	}
	save_001.stages[STAGE_N] = 1;

	FILE* fp;
	sprintf(tempBuffer, "%sdata/001.sav", FILEPATH);
	fp = fopen(tempBuffer, "wb");
	if (fp) {
		fwrite (&save_001, sizeof(struct st_save), 1, fp);
		fclose(fp);
	} else {
		printf("DEBUG - error saving to file '%s'\n", tempBuffer);
	}



	// 1st part - player walk to screen center and jumps
	//printf(">> got_weapon - real.p.x: %d\n", (p1Obj->sprite->x+map_pos_x));
	p1Obj->sprite->anim_type = ANIM_WALK;
	if (p1Obj->sprite->x+map_pos_x > RES_W/2) {
		keyState[player_n][KEYX] = 2;
	} else {
		keyState[player_n][KEYX] = 1;
	}
	while (fim == 0) {
		//printf(">> p1.pos: %d, target: %d <<\n", p1Obj->sprite->x+map_pos_x, RES_W/2);
		move_screen(game_screen, 0, 0);
		if (player_movement(p1Obj, keyState[player_n]) == 0) {
			//printf(">> anim_type - player is blocked, trying to jump\n");
			keyState[player_n][KEYJUMP] = '1';
		} else {
			if (keyState[player_n][KEYJUMP] == 1) { // release the jump button, so can jump again
				keyState[player_n][KEYJUMP] = 0;
			}
		}
		show_sprite(p1Obj->sprite, game_screen);

		if (keyState[player_n][KEYX] == 1 && p1Obj->sprite->x+map_pos_x >= (RES_W*0.5)-TILE_SIZE) {
			fim = 1;
		} else if (keyState[player_n][KEYX] == 2 && p1Obj->sprite->x+(p1Obj->sprite->w*0.5)+map_pos_x < (RES_W*0.5)) {
			fim = 1;
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(30);
	}
	// 2nd part energy goes to player
	p1Obj->sprite->anim_type = ANIM_JUMP;
	int originalY = p1Obj->sprite->y;
	while (p1Obj->sprite->y > RES_H * 0.3) {
		move_screen(game_screen, 0, 0);
		p1Obj->sprite->y -= 2;
		drawMap3rdLevel(game_screen);
		show_sprite(p1Obj->sprite, game_screen);
		updateScreen(game_screen);
		waitTime(8);
	}
	drawImplosion(p1Obj->sprite->x+map_pos_x+14, p1Obj->sprite->y+15);
	setPlayerColor(STAGE_N, 0);
	while (p1Obj->sprite->y < originalY) {
		move_screen(game_screen, 0, 0);
		p1Obj->sprite->y += 2;
		show_sprite(p1Obj->sprite, game_screen);
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(5);
	}
	move_screen(game_screen, 0, 0);
	updateScreen(game_screen);
	p1Obj->sprite->anim_type = ANIM_STAND;
	show_sprite(p1Obj->sprite, game_screen);
	drawMap3rdLevel(game_screen);
	updateScreen(game_screen);
	stop_music();
	play_sfx(SFX_GOT_WEAPON);
	waitTime(5000);
	p1Obj->sprite->anim_type = ANIM_TELEPORT;
	play_sfx(SFX_TELEPORT);
	while (p1Obj->sprite->y > -p1Obj->sprite->h) {
		move_screen(game_screen, 0, 0);
		p1Obj->sprite->y -= 6;
		show_sprite(p1Obj->sprite, game_screen);
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(5);
	}
	// 3. show the you got weapon
	SDL_Surface *tempImg;

	int text_x = MAX_W*0.5 - 90;
	char letter[2];
	char weaponName[100];
	char extra_name[100];

	blank_screen(game_screen);
	sprintf(tempBuffer, "%sdata/images/backgrounds/stage_boss_intro.png", FILEPATH);
	tempImg = loadSpriteFile(tempBuffer);
	copy_area(tempImg, 0, 0, tempImg->w, tempImg->h, 0, 0, game_screen);
	p1Obj->sprite->x = 20;
	p1Obj->sprite->y = 88;
	scrollX = 0;
	map_pos_x = 0;
	p1Obj->sprite->anim_type = ANIM_ATTACK;
	show_sprite(p1Obj->sprite, game_screen);
	updateScreen(game_screen);

	initStars();


	if (strstr(boss_name, "Ape Bot")) {
		sprintf(weaponName, "YOU GOT GOLDEN BANANA");
	} else if (strstr(boss_name, "Daisie Bot")) {
		sprintf(weaponName, "YOU GOT FLOWER RAINBOW");
	} else if (strstr(boss_name, "Dynamite Bot")) {
		sprintf(weaponName, "YOU GOT TNT FLASH");
	} else if (strstr(boss_name, "Mummy Bot")) {
		sprintf(weaponName, "YOU GOT DESERT SCORPION");
	} else if (strstr(boss_name, "Spike Bot")) {
		sprintf(weaponName, "YOU GOT SPIKED CHAIN");
	} else if (strstr(boss_name, "Techno Bot")) {
		sprintf(weaponName, "YOU GOT TIMER BOMB");
	} else if (strstr(boss_name, "Mage Bot")) {
		sprintf(weaponName, "YOU GOT MAGIC STAR");
	} else if (strstr(boss_name, "Seahorse Bot")) {
		sprintf(weaponName, "YOU GOT PALOMETA FISH");
	} else {
		sprintf(weaponName, "OOOPS - NONAME WEAPON");
	}

	for (i=0; i<strlen(weaponName); i++) {
		sprintf(letter, "%c", weaponName[i]);
		waitTimeWithStars(80);
		draw_text(text_x, 90, letter, game_screen);
		text_x += 8;
	}
	if (STAGE_N == 1) {
		sprintf(extra_name, "AND FROG COIL ADAPTOR");
	} else if (STAGE_N == 6) {
		sprintf(extra_name, "AND EAGLE JET ADAPTOR");
	} else {
		sprintf(extra_name, " ");
	}
	text_x = MAX_W*0.5 - 90;
	if (strlen(extra_name) > 0) {
		for (i=0; i<strlen(extra_name); i++) {
			sprintf(letter, "%c", extra_name[i]);
			waitTimeWithStars(80);
			draw_text(text_x, 104, letter, game_screen);
			text_x += 8;
		}
	}
	int timeInit = getTimer() + 2000;
	while (timeInit > getTimer()) {
		waitTimeWithStars(20);
	}

	// 4.B select a new stage and load it
	p1Obj->sprite->anim_type = ANIM_TELEPORT;
	p1Obj->sprite->x = RES_H * 0.2;
	p1Obj->sprite->y = RES_H * 0.5;
	updateScreen(game_screen);
	SDL_FreeSurface(tempImg);
	return_to_stage_select();
}
