#ifndef SDL_Surface
	#include "SDL/SDL.h"
#endif


void updateScreen(SDL_Surface *screen);
int waitScapeTime(int wait_period);

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}



Uint32 ReadPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

SDL_Surface *ScaleSurface(SDL_Surface *Surface, int initialX, int initialY, int limitW, int limitH, int Width, int Height)
{
	Sint32 x, y, o_y, o_x;

	if(!Surface || !Width || !Height)
		return NULL;

	SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel, Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
	SDL_SetColors(_ret, colormap, 0, COLOR_COUNT);
	double _stretch_factor_x = Width  / limitW;
	double _stretch_factor_y = Height / limitH;

	//printf(">> ScaleSurface - Width: %d, Height: %d, _stretch_factor_x: %f, _stretch_factor_y: %f <<\n", Width, Height, _stretch_factor_x, _stretch_factor_y);

	for(y = initialY; y < limitH; y++) {
		for(x = initialX; x < limitW; x++) {
			for(o_y = 0; o_y < _stretch_factor_y; ++o_y) {
				for(o_x = 0; o_x < _stretch_factor_x; ++o_x) {
					Uint32 pixel = ReadPixel(Surface, x, y);
					if (pixel == 0) {
						DrawPixel(_ret, (Sint32)(_stretch_factor_x * x) + o_x, (Sint32)(_stretch_factor_y * y) + o_y, Surface->format->colorkey);
					} else {
						DrawPixel(_ret, (Sint32)(_stretch_factor_x * x) + o_x, (Sint32)(_stretch_factor_y * y) + o_y, pixel);
					}
				}
			}
		}
	}
	SDL_SetColorKey(_ret, SDL_SRCCOLORKEY, Surface->format->colorkey);

	return _ret;
}

struct sprite_struct *create_sprite(short int x, short int y, short int sp_w, short int sp_h, SDL_Surface *screen) {
	SDL_Rect src, dest;
	struct sprite_struct *sprite_list;

	sprite_list = (struct sprite_struct*)malloc(sizeof(struct sprite_struct));
	init_sprite_vars(sprite_list);

	// carrega sprite do player
	sprite_list->x = x;
	sprite_list->y = y;
	sprite_list->w = sp_w;
	sprite_list->h = sp_h;

	src.x = x;
	src.y = y;
	src.w = sp_w;
	src.h = sp_h;
	// parte que vai ser colada
	dest.x = 0;
	dest.y = 0;
	dest.w = sp_w;
	dest.h = sp_h;
	show_sprite(sprite_list, screen);
	return sprite_list;
}


//Limpa a tela. Essa função é necessária já que não podemos utilizar a função SDL_FreeSurface na tela principal, a screen.
void clear_screen(SDL_Surface *screen) {
	SDL_Rect src, dest;
	// OBS: passar o tamanho da tela para viaráveis para poder usar + de uma resolução
	// parte que vai ser copiada
	src.x = 16;
	src.y = 16;
	src.w = MAX_W;
	src.h = MAX_H;
	// parte que vai ser colada
	dest.x = 0;
	dest.y = 0;
	dest.w = MAX_W;
	dest.h = MAX_H;
	SDL_BlitSurface (offscreen, &src, screen, &dest);
}


//Limpa a tela. Essa função é necessária já que não podemos utilizar a função SDL_FreeSurface na tela principal, a screen.
void clear_area(SDL_Surface *screen, short int x, short int y, short int w, short int h, short int r, short int g, short int b) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	//SDL_SetColors(screen, colormap, 0, COLOR_COUNT);
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, r, g, b));
}



void add_sprite(short int direction, short int anim_type, short int pos_x, short int pos_y, struct sprite_struct *sprite_list, short int duration, SDL_Surface *image) {
	SDL_Rect src, dest;
	short int pos_n=0, i;
	src.x = pos_x*sprite_list->w;
	src.y = pos_y*sprite_list->h;
	src.w = (pos_x*sprite_list->w)+sprite_list->w;
	src.h = (pos_y*sprite_list->h)+sprite_list->h;
	// parte que vai ser colada
	dest.x = 0;
	dest.y = 0;
	dest.w = sprite_list->w;
	dest.h = sprite_list->h;
	for (i=0; i<ANIM_N3 && sprite_list->sprite_frames[direction][anim_type][i].frame != NULL; i++) {
		pos_n++;
	}
	//printf("************* graphics.h::add_sprite - direction: %d, anim_type: %d, anim_pos: %d\n", direction, anim_type, pos_n);
	sprite_list->sprite_frames[direction][anim_type][pos_n].duration=duration;
	//printf("DEBUG.add_sprite - adicionando sprite, pos_n: %d, direction: %d, anim_type: %d, int pos_x: %d, int_pos_y: %d, duration: %d\n", pos_n, direction, anim_type, pos_x, pos_y, sprite_list->sprite_frames[direction][anim_type][pos_n].duration);
	sprite_list->sprite_frames[direction][anim_type][pos_n].frame = SDL_CreateRGBSurface(SDL_SWSURFACE , sprite_list->w, sprite_list->h, 8, 0, 0, 0, 0);
	SDL_SetColors(sprite_list->sprite_frames[direction][anim_type][pos_n].frame, colormap, 0, COLOR_COUNT);
	SDL_FillRect(sprite_list->sprite_frames[direction][anim_type][pos_n].frame, NULL, SDL_MapRGB(game_screen->format, 123, 123, 123));
	SDL_SetColorKey(sprite_list->sprite_frames[direction][anim_type][pos_n].frame, SDL_SRCCOLORKEY, SDL_MapRGB(game_screen->format, 123, 123, 123));
	SDL_BlitSurface(image, &src, sprite_list->sprite_frames[direction][anim_type][pos_n].frame, &dest);
}



