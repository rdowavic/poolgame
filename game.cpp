#include "game.h"
#include "utils.h"
#include "table.h"

#include <QJsonArray>
#include <stdexcept>
#include <cmath>

#include <iostream>

/* how large the window's width should at least be */
int Game::getMinimumWidth() const { return m_table->getWidth(); }
/* how large the window's height should at least be */
int Game::getMinimumHeight() const { return m_table->getHeight(); }

Game::~Game() {
    // cleanup ya boi
    for (auto b : *m_balls) delete b;
    delete m_balls;
    delete m_table;
}

void Game::render(QPainter &painter) const {
    // table is rendered first, as its the lowest
    m_table->render(painter);
    // then render all the balls
    for (Ball* b : *m_balls) b->render(painter);
}

void Game::animate(double dt) {
    // (test) collide the ball with each other ball exactly once
    // to achieve this, balls only check collisions with balls "after them"
    for (auto it = m_balls->begin(); it != m_balls->end(); ++it) {
        Ball* ballA = *it;
        // correct ball velocity if colliding with table
        m_table->resolveCollision(ballA, this);

        // check collision with all later balls
        for (auto nestedIt = it + 1; nestedIt != m_balls->end(); ++nestedIt) {
            Ball* ballB = *nestedIt;
            resolveCollision(ballA, ballB);
        }
        // move ball due to speed
        ballA->translate(ballA->getVelocity() * dt);
        // apply friction
        ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt);
    }
}

/**
 * @brief Game::removeBall deletes the given ball pointed to by b from the vector m_balls
 * @param b - the ball to remove from the game
 */
void Game::removeBall(Ball *b) {
    for (auto it = m_balls->begin(); it != m_balls->end(); ++it) {
        if (*it == b) {
            m_balls->erase(it);
            delete b;
        }
    }
}

void Game::resolveCollision(Ball* ballA, Ball* ballB) {
    // SOURCE : ASSIGNMENT SPEC

    // if not colliding (distance is larger than radii)
    QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
    if (collisionVector.length() > ballA->getRadius() + ballB->getRadius()) return;
    collisionVector.normalize();

    float mr = ballB->getMass() / ballA->getMass();
    double pa = QVector2D::dotProduct(collisionVector, ballA->getVelocity());
    double pb = QVector2D::dotProduct(collisionVector, ballB->getVelocity());

    if (pa <= 0 && pb >= 0) return;

    double a = -(mr + 1);
    double b = 2*(mr * pb + pa);
    double c = -((mr - 1)*pb*pb + 2*pa*pb);
    double disc = sqrt(b*b - 4*a*c);
    double root = (-b + disc)/(2*a);
    if (root - pb < 0.01) {
        root = (-b - disc)/(2*a);
    }

    ballA->changeVelocity(mr * (pb - root) * collisionVector);
    ballB->changeVelocity((root-pb) * collisionVector);
}
