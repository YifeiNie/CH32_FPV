

#ifndef _DP_FILTER_H
#define _DP_FILTER_H
#include "debug.h"
#include "control_handle.h"

#define FilterBuf_MaxLen 5

typedef struct{

	float Base[FilterBuf_MaxLen];
	int16_t   Head;
	int16_t   Rear;

}FilterBuf_STRUCT;


void    FilterSample( FilterBuf_STRUCT* p, float NewValue);
float   FilterAverage( FilterBuf_STRUCT* p);

#endif










