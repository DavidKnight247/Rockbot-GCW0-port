#ifndef COLISION_DETECTION_H
#define COLISION_DETECTION_H

#include "st_common.h"


class colision_detection
{
public:
	colision_detection();
	bool rect_overlap(st_rectangle A, st_rectangle B);

private:
	bool value_in_range(int value, int min, int max);
};
#endif // COLISION_DETECTION_H
