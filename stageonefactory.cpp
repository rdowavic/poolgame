#include "stageonefactory.h"
#include "utils.h"
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineF>

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
    StageTwoBall* ball = makeChildBall(ballData, nullptr);
    // but its velocity won't be set, so set it
    QJsonObject tVel = ballData.value("velocity").toObject();
    double xvel = tVel.value("x").toDouble();
    double yvel = tVel.value("y").toDouble();

    ball->setVelocity(QVector2D(xvel, yvel));

    return ball;
}

StageTwoBall* StageTwoFactory::makeChildBall(const QJsonObject &ballData, StageTwoBall* parent) {
    // child balls do not have a 'velocity field'

    // for m_brush
    QString defaultColour = DEFAULT_BALL_COLOUR;
    if (parent != nullptr) defaultColour = parent->getColour();

    QString col = ballData.value("colour").toString(defaultColour);

    // for m_pos
    QJsonObject tPos = ballData.value("position").toObject();
    double xpos = tPos.value("x").toDouble();
    double ypos = tPos.value("y").toDouble();

    // for mass and radius
    double mass = ballData.value("mass").toDouble(DEFAULT_BALL_MASS);
    double radius = ballData.value("radius").toDouble(DEFAULT_BALL_RADIUS);

    // make the result now so that we can
    // give the child ball (if there is one) a parent to refer to
    StageTwoBall* result = new StageTwoBall (
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
    // create a stage one table based on the fed in json data
    QString colour = tableData.value("colour").toString();

    // extract width and height from json vals
    QJsonObject tSize = tableData.value("size").toObject();
    double width = tSize.value("x").toDouble();
    double height = tSize.value("y").toDouble();

    double friction = tableData.value("friction").toDouble();

    // an empty pocketResults array that will get filled
    // as long as the key exists in the thing
    std::vector<Pocket*>* pocketResults = new std::vector<Pocket*>;

    if (tableData.contains("pockets")) {
        for (auto pocket : tableData.value("pockets").toArray()) {
            Pocket* p = makePocket(pocket.toObject());
            pocketResults->push_back(p);
        }
    }

    return new StageTwoTable (
       width, height,
       QColor(colour), friction,
       pocketResults
    );
}

Pocket* StageTwoFactory::makePocket(const QJsonObject& pocketData) {
    // check out what data pocket has
    // position key
    // radius key (maybe)
    QJsonObject position = pocketData.value("position").toObject();
    double x_pos = position.value("x").toDouble();
    double y_pos = position.value("y").toDouble();

    double radius = DEFAULT_POCKET_RADIUS;

    if (pocketData.contains("radius"))
        radius = pocketData.value("radius").toDouble();

    return new Pocket(radius, QVector2D(x_pos, y_pos));
}

bool StageTwoFactory::properlyContained(StageTwoBall* child, StageTwoBall* parent) {
    /* If you take the difference between their two positions, and then add
     * the baby's radius size,
     * this should be STRICTLY LESS than the parent's
     * radius size.
     */
    return child->getPosition()
            .distanceToPoint (
                parent->getPosition()
             )
           + child->getRadius()
           <= parent->getRadius();
}





