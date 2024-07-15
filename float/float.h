#ifndef FLOAT_H_
#define FLOAT_H_

#define FLOAT_ACCURACY 10e-5

#define fsqrt(x) squareRoot((x), FLOAT_ACCURACY)


#include <math.h>

#include "../common/types.h"


TYPE_FLOAT squareRoot(TYPE_FLOAT value, TYPE_FLOAT accuracy);

#endif
