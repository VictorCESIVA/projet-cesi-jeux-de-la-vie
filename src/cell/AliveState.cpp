#include "cell/AliveState.hpp"

std::unique_ptr<CellState> AliveState::clone() const {
    return std::make_unique<AliveState>();
}


