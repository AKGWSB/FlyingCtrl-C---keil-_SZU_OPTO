#ifndef __DATA_H__

#define __DATA_H__

#include "data.h"

Melon_Module melon;

dataAutoStruct autoData;

void sendThreshole()
{
	
}

void configMelon(void)
{
	melon.threshold.interface.function.saveThreshold = sendThreshole;
}

#endif
