#ifndef ALIVESTATE_HPP
#define ALIVESTATE_HPP

#include "CellState.hpp"

/**
 * @brief État représentant une cellule vivante
 */
class AliveState : public CellState {
public:
    AliveState() = default;
    ~AliveState() override = default;
    
    bool isAlive() const override { return true; }
    bool isObstacle() const override { return false; }
    std::unique_ptr<CellState> clone() const override;
    char toChar() const override { return 'O'; }
    int toInt() const override { return 1; }
};

#endif // ALIVESTATE_HPP



