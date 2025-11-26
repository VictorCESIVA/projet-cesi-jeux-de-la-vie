#include "game/Game.hpp"

Game::Game(std::unique_ptr<Grid> grid, std::unique_ptr<Rule> rule, int maxIterations)
    : m_grid(std::move(grid))
    , m_rule(std::move(rule))
    , m_iteration(0)
    , m_maxIterations(maxIterations)
    , m_running(false)
    , m_parallel(false)
    , m_previousGrid(*m_grid)
{
}

bool Game::step() {
    if (isFinished()) {
        return false;
    }
    
    // Sauvegarde l'état actuel pour détecter la stabilité
    m_previousGrid = *m_grid;
    
    // Met à jour la grille
    m_grid->update(*m_rule, m_parallel);
    ++m_iteration;
    
    return !isFinished();
}

bool Game::isFinished() const {
    // Terminé si le nombre max d'itérations est atteint
    if (m_maxIterations > 0 && m_iteration >= m_maxIterations) {
        return true;
    }
    
    // Terminé si la grille est stable (plus d'évolution)
    if (m_iteration > 0 && isStable()) {
        return true;
    }
    
    return false;
}

bool Game::isStable() const {
    return m_grid->equals(m_previousGrid);
}

void Game::reset(std::unique_ptr<Grid> grid) {
    m_grid = std::move(grid);
    m_previousGrid = *m_grid;
    m_iteration = 0;
    m_running = false;
}

void Game::setRule(std::unique_ptr<Rule> rule) {
    m_rule = std::move(rule);
}

int Game::countAliveCells() const {
    int count = 0;
    for (int y = 0; y < m_grid->getHeight(); ++y) {
        for (int x = 0; x < m_grid->getWidth(); ++x) {
            if (m_grid->getCell(x, y).isAlive()) {
                ++count;
            }
        }
    }
    return count;
}

