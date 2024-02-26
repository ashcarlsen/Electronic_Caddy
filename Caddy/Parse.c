#include "Parse.h"
#include <string.h>

void getGPGGA(char * totalBuffer, char* gpgga, unsigned int array_size)
{
	// Set entire sentence to nulls
	for(int i = 0; i < array_size; i++)
	{
		gpgga[i] = '\0';
	}
  char *delim = "$";
  char *ptr = strtok(totalBuffer, delim);
  while(ptr != NULL)
  {
    int len = strlen(ptr);
    if(ptr[0] == 'G' && ptr[1] == 'P' && ptr[2] == 'G' && ptr[3] == 'G' && ptr[4] == 'A' && ptr[len-1] == '\n')
    {
			if(array_size > len)
			{
				memcpy(gpgga, ptr, len);
			}
			else
			{
				memcpy(gpgga, ptr, array_size);
			} 
    }
    ptr = strtok(NULL, delim);
  }
}

int parseGPGGA(char *string, char **fields)
{
   int i = 0;
   fields[i++] = string;
   while ((i < GPGGA_FIELDS) && NULL != (string = strchr(string, ','))) {
      *string = '\0';
      fields[i++] = ++string;
   }
   return --i;
}

void getLatString(char* lat, int len, char* lats)
{
	for(int i = 0; i < 20; i++)
	{
		lats[i] = '\0';
	}
	lats[0] = 'L';
	lats[1] = 'a';
	lats[2] = 't';
	lats[3] = ':';
	for(int i = 0; i < len; i++)
	{
		lats[4+i] = lat[i];
	}
}
void getLonString(char* lon, int len, char* lons)
{
	lons[0] = 'L';
	lons[1] = 'o';
	lons[2] = 'n';
	lons[3] = ':';
	for(int i = 0; i < len; i++)
	{
		lons[4+i] = lon[i];
	}
}
void getAltString(char* alt, int len, char* alts)
{
	alts[0] = 'A';
	alts[1] = 'l';
	alts[2] = 't';
	alts[3] = ':';
	for(int i = 0; i < len; i++)
	{
		alts[4+i] = alt[i];
	}
}
void getTimeString(char* tim, int len, char* time)
{
	time[0] = 'T';
	time[1] = 'i';
	time[2] = 'm';
	time[3] = 'e';
	time[4] = ':';
	for(int i = 0; i < len; i++)
	{
		time[5+i] = tim[i];
	}
}