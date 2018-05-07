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

void Table::resolveCollision(Ball *ball, Game* game) {
    QVector2D bPos = ball->getPosition();

    // resulting multiplicity of direction. If a component is set to -1, it
    // will flip the velocity's corresponding component
    QVector2D vChange(1,1);

    // ball is beyond left side of table's bounds
    if (bPos.x() - ball->getRadius() <= getX()) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() <= 0) vChange.setX(-1);
    // ball is beyond right side of table's bounds
    } else if (bPos.x() + ball->getRadius() >= getX() + getWidth()) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() >= 0) vChange.setX(-1);
    }
    // ball is above top of the table's bounds
    if (bPos.y() - ball->getRadius() <= getY()) {
        // flip iff we're travelling in the wrong dir
        if (ball->getVelocity().y() <= 0) vChange.setY(-1);
    // ball is beyond bottom of table's bounds
    } else if (bPos.y() + ball->getRadius() >= getY() + getHeight()) {
        // if we're moving down (we want to let the ball bounce up if its heading back)
        if (ball->getVelocity().y() >= 0) vChange.setY(-1);
    }

    ball->multiplyVelocity(vChange);
}

void StageTwoTable::resolveCollision(Ball *ball, Game* game) {
    Table::resolveCollision(ball, game);
    //check if it goes into a pocket on the table
    for (Pocket* pocket : *m_pockets) {
       //if fell into table
        if (fellInside(ball, pocket)) {
            game->removeBall(ball);
        }
    }
}

bool StageTwoTable::fellInside(Ball* ball, Pocket* pocket) {
    return pocket->getRadius() >= ball->getRadius()
           && ball->getPosition().distanceToPoint(pocket->getPosition())
           <= pocket->getRadius();
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

