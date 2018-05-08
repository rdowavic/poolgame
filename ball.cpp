#include "ball.h"
#include <iostream>

void Ball::render(QPainter &painter) {
    // use our colour
    painter.setBrush(m_brush);
    // circle centered
    painter.drawEllipse(m_pos.toPointF(), m_radius, m_radius);
}

StageTwoBall::~StageTwoBall() {
        // this will invoke recursion - delete ball calls ~StageTwoBall()
        // so it's like a DFS deletion.
        for (StageTwoBall *ball : *m_children) delete ball;
        delete m_children;
}

void StageTwoBall::addChild(StageTwoBall* child) {
    m_children->push_back(child);
}

bool StageTwoBall::maybeBreakBall(QVector2D deltaV, std::vector<Ball*>* balls) {
    // make sure if it breaks, you set its parent to nullptr
    float energyOfCollision = m_mass * deltaV.lengthSquared();

    if (m_strength < energyOfCollision) {
        // ball should break and then disappear itself
        // the children need to graduate into m_balls array
        // I can do that based on the fact I receive true from this method,
        // and put that method inside Game!!
        // otherwise we get circular references and we are fucked
        // do not want to deal wit hthat shit again
        QVector2D preCollisionVelocity = getVelocity();
        float energyPerBall = energyOfCollision/numChildren();
        QVector2D pointOfCollision((-deltaV.normalized())*m_radius);
        // for each component ball
        for (auto it = m_children->begin(); it != m_children->end(); ++it) {
            float componentBallMass = (*it)->m_mass;
            QVector2D componentBallVelocity =
                    preCollisionVelocity
                    + sqrt(energyPerBall/componentBallMass)
                    * ((*it)->getPosition() - pointOfCollision).normalized();

            (*it)->setVelocity(componentBallVelocity);
            // fix its position, shouldn't be relative to the parent anymore
            (*it)->m_pos = (*it)->getPosition();
            // maybe if I graduate it here, nothing fucked will happen
            // it does mean other balls will check if they collided with them,
            // and why not? Live and let live bro, chuck in that m_balls
            // array and mutate the fuck out of it
            // (lol)
            // deleting reference to parent
            (*it)->m_parent = nullptr;
            // pushing it onto our balls array (fucking sneaK)
            balls->push_back(*it);
        }
        return true;
    }
    return false;
}

double StageTwoBall::getMass() const {
    double result = m_mass;

    for (StageTwoBall* child : *m_children)
        result += child->getMass();
    // absolutely gorge, so happy with this

    return result;
}

QVector2D StageTwoBall::getVelocity() const {
    // if the ball is just a child,
    // then when asked about the velocity of this ball,
    // just report its parent's velocity (a real snitch move)
    if (m_parent != nullptr) {
        return m_parent->getVelocity();
    }
    // otherwise it's not a child, this is the default behaviour.
    return Ball::getVelocity();
}

QVector2D StageTwoBall::getPosition() const {
    if (m_parent != nullptr) {
        return m_parent->getPosition() + m_pos;
    }
    return Ball::getPosition();
}

void StageTwoBall::render(QPainter &painter) {
    painter.setBrush(m_brush);
    // circle centered
    painter.drawEllipse(getPosition().toPointF(), m_radius, m_radius);
    // draw children inside if they exist
    for (StageTwoBall* child : *m_children) {
        child->render(painter);
    }
}
