enum GAME_KEYS {KEYX, KEYY, KEYJUMP, KEYATTACK, KEYQUIT, KEYSTART, KEYL, KEYR};

#ifdef PLAYSTATION
	#define JOYVAL 12000
#else
	#define JOYVAL 30000
#endif


/*
 PC - PS2-USB joystick
 TRIANGLE: 0
 CIRCLE: 1
 x: 2
 SQUARE: 3
 L2: 4
 R2: 5
 L1: 6
 L2: 7
 SELECT: 8
 START: 9
 L3: 10
 R3: 11
 */


#ifndef PLAYSTATION
	#define JOY_TRIANGLE 0
	#define JOY_CIRCLE 1
	#define JOY_X 2
	#define JOY_SQUARE 3
	#define JOY_L2 4
	#define JOY_R2 5
	#define JOY_L1 6
	#define JOY_R1 7
	#define JOY_SELECT 8
	#define JOY_START 9
	#define JOY_L3 10
	#define JOY_R3 11
#else
	#define JOY_TRIANGLE 3
	#define JOY_CIRCLE 2
	#define JOY_X 1
	#define JOY_SQUARE 0
	#define JOY_L2 7
	#define JOY_R2 9
	#define JOY_L1 6
	#define JOY_R1 8
	#define JOY_SELECT 4
	#define JOY_START 5
	#define JOY_L3 10
	#define JOY_R3 11
#endif

#define PLAYER1 0
#define PLAYER2 1

void updateJoyMove(int player_n);
void read_input();

void clean_input(int total_clean) {
	int i;
	for (i = 0; i < KEYNUMBER; i++) {
		if ((i != KEYJUMP && i != KEYATTACK) || total_clean == 1) {
			keyState[0][i] = 0;
			keyState[1][i] = 0;
		}
	}
	if (total_clean) {
		for (i=0; i<2; i++) {
			MOVE_LEFT[i] = 0;
			MOVE_RIGHT[i] = 0;
			MOVE_UP[i] = 0;
			MOVE_DOWN[i] = 0;
		}
	}
}


