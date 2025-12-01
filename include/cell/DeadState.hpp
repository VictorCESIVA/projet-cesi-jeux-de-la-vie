#ifndef DEADSTATE_HPP
#define DEADSTATE_HPP

#include "CellState.hpp"

/**
 * @brief État représentant une cellule morte
 */
class DeadState : public CellState {
public:
    DeadState() = default;
    ~DeadState() override = default;
    
    bool isAlive() const override { return false; }
    bool isObstacle() const override { return false; }
    std::unique_ptr<CellState> clone() const override;
    char toChar() const override { return '.'; }
    int toInt() const override { return 0; }
};

#endif // DEADSTATE_HPP



