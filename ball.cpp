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

void StageTwoBall::addChild(StageTwoBall* child) {
    if (m_children == nullptr)
        m_children = new std::vector<Ball*>;
    m_children->push_back(child);
}

QVector2D StageTwoBall::getVelocity() {
    // if the ball is just a child,
    // then when asked about the velocity of this ball,
    // just report its parent's velocity (a real snitch move)
    if (m_parent != nullptr) {
        return m_parent->getVelocity();
    }
    // otherwise it's not a child, this is the default behaviour.
    return Ball::getVelocity();
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
