#include<stdint.h>
#include"flow_decode.h"

UpixelsOpticalFlow updata;

int16_t flow_parse_char(uint8_t ch)
{
    int16_t ret = 1;
    static int16_t s  = 0, p = 0;
    static uint8_t Xor_r = 0x00, Xor_c = 0x00;

    switch(s)
    {
        case 0:
            if(ch == 0xFE)
            {
                s = 1;
                //printf("Got FE\n");
            }
        break;
        case 1:
            if(ch == 0x0A)
            {
                s = 2;
                p = 0;
                Xor_c = 0x00;
                //printf("Got 0A\n");
            }else
                s = 0;
        break;
        case 2:
            ((char *)&updata)[p++] = ch;
            Xor_c ^= ch;
            if(p == 10){
                s = 3;
                p = 0;
            }
        break;
        case 3: //crc
            s = 4;
            Xor_r = ch;
        break;
        case 4://end
            if(ch == 0x55){
               //printf("Got 0x55\n");
                if(Xor_r == Xor_c){
                    ret = 0;
                }
                else
                    ret = 2;
            }
            s = 0;
        break;
        default:
        break;
    }
    return ret;
}

