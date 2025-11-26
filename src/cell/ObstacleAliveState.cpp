#include "cell/ObstacleAliveState.hpp"

std::unique_ptr<CellState> ObstacleAliveState::clone() const {
    return std::make_unique<ObstacleAliveState>();
}