void read_input() {
	int PLAYERN = 0;
	if (game_config.two_players == 2) {
		PLAYERN = 1;
	}
	clean_input(0);
	waitTime(1);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN: // keyboard events are always for PLAYERN
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						keyState[PLAYERN][KEYQUIT] = 1;
						break;
					case SDLK_UP:
						MOVE_DOWN[PLAYERN] = 0;
						MOVE_UP[PLAYERN] = 1;
						break;
					case SDLK_DOWN:
						MOVE_DOWN[PLAYERN] = 1;
						MOVE_UP[PLAYERN] = 0;
						break;
					case SDLK_LEFT:
						MOVE_LEFT[PLAYERN] = 1;
						MOVE_RIGHT[PLAYERN] = 0;
						break;
					case SDLK_RIGHT:
						MOVE_LEFT[PLAYERN] = 0;
						MOVE_RIGHT[PLAYERN] = 1;
						break;
					case SDLK_x:
						keyState[PLAYERN][KEYJUMP] = 1;
						if (p1Obj != NULL) {
							p1Obj->jump_timer = SDL_GetTicks();
						}
						break;
					case SDLK_LCTRL:
						keyState[PLAYERN][KEYJUMP] = 1;
						if (p1Obj != NULL) {
							p1Obj->jump_timer = SDL_GetTicks();
						}
						break;
					case SDLK_a:
						keyState[PLAYERN][KEYATTACK] = 1;
						break;
					case SDLK_LALT:
						keyState[PLAYERN][KEYATTACK] = 1;
						break;
					case SDLK_RETURN:
						keyState[PLAYERN][KEYSTART] = 1;
						break;
					case SDLK_q:
						keyState[PLAYERN][KEYL] = 1;
						break;
					case SDLK_w:
						keyState[PLAYERN][KEYR] = 1;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						keyState[PLAYERN][KEYQUIT] = 1;
						break;
					case SDLK_UP:
						MOVE_UP[PLAYERN] = 0;
						break;
					case SDLK_DOWN:
						MOVE_DOWN[PLAYERN] = 0;
						break;
					case SDLK_LEFT:
						MOVE_LEFT[PLAYERN] = 0;
						break;
					case SDLK_RIGHT:
						MOVE_RIGHT[PLAYERN] = 0;
						break;
					case SDLK_x:
						keyState[PLAYERN][KEYJUMP] = 0;
						break;
					case SDLK_LCTRL:
						keyState[PLAYERN][KEYJUMP] = 0;
						break;
					case SDLK_a:
						keyState[PLAYERN][KEYATTACK] = 0;
						break;
					case SDLK_LALT:
						keyState[PLAYERN][KEYATTACK] = 0;
						break;
					case SDLK_RETURN:
						keyState[PLAYERN][KEYSTART] = 0;
						break;
					case SDLK_q:
						keyState[PLAYERN][KEYL] = 0;
						break;
					case SDLK_w:
						keyState[PLAYERN][KEYR] = 0;
						break;
					default:
						break;
				}
				break;
			case SDL_JOYAXISMOTION:
				//printf(">> SDL_JOYAXISMOTION - axis: %d, value: %d, which: %d <<\n", event.jaxis.axis, event.jaxis.value, event.jaxis.which);
				if (event.jaxis.axis == 0) {
					if (event.jaxis.value < -JOYVAL) {
						MOVE_RIGHT[event.jaxis.which] = 0;
						MOVE_LEFT[event.jaxis.which] = 1;
					} else if (event.jaxis.value > JOYVAL) {
						MOVE_LEFT[event.jaxis.which] = 0;
						MOVE_RIGHT[event.jaxis.which] = 1;
					} else if (event.jaxis.value < JOYVAL || event.jaxis.value > -JOYVAL) {
						MOVE_LEFT[event.jaxis.which] = 0;
						MOVE_RIGHT[event.jaxis.which] = 0;
					}
				}
				if (event.jaxis.axis == 1) {
					if (event.jaxis.value < -JOYVAL) {
						MOVE_DOWN[event.jaxis.which] = 0;
						MOVE_UP[event.jaxis.which] = 1;
						//printf(">> event.jaxis.value: %d\n", event.jaxis.value);
					} else if (event.jaxis.value > JOYVAL) {
						MOVE_UP[event.jaxis.which] = 0;
						MOVE_DOWN[event.jaxis.which] = 1;
						//printf(">> event.jaxis.value: %d\n", event.jaxis.value);
					} else if (event.jaxis.value < JOYVAL || event.jaxis.value > -JOYVAL) {
						MOVE_UP[event.jaxis.which] = 0;
						MOVE_DOWN[event.jaxis.which] = 0;
					}
				}
				break;

				// TODO - use SDL_JOYBUTTONUP to measure the time of jump
			case SDL_JOYBUTTONDOWN:
				//printf(">> SDL_JOYBUTTONDOWN - event.jbutton.button: %d <<\n", event.jbutton.button);
				//printf("DEBUG - button pressed, number: %d, witch: %d\n", event.jbutton.button, event.jbutton.which);
				if (event.jbutton.button == JOY_X) {
					keyState[event.jbutton.which][KEYJUMP] = 1;
					if (event.jbutton.which == 0) {
						p1Obj->jump_timer = SDL_GetTicks();
					} else if (event.jbutton.which == 1) {
						p2Obj->jump_timer = SDL_GetTicks();
					}
				}
				if (event.jbutton.button == JOY_START) {
					printf(">> JOY_START pressed, n: %d <<\n", event.jbutton.which);
					keyState[event.jbutton.which][KEYSTART] = 1;
				}
#if defined(PC) || defined(DINGUX)
				if (event.jbutton.button == JOY_SELECT) {
					keyState[event.jbutton.which][KEYQUIT] = 1;
				}
