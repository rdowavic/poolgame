#pragma once

#include <QVector2D>
#include <QString>
#include <limits>

/* filename of the config file */
constexpr char config_path[] = "/Users/racheldowavic/INFO3220/Poolgame copy/config.json";

constexpr int animFrameMS = 16;
constexpr int drawFrameMS = 16;

/* default values for config file when keys are missing */
/** table **/
const double DEFAULT_TABLE_HEIGHT = 300.0;
const double DEFAULT_TABLE_WIDTH = 600.0;
const QString DEFAULT_TABLE_COLOUR = "green";
const double DEFAULT_TABLE_FRICTION = 0.01;
/** pockets **/
const double DEFAULT_POCKET_RADIUS = 15.0;

/** balls **/
const QString DEFAULT_BALL_COLOUR = "white";
const double DEFAULT_BALL_MASS = 1.0;
const double DEFAULT_BALL_STRENGTH = std::numeric_limits<double>::infinity();
const double DEFAULT_BALL_RADIUS = 10.0;
const double DEFAULT_BALL_XPOS = 0.0;
const double DEFAULT_BALL_YPOS = 0.0;
const double DEFAULT_BALL_XVEL = 0.0;
const double DEFAULT_BALL_YVEL = 0.0;

/** child ball visibility **/

//bool CHILDREN_BALLS_VISIBLE = true;