// Nessa função desenharemos o paddle do jogador...
void show_sprite(struct sprite_struct *sprite_list, SDL_Surface *screen) {
	SDL_Rect src, dest;
	short int x_p=0, y_p=0;
	//printf("DEBUG - sprite_n: %d", sprite_n);
	// define qual a posição da tela onde irá desenhar o sprite
	x_p = sprite_list->x + map_pos_x;
	y_p = sprite_list->y + map_pos_y;

	src.w = sprite_list->w;
	src.h = sprite_list->h;
	// parte que vai ser colada
	dest.w = sprite_list->w;
	dest.h = sprite_list->h;

	// copia sprite para a tela
	src.x = 0;
	src.y = 0;
	// área da colagem
	dest.x = x_p;
	dest.y = y_p;
	//printf("DEBUG - dest.x: %d, dest.y: %d\n", dest.x, dest.y);
	
	// verifica se o frame da animação existe, caso contrário, usa o primeiro (0) da lista
	//printf(">>graphics.h::show_sprite - direction: %d, anim_type: %d, anim_pos: %d\n", sprite_list->direction, sprite_list->anim_type, sprite_list->anim_pos);
	//printf("DEBUG - sprite_N: %d\n");

	if (sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].frame == NULL) {
		if (sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][0].frame != NULL) {
			sprite_list->anim_pos=0;
		} else {
			//printf("WARNING - erro: Não achou frame para sprite - direction: %d,  type: %d, pos: %d\n", sprite_list->direction, sprite_list->anim_type, sprite_list->anim_pos);
			return;
		}
	}
	// copia o fundo da tela, guardado em .tile, para a tela
	//SDL_BlitSurface(sprite_list->tile, &src, screen, &dest);
	SDL_BlitSurface(sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].frame, &src, screen, &dest);
	// avança a animação
	//printf("############# > show_sprite - duration: %d, time-init: %d\n", sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].duration, (getTimer() - sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].init));
	if (getTimer() - sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].init > sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].duration) {
		if (sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos+1].frame != NULL) {
			sprite_list->anim_pos++;
			//printf(">>graphics.h::show_sprite - INC POS direction: %d, anim_type: %d, anim_pos: %d\n", sprite_list->direction, sprite_list->anim_type, sprite_list->anim_pos);
		// reseta animação
		} else {
			sprite_list->anim_pos=0;
			// if is turn, reset to stand (TODO - must be walk, fix it)
		}
		//printf("DEBUG - reseting animation\n");
		sprite_list->sprite_frames[sprite_list->direction][sprite_list->anim_type][sprite_list->anim_pos].init = getTimer();
	}
/*	
	// do not have the frame, try using the first frame of the move
	if (sprite_list->frames[sprite_list->frame_pos[0]][sprite_list->frame_pos[1]] == NULL && sprite_list->frames[sprite_list->frame_pos[0]][0] == NULL) {
		printf("DEBUG - no image in frame[%d][%d]\n", sprite_list->frame_pos[0], sprite_list->frame_pos[1]);
		SDL_BlitSurface(sprite_list->tile, &src, screen, &dest);
	} else {
		if (sprite_list->frames[sprite_list->frame_pos[0]][0] == NULL) {
			sprite_list->frame_pos[1]=0;
		}
		//printf("DEBUG - using frames\n");
		SDL_BlitSurface(sprite_list->frames[sprite_list->frame_pos[0]][sprite_list->frame_pos[1]], &src, screen, &dest);
		if (sprite_list->frames[sprite_list->frame_pos[0]][sprite_list->frame_pos[1]+1] != NULL) {
			sprite_list->frame_pos[1]++;
		} else {
			sprite_list->frame_pos[1]=0;
		}
	}
*/
}

void draw_text(short int x, short int y, char *text, SDL_Surface *screen) {
	SDL_Color font_color = {255,255,255};
	SDL_Rect text_pos={x, y, 0, 0};
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
		// handle error
	}
	SDL_Surface* textSF = TTF_RenderText_Solid(font, text, font_color);
	SDL_BlitSurface(textSF, 0, screen, &text_pos);
	SDL_FreeSurface(textSF);
}


// returns if was cut (by user  pressing a key) or not
int draw_progressive_text(short int x, short int y, char *text, SDL_Surface *screen, short int interrupt) {
	SDL_Color font_color = {255,255,255};
	char *temp_char;
	char temp_text[2];
	int text_x = 0;
	int text_y = 0;
	int cut = 0;

	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
	}


	temp_char = text;
	while (*temp_char != '\0') {
			sprintf(temp_text, "%c", *temp_char);
			draw_text(text_x*9+x, text_y*11+y, temp_text, screen);
			temp_char = temp_char+1;
			text_x++;
			if (*temp_char == '\n') {
					text_x = 0;
					text_y++;
					temp_char = temp_char+1;
			}
			exec_colorcycle(screen);
			updateScreen(screen);
			if (interrupt == 1) {
				#ifdef PC
					cut = waitScapeTime(5);
				#else
					cut = waitScapeTime(1);
				#endif
				if (cut) { return 1; }
			} else {
				#ifdef PC
					waitTime(5);
				#else
					waitTime(1);
				#endif
			}

	}
	return 0;
}

void blank_screen(SDL_Surface *screen) {
	//printf("DEBUG - filling screen with black\n");
	SDL_FillRect(screen, NULL, SDL_MapRGB(game_screen->format, 0, 0, 0));
	if (screen == game_screen) {
		updateScreen(screen);
	}
}

void blank_area(SDL_Surface *screen, short int x, short int y, short int w, short int h) {
	SDL_Rect dest;
	printf("DEBUG - filling screen with black\n");
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_FillRect(screen, &dest, SDL_MapRGB(game_screen->format, 0, 0, 0));
	updateScreen(screen);
}



