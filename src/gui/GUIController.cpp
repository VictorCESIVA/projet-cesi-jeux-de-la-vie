#include "gui/GUIController.hpp"
#include "cell/AliveState.hpp"
#include "cell/DeadState.hpp"
#include "cell/ObstacleAliveState.hpp"
#include "cell/ObstacleDeadState.hpp"
#include "io/FileHandler.hpp"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

GUIController::GUIController(Game& game, int width, int height, int cellSize)
    : m_window(sf::VideoMode(width, height), "Jeu de la Vie - Conway's Game of Life", 
               sf::Style::Titlebar | sf::Style::Close)
    , m_game(game)
    , m_renderer(m_window, cellSize)
    , m_updateInterval(100)
    , m_showHelp(false)
    , m_selectedPattern("glider")
    , m_patternIndex(0)
    , m_parallel(false)
    , m_initialGrid(std::make_unique<Grid>(game.getGrid()))
    , m_saveFilename("save")
    , m_saveCounter(0)
{
    m_window.setFramerateLimit(60);
}

void GUIController::run() {
    // Ne démarre PAS automatiquement - l'utilisateur doit appuyer sur ESPACE
    m_game.pause();
    
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void GUIController::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;
                
            case sf::Event::KeyPressed:
                handleKeyPress(event);
                break;
                
            case sf::Event::MouseButtonPressed:
                handleMouseClick(event);
                break;
                
            case sf::Event::MouseWheelScrolled:
                handleMouseWheel(event);
                break;
                
            default:
                break;
        }
    }
}

void GUIController::update() {
    if (m_game.isRunning() && !m_game.isFinished()) {
        if (m_clock.getElapsedTime().asMilliseconds() >= m_updateInterval) {
            m_game.step();
            m_clock.restart();
        }
    }
}

void GUIController::render() {
    m_window.clear(sf::Color(20, 20, 25));
    
    // Panneau latéral
    m_renderer.renderSidePanel(m_selectedPattern, m_game.getGrid().isToroidal(), m_parallel);
    
    // Grille
    m_renderer.render(m_game.getGrid());
    
    // Panneau info en haut avec statistiques
    m_renderer.renderInfoPanel(m_game.getIteration(), m_game.isRunning(), 
                               m_updateInterval, m_game.getGrid().isToroidal(), m_parallel,
                               m_game.countAliveCells(), m_game.isStable(),
                               m_game.getGrid().getWidth(), m_game.getGrid().getHeight());
    
    // Aide si activée
    if (m_showHelp) {
        m_renderer.renderHelp();
    }
    
    m_window.display();
}

