#ifndef OBSTACLEDEADSTATE_HPP
#define OBSTACLEDEADSTATE_HPP

#include "CellState.hpp"

/**
 * @brief État représentant une cellule obstacle morte
 * 
 * Les cellules obstacles ne changent jamais d'état au cours de la simulation.
 */
class ObstacleDeadState : public CellState {
public:
    ObstacleDeadState() = default;
    ~ObstacleDeadState() override = default;
    
    bool isAlive() const override { return false; }
    bool isObstacle() const override { return true; }
    std::unique_ptr<CellState> clone() const override;
    char toChar() const override { return 'X'; }
    int toInt() const override { return 0; }
};

#endif // OBSTACLEDEADSTATE_HPP