void show_animation(SDL_Surface *screen) {
	SDL_Rect src, dest;
	struct animation_sprite_struct *temp, *temp_prev, *temp_old;
	temp = animation_list;
	temp_prev = NULL;
	//printf("DEBUG.show_animation - START\n");

	// WARNING - bug around here

	while (temp) {
		src.w = temp->frame->tile->w;
		src.h = temp->frame->tile->h;
		// parte que vai ser colada
		//printf("DEBUG.show_animation - looping throught animations\n");
		src.x = temp->animation_pos*temp->frame->w;
		//src.x = 0;
		src.y = 0;
		// parte que vai ser colada
		dest.x = *temp->x + map_pos_x;
		dest.y = *temp->y + map_pos_y;



		//printf("DEBUG frame->background - src.x: %d, src.y: %d, src.w: %d, src.h: %d\n", src.x, src.y, src.w, src.h);
		//printf("DEBUG frame->background - dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);

		SDL_Surface *tempScaled=NULL;
		tempScaled = ScaleSurface(temp->frame->tile, src.x, src.y, 16, 16, temp->w, temp->h);
		if (tempScaled) {
			src.w = tempScaled->w;
			src.h = tempScaled->h;
			dest.w = tempScaled->w;
			dest.h = tempScaled->h;
			//printf(">> scaled - w: %d, h: %d <<\n", tempScaled->w, tempScaled->h);
			SDL_BlitSurface (tempScaled, &src, screen, &dest);
			SDL_FreeSurface(tempScaled);
		} else {
			src.w = temp->frame->w;
			src.h = temp->frame->h;
			dest.w = temp->frame->w;
			dest.h = temp->frame->h;
			//printf(">> FAIL scaled <<\n");
			SDL_BlitSurface (temp->frame->tile, &src, screen, &dest);
		}

		//printf(">> ANIMATION - timer passed time: %d, duration: %d\n", (getTimer()-temp->init), temp->frame->duration);
		if (getTimer()-temp->init > temp->frame->duration) {
			//temp->animation_pos = temp->animation_pos + 1;
			temp->animation_loop_count++;
			//printf(">> ANIMATION - tile-w: %d, frame->w: %d, n_division: %d\n", temp->frame->tile->w, temp->frame->w, (temp->frame->tile->w/temp->frame->w-1));
			if (temp->animation_pos > (temp->frame->tile->w/temp->frame->w-1)) {
				//printf(">> ANIMATION - reset pos\n");
				temp->animation_pos = 0;
				temp->animation_loop_count++;
				//printf("DEBUG - temp->animation_loop_count: %d, temp->animation_loops: %d\n", temp->animation_loop_count, temp->animation_loops);
			} else {
				//printf(">> ANIMATION - advance pos\n");
			}
			temp->init=getTimer();
		}
		//printf("DEBUG.show_animation - temp->animation_pos: %d\n", temp->animation_pos);
		// if finished, remove animation from animation_list
		if (temp->animation_loop_count >= temp->animation_loops) {
			//printf(">> ANIMATION - finishing\n");
			if (temp_prev != NULL) {
				temp_prev->next = temp->next;
			} else { // if does not have a previous value, means it is the start of the list
				animation_list = temp->next;
			}
			temp_old = temp;
			free(temp);
			temp = NULL;
			temp_prev = NULL;
			temp = temp_old->next;

		} else {
			temp_prev = temp;
			temp = temp->next;
		}

		if (!temp) {
			return;
		}
	}
}


void draw_hp_bar(struct player_struct *playerObj) {
	short int x, y, i;
	short int r, g, b;
	short int y_inc;

	x = 2;
	if (playerObj == p1Obj) {
		r = 127;
		g = 11;
		b = 0;
		y_inc = 2;
	} else {
		r = 255;
		g = 102;
		b = 0;
		y_inc = 61;
	}
	clear_area(game_screen, x+1, y_inc, 7, 57, r, g, b);
	for (i=0; i<playerObj->HP; i++) {
		y = ((28-i)*2+1)+y_inc-2;
		clear_area(game_screen, x+2, y, 5, 1, 255, 255, 255);
	}
	// if player have a weapon selected, draw it too
	if (playerObj->selected_weapon != -1) {
		clear_area(game_screen, x+11, y_inc, 7, 57, 27, 63, 95);
		for (i=0; i<playerObj->items.weapons[playerObj->selected_weapon]; i++) {
			y = ((28-i)*2+1)+y_inc-2;
			clear_area(game_screen, x+12, y, 5, 1, 255, 255, 255);
		}
	}
}

void draw_npc_hp_bar(struct npc_struct *npc_obj) {
	short int x, y, i;
	short int r, g, b;
	short int y_inc = 2;
	x = MAX_W-12;
	r = 255;
	g = 50;
	b = 50;
	clear_area(game_screen, x+1, y_inc, 7, 57, r, g, b);
	for (i=0; i<npc_obj->HP; i++) {
		y = ((28-i)*2+1)+y_inc-2;
		clear_area(game_screen, x+2, y, 5, 1, 255, 255, 255);
	}
	if (npc_obj->icon != NULL) {
		copy_area(npc_obj->icon, 0, 0, npc_obj->icon->w, npc_obj->icon->h, x, 60, game_screen);
	}
}


void create_hit_animation(short *x, short *y, int diffx, int diffy) {
	struct animation_sprite_struct *hit_animation;
	hit_animation = create_animation(x, y, hit_frame, diffx, diffy);
	hit_animation->animation_loops = 3;
}


void create_kill_animation(short *x, short *y, int diffx, int diffy) {
	//printf("DEBUG.create_kill_animation - START\n");
	struct animation_sprite_struct *kill_animation;
        kill_animation = create_animation(x, y, kill_frame, diffx, diffy);
	kill_animation->animation_loops = 2;
}

