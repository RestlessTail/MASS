#include "Button.h"

Button::Button()
{
    disabled = false;
    responseRange = {0};
}

bool Button::isClicked(float x, float y)
{
    if (x < responseRange.topLeft.x || x > responseRange.bottomRight.x) {
        return false;
    }
    if (y > responseRange.topLeft.y || y < responseRange.bottomRight.y) {
        return false;
    }
    return true;
}

void Button::setResponseRange(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight)
{
    responseRange.topLeft = topLeft;
    responseRange.topRight = topRight;
    responseRange.bottomLeft = bottomLeft;
    responseRange.bottomRight = bottomRight;
}

void Button::useDefaultResponseRange()
{
    float* v = getVertex();
    memcpy(&responseRange.topLeft, v, 3 * sizeof(float));
    memcpy(&responseRange.topRight, v + 5, 3 * sizeof(float));
    memcpy(&responseRange.bottomLeft, v + 10, 3 * sizeof(float));
    memcpy(&responseRange.bottomRight, v + 15, 3 * sizeof(float));
}
