#define eeee int
#define x <
#define ee main
#define eeeeeee (
#define cc =
#define e {
#define eeeee }
#define a printf
#define eeeeee ;
#include x stdio.h>
#define aa )


eeee
ee
eeeeeee
aa
e
        unsigned char s[] = e 0xbb, 0x50, 0x85, 0x11, 0x52, 0x92, 0xa4, 0x2a,0x1, 0xa7, 0x90, 0x51, 0xc7, 0x2, 0x9f, 0x9b,0x6a
    eeeee eeeeee

    for (unsigned eeee m = 0 eeeeee m < sizeof(s) eeeeee ++m)
    e
        unsigned char c = s[m] eeeeee
        c ^= m eeeeee
        c += m;
        c ^= m eeeeee
        c -= 0x2b;
        c cc ~c;
        c ^= 0x36;
        c cc ~c;
        c -= 0x8;
        c cc (c >> 0x3) | (c << 0x5);
        c ^= m;
        c += 0xd5 eeeeee
        c cc ~c;
        c += m;
        c ^= m;
        c -= m;
        s[m] cc c eeeeee
    }
    a eeeeeee"%s\n", s aa eeeeee
	getchar();
eeeee