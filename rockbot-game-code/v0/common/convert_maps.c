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
	char filename[512], buffer[512];
	char temp_cent_number[4];
	FILE *fp, *fp_old;
	struct file_map_v1 map_exp;
	struct file_map_v0 map_old;
	int i, j;

	printf("****************** convert Map ****************** \n");

	sprintf(filename, "../bin/data/game/game_001/001.map");
	fp_old = fopen(filename, "rb");
	if (fp_old) {
		printf(">> WARNING: loading old-map file: '%s' <<\n", filename);
		if (fread(&map_old, sizeof(struct file_map_v0), 1, fp_old) != 1) {
			printf(">> error loading from old map <<\n");
			fclose(fp_old);
			exit(-1);
		}
		fclose(fp_old);
	} else {
		printf(">> error: could not open file: '%s' <<\n", filename);
		exit(-1);
	}


	// ------------------- EXPORT MAP TO NEW FORMAT AND LOCATION ---------------------- //
	sprintf(filename, "../bin/data/game/game_001/stage_001/000_map.dat");
	// copy data from old struct to the newer one

	sprintf(map_exp.filename, "%s", map_old.filename);
	printf(">> filename: '%s' <<\n", map_old.filename);
	for (i=0; i<MAP_W; i++) {
		for (j=0; j<MAP_H; j++) {
			map_exp.tiles[i][j].locked = map_old.tiles[i][j].locked;
			map_exp.tiles[i][j].tile1.x = map_old.tiles[i][j].tile1.x;
			map_exp.tiles[i][j].tile1.y = map_old.tiles[i][j].tile1.y;
			map_exp.tiles[i][j].tile2.x = map_old.tiles[i][j].tile2.x;
			map_exp.tiles[i][j].tile2.y = map_old.tiles[i][j].tile2.y;
			map_exp.tiles[i][j].tile3.x = map_old.tiles[i][j].tile3.x;
			map_exp.tiles[i][j].tile3.y = map_old.tiles[i][j].tile3.y;
			printf(">> locked: %d, tile1.x: %d, tile1. y: %d <<\n", map_old.tiles[i][j].locked, map_old.tiles[i][j].tile1.x, map_old.tiles[i][j].tile1.y);
		}
	}
	FILE *fp_exp = fopen(filename, "wb");
	if (fp_exp) {
		fwrite(&map_exp, sizeof(struct file_map_v1), 1, fp_exp);
		fclose(fp_exp);
	}
	// ------------------- EXPORT MAP TO NEW FORMAT AND LOCATION ---------------------- //


}
