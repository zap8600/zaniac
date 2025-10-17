#ifndef DISP_H
#define DISP_H

void initdisp(unsigned int hr, unsigned int vr, unsigned int p);
void wchscr(const char c);
void wstrscr(const char* s);
void drawcheckerboardpattern(unsigned int color);
void clearscr();
void resetcon();

#endif