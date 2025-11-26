/**
 * @file main_gui.cpp
 * @brief Point d'entrée du mode graphique du Jeu de la Vie
 * 
 * Ce programme affiche une interface graphique SFML pour visualiser
 * et interagir avec le Jeu de la Vie.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include "game/Game.hpp"
#include "grid/Grid.hpp"
#include "rules/ClassicRule.hpp"
#include "io/FileHandler.hpp"
#include "gui/GUIController.hpp"

void printUsage(const char* programName) {
    std::cout << "Jeu de la Vie - Mode Graphique\n";
    std::cout << "==============================\n\n";
    std::cout << "Usage: " << programName << " [fichier_entree] [options]\n\n";
    std::cout << "Si aucun fichier n'est fourni, une grille vide de 50x50 est creee.\n\n";
    std::cout << "Options:\n";
    std::cout << "  -s <taille>    Taille des cellules en pixels (defaut: 12)\n";
    std::cout << "  -g <WxH>       Taille de la grille (ex: 50x40, defaut: 50x50)\n";
    std::cout << "  -w <largeur>   Largeur de la fenetre (defaut: auto)\n";
    std::cout << "  -h <hauteur>   Hauteur de la fenetre (defaut: auto)\n";
    std::cout << "  -t             Activer le mode torique\n";
    std::cout << "  --help         Afficher cette aide\n";
}

int main(int argc, char* argv[]) {
    // Paramètres par défaut
    std::string inputFile;
    int cellSize = 12;
    int gridWidth = 50;
    int gridHeight = 50;
    int windowWidth = 0;   // Auto
    int windowHeight = 0;  // Auto
    bool toroidal = false;
    
    // Parse des arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-s" && i + 1 < argc) {
            cellSize = std::atoi(argv[++i]);
            if (cellSize < 4) cellSize = 4;
            if (cellSize > 50) cellSize = 50;
        } else if (arg == "-g" && i + 1 < argc) {
            std::string sizeStr = argv[++i];
            size_t xPos = sizeStr.find('x');
            if (xPos != std::string::npos) {
                gridWidth = std::atoi(sizeStr.substr(0, xPos).c_str());
                gridHeight = std::atoi(sizeStr.substr(xPos + 1).c_str());
                if (gridWidth < 10) gridWidth = 10;
                if (gridHeight < 10) gridHeight = 10;
                if (gridWidth > 200) gridWidth = 200;
                if (gridHeight > 200) gridHeight = 200;
            }
        } else if (arg == "-w" && i + 1 < argc) {
            windowWidth = std::atoi(argv[++i]);
        } else if (arg == "-h" && i + 1 < argc) {
            windowHeight = std::atoi(argv[++i]);
        } else if (arg == "-t") {
            toroidal = true;
        } else if (arg[0] != '-') {
            inputFile = arg;
        }
    }
    
    try {
        std::unique_ptr<Grid> grid;
        
        // Charge ou crée la grille
        if (!inputFile.empty()) {
            std::cout << "Chargement de la grille depuis: " << inputFile << "\n";
            grid = FileHandler::loadFromFile(inputFile, toroidal);
        } else {
            std::cout << "Creation d'une grille vide " << gridWidth << "x" << gridHeight << "\n";
            grid = std::make_unique<Grid>(gridWidth, gridHeight, toroidal);
        }
        
        // Calcule les dimensions de la fenêtre si non spécifiées
        if (windowWidth == 0) {
            windowWidth = grid->getWidth() * cellSize + 220; // +220 pour le panneau latéral
            if (windowWidth < 800) windowWidth = 800;
            if (windowWidth > 1800) windowWidth = 1800;
        }
        if (windowHeight == 0) {
            windowHeight = grid->getHeight() * cellSize + 80;
            if (windowHeight < 600) windowHeight = 600;
            if (windowHeight > 1000) windowHeight = 1000;
        }
        
        std::cout << "Grille: " << grid->getWidth() << "x" << grid->getHeight();
        if (toroidal) std::cout << " (mode torique)";
        std::cout << "\n";
        std::cout << "Fenetre: " << windowWidth << "x" << windowHeight << "\n";
        std::cout << "Taille cellules: " << cellSize << " pixels\n\n";
        std::cout << "Appuyez sur H pour afficher l'aide\n";
        
        // Crée le jeu
        auto rule = std::make_unique<ClassicRule>();
        Game game(std::move(grid), std::move(rule), 0);
        
        // Lance l'interface graphique
        GUIController controller(game, windowWidth, windowHeight, cellSize);
        controller.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
