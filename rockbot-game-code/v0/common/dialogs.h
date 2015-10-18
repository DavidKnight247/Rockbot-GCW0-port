#ifndef DIALOGS_H
#define DIALOGS_H


void init_dialogs() {
	int i, j, k, l;
	char playerName[50];
	int playerFace;
	int player_id = 0;

	if (game_config.two_players == 2 || game_config.selected_player == 1) {
		sprintf(playerName, "Rockbot");
		playerFace = FACES_ROCKBOT;
	} else {
		sprintf(playerName, "Betabot");
		playerFace = FACES_BETABOT;
		player_id = 1;
	}

	for (i=0; i<DIALOG_MAX_STAGE; i++) {
		for (j=0; j<DIALOG_MAX_PLACES; j++) {
			for (k=0; k<DIALOG_MAX_PARTS; k++) {
				dialog_list[i][j][k].face_n = -1;
				sprintf(dialog_list[i][j][k].name, "\0");
				for (l=0; l<3; l++) {
					sprintf(dialog_list[i][j][k].phrase[j], "\0");
				}
			}
		}
	}
	// APEBOT
	dialog_list[APEBOT][0][0].face_n = FACES_APEBOT;
	sprintf(dialog_list[APEBOT][0][0].name, "Ape Bot");
	sprintf(dialog_list[APEBOT][0][0].phrase[0], "OOK! OOK! WELCOME TO");
	sprintf(dialog_list[APEBOT][0][0].phrase[1], "AFRICAN SAVANAS.");
	sprintf(dialog_list[APEBOT][0][0].phrase[2], "OOK!");
	dialog_list[APEBOT][0][1].face_n = playerFace;
	sprintf(dialog_list[APEBOT][0][1].name, playerName);
	if (player_id == 0) {
		sprintf(dialog_list[APEBOT][0][1].phrase[0], "I HOPE YOU HAVE SOME");
		sprintf(dialog_list[APEBOT][0][1].phrase[1], "BANANAS WITH YOU,");
		sprintf(dialog_list[APEBOT][0][1].phrase[2], "'CAUSE I'LL TAKE IT!");
	} else {
		sprintf(dialog_list[APEBOT][0][1].phrase[0], "SORRY LITTLE MONKEY.");
		sprintf(dialog_list[APEBOT][0][1].phrase[1], "BUT I HAVE TO BURN");
		sprintf(dialog_list[APEBOT][0][1].phrase[2], "IT, IF NEEDED TO WIN.");
	}

	// DYNAMITEBOT
	dialog_list[DYNAMITEBOT][0][0].face_n = FACES_DYNAMITEBOT;
	sprintf(dialog_list[DYNAMITEBOT][0][0].name, "Dynamite Bot");
	sprintf(dialog_list[DYNAMITEBOT][0][0].phrase[0], "COME! I'LL SHOW YOU");
	sprintf(dialog_list[DYNAMITEBOT][0][0].phrase[1], "SOME NICE FIREWORKS.");
	dialog_list[DYNAMITEBOT][0][1].face_n = playerFace;
	sprintf(dialog_list[DYNAMITEBOT][0][1].name, playerName);
	if (player_id == 0) {
		sprintf(dialog_list[DYNAMITEBOT][0][1].phrase[0], "YOU WANT TO CELEBRATE");
		sprintf(dialog_list[DYNAMITEBOT][0][1].phrase[1], "MY VICTORY?");
		sprintf(dialog_list[DYNAMITEBOT][0][1].phrase[2], "HOW NICE.");
	} else {
		sprintf(dialog_list[DYNAMITEBOT][0][1].phrase[0], "NOT IF I BLOW");
		sprintf(dialog_list[DYNAMITEBOT][0][1].phrase[1], "YOUR HEAD FIRST.");
	}

	// TECHNOBOT
	dialog_list[TECHNOBOT][0][0].face_n = FACES_TECHNOBOT;
	sprintf(dialog_list[TECHNOBOT][0][0].name, "Techno Bot");
	sprintf(dialog_list[TECHNOBOT][0][0].phrase[0], "101010101010101010");
	sprintf(dialog_list[TECHNOBOT][0][0].phrase[1], "010010001001110010.");
	dialog_list[TECHNOBOT][0][1].face_n = playerFace;
	sprintf(dialog_list[TECHNOBOT][0][1].name, playerName);
	if (player_id == 0) {
		sprintf(dialog_list[TECHNOBOT][0][1].phrase[0], "YOU WHAT??");
		sprintf(dialog_list[TECHNOBOT][0][1].phrase[1], "SORRY PAL, BUT I DO");
		sprintf(dialog_list[TECHNOBOT][0][1].phrase[2], "NOT UNDERSTAND THAT.");
	} else {
		sprintf(dialog_list[TECHNOBOT][0][1].phrase[0], "YES. I'VE COME TO");
		sprintf(dialog_list[TECHNOBOT][0][1].phrase[1], "FIGHT YOU, 0101.");
	}
	dialog_list[TECHNOBOT][1][0].face_n = FACES_TECHNOBOT;
	sprintf(dialog_list[TECHNOBOT][1][0].name, "Techno Bot");
	sprintf(dialog_list[TECHNOBOT][1][0].phrase[0], "101010101010101010");
	sprintf(dialog_list[TECHNOBOT][1][0].phrase[1], "010010001001110010.");
	dialog_list[TECHNOBOT][1][1].face_n = playerFace;
	sprintf(dialog_list[TECHNOBOT][1][1].name, playerName);
	if (player_id == 0) {
		sprintf(dialog_list[TECHNOBOT][1][1].phrase[0], "OK, OK. ENOUGHT WITH");
		sprintf(dialog_list[TECHNOBOT][1][1].phrase[1], "THE BIT TALKING.");
		sprintf(dialog_list[TECHNOBOT][1][1].phrase[2], "LET'S BATTLE NOW!");
	} else {
		sprintf(dialog_list[TECHNOBOT][1][1].phrase[0], "SORRY. I HAVE ORDERS.");
		sprintf(dialog_list[TECHNOBOT][1][1].phrase[1], "YOU DIE, I LIVE.");
	}


	// SPIKEBOT
	dialog_list[SPIKEBOT][0][0].face_n = FACES_SPIKEBOT;
	sprintf(dialog_list[SPIKEBOT][0][0].name, "Spike Bot");
	sprintf(dialog_list[SPIKEBOT][0][0].phrase[0], "TO REACH ME, YOU'LL");
	sprintf(dialog_list[SPIKEBOT][0][0].phrase[1], "HAVE TO AVOID THE.");
	sprintf(dialog_list[SPIKEBOT][0][0].phrase[2], "DEADLY TRAPS.");
	dialog_list[SPIKEBOT][0][1].face_n = playerFace;
	sprintf(dialog_list[SPIKEBOT][0][1].name, playerName);
	if (player_id == 0) {
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[0], "YOU CAN'T TRAP ME.");
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[1], "I'LL FIND YOU AND");
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[2], "YOU'LL BECOME MY TOY.");
	} else {
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[0], "I'M NO GINEA PIG,");
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[1], "TO PLAY IN YOUR MAZE.");
		sprintf(dialog_list[SPIKEBOT][0][1].phrase[2], "I'LL SCAPE AND WIN.");
	}


}

void show_dialog(int n, int place) {
	char phrase[70];
	int k;

	if (dialog_list[n][place][0].face_n == -1) {
		return;
	}
	for (k=0; k<DIALOG_MAX_PARTS; k++) {
		if (dialog_list[n][place][k].face_n != -1) {
			update_sprites(0);
			sprintf(phrase, "%s\n%s\n%s\0", dialog_list[n][place][k].phrase[0], dialog_list[n][place][k].phrase[1], dialog_list[n][place][k].phrase[2]);
			showDialog(dialog_list[n][place][k].face_n, phrase, 0);
			resetDialog();
		}
	}
	removeDialog();
	clean_input(1);
	waitTime(200);
}

#endif // DIALOGS_H
