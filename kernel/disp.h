#ifndef DISP_H
#define DISP_H

void initdisp(void* fb, unsigned int hres, unsigned int vres, unsigned int pitch);
void wchscr(const char c);
void wstrscr(const char* s);
void drawcheckerboardpattern(unsigned int color);
void clearscr();
void resetcon();

#endif