void change_colormap(SDL_Surface *screen, int pos, int r, int g, int b) {
	int res;

	//printf(">>>> change_colormap - START - screen: %p, pos: %d, r: %d, g: %d, b: %d <<<<<\n", screen, pos, r, g, b);
	if (!screen) {
		return;
	}
	colormap[pos].r = r;
	colormap[pos].g = g;
	colormap[pos].b = b;
	res = SDL_SetColors(screen, colormap, 0, COLOR_COUNT);
	//printf(">>>> change_colormap - pos: %d, res: %d <<<<<\n", pos, res);
}

void change_player_colormap(struct player_struct* pObj, int pos, int r, int g, int b) {
	int res;
	int i, j, k;

	pObj->colormap[pos].r = r;
	pObj->colormap[pos].g = g;
	pObj->colormap[pos].b = b;
	for (i=0; i<ANIM_N1; i++) {
		for (j=0; j<ANIM_N2; j++) {
			for (k=0; k<ANIM_N3; k++) {
				if (pObj->sprite->sprite_frames[i][j][k].frame) {
					res = SDL_SetColors(pObj->sprite->sprite_frames[i][j][k].frame, pObj->colormap, 0, COLOR_COUNT);
				}
			}
		}
	}
}



struct struct_colorcycle_list *add_colorcycle_list(short int color_n) {
	struct struct_colorcycle_list *temp_list;
	struct struct_colorcycle_list *new_cl_list=NULL;


	new_cl_list = (struct struct_colorcycle_list*)malloc(sizeof(struct struct_colorcycle_list));
	new_cl_list->colorcycle = NULL;
	new_cl_list->colorcycle_current = NULL;
	new_cl_list->next = NULL;
	new_cl_list->map_color = color_n;
	if (colorcycle_list == NULL) {
		colorcycle_list = new_cl_list;
	} else {
		temp_list = colorcycle_list;
		while (temp_list->next) {
			temp_list = temp_list->next;
		}
		temp_list->next = new_cl_list;
	}
	return new_cl_list;
}

void add_colorcycle(struct struct_colorcycle_list *list, int duration, int r, int g, int b) {
	struct struct_colorcycle *temp=NULL;
	struct struct_colorcycle *new_colorcycle=NULL;

	new_colorcycle = (struct struct_colorcycle*)malloc(sizeof(struct struct_colorcycle));
	new_colorcycle->r = r;
	new_colorcycle->g = g;
	new_colorcycle->b = b;
	new_colorcycle->duration = duration;
	new_colorcycle->timer_ini = -1;
	new_colorcycle->next = NULL;

	temp = list->colorcycle;
	// first item in the list
	if (temp == NULL) {
		list->colorcycle = new_colorcycle;
	} else {
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_colorcycle;
	}
}


void load_colorcycle(int map_n) {
	freeColorCycling();
	struct struct_colorcycle_list *cl_item;
	//add colorcycling

	if (map_n == DAISIEBOT || map_n == DAISIEBOT+9 || map_n == DAISIEBOT+18) {
		/*
		// CLOUDS
		// first
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 2000, 35, 59, 239); // azul-escuro
		add_colorcycle(cl_item, 1000, 95, 151, 255); // azul-claro
		// second
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 2000, 95, 151, 255); // azul-claro
		add_colorcycle(cl_item, 1000, 199, 215, 255); // branco
		*/
		// first
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 1100, 255, 155, 59); // laranja
		add_colorcycle(cl_item, 2000, 219, 43, 0); // marrom
		add_colorcycle(cl_item, 800, 255, 219, 171); // bege
		// second
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 800, 219, 43, 0); // marrom
		add_colorcycle(cl_item, 2000, 255, 219, 171); // bege
		add_colorcycle(cl_item, 1100, 255, 155, 59); // laranja
		// third
		colorcycle_n = SDL_MapRGB(tileset->format, 0, 255, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 2000, 255, 219, 171); // bege
		add_colorcycle(cl_item, 1100, 255, 155, 59); // laranja
		add_colorcycle(cl_item, 800, 219, 43, 0); // marrom
	} else if (map_n == TECHNOBOT || map_n == TECHNOBOT+9 || map_n == TECHNOBOT+18) {
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0); // green
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 1500, 255, 51, 0); // red-alive
		add_colorcycle(cl_item, 1000, 0, 0, 0); // black

		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255); // pink
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 100, 0, 0, 0); // black
		add_colorcycle(cl_item, 200, 230, 255, 0); // golden-yellow


		colorcycle_n = SDL_MapRGB(tileset->format, 0, 255, 255); // cyan
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 1500, 0, 0, 0); // black
		add_colorcycle(cl_item, 1000, 79, 223, 75); // green

	} else if (map_n == MAGEBOT || map_n == MAGEBOT+9 || map_n == MAGEBOT+18) {
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255); // pink
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 100, 255, 234, 0); // yelloow
		add_colorcycle(cl_item, 200, 0, 0, 0); // black
		add_colorcycle(cl_item, 100, 255, 234, 0); // yelloow
		add_colorcycle(cl_item, 4500, 0, 0, 0); // black

		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0); // green
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 80, 255, 234, 0); // yelloow
		add_colorcycle(cl_item, 1500, 0, 0, 0); // black
		add_colorcycle(cl_item, 80, 255, 234, 0); // yelloow
		add_colorcycle(cl_item, 2500, 0, 0, 0); // black

	} else if (map_n == DYNAMITEBOT || map_n == DYNAMITEBOT+9 || map_n == DYNAMITEBOT+18) {
		// LAVA WAVE
		// first
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 255, 51, 0); // live-red
		add_colorcycle(cl_item, 300, 127, 11, 0); // dark-red
		add_colorcycle(cl_item, 300, 255, 102, 0); // orange
		// second
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 127, 11, 0); // dark-red
		add_colorcycle(cl_item, 300, 255, 102, 0); // orange
		add_colorcycle(cl_item, 300, 255, 51, 0); // live-red
		// third
		colorcycle_n = SDL_MapRGB(tileset->format, 0, 255, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 255, 102, 0); // orange
		add_colorcycle(cl_item, 300, 255, 51, 0); // live-red
		add_colorcycle(cl_item, 300, 127, 11, 0); // dark-red
	} else if (map_n == SEAHORSEBOT || map_n == SEAHORSEBOT+9 || map_n == SEAHORSEBOT+18) {
		// WATER
		// first
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 100, 171, 231, 255); // azul-claro
		add_colorcycle(cl_item, 100, 80, 109, 142); // azul-escuro
		add_colorcycle(cl_item, 100, 235, 235, 235); // branco
		// second
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 100, 80, 109, 142); // azul-escuro
		add_colorcycle(cl_item, 100, 235, 235, 235); // branco
		add_colorcycle(cl_item, 100, 171, 231, 255); // azul-claro
		// third
		colorcycle_n = SDL_MapRGB(tileset->format, 0, 255, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 100, 235, 235, 235); // branco
		add_colorcycle(cl_item, 100, 171, 231, 255); // azul-claro
		add_colorcycle(cl_item, 100, 80, 109, 142); // azul-escuro
	} else {
		// DESERT SAND
		// first
		colorcycle_n = SDL_MapRGB(tileset->format, 55, 255, 0);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 255, 155, 59); // laranja
		add_colorcycle(cl_item, 300, 196, 93, 0); // marrom
		add_colorcycle(cl_item, 300, 255, 219, 171); // bege
		// second
		colorcycle_n = SDL_MapRGB(tileset->format, 255, 0, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 196, 93, 0); // marrom
		add_colorcycle(cl_item, 300, 255, 219, 171); // bege
		add_colorcycle(cl_item, 300, 255, 155, 59); // laranja
		// third
		colorcycle_n = SDL_MapRGB(tileset->format, 0, 255, 255);
		cl_item = add_colorcycle_list(colorcycle_n);
		add_colorcycle(cl_item, 300, 255, 219, 171); // bege
		add_colorcycle(cl_item, 300, 255, 155, 59); // laranja
		add_colorcycle(cl_item, 300, 196, 93, 0); // marrom
	}
}


