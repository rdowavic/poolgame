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
        // or if stage 2, deletes the ball... omg
        // maybe I should have a batchDelete at the end of one animation
        // so that I don't get issues with like null pointers
        // because ballA may not be a valid pointer after resolveCollision
        // is called, and yet I call resolveCollision(ballA, ballB)
        // before the thing is over
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
    deleteDeadBalls();
}

/**
 * @brief Game::makeDead deletes the given ball pointed to by b from the vector m_balls
 * @param b - the ball to remove from the game
 */
void Game::makeDead(Ball *b) {
    m_dead_balls->push_back(b);
}

void Game::deleteDeadBalls() {

    for (auto it = m_dead_balls->begin(); it != m_dead_balls->end(); ++it) {
        // for each dead ball, go through and find a match with
        // the one sitting inside the array that's well actually the same
        // element
        for (auto jt = m_balls->begin(); jt != m_balls->end(); ++jt) {
            if (*it == *jt) {
                // it is the ball inside dead_balls
                // jt is the ball inside m_balls
                m_balls->erase(jt);
                break; // ie stop looking for that shit
            }
        }
    }
    // actually deleting the balls from memory
    for (Ball* b : *m_dead_balls) delete b;
    m_dead_balls->clear();
}

void Game::resolveCollision(Ball* ballA, Ball* ballB) {
    // SOURCE : ASSIGNMENT SPEC

    // if not colliding (distance is larger than radii)
    QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
    if (collisionVector.length() > ballA->getRadius() + ballB->getRadius()) return;

    // at this point, ball B and A definitely collided.
    // we need to check just about here whether either of them will break.

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

    QVector2D deltaVa = mr * (pb - root) * collisionVector;
    QVector2D deltaVb = (root-pb) * collisionVector;

    // now call maybeBreakBall()
    // remember to changeVelocity(deltaVa) if it DIDN'T BREAK
    if (ballA->maybeBreakBall(deltaVa, m_balls))
        makeDead(ballA);
    else
        ballA->changeVelocity(deltaVa);

    if (ballB->maybeBreakBall(deltaVb, m_balls))
        makeDead(ballB);
    else
        ballB->changeVelocity(deltaVb);
}
