#pragma once
#include <U8g2lib.h>

class SimpleDisplay
{
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C display;
    unsigned short currentY;
    unsigned short xOffset;
    unsigned short yOffset;

public:
    SimpleDisplay(
        int sdPin,
        int sclPin,
        const uint8_t *fontStyle = u8g2_font_5x8_tf,
        unsigned short xOffset = 2,
        unsigned short yOffset = 9
    );

    SimpleDisplay *write(String value);

    SimpleDisplay *render();

    SimpleDisplay *clean();
};