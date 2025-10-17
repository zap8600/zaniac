#ifndef PIC_H
#define PIC_H

void remappic(int offset1, int offset2);
void setmaskpic(unsigned char irqline);
void clearmaskpic(unsigned char irqline);

#endif