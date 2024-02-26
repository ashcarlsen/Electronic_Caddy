#ifndef PARSE_H
#define PARSE_H
#define GPGGA_FIELDS 15

void getGPGGA(char * totalBuffer, char* gpgga, unsigned int array_size);
int parseGPGGA(char *string, char **fields);
void getLatString(char* lat, int len, char* lats);
void getLonString(char* lon, int len, char* lons);
void getAltString(char* alt, int len, char* alts);
void getTimeString(char* tim, int len, char* time);

#endif