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
	//printf("DEBUG.Mediator::centNumberFormat - centNumber: %s\n", centNumber);
}

int main(void) {
	FILE *fp_old_map, *fp_npc_main_list, *fp_map_npc;
	struct file_map_v0 old_map;
	char filename[255];
	char cent_stage[4], cent_map[4];
	int i, j, k, old_map_n, main_npc_pos, found_npc_pos, npc_count;
	struct file_npc map_npc;
	struct file_map_npc temp_map_npc;



	// open the main npc file
	sprintf(filename, "../bin/data/game/game_001/main_list.npc");
	fp_npc_main_list = fopen(filename, "rb");


	for (i=1; i<=9; i++) {
		for (j=0; j<3; j++) {

			centNumberFormat(i);
			sprintf(cent_stage, "%s", centNumber);
			centNumberFormat(j);
			sprintf(cent_map, "%s", centNumber);

			//printf(">>>>> old_map: %d, new.stage: %d, new.map: %d, cent_map: '%s', centNumber: '%s' <<<<< \n", old_map_n, i, j, cent_map, centNumber);


			old_map_n = i + (j*9);
			centNumberFormat(old_map_n);


			sprintf(filename, "../bin/data/game/game_001/%s.map", centNumber);
			fp_old_map = fopen(filename, "rb");
			if (fp_old_map) {
				//printf(">> WARNING: Converting old map '%s' <<\n", filename);

				npc_count = 0;

				fread(&old_map, sizeof(struct file_map_v0), 1, fp_old_map);

				sprintf(filename, "../bin/data/game/game_001/stage_%s/map_%s_npc_list.dat", cent_stage, cent_map);
				//printf(">>> exportNPCList - filename: %s\n", filename);
				fp_map_npc = fopen(filename, "wb");


				for (k=0; k<STAGE_NPC_LIMIT; k++) {
					if (old_map.npc_list[k].hp.total != -1) {

						// search for the npc position in main list
						rewind(fp_npc_main_list);
						main_npc_pos = 0;
						found_npc_pos = -1;
						while (!feof(fp_npc_main_list)) {
							fread(&map_npc, sizeof(struct file_npc), 1, fp_npc_main_list);
							if (strcmp(map_npc.name, old_map.npc_list[k].name) == 0) {
								found_npc_pos = main_npc_pos;
								break;
							}
							main_npc_pos++;
						}

						if (found_npc_pos != -1) {
							//printf(">> SAVEMAP - Found NPC '%s' in main list, MAP: %d, count: %d <<\n", old_map.npc_list[k].name, found_npc_pos, npc_count);
							// write the npc into the npc.map file
							temp_map_npc.id = found_npc_pos;
							temp_map_npc.start_point.x = old_map.npc_list[k].start_point.x;
							temp_map_npc.start_point.y = old_map.npc_list[k].start_point.y;
							temp_map_npc.direction = old_map.npc_list[k].direction;
							if (fwrite(&temp_map_npc, sizeof(struct file_map_npc), 1, fp_map_npc) != 1) {
								printf(">>>>>>>>>>>>>>>> ERROR Writing NPC to file <<<<<<<<<<<<<<<\n");
							} else {
								printf(">> GOOD - Added NPC '%s' old_map_n: %d, stage %d, map: %d <<\n", old_map.npc_list[k].name, old_map_n, i, j);
							}
							//printf("+++ exporting npc '%s', direction: %d +++\n", map.npc_list[i].name, map.npc_list[i].direction);
							npc_count++;
						} else {
							printf(">> ERROR - NPC called '%s' was not found in the main list <<\n", old_map.npc_list[k].name);
						}
					}
				}

				fclose(fp_map_npc);

				fclose(fp_old_map);
			} else {
				printf(">> ERROR: Could not open file '%s' <<\n", filename);
			}
		}
	}
}