void exec_colorcycle(SDL_Surface *screen) {
	int n=0;
	struct struct_colorcycle_list *temp_list=NULL;
	temp_list = colorcycle_list;


	while (temp_list != NULL) {
		n++;
		if (temp_list->colorcycle != NULL) {
			if (temp_list->colorcycle_current == NULL) {
				temp_list->colorcycle_current = temp_list->colorcycle;
			} else {
				if (temp_list->colorcycle_current->timer_ini + temp_list->colorcycle_current->duration < getTimer()) {
					if (temp_list->colorcycle_current->next == NULL) {
						temp_list->colorcycle_current = temp_list->colorcycle;
					} else {
						temp_list->colorcycle_current = temp_list->colorcycle_current->next;
					}
					if (screen == NULL) {
						change_colormap(offscreen, temp_list->map_color, temp_list->colorcycle_current->r, temp_list->colorcycle_current->g, temp_list->colorcycle_current->b);
						move_screen(game_screen, 0, 0);
					} else {
						change_colormap(screen, temp_list->map_color, temp_list->colorcycle_current->r, temp_list->colorcycle_current->g, temp_list->colorcycle_current->b);
						if (offscreen != NULL) {
							change_colormap(offscreen, temp_list->map_color, temp_list->colorcycle_current->r, temp_list->colorcycle_current->g, temp_list->colorcycle_current->b);
						}
						if (temp_list->colorcycle_current) {
							change_colormap(tileset, temp_list->map_color, temp_list->colorcycle_current->r, temp_list->colorcycle_current->g, temp_list->colorcycle_current->b);
						}
					}
					temp_list->colorcycle_current->timer_ini = getTimer();
				}
			}
		} else {
			exit(-1);
		}
		temp_list = temp_list->next;
	}
}

SDL_Surface *surface_from_image(char filename[255]) {
	SDL_Surface *tile;
	SDL_RWops *rwop;

	rwop=SDL_RWFromFile(filename, "rb");
	if (!rwop) {
		printf("Error in surface_from_image: '%s' file not found\n", filename);
		exit(-1);
	}
	tile=IMG_Load_RW(rwop, 1);
	//printf("<< tile: %p\n", tile);
	SDL_SetColors(tile, colormap, 0, COLOR_COUNT);
	//printf("<< tile: %p\n", tile);
	return tile;
}

SDL_Surface *surface_region_from_image(char filename[255], int w, int h) {
	SDL_Surface *tile;
	SDL_RWops *rwop;


	rwop=SDL_RWFromFile(filename, "rb");
	if (!rwop) {
		printf("Error in surface_region_from_image: '%s' file not found\n", filename);
		return NULL;
	}
	tile=IMG_Load_RW(rwop, 1);
	SDL_SetColors(tile, colormap, 0, COLOR_COUNT);
	return tile;
}