void GUIController::handleKeyPress(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::Escape:
            m_window.close();
            break;
            
        case sf::Keyboard::Space:
            m_game.togglePause();
            break;
            
        case sf::Keyboard::S:
            // Avance d'une seule itération (même en pause)
            m_game.step();
            break;
            
        case sf::Keyboard::C:
            // Efface la grille
            clearGrid();
            break;
            
        case sf::Keyboard::R:
            // Réinitialise la grille
            resetGrid();
            break;
            
        case sf::Keyboard::H:
            m_showHelp = !m_showHelp;
            break;
            
        case sf::Keyboard::G:
            m_renderer.toggleShowGrid();
            break;
            
        case sf::Keyboard::T:
            m_game.getGrid().setToroidal(!m_game.getGrid().isToroidal());
            break;
            
        case sf::Keyboard::P:
            m_parallel = !m_parallel;
            m_game.setParallel(m_parallel);
            break;
        
        case sf::Keyboard::F5:
            // Sauvegarde l'état actuel
            saveCurrentState();
            break;
            
        case sf::Keyboard::LBracket:
            // [ : Diminue la taille de la grille
            decreaseGridSize(5);
            break;
            
        case sf::Keyboard::RBracket:
            // ] : Augmente la taille de la grille
            increaseGridSize(5);
            break;
            
        case sf::Keyboard::Up:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                // Ctrl+Haut : Augmente la hauteur
                resizeGrid(m_game.getGrid().getWidth(), m_game.getGrid().getHeight() + 5);
            }
            break;
            
        case sf::Keyboard::Down:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                // Ctrl+Bas : Diminue la hauteur
                resizeGrid(m_game.getGrid().getWidth(), m_game.getGrid().getHeight() - 5);
            }
            break;
            
        case sf::Keyboard::Right:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                // Ctrl+Droite : Augmente la largeur
                resizeGrid(m_game.getGrid().getWidth() + 5, m_game.getGrid().getHeight());
            } else {
                // Droite seule : Pattern suivant
                nextPattern();
            }
            break;
            
        case sf::Keyboard::Left:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                // Ctrl+Gauche : Diminue la largeur
                resizeGrid(m_game.getGrid().getWidth() - 5, m_game.getGrid().getHeight());
            } else {
                // Gauche seule : Pattern précédent
                previousPattern();
            }
            break;
            
        case sf::Keyboard::Add:
        case sf::Keyboard::Equal:
            // Augmente la vitesse (diminue l'intervalle)
            if (m_updateInterval > 20) {
                m_updateInterval -= 20;
            }
            break;
            
        case sf::Keyboard::Subtract:
        case sf::Keyboard::Hyphen:
            // Diminue la vitesse (augmente l'intervalle)
            if (m_updateInterval < 1000) {
                m_updateInterval += 20;
            }
            break;
            
        // Sélection de patterns par numéro
        case sf::Keyboard::Num1:
        case sf::Keyboard::Numpad1:
            m_selectedPattern = "glider";
            m_patternIndex = 0;
            break;
        case sf::Keyboard::Num2:
        case sf::Keyboard::Numpad2:
            m_selectedPattern = "blinker";
            m_patternIndex = 1;
            break;
        case sf::Keyboard::Num3:
        case sf::Keyboard::Numpad3:
            m_selectedPattern = "block";
            m_patternIndex = 2;
            break;
        case sf::Keyboard::Num4:
        case sf::Keyboard::Numpad4:
            m_selectedPattern = "beehive";
            m_patternIndex = 3;
            break;
        case sf::Keyboard::Num5:
        case sf::Keyboard::Numpad5:
            m_selectedPattern = "toad";
            m_patternIndex = 4;
            break;
        case sf::Keyboard::Num6:
        case sf::Keyboard::Numpad6:
            m_selectedPattern = "beacon";
            m_patternIndex = 5;
            break;
        case sf::Keyboard::Num7:
        case sf::Keyboard::Numpad7:
            m_selectedPattern = "pulsar";
            m_patternIndex = 6;
            break;
        case sf::Keyboard::Num8:
        case sf::Keyboard::Numpad8:
            m_selectedPattern = "lwss";
            m_patternIndex = 7;
            break;
        case sf::Keyboard::Num9:
        case sf::Keyboard::Numpad9:
            m_selectedPattern = "pentadecathlon";
            m_patternIndex = 8;
            break;
            
        default:
            break;
    }
}

void GUIController::handleMouseClick(const sf::Event& event) {
    int gridX, gridY;
    
    if (m_renderer.screenToGrid(event.mouseButton.x, event.mouseButton.y, gridX, gridY, m_game.getGrid())) {
        bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || 
                          sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
        bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
                           sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
        
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (ctrlPressed) {
                // Ctrl+Clic : Créer un obstacle vivant
                m_game.getGrid().setCellState(gridX, gridY, std::make_unique<ObstacleAliveState>());
            } else if (shiftPressed) {
                // Shift+Clic : Active juste une cellule
                m_game.getGrid().setCellState(gridX, gridY, std::make_unique<AliveState>());
            } else {
                // Clic simple : Place le pattern
                placeSelectedPattern(gridX, gridY);
            }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            if (ctrlPressed) {
                // Ctrl+Clic droit : Créer un obstacle mort
                m_game.getGrid().setCellState(gridX, gridY, std::make_unique<ObstacleDeadState>());
            } else {
                // Clic droit : Désactive une cellule (remet à l'état normal mort)
                m_game.getGrid().setCellState(gridX, gridY, std::make_unique<DeadState>());
            }
        }
    }
}

void GUIController::placeSelectedPattern(int gridX, int gridY) {
    auto pattern = Patterns::getByName(m_selectedPattern);
    if (!pattern.empty()) {
        m_game.getGrid().placePattern(pattern, gridX, gridY);
    }
}

void GUIController::nextPattern() {
    auto patterns = Patterns::listPatterns();
    m_patternIndex = (m_patternIndex + 1) % static_cast<int>(patterns.size());
    m_selectedPattern = patterns[m_patternIndex];
}

