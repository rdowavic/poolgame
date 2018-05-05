#pragma once
#include <QJsonObject>

#include "abstractstagefactory.h"

class Game {
    std::vector<Ball*>* m_balls;
    Table* m_table;
public:
    ~Game();
    Game(std::vector<Ball*>* balls, Table* table) :
        m_balls(balls), m_table(table) {}
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
    int getMinimumWidth() const { return m_table->getWidth(); }
    /* how large the window's height should at least be */
    int getMinimumHeight() const { return m_table->getHeight(); }

    /**
     * @brief resolveCollision - modify the ball's velocity if it is colliding with the table
     * @param table - the table to be bounds checked
     * @param ball - the ball to move
     */
    void resolveCollision(const Table* table, Ball* ball);
    /**
     * @brief resolveCollision - resolve both ball's velocity whether these balls collide
     * @param ballA - first ball
     * @param ballB - second ball
     */
    void resolveCollision(Ball* ballA, Ball* ballB);
};