struct simple_sprite *create_simple_sprite(short int w, short int h) {
	struct simple_sprite *new_sprite;
	//printf(">>graphics.h::create_simple_sprite - w: %d, h: %d\n", w, h);
	new_sprite = (struct simple_sprite*)malloc(sizeof(struct simple_sprite));
	new_sprite->sprite = NULL;
	new_sprite->sprite = SDL_CreateRGBSurface(SDL_SWSURFACE , w, h, 8, 0, 0, 0, 0);
	//printf(">>graphics.h::create_simple_sprite - sp: %p, sp->sprite: %p, sp->w: %d, sp->h: %d\n", new_sprite, new_sprite->sprite, new_sprite->sprite->w, new_sprite->sprite->h);
	//SDL_FillRect(new_sprite->sprite, NULL, SDL_MapRGB(game_screen->format, 123, 123, 123));
	//SDL_SetColorKey(new_sprite->sprite, SDL_SRCCOLORKEY, SDL_MapRGB(game_screen->format, 123, 123, 123));
	SDL_SetColors(new_sprite->sprite, colormap, 0, COLOR_COUNT);
	return new_sprite;
}


void show_simple_sprite(SDL_Surface *screen, struct simple_sprite *temp, short int x, short int y) {
	SDL_Rect src, dest;
	//printf(">>graphics.h::show_simple_sprite - x: %d, y: %d\n", x, y);
	src.w = temp->sprite->w;
	src.h = temp->sprite->h;
	// parte que vai ser colada
	dest.w = temp->sprite->w;
	dest.h = temp->sprite->h;


	//printf("DEBUG.show_animation - looping throught animations\n");
	src.x = 0;
	src.y = 0;
	src.w = temp->sprite->w;
	src.h = temp->sprite->h;
	// parte que vai ser colada
	dest.x = x + map_pos_x;
	dest.y = y + map_pos_y;
	dest.w = temp->sprite->w;
	dest.h = temp->sprite->h;
	temp->x = x;
	temp->y = y;
	//printf(">>graphics.h::show_simple_sprite -  sprite->screen - src.x: %d, src.y: %d, src.w: %d, src.h: %d\n", src.x, src.y, src.w, src.h);
	//printf(">>graphics.h::show_simple_sprite -  sprite->screen- dest.x: %d, dest.y: %d, dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);
	SDL_BlitSurface (temp->sprite, &src, screen, &dest);
}


/*
void rasterCircle(int x0, int y0, int radius) {
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	setPixel(x0, y0 + radius);
	setPixel(x0, y0 - radius);
	setPixel(x0 + radius, y0);
	setPixel(x0 - radius, y0);

	while(x < y) {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		setPixel(x0 + x, y0 + y);
		setPixel(x0 - x, y0 + y);
		setPixel(x0 + x, y0 - y);
		setPixel(x0 - x, y0 - y);
		setPixel(x0 + y, y0 + x);
		setPixel(x0 - y, y0 + x);
		setPixel(x0 + y, y0 - x);
		setPixel(x0 - y, y0 - x);
	}
}
*/








SDL_Surface *surfaceFromFile(char filename[255]) {
	SDL_RWops *rwop;

	//printf(">> surfaceFromFile - filename: '%s' <<\n", filename);

	// carrega paleta
	rwop=SDL_RWFromFile(filename, "rb");
	if (!rwop) {
		printf("Error in surfaceFromFile: '%s' file not found\n",filename);
		exit(-1);
	}
	return IMG_Load_RW(rwop, 1);
}

struct sprite_struct *create_sprite_with_surface(short int x, short int y, short int sp_w, short int sp_h, SDL_Surface *screen) {
	SDL_Rect src, dest;
	// carrega sprite do player
	struct sprite_struct *sprite_list;

	sprite_list = (struct sprite_struct*)malloc(sizeof(struct sprite_struct));
	init_sprite_vars(sprite_list);
	sprite_list->x = x;
	sprite_list->y = y;
	sprite_list->w = sp_w;
	sprite_list->h = sp_h;

	src.w = sp_w;
	src.h = sp_h;
	// parte que vai ser colada
	dest.w = sp_w;
	dest.h = sp_h;

	show_sprite(sprite_list, screen);

	return sprite_list;
}



void showReadyAnimation() {
	SDL_Rect src, dest;
	int i;
	char buffer_filename[255];
	SDL_Surface* readyImg;
	sprintf(buffer_filename, "%sdata/images/tilesets/ready.png", FILEPATH);
	readyImg = surfaceFromFile(buffer_filename);

	for (i=0; i<4; i++) {
		src.x = 0;
		src.y = 0;
		src.w = 46;
		src.h = 13;
		dest.x = (RES_W/2)-26;
		dest.y = (RES_H/2)-6;
		dest.w = 46;
		dest.h = 13;
		// go ahread to loop
		drawMap3rdLevel(game_screen);
		//printf(">> %i-dest.x: %d, dest.y: %d\n", i, dest.x, dest.y);
		SDL_BlitSurface(readyImg, &src, game_screen, &dest);
		updateScreen(game_screen);
		waitTime(400);
		move_screen(game_screen, 0, 0);
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(400);
	}

	SDL_FreeSurface(readyImg);
}

