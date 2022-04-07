#include  <msp430x43x.h>

#define Back_Light_On   P6OUT &= 0xFE
#define Back_Light_Off  P6OUT |= 0x01

#define FDC1004_SDA_IN    P2DIR&=0x7F

#define FDC1004_SDA_OUT   P2DIR|=0x80
#define FDC1004_SCL_OUT   P2DIR|=0x40

#define FDC1004_SDA_HIG   P2OUT |= 0x80
#define FDC1004_SCL_HIG   P2OUT |= 0x40

#define FDC1004_SDA_LOW   P2OUT &= 0x7F
#define FDC1004_SCL_LOW   P2OUT &= 0xBF

#define NO_KEY           0
#define ON_OFF_DOWN      1
#define ON_OFF_UP        2
#define BL_SET_DOWN      3
#define BL_SET_UP        4
#define STORE_SET_DOWN   5
#define STORE_SET_UP     6
#define UPLOAD_SET_DOWN  7
#define UPLOAD_SET_UP    8
#define ZERO_SET_DOWN    9
#define ZERO_SET_UP      10
#define ENTER_SET_DOWN   11
#define ENTER_SET_UP     12