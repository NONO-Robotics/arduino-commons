#include "SimpleDisplay.h"

SimpleDisplay::SimpleDisplay(
    int sdPin,
    int sclPin,
    const uint8_t *fontStyle,
    unsigned short xOffset,
    unsigned short yOffset
    ) : display(U8G2_R0, U8X8_PIN_NONE, sclPin, sdPin)
{
    display.begin();
    display.setFont(fontStyle);
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    clean();
}

SimpleDisplay *SimpleDisplay::write(String value)
{
    currentY += yOffset;
    display.drawStr(this->xOffset, currentY, value.c_str());
    return this;
}

SimpleDisplay *SimpleDisplay::render()
{
    display.sendBuffer();
    return this;
}

SimpleDisplay *SimpleDisplay::clean()
{
    display.clearBuffer();
    currentY = 0;
    return this;
}