#pragma once

#include <QPoint>
#include <QPainter>
#include <QVector2D>
#include <math.h>

class Ball {
protected:
    QBrush m_brush;
    QVector2D m_pos;
    QVector2D m_velocity;
    double m_mass;
    int m_radius;
public:
    virtual ~Ball() {}
    Ball(QColor colour, QVector2D position,
         QVector2D velocity, double mass, int radius) :
        m_brush(colour), m_pos(position), m_velocity(velocity),
        m_mass(mass), m_radius(radius) {}

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    virtual void render(QPainter &painter);
    /**
     * @brief translate - Move the ball's position by provided vector
     * @param vec - vector
     */
    virtual void translate(QVector2D vec) { m_pos += vec; }

    virtual QVector2D getVelocity() const { return m_velocity; }
    /**
     * @brief changeVelocity - modify speed by a constant amount
     * @param delta - change in velocity (x,y)
     */
    void changeVelocity(const QVector2D& delta) { m_velocity += delta; }
    /**
     * @brief multiplyVelocity - apply vector multiplicatively
     * @param vel - vector
     */
    void multiplyVelocity(const QVector2D& vel) { m_velocity *= vel; }
    virtual double getMass() const { return m_mass; }
    void setVelocity(QVector2D vel) { m_velocity = vel; }
    double getRadius() const { return m_radius; }
    QString getColour() const { return m_brush.color().name(); }
    virtual QVector2D getPosition() const { return m_pos; }
    virtual bool maybeBreakBall(QVector2D deltaV, std::vector<Ball*>* balls) { return false; }
};

class StageOneBall : public Ball {
public:
    StageOneBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius) :
        Ball(colour, position, velocity, mass, radius) {}
};

class StageTwoBall : public Ball {
public:
    StageTwoBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius,
                 StageTwoBall* parent, double strength)
        : Ball(colour, position, velocity, mass, radius),
          m_parent(parent), m_strength(strength) {}
    ~StageTwoBall();
    virtual QVector2D getVelocity() const override;
    virtual double getMass() const override;
    virtual QVector2D getPosition() const override;
    virtual void render(QPainter& painter) override;
    /**
     * @brief addChild - add a child ball to the given StageTwoBall
     * @param child - the child ball to add
     */
    void addChild(StageTwoBall* child);
    /**
     * @brief numChildren - return the number of children this Ball has
     * @return the number of children
     */
    int numChildren() { return m_children->size(); }
    virtual bool maybeBreakBall(QVector2D deltaV, std::vector<Ball *> *balls) override;
protected:
    std::vector<StageTwoBall*>* m_children = new std::vector<StageTwoBall*>();
    StageTwoBall* m_parent = nullptr; // do not own this, don't attempt to delete
    double m_strength;

};
