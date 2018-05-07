#include "table.h"

void Table::render(QPainter &painter) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(
       getX(),
       getY(),
       getWidth(),
       getHeight()
    );
}

void StageTwoTable::render(QPainter &painter) {
    // this renders the table in the old fashioned style tick tick
    Table::render(painter);
    // this gives us our pockety pockets sitting on the table
    for (Pocket* p : *m_pockets) p->render(painter);
}

StageTwoTable::~StageTwoTable() {
    for (auto pocket : *m_pockets) delete pocket;
    delete m_pockets;
}

Pocket::~Pocket() { delete m_ball; }

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

double Pocket::getRadius() const {
    return m_ball->getRadius();
}

QVector2D Pocket::getPosition() const {
    return m_ball->getPosition();
}

