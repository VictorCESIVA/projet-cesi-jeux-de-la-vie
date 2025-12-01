#ifndef OBSTACLEALIVESTATE_HPP
#define OBSTACLEALIVESTATE_HPP

#include "CellState.hpp"

/**
 * @brief État représentant une cellule obstacle vivante
 * 
 * Les cellules obstacles ne changent jamais d'état au cours de la simulation.
 */
class ObstacleAliveState : public CellState {
public:
    ObstacleAliveState() = default;
    ~ObstacleAliveState() override = default;
    
    bool isAlive() const override { return true; }
    bool isObstacle() const override { return true; }
    std::unique_ptr<CellState> clone() const override;
    char toChar() const override { return '#'; }
    int toInt() const override { return 1; }
};

#endif // OBSTACLEALIVESTATE_HPP