#endif
				if (event.jbutton.button == JOY_SQUARE) {
					printf(">> p(%d) ATTACK KEY <<\n", event.jbutton.which);
					keyState[event.jbutton.which][KEYATTACK] = 1;
				}
				if (event.jbutton.button == JOY_L1) {
					keyState[event.jbutton.which][KEYL] = 1;
				}
				if (event.jbutton.button == JOY_R1) {
					keyState[event.jbutton.which][KEYR] = 1;
				}
				
				break;
			case SDL_JOYBUTTONUP:
				//printf("DEBUG - button released, number: %d\n", event.jbutton.button);
				if (event.jbutton.button == JOY_X) {
					keyState[event.jbutton.which][KEYJUMP] = 0;
				}
				if (event.jbutton.button == JOY_SQUARE) {
					printf(">> p(%d) ATTACK KEY RELEASED <<\n", event.jbutton.which);
					keyState[event.jbutton.which][KEYATTACK] = 0;
				}
				break;
			#if !defined(DINGUX) && !defined(PLAYSTATION)
			case SDL_QUIT:
				leave_game();
				exit(-1);
				break;
			#endif
			case SDL_JOYHATMOTION:
				//printf(">> SDL_JOYHATMOTION joy_n: %d, hat: %d, value: %d <<\n", event.jhat.which, event.jhat.hat, event.jhat.value);
				// CODES: up - 1, right: 2, down: 4, left: 8
				if (event.jhat.value == 1 || event.jhat.value == 3 || event.jhat.value == 9) { // up
					MOVE_DOWN[event.jaxis.which] = 0;
					MOVE_UP[event.jaxis.which] = 1;
				}
				if (event.jhat.value == 2 || event.jhat.value == 3 || event.jhat.value == 6) { // right
					MOVE_RIGHT[event.jaxis.which] = 1;
					MOVE_LEFT[event.jaxis.which] = 0;
				}
				if (event.jhat.value == 4 || event.jhat.value == 6 || event.jhat.value == 12) { // down
					MOVE_DOWN[event.jaxis.which] = 1;
					MOVE_UP[event.jaxis.which] = 0;
				}
				if (event.jhat.value == 8 || event.jhat.value == 9 || event.jhat.value == 12) { // left
					MOVE_RIGHT[event.jaxis.which] = 0;
					MOVE_LEFT[event.jaxis.which] = 1;
				}
				if (event.jhat.value == 0) {
					MOVE_RIGHT[event.jaxis.which] = 0;
					MOVE_LEFT[event.jaxis.which] = 0;
					MOVE_DOWN[event.jaxis.which] = 0;
					MOVE_UP[event.jaxis.which] = 0;
				}
				break;
			default:
				//printf(">> other event <<\n");
				break;
		}
	}

	SDL_PumpEvents(); // check keyboard events
	updateJoyMove(0);
	updateJoyMove(1);
}

void updateJoyMove(int player_n) {
	if (MOVE_LEFT[player_n]) {
			keyState[player_n][KEYX] = 2;
	}
	if (MOVE_RIGHT[player_n]) {
			keyState[player_n][KEYX] = 1;
	}
	if (MOVE_UP[player_n]) {
			keyState[player_n][KEYY] = 2;
	}
	if (MOVE_DOWN[player_n]) {
			keyState[player_n][KEYY] = 1;
	}
}

void wait_player_start() {
	// this small wait is meant to not get keystrokes still in execution (kind of wait to clear the cache)
	waitTime(50);
	while (1) {
		read_input();
		if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) {
			break;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
			exit(-1);
		}
		exec_colorcycle(game_screen);
	}
}


void wait_player_key() {
    // this small wait is meant to not get keystrokes still in execution (kind of wait to clear the cache)
    waitTime(50);

    while (1) {
		read_input();
		if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) {
			break;
		} else if (keyState[0][KEYJUMP] == 1 || keyState[1][KEYJUMP] == 1) {
			break;
		} else if (keyState[0][KEYATTACK] == 1 || keyState[1][KEYATTACK] == 1) {
			break;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
			exit(-1);
		}
		if (game_screen) {
			exec_colorcycle(game_screen);
			updateScreen(game_screen);
		}
		waitTime(5);
    }
}

int waitScapeTime(int wait_period) {
	int now_time = 0;

	waitTime(50);
	now_time = getTimer();
	wait_period = now_time + wait_period;

	while (now_time < wait_period) {
		read_input();
		if (keyState[0][KEYSTART] == 1 || keyState[1][KEYSTART] == 1) {
			return 1;
		} else if (keyState[0][KEYQUIT] == 1 || keyState[1][KEYQUIT] == 1) {
			leave_game();
			exit(-1);
		}
		now_time = getTimer();
		exec_colorcycle(game_screen);
		#ifdef PLAYSTATION
			RotateThreadReadyQueue(_MIXER_THREAD_PRIORITY);
		#endif
	}
	return 0;
}