void GUIController::previousPattern() {
    auto patterns = Patterns::listPatterns();
    m_patternIndex = (m_patternIndex - 1 + static_cast<int>(patterns.size())) % static_cast<int>(patterns.size());
    m_selectedPattern = patterns[m_patternIndex];
}

void GUIController::clearGrid() {
    // Efface toutes les cellules
    Grid& grid = m_game.getGrid();
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (!grid.getCell(x, y).isObstacle()) {
                grid.setCellState(x, y, std::make_unique<DeadState>());
            }
        }
    }
    m_game.pause();
}

void GUIController::resetGrid() {
    // Remet la grille à l'état initial
    Grid& grid = m_game.getGrid();
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (m_initialGrid->getCell(x, y).isAlive()) {
                grid.setCellState(x, y, std::make_unique<AliveState>());
            } else {
                grid.setCellState(x, y, std::make_unique<DeadState>());
            }
        }
    }
    m_game.pause();
    m_game.resetIteration();
}

void GUIController::saveCurrentState() {
    // Génère un nom de fichier unique
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    
    std::ostringstream filename;
    filename << "save_" 
             << std::setfill('0') << std::setw(2) << ltm->tm_hour
             << std::setw(2) << ltm->tm_min
             << std::setw(2) << ltm->tm_sec
             << "_" << m_saveCounter++ << ".txt";
    
    try {
        FileHandler::saveToFile(m_game.getGrid(), filename.str());
        std::cout << "Sauvegarde effectuee: " << filename.str() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur de sauvegarde: " << e.what() << std::endl;
    }
}

void GUIController::handleMouseWheel(const sf::Event& event) {
    int currentSize = m_renderer.getCellSize();
    
    if (event.mouseWheelScroll.delta > 0) {
        // Zoom avant
        if (currentSize < 30) {
            m_renderer.setCellSize(currentSize + 2);
        }
    } else {
        // Zoom arrière
        if (currentSize > 4) {
            m_renderer.setCellSize(currentSize - 2);
        }
    }
}

void GUIController::resizeGrid(int newWidth, int newHeight) {
    // Limites de taille
    if (newWidth < 10) newWidth = 10;
    if (newHeight < 10) newHeight = 10;
    if (newWidth > 200) newWidth = 200;
    if (newHeight > 200) newHeight = 200;
    
    Grid& oldGrid = m_game.getGrid();
    int oldWidth = oldGrid.getWidth();
    int oldHeight = oldGrid.getHeight();
    
    // Si pas de changement, on sort
    if (newWidth == oldWidth && newHeight == oldHeight) return;
    
    // Crée une nouvelle grille
    auto newGrid = std::make_unique<Grid>(newWidth, newHeight, oldGrid.isToroidal());
    
    // Copie les cellules existantes (celles qui rentrent dans la nouvelle taille)
    int copyWidth = std::min(oldWidth, newWidth);
    int copyHeight = std::min(oldHeight, newHeight);
    
    for (int y = 0; y < copyHeight; ++y) {
        for (int x = 0; x < copyWidth; ++x) {
            const Cell& oldCell = oldGrid.getCell(x, y);
            if (oldCell.isAlive()) {
                if (oldCell.isObstacle()) {
                    newGrid->setCellState(x, y, std::make_unique<ObstacleAliveState>());
                } else {
                    newGrid->setCellState(x, y, std::make_unique<AliveState>());
                }
            } else if (oldCell.isObstacle()) {
                newGrid->setCellState(x, y, std::make_unique<ObstacleDeadState>());
            }
        }
    }
    
    // Met à jour la grille initiale pour le reset
    m_initialGrid = std::make_unique<Grid>(*newGrid);
    
    // Remplace la grille dans le jeu
    m_game.reset(std::move(newGrid));
    
    std::cout << "Grille redimensionnee: " << newWidth << "x" << newHeight << std::endl;
}

void GUIController::increaseGridSize(int amount) {
    resizeGrid(m_game.getGrid().getWidth() + amount, m_game.getGrid().getHeight() + amount);
}

void GUIController::decreaseGridSize(int amount) {
    resizeGrid(m_game.getGrid().getWidth() - amount, m_game.getGrid().getHeight() - amount);
}
