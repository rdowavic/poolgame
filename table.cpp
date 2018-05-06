#include "table.h"

void StageOneTable::render(QPainter &painter) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(
       this->getX(),
       this->getY(),
       this->getWidth(),
       this->getHeight()
    );
}

void StageTwoTable::~StageTwoTable() {
    for (auto pocket : *m_pockets) delete pocket;
    delete m_pockets;
}

Pocket::~Pocket() {
    delete m_ball;
}

Pocket::Pocket(int radius, QVector2D position)
    : m_ball(
          new StageOneBall(QColor("black"),
          QVector2D(position),
          QVector2D(0.0, 0.0),
          0,
          radius)) {}

void Pocket::render(QPainter &painter) {
    m_ball->render(painter);
}

QVector2D Pocket::getRadius() const {
    return m_ball->getRadius();
}

QVector2D Pocket::getPosition() const {
    return m_ball->getPosition();
}

