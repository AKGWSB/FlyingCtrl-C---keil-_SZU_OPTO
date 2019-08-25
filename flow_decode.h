#include<stdint.h>

typedef  struct  optical_flow_data
{
    int16_t		flow_x_integral;
    int16_t		flow_y_integral;
    uint16_t   	integration_timespan;
    uint16_t   	ground_distance;
    uint8_t    	quality;
    uint8_t    	version;
}UpixelsOpticalFlow;

//extern UpixelsOpticalFlow updata;

int16_t flow_parse_char(uint8_t ch);
