#include "grid/Grid.hpp"
#include "cell/AliveState.hpp"
#include "cell/DeadState.hpp"
#include <sstream>
#include <thread>
#include <algorithm>
#include <stdexcept>

Grid::Grid(int width, int height, bool toroidal)
    : m_width(width)
    , m_height(height)
    , m_toroidal(toroidal)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Grid dimensions must be positive");
    }
    
    m_cells.resize(height);
    for (int y = 0; y < height; ++y) {
        m_cells[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            m_cells[y].push_back(std::make_unique<Cell>(x, y, std::make_unique<DeadState>()));
        }
    }
}

Grid::Grid(const Grid& other)
    : m_width(other.m_width)
    , m_height(other.m_height)
    , m_toroidal(other.m_toroidal)
{
    m_cells.resize(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_cells[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            m_cells[y].push_back(std::make_unique<Cell>(*other.m_cells[y][x]));
        }
    }
}

Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {
        m_width = other.m_width;
        m_height = other.m_height;
        m_toroidal = other.m_toroidal;
        
        m_cells.clear();
        m_cells.resize(m_height);
        for (int y = 0; y < m_height; ++y) {
            m_cells[y].reserve(m_width);
            for (int x = 0; x < m_width; ++x) {
                m_cells[y].push_back(std::make_unique<Cell>(*other.m_cells[y][x]));
            }
        }
    }
    return *this;
}

int Grid::normalizeCoord(int coord, int max) const {
    if (m_toroidal) {
        coord = coord % max;
        if (coord < 0) coord += max;
        return coord;
    }
    return coord;
}

Cell& Grid::getCell(int x, int y) {
    if (m_toroidal) {
        x = normalizeCoord(x, m_width);
        y = normalizeCoord(y, m_height);
    }
    
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return *m_cells[y][x];
}

const Cell& Grid::getCell(int x, int y) const {
    if (m_toroidal) {
        x = ((x % m_width) + m_width) % m_width;
        y = ((y % m_height) + m_height) % m_height;
    }
    
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::out_of_range("Cell position out of bounds");
    }
    return *m_cells[y][x];
}

void Grid::setCellState(int x, int y, std::unique_ptr<CellState> state) {
    if (isValidPosition(x, y)) {
        m_cells[y][x]->setState(std::move(state));
    }
}

bool Grid::isValidPosition(int x, int y) const {
    if (m_toroidal) return true;
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

int Grid::countAliveNeighbors(int x, int y) const {
    int count = 0;
    
    // Les 8 directions de voisinage
    static const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    static const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (m_toroidal) {
            // Mode torique : normalise les coordonnées
            nx = ((nx % m_width) + m_width) % m_width;
            ny = ((ny % m_height) + m_height) % m_height;
            if (m_cells[ny][nx]->isAlive()) {
                ++count;
            }
        } else {
            // Mode non-torique : ignore les cellules hors limites
            if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                if (m_cells[ny][nx]->isAlive()) {
                    ++count;
                }
            }
        }
    }
    
    return count;
}

void Grid::update(const Rule& rule, bool parallel) {
    if (parallel) {
        // Version parallélisée
        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 4;
        
        std::vector<std::thread> threads;
        int rowsPerThread = m_height / numThreads;
        
        auto processRows = [this, &rule](int startY, int endY) {
            for (int y = startY; y < endY; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    Cell& cell = *m_cells[y][x];
                    
                    // Les obstacles ne changent pas d'état
                    if (cell.isObstacle()) {
                        cell.setNextState(cell.getState().clone());
                    } else {
                        int neighbors = countAliveNeighbors(x, y);
                        cell.setNextState(rule.computeNextState(cell.isAlive(), neighbors));
                    }
                }
            }
        };
        
        for (unsigned int i = 0; i < numThreads; ++i) {
            int startY = i * rowsPerThread;
            int endY = (i == numThreads - 1) ? m_height : (i + 1) * rowsPerThread;
            threads.emplace_back(processRows, startY, endY);
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Version séquentielle
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                Cell& cell = *m_cells[y][x];
                
                // Les obstacles ne changent pas d'état
                if (cell.isObstacle()) {
                    cell.setNextState(cell.getState().clone());
                } else {
                    int neighbors = countAliveNeighbors(x, y);
                    cell.setNextState(rule.computeNextState(cell.isAlive(), neighbors));
                }
            }
        }
    }
    
    // Applique tous les nouveaux états
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            m_cells[y][x]->applyNextState();
        }
    }
}

bool Grid::equals(const Grid& other) const {
    if (m_width != other.m_width || m_height != other.m_height) {
        return false;
    }
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (m_cells[y][x]->isAlive() != other.m_cells[y][x]->isAlive()) {
                return false;
            }
        }
    }
    return true;
}

std::string Grid::toString() const {
    std::ostringstream oss;
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            oss << m_cells[y][x]->toChar();
            if (x < m_width - 1) oss << ' ';
        }
        if (y < m_height - 1) oss << '\n';
    }
    return oss.str();
}

void Grid::placePattern(const std::vector<std::vector<bool>>& pattern, int startX, int startY) {
    int patternHeight = static_cast<int>(pattern.size());
    if (patternHeight == 0) return;
    int patternWidth = static_cast<int>(pattern[0].size());
    
    for (int py = 0; py < patternHeight; ++py) {
        for (int px = 0; px < patternWidth; ++px) {
            int x = startX + px;
            int y = startY + py;
            
            if (m_toroidal) {
                x = ((x % m_width) + m_width) % m_width;
                y = ((y % m_height) + m_height) % m_height;
            } else if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
                continue;
            }
            
            if (pattern[py][px]) {
                m_cells[y][x]->setState(std::make_unique<AliveState>());
            } else {
                m_cells[y][x]->setState(std::make_unique<DeadState>());
            }
        }
    }
}



