#pragma once

#include <QColor>
#include <QPainter>
#include "ball.h"
#include "game.h"

class Game;

class Table {
protected:
    int m_width;
    int m_height;
    int m_xpos;
    int m_ypos;
    QBrush m_brush;
    double m_friction;
public:
    virtual ~Table() {}
    Table(int width, int height, QColor colour, double friction, int xpos, int ypos) :
        m_width(width), m_height(height), m_xpos(xpos), m_ypos(ypos),
        m_brush(colour), m_friction(friction) {}
    /**
     * @brief render - draw the table to screen using the specified painter
     * @param painter - painter to use
     */
    virtual void render(QPainter& painter);

    /**
     * @brief resolveCollision - reacts to the situation when a ball touches the side of the table, or interacts
     * in some way with the table's components
     * @return void
     */
    virtual void resolveCollision(Ball* ball, Game* game);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getX() const { return m_xpos; }
    int getY() const { return m_ypos; }
    double getFriction() const { return m_friction; }
};

class StageOneTable : public Table {
public:
    StageOneTable(int width, int height, QColor colour, double friction) :
        // table isn't translated anywhere - starts at (0,0)
        Table(width, height, colour, friction, 0, 0) {}
};

/**
 * @brief The Pocket class
 * An example of Object Adaptor - "A Pocket is kind of like a Ball,
 * but with more restrictions".
 */
class Pocket {
public:
    ~Pocket();
    Pocket(int radius, QVector2D position);
    void render(QPainter &painter);
    double getRadius() const;
    QVector2D getPosition() const;

protected:
    Ball* m_ball;
};

/**
 * @brief The StageTwoTable class
 * A Stage Two Table is graced with the possibility of pockets - So this is just
 * the same as a Stage One Table but with a vector of pockets chucked in!
 */
class StageTwoTable : public Table {
public:
    StageTwoTable(int width, int height, QColor colour, double friction, std::vector<Pocket*>* pockets) :
        Table(width, height, colour, friction, 0, 0), m_pockets(pockets) {}
    ~StageTwoTable();

    /**
     * @brief render - draw the stagetwotable to screen with pockets
     * @param painter - the painter to use
     */
    void render(QPainter &painter) override;

    /**
     * @brief resolveCollision checks whether the ball has fallen into some pocket on the table, or has hit
     * the side of the table. Will update velocity/positions accordingly.
     * @param ball - the ball to check whether it has fallen inside a pocket or hit the side of the table
     */
    virtual void resolveCollision(Ball* ball, Game* game) override;
protected:
    bool fellInside(Ball* ball, Pocket* pocket);
protected:
    std::vector<Pocket*>* m_pockets;
};
