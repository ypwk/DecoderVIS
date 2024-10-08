#include "RotatedPlanarCode.h"

RotatedPlanarCode::RotatedPlanarCode(int dist) : distance(dist)
{
    for (int i = 0; i < dist * dist; i++) {
        dataQubits.push_back(Qubit{ i });
    }

    int mq_idx = 0;
    for (mq_idx; mq_idx < (dist - 1) * (dist - 1); mq_idx++) {
        Stabilizer ns = Stabilizer{ mq_idx, StabilizerType(((mq_idx + mq_idx / (dist - 1)) % 2 )), StabilizerState(0) };
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1));
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + 1);
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + dist);
        ns.qubits.push_back(mq_idx / (dist - 1) * dist + mq_idx % (dist - 1) + dist + 1);
        stabilizers.push_back(ns);
    }
    
    int vs_idx = 0;
    for (int i = 0; i < dist / 2; i++) {
        Stabilizer ns = Stabilizer{ mq_idx++, StabilizerType(1), StabilizerState(0) };
        ns.qubits.push_back(2 * i);
        ns.qubits.push_back(2 * i + 1);
        stabilizers.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerType(0), StabilizerState(0) };
        ns.qubits.push_back((dist - 1) + dist * 2 * i);
        ns.qubits.push_back((dist - 1) + dist * (2 * i + 1));
        stabilizers.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerType(0), StabilizerState(0) };
        ns.qubits.push_back(dist + dist * 2 * i);
        ns.qubits.push_back(dist + dist * (2 * i + 1));
        stabilizers.push_back(ns);

        ns = Stabilizer{ mq_idx++, StabilizerType(1), StabilizerState(0) };
        ns.qubits.push_back(dist * (dist - 1) + 1 + 2 * i);
        ns.qubits.push_back(dist * (dist - 1) + 1 + 2 * i + 1);
        stabilizers.push_back(ns);

        VirtualStabilizer vs = VirtualStabilizer{ vs_idx++, StabilizerType(0), StabilizerState(0) };
        vs.qubits.push_back(2 * i + 1);
        vs.qubits.push_back(2 * i + 2);
        virtualStabilizers.push_back(vs);

        vs = VirtualStabilizer{ vs_idx++, StabilizerType(1), StabilizerState(0) };
        vs.qubits.push_back((2 * dist - 1) + dist * 2 * i);
        vs.qubits.push_back((2 * dist - 1) + dist * (2 * i + 1));
        virtualStabilizers.push_back(vs);

        vs = VirtualStabilizer{ vs_idx++, StabilizerType(1), StabilizerState(0) };
        vs.qubits.push_back(dist * 2 * i);
        vs.qubits.push_back(dist * (2 * i + 1));
        virtualStabilizers.push_back(vs);

        vs = VirtualStabilizer{ vs_idx++, StabilizerType(0), StabilizerState(0) };
        vs.qubits.push_back(dist * (dist - 1) + 2 * i);
        vs.qubits.push_back(dist * (dist - 1) + 2 * i + 1);
        virtualStabilizers.push_back(vs);
    }

    // corner virtual stabilizers
    VirtualStabilizer vs = VirtualStabilizer{ vs_idx++, StabilizerType(0), StabilizerState(0) };
    vs.qubits.push_back(0);
    virtualStabilizers.push_back(vs);

    vs = VirtualStabilizer{ vs_idx++, StabilizerType(1), StabilizerState(0) };
    vs.qubits.push_back(dist * (dist - 1));
    virtualStabilizers.push_back(vs);

    vs = VirtualStabilizer{ vs_idx++, StabilizerType(1), StabilizerState(0) };
    vs.qubits.push_back(dist - 1);
    virtualStabilizers.push_back(vs);

    vs = VirtualStabilizer{ vs_idx++, StabilizerType(0), StabilizerState(0) };
    vs.qubits.push_back(dist * dist - 1);
    virtualStabilizers.push_back(vs);
}

void RotatedPlanarCode::render(Engine* e)
{
    for (int i = - distance / 2; i <= distance / 2; i++) {
        e->AddLine(glm::vec3(-distance / 2, i, 0) * RENDER_UNIT_LENGTH, glm::vec3(distance / 2, i, 0) * RENDER_UNIT_LENGTH, \
            10, LINE_COLOR);
        e->AddLine(glm::vec3(i, -distance / 2, 0) * RENDER_UNIT_LENGTH, glm::vec3(i, distance / 2, 0) * RENDER_UNIT_LENGTH, \
            10, LINE_COLOR);
    }

    for (int i = 0; i < stabilizers.size(); i++) {
        AddStabilizerToRender(stabilizers[i], e);
    }

    for (int i = 0; i < dataQubits.size(); i++) {
        AddQubitToRender(dataQubits[i], e);
    }

    for (int i = 0; i < virtualStabilizers.size(); i++) {
        AddVirtualStabilizerToRender(virtualStabilizers[i], e);
    }
}

