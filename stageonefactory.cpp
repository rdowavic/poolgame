#include "stageonefactory.h"
#include <QJsonObject>

Ball* StageOneFactory::makeBall(const QJsonObject& ballData) {
    // construct a ball from its values

    QString col = ballData.value("colour").toString();

    // extract pos into two doubles
    QJsonObject tPos = ballData.value("position").toObject();
    double xpos = tPos.value("x").toDouble();
    double ypos = tPos.value("y").toDouble();

    // extract velocity into two doubles
    QJsonObject tVel = ballData.value("velocity").toObject();
    double xvel = tVel.value("x").toDouble();
    double yvel = tVel.value("y").toDouble();

    double mass = ballData.value("mass").toDouble();
    double radius = ballData.value("radius").toDouble();

    return new StageOneBall(
       QColor(col),
       QVector2D(xpos, ypos),
       QVector2D(xvel, yvel),
       mass,
       radius
     );
}
Table* StageOneFactory::makeTable(const QJsonObject& tableData) {
    // create a stage one table based on the fed in json data
    QString colour = tableData.value("colour").toString();

    // extract width and height from json vals
    QJsonObject tSize = tableData.value("size").toObject();
    double width = tSize.value("x").toDouble();
    double height = tSize.value("y").toDouble();

    double friction = tableData.value("friction").toDouble();

    return new StageOneTable(width, height, QColor(colour), friction);
}

Ball* StageTwoFactory::makeBall(const QJsonObject &ballData) {
    // construct a ball from its values
    // how to deal with composite shit?? Have to think about that right now? Damn.
    // TODO!!

    QString col = ballData.value("colour").toString();

    // extract pos into two doubles
    QJsonObject tPos = ballData.value("position").toObject();
    double xpos = tPos.value("x").toDouble();
    double ypos = tPos.value("y").toDouble();

    // extract velocity into two doubles
    QJsonObject tVel = ballData.value("velocity").toObject();
    double xvel = tVel.value("x").toDouble();
    double yvel = tVel.value("y").toDouble();

    double mass = ballData.value("mass").toDouble();
    double radius = ballData.value("radius").toDouble();

    // for blah in balls
    // makeChildBall

    return new StageTwoBall(
      QColor(col),
      QVector2D(xpos, ypos),
      QVector2D(xvel, yvel),
      mass,
      radius,
      nullptr
    );
}

StageTwoBall* StageTwoFactory::makeChildBall(const QJsonObject &ballData, StageTwoBall* parent) {
    // child balls do not have a 'velocity field'

    // for m_brush
    QString col = ballData.value("colour").toString();

    // for m_pos
    QJsonObject tPos = ballData.value("position").toObject();
    double xpos = tPos.value("x").toDouble();
    double ypos = tPos.value("y").toDouble();

    // for mass and radius
    double mass = ballData.value("mass").toDouble();
    double radius = ballData.value("radius").toDouble();

    // make the result now so that we can
    // give the child ball (if there is one) a parent to refer to
    StageTwoBall* result = new StageTwoBall(
      QColor(col),
      QVector2D(xpos, ypos),
      QVector2D(0, 0),
      mass,
      radius,
      parent
    );
    // they may or may not have a 'balls' field
    if (ballData.contains("balls")) {
        // then this ball has children, make them
        QJsonArray childBallData = ballData.value("balls").toArray();

        for (const auto& ball : childBallData) {
            // check if it's actually a valid coordinate
            StageTwoBall* child = makeChildBall(ball.toObject(), result);

            if (properlyContained(child, result))
                result->addChild(child);
            // otherwise ignore that whole thing going on there
        }
    }
    // ###### Return the Resultant Ball with #########
    // ###### all its children constructed nicely #########
    return result;
}

Table* StageTwoFactory::makeTable(const QJsonObject &tableData) {
    //construct a Table that has pockets
    Table* table = StageOneFactory::makeTable();
    //TODO
}

bool StageTwoFactory::properlyContained(StageTwoBall* child, StageTwoBall* parent) {
    /* If you take the difference between their two positions, and then add
     * the baby's radius size,
     * this should be STRICTLY LESS than the parent's
     * radius size.
     */
    return child->getPosition()
            .distanceToPoint(
                parent->getPosition()
             )
           + child->getRadius()
           < parent->getRadius();
}





