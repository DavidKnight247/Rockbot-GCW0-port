#ifndef PLAYER_AND_NPC_H
#define PLAYER_AND_NPC_H



void create_kill_animation(short *x, short *y, int diffx, int diffy);
void create_hit_animation(short *x, short *y, int diffx, int diffy);
void drawExplosion(short int centerX, short int centerY, const short int showPlayer);
void got_weapon(char boss_name[50]);
unsigned int getTimer();
struct struct_object *dropItem(int obj_type, int x, int y);
struct npc_struct *boss_energy = NULL; // flag to indicate that boss energy must be shown in screen

struct struct_projectile {
	int x;
	int y;
	int x0;
	int y0;
	int y1;
	int timer;
	int status0; // progress
	int status1; // flag for inversion, etc
	short int type;
	short int direction;
	SDL_Surface* sprite;
};

struct struct_move_info {
	unsigned long int time;
	unsigned short int distance;
	unsigned short int status; // used as IA sub-type
	unsigned short int sub_status; // used as IA sub-sub-type
	struct pos initial_position;
	struct pos secondary_position;
};



struct struct_items {
	unsigned short int energy_tanks;
	unsigned short int weapon_tanks;
	unsigned short int lifes;
	unsigned short int bolts;
	unsigned short int weapons[WEAPON_COUNT]; // 0 position is not used, 5 is for skullcastle (not used too), 10 is for item coil and 11 for item jet
};

struct player_struct {
	short int HP;
	short int HP_MAX;
	char name[20];
	short int x, y;
	short int shield;
	struct sprite_struct *sprite;
	short int jumpButtonReleased;
	short int attackButtonReleased;
	struct player_struct *next;
	short int hit;
	short int direction;
	short int shots;
	short int teleporter;			// holds the teleporting link used last time, so that the user won't use the same one whil not leaving it
	unsigned long int jump_timer;		// holds the time when player jumped, used to calc the jump size if he releases the jump button quickly
	short int jump_mark;
	unsigned int hit_timer;                          // holds the time when player was hit, used to calc the time player will remain invulnerable
	struct struct_object *platform;		// number of the object that player is over. used to move player together with the platform
	unsigned short int jump_max;
	unsigned short int shots_max;
	struct struct_items items;
	SDL_Color colormap[COLOR_COUNT]; // a copy of the colormap, used to change weapons colors
	short int selected_weapon;
	int weapon_timer; // used for weapons that are not projectiles
	short int weapon_state;
};



struct npc_struct {
	short int HP;
	short int TOTAL_HP; // stores original HP to calc damage as percentage
	char name[50];
	short int x, y;
	short int shield;
	short int can_shoot;
	short int shots;
	short int direction;
	short int speed;
	short int walk_range;
	short int IA;
	struct struct_move_info move_info;
	struct sprite_struct *sprite_list;
	unsigned short int is_boss;
	struct struct_projectile projectile;
	int timer;
	struct player_struct *target;
	short int first_run; // indicates if this NPC was stoped or already moved once - used for bosses to change music and display a dialog, for example
	unsigned int hit_timer; // holds the time when player was hit, used to calc the time player will remain invulnerable
	SDL_Surface *icon; // stores the icon graphic
	struct npc_struct *next;
};

struct npc_struct *npc_list[3];
struct player_struct *pc_list = NULL;




struct npc_static_data_list *static_npc_list = NULL;

struct npc_struct *add_npc(short int x, short int y, const int stage_pos) {
	int i;
	struct npc_struct *temp, *new_npc;

	new_npc = (struct npc_struct *)malloc(sizeof(struct npc_struct));
	new_npc->next = NULL;
	new_npc->x = x;
	new_npc->y = y;
	new_npc->shield = 0;
	new_npc->shots = 0;
	new_npc->direction = ANIM_LEFT;
	new_npc->IA = 0;
	new_npc->timer = 0;
	new_npc->move_info.distance = 0;
	new_npc->move_info.time = 0;
	new_npc->move_info.status = 0;
	new_npc->move_info.sub_status = 0;
	new_npc->move_info.initial_position.x = 0;
	new_npc->move_info.initial_position.y = 0;
	new_npc->move_info.secondary_position.x = 0;
	new_npc->move_info.secondary_position.y = 0;
	new_npc->projectile.type = -1;
	new_npc->projectile.x = -1;
	new_npc->projectile.y = -1;
	new_npc->projectile.x0 = -1;
	new_npc->projectile.y0 = -1;
	new_npc->projectile.status0 = 0;
	new_npc->projectile.status1 = 0;
	new_npc->projectile.sprite = NULL;
	new_npc->projectile.direction = ANIM_RIGHT;
	new_npc->target = NULL;
	new_npc->is_boss = 0;
	new_npc->first_run = 0;
	new_npc->hit_timer = 0;
	new_npc->icon = NULL;
	new_npc->sprite_list = NULL;
	new_npc->speed = 2;
	new_npc->walk_range = 48;

	//printf(">> DEBUG 1 - add_npc - stage_pos: %d <<\n", stage_pos);

	if (npc_list[stage_pos] == NULL) {
		npc_list[stage_pos] = new_npc;
	} else {
		temp = npc_list[stage_pos];
		i = 1;	// must start in one to count the first element
		if (temp != NULL) {
			while (temp->next != NULL) {
				temp = temp->next;
				i++;
			}
		}
		temp->next = new_npc;
	}
	return new_npc;
}

