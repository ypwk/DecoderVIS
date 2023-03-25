#include "RotatedPlanarCode.h"

RotatedPlanarCode::RotatedPlanarCode(int dist) : distance(dist)
{
    for (int i = 0; i < dist * dist; i++) {
        dataQubits.push_back(Qubit{ i });
    }

    for (int i = 0; i < (dist - 1) * (dist - 1); i++) {
        measurementQubits.push_back(Stabilizer{i});
    }

    for (int i = 0; i < 4 * (dist / 2); i++) {

    }
}

void RotatedPlanarCode::render(Engine* e)
{
    for (int i = 0; i < dataQubits.size(); i++) {
        AddQubitToRender(dataQubits[i], NORMAL, e);
    }

    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0, glm::vec4(1, 1, 1, 0.1f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0, glm::vec4(1, 1, 1, 0.1f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0, glm::vec4(1, 1, 1, 0.1f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0, glm::vec4(1, 1, 1, 0.1f));

    //e->AddLine(glm::vec3(GenericCode::stabilizerSize, 0, 0), glm::vec3(-GenericCode::stabilizerSize, 0, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddLine(glm::vec3(GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), glm::vec3(-GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddLine(glm::vec3(GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), glm::vec3(-GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddLine(glm::vec3(0, GenericCode::stabilizerSize, 0), glm::vec3(0, -GenericCode::stabilizerSize, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddLine(glm::vec3(GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), glm::vec3(GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddLine(glm::vec3(-GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), glm::vec3(-GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), \
    //    10, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));


    ///*e->AddSemiCircle(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize, 0), GenericCode::stabilizerSize / 2, \
    //    0, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));*/
    //e->AddSemiCircle(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize, 0), GenericCode::stabilizerSize / 2, \
    //    0, glm::vec4(1, 1, 1, 0.1f));
    //e->AddSemiCircle(glm::vec3(GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize, 0), GenericCode::stabilizerSize / 2, \
    //    180, glm::vec4(1, 1, 1, 0.1f));
    //e->AddSemiCircle(glm::vec3(GenericCode::stabilizerSize, GenericCode::stabilizerSize / 2, 0), GenericCode::stabilizerSize / 2, \
    //    270, glm::vec4(1, 1, 1, 0.1f));
    //e->AddSemiCircle(glm::vec3(-GenericCode::stabilizerSize, - GenericCode::stabilizerSize / 2, 0), GenericCode::stabilizerSize / 2, \
    //    90, glm::vec4(1, 1, 1, 0.1f));
    //e->AddSemiCircleArc(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize, 0), GenericCode::stabilizerSize / 2, \
    //    10, 0, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddSemiCircleArc(glm::vec3(GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize, 0), GenericCode::stabilizerSize / 2, \
    //    10, 180, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddSemiCircleArc(glm::vec3(GenericCode::stabilizerSize, GenericCode::stabilizerSize / 2, 0), GenericCode::stabilizerSize / 2, \
    //    10, 270, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddSemiCircleArc(glm::vec3(-GenericCode::stabilizerSize, -GenericCode::stabilizerSize / 2, 0), GenericCode::stabilizerSize / 2, \
    //    10, 90, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));


    //AddQubitToRender(glm::vec3(0, 0, 0), NORMAL, e);
    //AddQubitToRender(glm::vec3(GenericCode::stabilizerSize, 0, 0), DATA_ERROR_X, e);
    //AddQubitToRender(glm::vec3(-GenericCode::stabilizerSize, 0, 0), TIME_ERROR, e);
    //AddQubitToRender(glm::vec3(0, GenericCode::stabilizerSize, 0), NORMAL, e);
    //AddQubitToRender(glm::vec3(0, -GenericCode::stabilizerSize, 0), DATA_ERROR_X, e);
    //AddQubitToRender(glm::vec3(GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), NORMAL, e);
    //AddQubitToRender(glm::vec3(-GenericCode::stabilizerSize, GenericCode::stabilizerSize, 0), NORMAL, e);
    //AddQubitToRender(glm::vec3(GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), NORMAL, e);
    //AddQubitToRender(glm::vec3(-GenericCode::stabilizerSize, -GenericCode::stabilizerSize, 0), NORMAL, e);

    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(0, 0, 1, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(1, 0, 0, 1.0f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(1, 0, 0, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(0, 0, 1, 1.0f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, -3 * GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(GenericCode::stabilizerSize / 2, -3 * GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(0, 0, 1, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, 3 * GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(-GenericCode::stabilizerSize / 2, 3 * GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(0, 0, 1, 1.0f));
    //e->AddQuad(glm::vec3(3 * GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(3 * GenericCode::stabilizerSize / 2, GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(1, 0, 0, 1.0f));
    //e->AddQuad(glm::vec3(-3 * GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    100, 100, 45, glm::vec4(133.0f / 256, 133.0f / 256, 133.0f / 256, 1.0f));
    //e->AddQuad(glm::vec3(-3 * GenericCode::stabilizerSize / 2, -GenericCode::stabilizerSize / 2, 0), \
    //    80, 80, 45, glm::vec4(1, 0, 0, 1.0f));
}

void RotatedPlanarCode::AddStabilizerToRender(Stabilizer s, StabilizerState ss, Engine* e)
{

}

void RotatedPlanarCode::AddQubitToRender(Qubit q, QubitState qs, Engine* e)
{
    glm::vec3 loc = GetQubitLocation(q);
    e->AddCircle(loc, 50.0f, LineColor);
    e->AddCircle(loc, 40.0f, QStateToColor[qs]);
}

glm::vec3 RotatedPlanarCode::GetQubitLocation(Qubit q)
{
    return glm::vec3((q.index % distance - distance / 2) * RENDER_UNIT_LENGTH, (q.index / distance - distance / 2) * RENDER_UNIT_LENGTH, 0);
}

