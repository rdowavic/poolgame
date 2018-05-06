#pragma once
#include "abstractstagefactory.h"

class StageOneFactory : public AbstractStageFactory {
public:
    /**
     * @brief makeBall - construct a ball based on json
     * @param ballData - our json data for this ball
     * @return
     */
    virtual Ball* makeBall(const QJsonObject& ballData) override;

    /**
     * @brief makeTable - construct a table based on json
     * @param tableData - our json data for this table
     * @return
     */
    virtual Table* makeTable(const QJsonObject& tableData) override;
};

class StageTwoFactory : public AbstractStageFactory {
public:
    /**
     * @brief makeBall- returns a Stage 2 ball based on json input
     * @param ballData - our json data for this ball
     * @return pointer to a Ball object
     */
    virtual Ball* makeBall(const QJsonObject &ballData) override;

    /**
     * @brief makeTable - returns a Stage 2 Table (one with pockets)
     * based on json input
     * @param tableData
     * @return pointer to a Table object
     */
    virtual Table* makeTable(const QJsonObject &tableData) override;
private:
    StageTwoBall *makeChildBall(const QJsonObject &ballData, StageTwoBall *parent);
    bool properlyContained(StageTwoBall *child, StageTwoBall *parent);
};
