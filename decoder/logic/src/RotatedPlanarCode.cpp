#include "RotatedPlanarCode.h"

RotatedPlanarCode::RotatedPlanarCode(int dist) : distance(dist)
{
    for (int i = 0; i < dist * dist; i++) {
        dataQubits.push_back(Qubit{ i });
    }

    int mq_idx = 0;
    for (mq_idx; mq_idx < (dist - 1) * (dist - 1); mq_idx++) {
        Stabilizer ns = Stabilizer{ mq_idx, StabilizerState(((mq_idx + mq_idx / (dist - 1)) % 2 ))};
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1));
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + 1);
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + dist);
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + dist + 1);
        measurementQubits.push_back(ns);
    }
    
    for (int i = 0; i < dist / 2; i++) {
        Stabilizer ns = Stabilizer{ mq_idx++, StabilizerState((mq_idx + i) % 2)};
        ns.qubits.push_back(2 * i);
        ns.qubits.push_back(2 * i + 1);
        measurementQubits.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerState((mq_idx + i) % 2) };
        ns.qubits.push_back((dist - 1) + dist * 2 * i);
        ns.qubits.push_back((dist - 1) + dist * (2 * i + 1));
        measurementQubits.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerState((mq_idx + i) % 2) };
        ns.qubits.push_back(dist + dist * 2 * i);
        ns.qubits.push_back(dist + dist * (2 * i + 1));
        measurementQubits.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerState((mq_idx + i) % 2) };
        ns.qubits.push_back(dist * (dist - 1) + 1 + 2 * i);
        ns.qubits.push_back(dist * (dist - 1) + 1 + 2 * i + 1);
        measurementQubits.push_back(ns);
    }
}

void RotatedPlanarCode::render(Engine* e)
{
    for (int i = - distance / 2; i <= distance / 2; i++) {
        e->AddLine(glm::vec3(-distance / 2, i, 0) * RENDER_UNIT_LENGTH, glm::vec3(distance / 2, i, 0) * RENDER_UNIT_LENGTH, \
            10, LINE_COLOR);
        e->AddLine(glm::vec3(i, -distance / 2, 0) * RENDER_UNIT_LENGTH, glm::vec3(i, distance / 2, 0) * RENDER_UNIT_LENGTH, \
            10, LINE_COLOR);
    }

    for (int i = 0; i < measurementQubits.size(); i++) {
        AddStabilizerToRender(measurementQubits[i], StabilizerState(i % 2), e);
    }

    for (int i = 0; i < dataQubits.size(); i++) {
        AddQubitToRender(dataQubits[i], NORMAL, e);
    }
}

void RotatedPlanarCode::AddStabilizerToRender(Stabilizer s, StabilizerState ss, Engine* e)
{
    if (s.qubits.size() == 2) {
        // calculate location
        glm::vec3 target = (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]])) * 0.5f;

        float orientation = 180;
        glm::vec3 translation(0, 0, 0);
        // calculate orientation
        if (s.qubits[1] - s.qubits[0] == 1) {// top or bottom
            if (s.qubits[0] > distance + 1){ 
                orientation = 0;
                translation.y = RENDER_UNIT_LENGTH / 2.0f;
            }
            else {
                translation.y = -RENDER_UNIT_LENGTH / 2.0f;
            }
        }
        else { // one of the sides
            if (s.qubits[0] % distance) {
                orientation = 270;
                translation.x = RENDER_UNIT_LENGTH / 2.0f;
            }
            else {
                orientation = 90;
                translation.x = -RENDER_UNIT_LENGTH / 2.0f;
            }
                
        }
        // draw semicircle
        e->AddSemiCircleArc(target, RENDER_UNIT_LENGTH / 2, \
        10, orientation, LINE_COLOR);
        e->AddSemiCircle(target, RENDER_UNIT_LENGTH / 2, \
        orientation, BACK_STAB);

        // draw stabilizer
        e->AddQuad(target + translation, 2 * QUBIT_SIZE_OUTER, 2 * QUBIT_SIZE_OUTER, 45, LINE_COLOR);
        e->AddQuad(target + translation, 2 * QUBIT_SIZE_INNER, 2 * QUBIT_SIZE_INNER, 45, STABILIZER_STATE_TO_COLOR[s.type]);

    }
    else if (s.qubits.size() == 4) {
        // calculate location
        glm::vec3 target = (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]]) \
            + GetDataQubitLocation(dataQubits[s.qubits[2]]) + GetDataQubitLocation(dataQubits[s.qubits[3]])) * 0.25f;

        // draw quad
        e->AddQuad(target, RENDER_UNIT_LENGTH, RENDER_UNIT_LENGTH, 0, BACK_STAB);

        // draw stabilizer
        e->AddQuad(target, 2 * QUBIT_SIZE_OUTER, 2 * QUBIT_SIZE_OUTER, 45, LINE_COLOR);
        e->AddQuad(target, 2 * QUBIT_SIZE_INNER, 2 * QUBIT_SIZE_INNER, 45, STABILIZER_STATE_TO_COLOR[s.type]);

    }
}

void RotatedPlanarCode::AddQubitToRender(Qubit q, QubitState qs, Engine* e)
{
    glm::vec3 loc = GetDataQubitLocation(q);
    e->AddCircle(loc, QUBIT_SIZE_OUTER, LINE_COLOR);
    e->AddCircle(loc, QUBIT_SIZE_INNER, Q_STATE_TO_COLOR[qs]);
}

glm::vec3 RotatedPlanarCode::GetDataQubitLocation(Qubit q)
{
    return glm::vec3((q.index % distance - distance / 2) * RENDER_UNIT_LENGTH, (q.index / distance - distance / 2) * RENDER_UNIT_LENGTH, 0);
}