struct npc_struct *getNPCN(int n) {
	int i;
	struct npc_struct *temp;
	temp = npc_list[currentMap];
	for (i=0; i<n; i++) {
		// did not reached desired number, return NULL
		if (i<n-1 && temp->next == NULL) {
			return NULL;
		} else if (temp->next != NULL) {
			temp = temp->next;
		}
	}
	return temp;
}

int delete_npc(int n) {
	int i;
	struct npc_struct *temp1=NULL, *temp2=NULL;
	temp1 = npc_list[currentMap];
	for (i=0; i<n; i++) {
		temp2 = temp1;
		if (temp1->next != NULL) {
			temp1 = temp1->next;
		// failed to reach the desired number, return error
		} else {
			return 0;
		}
	}

	if (temp1 != NULL) {
		temp2->next = temp1->next;
	} else {
		temp2->next = NULL;
	}
	return 1;
}



// modificar game.h->colision_verify para usar a player_list no lugar da sprite_list
// eliminar sprite_list e usar apenas npc->sprite e player->sprite
int add_player(void) {
	int i=0;
	struct player_struct *temp, *new_player;
	new_player = (struct player_struct*)malloc(sizeof(struct player_struct));
	new_player->next=NULL;
	new_player->attackButtonReleased=0;
	new_player->jumpButtonReleased=0;
	new_player->hit = 0;
	new_player->HP = INITIAL_HP;
	new_player->HP_MAX = INITIAL_HP;
	new_player->shots = 0;
	new_player->teleporter = -1;
	new_player->jump_mark = 0;
	new_player->hit_timer = 0;
	new_player->jump_max = 64;
	new_player->shots_max = 3;
	new_player->hit_timer = 0;
	new_player->selected_weapon = -1;
	new_player->weapon_timer = -1;
	new_player->weapon_state = -1;
	new_player->platform = NULL;

	new_player->items.energy_tanks = 0;
	new_player->items.weapon_tanks = 0;
	new_player->items.bolts = 0;
	new_player->items.lifes = 3;
	for (i=0; i< WEAPON_COUNT; i++) {
		new_player->items.weapons[i] = INITIAL_HP;
	}

	for (i=0; i<COLOR_COUNT; i++) {
		new_player->colormap[i].r = colormap[i].r;
		new_player->colormap[i].g = colormap[i].g;
		new_player->colormap[i].b = colormap[i].b;
	}


	if (player_list == NULL) {
		player_list = new_player;
		return 0;
	} else {
		temp = player_list;
		for (i=0; temp->next != NULL; i++) {
			temp = temp->next;
		}
		temp->next = new_player;
		return (i+1);
	}
}

struct player_struct *getPlayerN(int n) {
	int i;
	struct player_struct *temp;
	temp = player_list;
	for (i=0; i<n; i++) {
		// did not reached desired number, return NULL
		if (i<n-1 && temp->next == NULL) {
			return NULL;
		} else if (temp->next != NULL) {
			temp = temp->next;
		}
	}
	return temp;
}

int delete_player(int n) {
	int i;
	struct player_struct *temp1=NULL, *temp2=NULL;
	temp1 = player_list;
	for (i=0; i<n; i++) {
		temp2 = temp1;
		if (temp1->next != NULL) {
			temp1 = temp1->next;
		// failed to reach the desired number, return error
		} else {
			return 0;
		}
	}

	if (temp1 != NULL) {
		temp2->next = temp1->next;
	} else {
		temp2->next = NULL;
	}
	return 1;
}

void damage_npc(struct npc_struct *temp_npc, short int damage) {
    if (temp_npc->hit_timer > 0) {
        return;
    }
	if (temp_npc->is_boss) {
		temp_npc->hit_timer = getTimer() + NPC_HIT_INVENCIBLE_TIME*2;
	} else {
		temp_npc->hit_timer = getTimer() + NPC_HIT_INVENCIBLE_TIME;
	}
	temp_npc->HP = temp_npc->HP - damage;
	if (temp_npc->HP <= 0) {
		create_kill_animation(&temp_npc->x, &temp_npc->y, temp_npc->sprite_list->w/2, temp_npc->sprite_list->h/2);
		//play_sfx(SFX_NPC_HIT);
		if (temp_npc->is_boss == 1) {
			boss_energy = NULL;
			//printf("is boss, must show demo end, npc.x: %d, map_pos_x: %d\n", temp_npc->x, map_pos_x);
			drawExplosion(temp_npc->x+map_pos_x, temp_npc->y, 1);
			got_weapon(temp_npc->name);
		} else { // drop item chance
			srand ( getTimer() );
			int chance = rand() % 100;
			if (chance < 5) { // energy big
				dropItem(OBJ_ENERGY_PILL_BIG, temp_npc->x, temp_npc->y);
			} else if (chance < 10) { // weapon big
				dropItem(OBJ_WEAPON_PILL_BIG, temp_npc->x, temp_npc->y);
			} else if (chance < 20) { // energy small
				dropItem(OBJ_ENERGY_PILL_SMALL, temp_npc->x, temp_npc->y);
			} else if (chance < 30) { // weapon small
				dropItem(OBJ_WEAPON_PILL_SMALL, temp_npc->x, temp_npc->y);
			}
		}
	} else {
		create_hit_animation(&temp_npc->x, &temp_npc->y, temp_npc->sprite_list->w/2, temp_npc->sprite_list->h/2);
		play_sfx(SFX_NPC_KILLED);
	}
}


#endif // PLAYER_AND_NPC_H
