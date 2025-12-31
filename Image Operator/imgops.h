#include "common.h"
#include <stdlib.h>
#include <math.h>

void reflect(struct image* image);
void greenscreen(struct image* image, struct image* background);
struct image* make_pink_gradient(int width, int height);