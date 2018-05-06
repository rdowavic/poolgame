#include "ball.h"
#include <iostream>

void StageOneBall::render(QPainter &painter) {
    // use our colour
    painter.setBrush(m_brush);
    // circle centered
    painter.drawEllipse(m_pos.toPointF(), m_radius, m_radius);
}

~StageTwoBall() {
    if (m_children != nullptr) {
        // this will invoke recursion - delete ball calls ~StageTwoBall()
        // so it's like a DFS deletion.
        for (StageTwoBall *ball : m_children) delete ball;
        delete m_children;
    }
}

void StageTwoBall::render(QPainter &painter) {
    painter.setBrush(m_brush);
    // circle centered
    painter.drawEllipse(m_pos.toPointF(), m_radius, m_radius);
    // draw children inside
    for (StageTwoBall* child : m_children) {
        child->render(painter);
    }
}
