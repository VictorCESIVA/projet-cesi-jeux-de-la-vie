#include "cell/Cell.hpp"
#include "cell/DeadState.hpp"

Cell::Cell(int x, int y, std::unique_ptr<CellState> state)
    : m_x(x)
    , m_y(y)
    , m_state(std::move(state))
    , m_nextState(nullptr)
{
    if (!m_state) {
        m_state = std::make_unique<DeadState>();
    }
}

Cell::Cell(const Cell& other)
    : m_x(other.m_x)
    , m_y(other.m_y)
    , m_state(other.m_state->clone())
    , m_nextState(other.m_nextState ? other.m_nextState->clone() : nullptr)
{
}

Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        m_x = other.m_x;
        m_y = other.m_y;
        m_state = other.m_state->clone();
        m_nextState = other.m_nextState ? other.m_nextState->clone() : nullptr;
    }
    return *this;
}

void Cell::setNextState(std::unique_ptr<CellState> state) {
    m_nextState = std::move(state);
}

void Cell::applyNextState() {
    if (m_nextState) {
        m_state = std::move(m_nextState);
        m_nextState = nullptr;
    }
}

void Cell::setState(std::unique_ptr<CellState> state) {
    if (state) {
        m_state = std::move(state);
    }
}