//TRANSITION_TOP_TO_BOTTOM, TRANSITION_BOTTOM_TO_TOP
void transitionScreen(short int type, short int map_n, short int adjust_x) {
	SDL_Surface *temp_screen;
	short int i, j;
	struct player_struct *p1;

	//printf(">>>>>>>>>>> transitionScreen.adjust_x: %d, map_n: %d, type: %d\n", adjust_x, map_n, type);
	// draw the offscreen with the new loaded map
	loadMap(currentGame, map_n);

	if (type == TRANSITION_TOP_TO_BOTTOM || type == TRANSITION_BOTTOM_TO_TOP) {
		temp_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, MAX_W, MAX_H*2, 8, 255, 255, 0, 0);
		SDL_SetColors(temp_screen, colormap, 0, COLOR_COUNT);
		// copy current screen to temp
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			copy_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, temp_screen);
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			copy_area(game_screen, 0, 0, MAX_W, MAX_H, 0, MAX_H, temp_screen);
		}
		// copy the new screen to the temp_area
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			copy_area(offscreen, TILE_SIZE+adjust_x, TILE_SIZE, MAX_W, MAX_H, 0, MAX_H, temp_screen);
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			copy_area(offscreen, TILE_SIZE+adjust_x, TILE_SIZE, MAX_W, MAX_H, 0, 0, temp_screen);
		}


		// now, show the transition
		for (i=0; i<(MAX_H+TILE_SIZE*0.5)/TRANSITION_STEP; i++) {
			exec_colorcycle(temp_screen);
			if (type == TRANSITION_TOP_TO_BOTTOM) {
				copy_area(temp_screen, 0, i*TRANSITION_STEP, MAX_W, MAX_H, 0, 0, game_screen);
			} else if (type == TRANSITION_BOTTOM_TO_TOP) {
				copy_area(temp_screen, 0, MAX_H-i*TRANSITION_STEP, MAX_W, MAX_H, 0, 0, game_screen);
			}
			for (j=0; j<game_config.two_players; j++) {	// players
				p1 = getPlayerN(j);
				if (type == TRANSITION_TOP_TO_BOTTOM) {
					if (p1->sprite->y > TILE_SIZE) {
						p1->sprite->y = p1->sprite->y - TRANSITION_STEP;
						p1->y = p1->y - TRANSITION_STEP;
					}
				} else if (type == TRANSITION_BOTTOM_TO_TOP) {
					if (p1->sprite->y < MAX_H-TILE_SIZE*2) {
						p1->sprite->y = p1->sprite->y + TRANSITION_STEP;
						p1->y = p1->y + TRANSITION_STEP;
					}
				}
				//printf("*********** i: %d, p1->sprite->y: %d\n", i, p1->sprite->y);
				show_sprite(p1->sprite, game_screen);
			}
			updateScreen(game_screen);
			waitTime(6);
		}
		SDL_FreeSurface(temp_screen);
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			map_pos_y = 0;
		}
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			if (p1->sprite->y > TILE_SIZE) {
				p1->sprite->y = p1->sprite->y - TRANSITION_STEP;
				p1->y = p1->y - TRANSITION_STEP;
			}
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			if (p1->sprite->y < MAX_H-TILE_SIZE*2) {
				p1->sprite->y = p1->sprite->y + TRANSITION_STEP;
				p1->y = p1->y + TRANSITION_STEP;
			}
		}
	}
}

void blink_screen(int r, int g, int b) {
    int i;
    SDL_Surface *tmp, *screen_copy;

	screen_copy = SDL_CreateRGBSurface(SDL_SWSURFACE , game_screen->w, game_screen->h, 32, 0, 0, 0, 0);
    copy_area(game_screen, 0, 0, game_screen->w, game_screen->h, 0, 0, screen_copy);

    tmp=SDL_DisplayFormat(game_screen);
    SDL_FillRect(tmp, 0, SDL_MapRGB(game_screen->format, r, g, b));
    /*
     for 32 bits-color
    for (i=0; i<99; i++) {
        SDL_SetAlpha(tmp, SDL_SRCALPHA, i);
        SDL_BlitSurface(tmp, 0, game_screen, 0);
		updateScreen(game_screen);
		waitTime(1);
    }
    for (i=99; i>=0; i--) {
        SDL_SetAlpha(tmp, SDL_SRCALPHA, i);
        SDL_BlitSurface(screen_copy, 0, game_screen, 0);
        SDL_BlitSurface(tmp, 0, game_screen, 0);
		updateScreen(game_screen);
		waitTime(1);
    }
    for (i=0; i<99; i++) {
        SDL_SetAlpha(tmp, SDL_SRCALPHA, i);
        SDL_BlitSurface(tmp, 0, game_screen, 0);
		updateScreen(game_screen);
		waitTime(1);
    }
    */
    for (i=0; i<4; i++) {
        SDL_BlitSurface(tmp, 0, game_screen, 0);
		updateScreen(game_screen);
		waitTime(80);

        SDL_BlitSurface(screen_copy, 0, game_screen, 0);
		updateScreen(game_screen);
		waitTime(80);
    }
    SDL_FreeSurface(screen_copy);
    SDL_FreeSurface(tmp);
}


void show_error(int ERROR_LEVEL, char *error_message) {
	if (ERROR_LEVEL == WARNING) {
		draw_text(10, 10, error_message, game_screen);
		waitTime(1000);
	} else {
		blank_screen(game_screen);
		draw_text(10, 10, error_message, game_screen);
		waitTime(5000);
	}
}

