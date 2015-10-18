#include <stdio.h>
#include <stdlib.h>

#include "format/file_structs.h"

char centNumber[4];

void centNumberFormat(int n) {
	if (n >= 100) {
		sprintf(centNumber, "%d\0", n);
	} else if (n >= 10) {
		sprintf(centNumber, "0%d\0", n);
	} else {
		sprintf(centNumber, "00%d\0", n);
	}
}

int get_stage_n(const int map_n) {
	if (map_n < 10) {
		return map_n;
	}
	if (map_n < 19) {
		return map_n-9;
	}
	return map_n-18;
}

int get_stage_pos(const int map_n) {
	// to prevent errors
	if (map_n > 100) {
		return -1;
	}
	if (map_n < 9) {
		return 0;
	}
	if (map_n < 18) {
		return 1;
	}
	return 2;
}


int main(void) {
	char buffer[255];
	sprintf(buffer, "../bin/data/game/001.gme");
	struct file_game game;
	FILE *fp_game, *fp_links;
	int i, origin_pos, destiny_pos, stage_n;
	struct map_link_v1 new_link;

	fp_game = fopen(buffer, "rb");
	if (fp_game) {
		rewind (fp_game);
		fread(&game, sizeof(struct file_game), 1, fp_game);

		// open the links file
		sprintf(buffer, "../bin/data/game/game_001/map_links.dat");
		fp_links = fopen(buffer, "wb");
		if (fp_links) {
			for (i=0; i<LINKS_NUMBER; i++) {
				if (game.map_links[i].map_origin != -1) {
					origin_pos = get_stage_pos(game.map_links[i].map_origin);
					destiny_pos = get_stage_pos(game.map_links[i].map_dest);
					// there is no way to teleport between stages - we do not want it
					stage_n = get_stage_n(game.map_links[i].map_origin);
					//struct map_link_v1
					if (origin_pos == -1 || destiny_pos == -1) {
						printf(">> ERROR invalid map - Origin:  % d, Destiny: %d <<\n", game.map_links[i].map_origin, game.map_links[i].map_dest);
					} else {

						new_link.link_pos.x = game.map_links[i].link_pos.x;
						new_link.link_pos.y = game.map_links[i].link_pos.y;
						new_link.link_dest.x = game.map_links[i].link_dest.x;
						new_link.link_dest.y = game.map_links[i].link_dest.y;

						new_link.map_origin = origin_pos;
						new_link.map_dest = destiny_pos;
						new_link.stage = stage_n;

						new_link.link_size = game.map_links[i].link_size;
						new_link.link_type = game.map_links[i].link_type;
						new_link.link_bidi = game.map_links[i].link_bidi;
						new_link.is_door = game.map_links[i].is_door;

						if (fwrite(&new_link, sizeof(struct map_link_v1), 1, fp_links) == 1) {
							printf(">> SUCCESS writing link into file. Origin:  % d, Destiny: %d, origin_pos: %d, destiny_pos: %d, Stage; %d <<\n", game.map_links[i].map_origin, game.map_links[i].map_dest, origin_pos, destiny_pos, stage_n);
						} else {
							printf(">> ERROR - could not write a map link to the file <<\n");
						}
					}
				}
			}
			fclose(fp_links);
		} else {
			printf("ERROR: Could not create map_links file '%s' <<\n", buffer);
			return;
		}
		fclose(fp_game);
	} else {
		printf("DEBUG - no game file '%s', ignoring load.\n", buffer);
	}
}
