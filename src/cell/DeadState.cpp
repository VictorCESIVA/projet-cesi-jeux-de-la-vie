#include "cell/DeadState.hpp"

std::unique_ptr<CellState> DeadState::clone() const {
    return std::make_unique<DeadState>();
}



