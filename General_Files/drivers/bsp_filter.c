

#include "bsp_filter.h"



void FilterSample( FilterBuf_STRUCT* p, float NewValue)
{
	if(p->Head == 0 && p->Rear == 0 && p->Base[0] == 0)
	{
		p->Base[p->Rear]  = NewValue;
	}
	else
	{
		p->Rear = (p->Rear+1)%FilterBuf_MaxLen;
		
		p->Base[p->Rear] = NewValue;
		
		if(p->Head == p->Rear)p->Head = (p->Head+1)%FilterBuf_MaxLen;
	}
}



float FilterAverage( FilterBuf_STRUCT* p)
{
	float sum	= 0.0f;
	uint16_t  i;
	for( i = 0; i<FilterBuf_MaxLen; i++)
	{
		sum += p->Base[i];
	}
	return (float)(sum/FilterBuf_MaxLen);
}



