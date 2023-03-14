#include "RotatedPlanarCode.h"

RotatedPlanarCode::RotatedPlanarCode(int dist)
{
    distance = dist;
}

void RotatedPlanarCode::render(Engine &e)
{
    glm::vec3 translation(100, 100, 100);
    e.RenderQubit(translation);
}
