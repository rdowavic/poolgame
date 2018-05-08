#pragma once
#include <QJsonObject>

#include "abstractstagefactory.h"
#include "table.h"

class Table;

class Game {
    std::vector<Ball*>* m_balls;
    Table* m_table;
    std::vector<Ball*>* m_dead_balls;
public:
    ~Game();
    Game(std::vector<Ball*>* balls, Table* table) :
        m_balls(balls), m_table(table),
        m_dead_balls(new std::vector<Ball*>()) {}
    /**
     * @brief Draws all owned objects to the screen (balls and table)
     * @param painter - qtpainter to blit to screen with
     */
    void render(QPainter& painter) const;
    /**
     * @brief Updates the positions of all objects within, based on how much time has changed
     * @param dt - time elapsed since last frame in seconds
     */
    void animate(double dt);

    /* how large the window's width should at least be */
    int getMinimumWidth() const;
    /* how large the window's height should at least be */
    int getMinimumHeight() const;

    /**
     * @brief resolveCollision - resolve both ball's velocity whether these balls collide
     * @param ballA - first ball
     * @param ballB - second ball
     */
    void resolveCollision(Ball* ballA, Ball* ballB);
    /**
     * @brief makeDead - once a ball has fallen into a table pocket, it needs to go
     * @param b - the ball to remove, it's gotta go, out the fuck here
     * Can also use this thing when a ball dies from getting broken.
     */
    void makeDead(Ball* b);
protected:
    /**
     * @brief deleteDeadBalls - in order to not fuck the for loop,
     * I kinda have to keep the balls 'alive' and inside the m_balls
     * vector. I'll also put them in this m_dead_balls vector so there will
     * actually be two pointers to the same ball. But at the end of an animation
     * rendering iteration, I will go and delete all the balls
     * that really did die in this iteration. Because I am scared to call
     * resolveCollision(Ball*, Ball*) with a dead pointer. They can also die
     * when they break and their children get released into the wild scene
     * that is this elementary pool game. Both situations cause me angst in that
     * I could dereference a null pointer. Therefore,
     * deleteDeadBalls will run at the END of a call to Game::animate().
     * :)
     */
    void deleteDeadBalls();
};
