#include "cell/ObstacleDeadState.hpp"

std::unique_ptr<CellState> ObstacleDeadState::clone() const {
    return std::make_unique<ObstacleDeadState>();
}



