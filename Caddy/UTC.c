#include "UTC.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void utcToMST(const char* utc, char* out)
{
	  char ap = 0;
    double temp = strtod(utc, NULL);
    int hours = (int)temp/10000;
    temp = fmod(temp,10000.0);
    int mins = (int)temp/100.0;
    temp = fmod(temp,100.0);
    int sec = (int)temp;
    if(hours >= 7)
    {
        if(hours >= 19)
        {
            ap = 1;
        }
        hours -= 7;
    }
    else
    {
        switch(hours)
        {
            case 0: hours = 17; ap = 1; break;
            case 1: hours = 18; ap = 1; break;
            case 2: hours = 19; ap = 1; break;
            case 3: hours = 20; ap = 1; break;
            case 4: hours = 21; ap = 1; break;
            case 5: hours = 22; ap = 1; break;
            case 6: hours = 23; ap = 1; break;
            default: break;
        }
    }
    hours = hours % 12;
    if(hours == 0)
    {
        hours = 12;
    }
    if(ap == 1)
    {
        sprintf(out, "%02d:%02d:%02d PM", hours, mins, sec);
    }
    else
    {
        sprintf(out, "%02d:%02d:%02d AM", hours, mins, sec);
    }
}
