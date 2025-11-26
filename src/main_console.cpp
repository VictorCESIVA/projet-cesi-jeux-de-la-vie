/**
 * @file main_console.cpp
 * @brief Point d'entrée du mode console du Jeu de la Vie
 * 
 * Ce programme lit un fichier de configuration initial et génère
 * les n premières itérations dans des fichiers de sortie.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include "game/Game.hpp"
#include "grid/Grid.hpp"
#include "rules/ClassicRule.hpp"
#include "io/FileHandler.hpp"

void printUsage(const char* programName) {
    std::cout << "Jeu de la Vie - Mode Console\n";
    std::cout << "============================\n\n";
    std::cout << "Usage: " << programName << " <fichier_entree> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -n <nombre>    Nombre d'itérations (défaut: 100)\n";
    std::cout << "  -t             Activer le mode torique\n";
    std::cout << "  -p             Activer le calcul parallèle\n";
    std::cout << "  -v             Mode verbeux (affiche la grille à chaque itération)\n";
    std::cout << "  -h             Afficher cette aide\n\n";
    std::cout << "Format du fichier d'entrée:\n";
    std::cout << "  Première ligne: <hauteur> <largeur>\n";
    std::cout << "  Lignes suivantes: matrice de 0 (mort) et 1 (vivant)\n";
    std::cout << "  Extension: 2 = obstacle mort, 3 = obstacle vivant\n\n";
    std::cout << "Exemple:\n";
    std::cout << "  5 10\n";
    std::cout << "  0 0 1 0 0 0 0 0 0 0\n";
    std::cout << "  0 0 0 1 0 0 0 0 0 0\n";
    std::cout << "  0 1 1 1 0 0 0 0 0 0\n";
    std::cout << "  0 0 0 0 0 0 0 0 0 0\n";
    std::cout << "  0 0 0 0 0 0 0 0 0 0\n";
}

int main(int argc, char* argv[]) {
    // Paramètres par défaut
    std::string inputFile;
    int iterations = 100;
    bool toroidal = false;
    bool parallel = false;
    bool verbose = false;
    
    // Parse des arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-n" && i + 1 < argc) {
            iterations = std::atoi(argv[++i]);
        } else if (arg == "-t") {
            toroidal = true;
        } else if (arg == "-p") {
            parallel = true;
        } else if (arg == "-v") {
            verbose = true;
        } else if (arg[0] != '-') {
            inputFile = arg;
        } else {
            std::cerr << "Option inconnue: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Vérifie qu'un fichier a été fourni
    if (inputFile.empty()) {
        std::cerr << "Erreur: Aucun fichier d'entrée spécifié.\n\n";
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        std::cout << "Chargement de la grille depuis: " << inputFile << "\n";
        
        // Charge la grille
        auto grid = FileHandler::loadFromFile(inputFile, toroidal);
        std::cout << "Grille chargée: " << grid->getWidth() << "x" << grid->getHeight();
        if (toroidal) std::cout << " (mode torique)";
        std::cout << "\n";
        
        // Crée le jeu
        auto rule = std::make_unique<ClassicRule>();
        Game game(std::move(grid), std::move(rule), iterations);
        game.setParallel(parallel);
        
        // Crée le dossier de sortie
        std::string outputDir = FileHandler::createOutputDirectory(inputFile);
        std::cout << "Dossier de sortie: " << outputDir << "\n";
        
        // Sauvegarde l'état initial
        std::string filename = FileHandler::getIterationFilename(outputDir, 0);
        FileHandler::saveToFile(game.getGrid(), filename);
        
        if (verbose) {
            std::cout << "\n=== Itération 0 ===\n";
            std::cout << game.getGrid().toString() << "\n";
        }
        
        // Exécute la simulation
        std::cout << "\nDémarrage de la simulation...\n";
        
        while (game.step()) {
            int iter = game.getIteration();
            filename = FileHandler::getIterationFilename(outputDir, iter);
            FileHandler::saveToFile(game.getGrid(), filename);
            
            if (verbose) {
                std::cout << "\n=== Itération " << iter << " ===\n";
                std::cout << game.getGrid().toString() << "\n";
            } else if (iter % 10 == 0) {
                std::cout << "Itération " << iter << "...\n";
            }
            
            // Vérifie si la grille est stable
            if (game.isStable()) {
                std::cout << "\nL'automate est stable à l'itération " << iter << ".\n";
                break;
            }
        }
        
        std::cout << "\nSimulation terminée après " << game.getIteration() << " itérations.\n";
        std::cout << "Résultats sauvegardés dans: " << outputDir << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}