void RotatedPlanarCode::AddStabilizerToRender(Stabilizer s, Engine* e)
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
        e->AddQuad(target + translation, 2 * QUBIT_SIZE_OUTER, 2 * QUBIT_SIZE_OUTER, 45.0f * (1 - s.state), LINE_COLOR);
        e->AddQuad(target + translation, 2 * QUBIT_SIZE_INNER, 2 * QUBIT_SIZE_INNER, 45.0f * (1 - s.state), STABILIZER_TYPE_AND_STATE_TO_COLOR[s.type * 2]);

    }
    else if (s.qubits.size() == 4) {
        // calculate location
        glm::vec3 target = (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]]) \
            + GetDataQubitLocation(dataQubits[s.qubits[2]]) + GetDataQubitLocation(dataQubits[s.qubits[3]])) * 0.25f;

        // draw quad
        e->AddQuad(target, RENDER_UNIT_LENGTH, RENDER_UNIT_LENGTH, 0, BACK_STAB);

        // draw stabilizer
        e->AddQuad(target, 2 * QUBIT_SIZE_OUTER, 2 * QUBIT_SIZE_OUTER, 45.0f * (1 - s.state), LINE_COLOR);
        e->AddQuad(target, 2 * QUBIT_SIZE_INNER, 2 * QUBIT_SIZE_INNER, 45.0f * (1 - s.state), STABILIZER_TYPE_AND_STATE_TO_COLOR[s.type * 2]);
    }
}

void RotatedPlanarCode::AddQubitToRender(Qubit q, Engine* e)
{
    glm::vec3 loc = GetDataQubitLocation(q);
    
    e->AddCircle(loc, QUBIT_SIZE_OUTER, LINE_COLOR);
    e->AddCircle(loc, QUBIT_SIZE_INNER, Q_STATE_TO_COLOR[(q.state.find('X') != std::string::npos) + 2 * (q.state.find('Z') != std::string::npos)]);
}

glm::vec3 RotatedPlanarCode::GetDataQubitLocation(Qubit q)
{
    return glm::vec3((q.index % distance - distance / 2) * RENDER_UNIT_LENGTH, (q.index / distance - distance / 2) * RENDER_UNIT_LENGTH, 0);
}

glm::vec3 RotatedPlanarCode::GetStabilizerLocation(AbstractStabilizer s)
{
    if (s.stab_class == 1) {
        glm::vec3 translation(0, 0, 0);
        if (s.qubits.size() == 1) {
            if (s.qubits[0] == 0) {// top or bottom
                translation.x = -RENDER_UNIT_LENGTH / 1.75f;
                translation.y = -RENDER_UNIT_LENGTH / 1.75f;
            }
            else if (s.qubits[0] == distance * distance - 1) {
                translation.x = RENDER_UNIT_LENGTH / 1.75f;
                translation.y = RENDER_UNIT_LENGTH / 1.75f;
            }
            else if (s.qubits[0] == distance - 1) {
                translation.x = RENDER_UNIT_LENGTH / 1.75f;
                translation.y = -RENDER_UNIT_LENGTH / 1.75f;
            }
            else {
                translation.x = -RENDER_UNIT_LENGTH / 1.75f;
                translation.y = RENDER_UNIT_LENGTH / 1.75f;
            }
            return GetDataQubitLocation(dataQubits[s.qubits[0]]) + translation;
        }
        else {
            if (s.qubits[1] - s.qubits[0] == 1) {// top or bottom
                if (s.qubits[0] > distance + 1) {
                    translation.y = RENDER_UNIT_LENGTH / 1.75f;
                }
                else {
                    translation.y = -RENDER_UNIT_LENGTH / 1.75f;
                }
            }
            else { // one of the sides
                if (s.qubits[0] % distance) {
                    translation.x = RENDER_UNIT_LENGTH / 1.75f;
                }
                else {
                    translation.x = -RENDER_UNIT_LENGTH / 1.75f;
                }
            }
            return (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]])) * 0.5f + translation;
        }
    }
    else if (s.stab_class == 0) {
        glm::vec3 translation(0, 0, 0);
        if (s.qubits.size() == 2) {
            if (s.qubits[1] - s.qubits[0] == 1) {// top or bottom
                if (s.qubits[0] > distance + 1) {
                    translation.y = RENDER_UNIT_LENGTH / 2.0f;
                }
                else {
                    translation.y = -RENDER_UNIT_LENGTH / 2.0f;
                }
            }
            else { // one of the sides
                if (s.qubits[0] % distance) {
                    translation.x = RENDER_UNIT_LENGTH / 2.0f;
                }
                else {
                    translation.x = -RENDER_UNIT_LENGTH / 2.0f;
                }
            }
            return (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]])) * 0.5f + translation;
        }
        else {
            return (GetDataQubitLocation(dataQubits[s.qubits[0]]) + GetDataQubitLocation(dataQubits[s.qubits[1]]) \
                + GetDataQubitLocation(dataQubits[s.qubits[2]]) + GetDataQubitLocation(dataQubits[s.qubits[3]])) * 0.25f;
        }
    }
}


void RotatedPlanarCode::AddVirtualStabilizerToRender(VirtualStabilizer v, Engine* e)
{
    glm::vec3 location = GetStabilizerLocation(v);

    // draw circle
    e->AddCircleArc(location, RENDER_UNIT_LENGTH / 2, \
        10, LINE_COLOR);
    e->AddCircle(location, RENDER_UNIT_LENGTH / 2, \
        BACK_STAB);

    // draw stabilizer
    e->AddQuad(location, 2 * QUBIT_SIZE_OUTER, 2 * QUBIT_SIZE_OUTER, 45.0f * (1 - v.state), LINE_COLOR);
    e->AddQuad(location, 2 * QUBIT_SIZE_INNER, 2 * QUBIT_SIZE_INNER, 45.0f * (1 - v.state), STABILIZER_TYPE_AND_STATE_TO_COLOR[v.type * 2]);
}