void scale2x(SDL_Surface *src, SDL_Surface *dst)
{
	int looph, loopw;

	Uint8* srcpix = (Uint8*)src->pixels;
	Uint8* dstpix = (Uint8*)dst->pixels;

	const int srcpitch = src->pitch;
	const int dstpitch = dst->pitch;
	const int width = src->w;
	const int height = src->h;

	switch(src->format->BytesPerPixel)
	{
	case 1: {
			Uint8 E0, E1, E2, E3, B, D, E, F, H;
		for(looph = 0; looph < height; ++looph)
		{
			for(loopw = 0; loopw < width; ++ loopw)
			{
					B = *(Uint8*)(srcpix + (MAX(0,looph-1)*srcpitch) + (1*loopw));
					D = *(Uint8*)(srcpix + (looph*srcpitch) + (1*MAX(0,loopw-1)));
					E = *(Uint8*)(srcpix + (looph*srcpitch) + (1*loopw));
					F = *(Uint8*)(srcpix + (looph*srcpitch) + (1*MIN(width-1,loopw+1)));
					H = *(Uint8*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (1*loopw));


					/*
				E0 = D == B && B != F && D != H ? D : E;
				E1 = B == F && B != D && F != H ? F : E;
				E2 = D == H && D != B && H != F ? D : E;
				E3 = H == F && D != H && B != F ? F : E;
					*/
					E0 = B;
					E1 = B;
					E2 = B;
					E3 = B;

				*(Uint8*)(dstpix + looph*2*dstpitch + loopw*2*1) = E0;
				*(Uint8*)(dstpix + looph*2*dstpitch + (loopw*2+1)*1) = E1;
				*(Uint8*)(dstpix + (looph*2+1)*dstpitch + loopw*2*1) = E2;
				*(Uint8*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*1) = E3;
			}
		}break;}
	case 2: {
			Uint16 E0, E1, E2, E3, B, D, E, F, H;
		for(looph = 0; looph < height; ++looph)
		{
			for(loopw = 0; loopw < width; ++ loopw)
			{
					B = *(Uint16*)(srcpix + (MAX(0,looph-1)*srcpitch) + (2*loopw));
					D = *(Uint16*)(srcpix + (looph*srcpitch) + (2*MAX(0,loopw-1)));
					E = *(Uint16*)(srcpix + (looph*srcpitch) + (2*loopw));
					F = *(Uint16*)(srcpix + (looph*srcpitch) + (2*MIN(width-1,loopw+1)));
					H = *(Uint16*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (2*loopw));

				//E0 = D == B && B != F && D != H ? D : E;
				//E1 = B == F && B != D && F != H ? F : E;
				//E2 = D == H && D != B && H != F ? D : E;
				//E3 = H == F && D != H && B != F ? F : E;
					E0 = B;
					E1 = B;
					E2 = E;
					E3 = F;

				*(Uint16*)(dstpix + looph*2*dstpitch + loopw*2*2) = E0;
				*(Uint16*)(dstpix + looph*2*dstpitch + (loopw*2+1)*2) = E1;
				*(Uint16*)(dstpix + (looph*2+1)*dstpitch + loopw*2*2) = E2;
				*(Uint16*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*2) = E3;
			}
		}break;}
	case 3: {
			int E0, E1, E2, E3, B, D, E, F, H;
		for(looph = 0; looph < height; ++looph)
		{
			for(loopw = 0; loopw < width; ++ loopw)
			{
					B = READINT24(srcpix + (MAX(0,looph-1)*srcpitch) + (3*loopw));
					D = READINT24(srcpix + (looph*srcpitch) + (3*MAX(0,loopw-1)));
					E = READINT24(srcpix + (looph*srcpitch) + (3*loopw));
					F = READINT24(srcpix + (looph*srcpitch) + (3*MIN(width-1,loopw+1)));
					H = READINT24(srcpix + (MIN(height-1,looph+1)*srcpitch) + (3*loopw));

				E0 = D == B && B != F && D != H ? D : E;
								E1 = B == F && B != D && F != H ? F : E;
				E2 = D == H && D != B && H != F ? D : E;
				E3 = H == F && D != H && B != F ? F : E;

				WRITEINT24((dstpix + looph*2*dstpitch + loopw*2*3), E0);
				WRITEINT24((dstpix + looph*2*dstpitch + (loopw*2+1)*3), E1);
				WRITEINT24((dstpix + (looph*2+1)*dstpitch + loopw*2*3), E2);
				WRITEINT24((dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*3), E3);
			}
		}break;}
	default: { /*case 4:*/
			Uint32 E0, E1, E2, E3, B, D, E, F, H;
		for(looph = 0; looph < height; ++looph)
		{
			for(loopw = 0; loopw < width; ++ loopw)
			{
					B = *(Uint32*)(srcpix + (MAX(0,looph-1)*srcpitch) + (4*loopw));
					D = *(Uint32*)(srcpix + (looph*srcpitch) + (4*MAX(0,loopw-1)));
					E = *(Uint32*)(srcpix + (looph*srcpitch) + (4*loopw));
					F = *(Uint32*)(srcpix + (looph*srcpitch) + (4*MIN(width-1,loopw+1)));
					H = *(Uint32*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (4*loopw));

				E0 = D == B && B != F && D != H ? D : E;
								E1 = B == F && B != D && F != H ? F : E;
				E2 = D == H && D != B && H != F ? D : E;
				E3 = H == F && D != H && B != F ? F : E;

				*(Uint32*)(dstpix + looph*2*dstpitch + loopw*2*4) = E0;
				*(Uint32*)(dstpix + looph*2*dstpitch + (loopw*2+1)*4) = E1;
				*(Uint32*)(dstpix + (looph*2+1)*dstpitch + loopw*2*4) = E2;
				*(Uint32*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*4) = E3;
			}
		}break;}
	}
}



void updateScreen(SDL_Surface *screen) {
	SDL_Flip(screen);
	#ifdef SCALE
		scale2x(screen, game_screen_scaled);
		SDL_SetColors(game_screen_scaled, colormap, 0, COLOR_COUNT);
		SDL_Flip(game_screen_scaled);
/*
	#elif DINGUX
		clear_area(game_screen_scaled, 0, 0, 320, 240, 0, 0, 0);
		copy_area_debug(screen, 0, 0, screen->w, screen->h, 0, 20, game_screen_scaled);
		SDL_SetColors(game_screen_scaled, colormap, 0, COLOR_COUNT);
		SDL_Flip(game_screen_scaled);
*/
	#endif
}


void backup_colormap() {
	int i;

	for (i=0; i<COLOR_COUNT; i++) {
		colormap_original[i].r = colormap[i].r;
		colormap_original[i].g = colormap[i].g;
		colormap_original[i].b = colormap[i].b;
	}
}

void restore_colormap() {
	int i;
	for (i=0; i<COLOR_COUNT; i++) {
		colormap[i].r = colormap_original[i].r;
		colormap[i].g = colormap_original[i].g;
		colormap[i].b = colormap_original[i].b;
	}
}
