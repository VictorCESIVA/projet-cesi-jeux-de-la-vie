/**
 * @file test_main.cpp
 * @brief Tests unitaires pour le Jeu de la Vie
 * 
 * Vérifie le bon fonctionnement des différentes composantes du jeu.
 */

#include <iostream>
#include <cassert>
#include <memory>
#include <sstream>
#include "grid/Grid.hpp"
#include "game/Game.hpp"
#include "rules/ClassicRule.hpp"
#include "cell/AliveState.hpp"
#include "cell/DeadState.hpp"
#include "cell/ObstacleAliveState.hpp"
#include "io/FileHandler.hpp"
#include "patterns/Patterns.hpp"

// Compteurs de tests
int testsRun = 0;
int testsPassed = 0;

/**
 * @brief Macro pour exécuter un test
 */
#define RUN_TEST(test) \
    do { \
        std::cout << "Test: " << #test << "... "; \
        ++testsRun; \
        try { \
            test(); \
            ++testsPassed; \
            std::cout << "OK\n"; \
        } catch (const std::exception& e) { \
            std::cout << "ÉCHEC: " << e.what() << "\n"; \
        } \
    } while(0)

/**
 * @brief Assertion personnalisée avec message
 */
#define ASSERT(condition, message) \
    if (!(condition)) { \
        throw std::runtime_error(message); \
    }

// ============================================================================
// Tests des cellules et états
// ============================================================================

void testCellStates() {
    // Test AliveState
    auto alive = std::make_unique<AliveState>();
    ASSERT(alive->isAlive(), "AliveState devrait être vivant");
    ASSERT(!alive->isObstacle(), "AliveState ne devrait pas être un obstacle");
    ASSERT(alive->toInt() == 1, "AliveState devrait retourner 1");
    
    // Test DeadState
    auto dead = std::make_unique<DeadState>();
    ASSERT(!dead->isAlive(), "DeadState ne devrait pas être vivant");
    ASSERT(!dead->isObstacle(), "DeadState ne devrait pas être un obstacle");
    ASSERT(dead->toInt() == 0, "DeadState devrait retourner 0");
    
    // Test ObstacleAliveState
    auto obstAlive = std::make_unique<ObstacleAliveState>();
    ASSERT(obstAlive->isAlive(), "ObstacleAliveState devrait être vivant");
    ASSERT(obstAlive->isObstacle(), "ObstacleAliveState devrait être un obstacle");
}

void testCell() {
    Cell cell(5, 10, std::make_unique<AliveState>());
    ASSERT(cell.getX() == 5, "Position X incorrecte");
    ASSERT(cell.getY() == 10, "Position Y incorrecte");
    ASSERT(cell.isAlive(), "La cellule devrait être vivante");
    
    cell.setNextState(std::make_unique<DeadState>());
    ASSERT(cell.isAlive(), "La cellule devrait encore être vivante avant apply");
    
    cell.applyNextState();
    ASSERT(!cell.isAlive(), "La cellule devrait être morte après apply");
}

// ============================================================================
// Tests des règles
// ============================================================================

void testClassicRule() {
    ClassicRule rule;
    
    // Cellule morte avec 3 voisins -> vivante
    auto state1 = rule.computeNextState(false, 3);
    ASSERT(state1->isAlive(), "Cellule morte + 3 voisins = vivante");
    
    // Cellule morte avec 2 voisins -> morte
    auto state2 = rule.computeNextState(false, 2);
    ASSERT(!state2->isAlive(), "Cellule morte + 2 voisins = morte");
    
    // Cellule vivante avec 2 voisins -> vivante
    auto state3 = rule.computeNextState(true, 2);
    ASSERT(state3->isAlive(), "Cellule vivante + 2 voisins = vivante");
    
    // Cellule vivante avec 3 voisins -> vivante
    auto state4 = rule.computeNextState(true, 3);
    ASSERT(state4->isAlive(), "Cellule vivante + 3 voisins = vivante");
    
    // Cellule vivante avec 4 voisins -> morte
    auto state5 = rule.computeNextState(true, 4);
    ASSERT(!state5->isAlive(), "Cellule vivante + 4 voisins = morte");
    
    // Cellule vivante avec 1 voisin -> morte
    auto state6 = rule.computeNextState(true, 1);
    ASSERT(!state6->isAlive(), "Cellule vivante + 1 voisin = morte");
}

// ============================================================================
// Tests de la grille
// ============================================================================

void testGridCreation() {
    Grid grid(10, 20, false);
    ASSERT(grid.getWidth() == 10, "Largeur incorrecte");
    ASSERT(grid.getHeight() == 20, "Hauteur incorrecte");
    ASSERT(!grid.isToroidal(), "Mode torique devrait être désactivé");
    
    // Toutes les cellules doivent être mortes initialement
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 10; ++x) {
            ASSERT(!grid.getCell(x, y).isAlive(), "Cellule devrait être morte initialement");
        }
    }
}

void testGridNeighbors() {
    Grid grid(5, 5, false);
    
    // Place une cellule vivante au centre et une autre à côté
    grid.setCellState(2, 2, std::make_unique<AliveState>());
    grid.setCellState(3, 2, std::make_unique<AliveState>());
    
    // Vérifie le comptage des voisins
    ASSERT(grid.countAliveNeighbors(2, 2) == 1, "Le centre devrait avoir 1 voisin vivant");
    ASSERT(grid.countAliveNeighbors(1, 1) == 1, "Position (1,1) devrait avoir 1 voisin");
    ASSERT(grid.countAliveNeighbors(0, 0) == 0, "Coin (0,0) ne devrait avoir aucun voisin vivant");
}

void testGridToroidal() {
    Grid grid(5, 5, true);  // Mode torique activé
    
    // Place une cellule dans un coin
    grid.setCellState(0, 0, std::make_unique<AliveState>());
    
    // En mode torique, elle devrait être voisine de (4, 4)
    ASSERT(grid.countAliveNeighbors(4, 4) == 1, "Mode torique: (4,4) devrait voir (0,0)");
    
    // Test wrap-around X
    grid.setCellState(4, 2, std::make_unique<AliveState>());
    ASSERT(grid.countAliveNeighbors(0, 2) == 1, "Mode torique: wrap-around X");
}

void testGridEquality() {
    Grid grid1(3, 3, false);
    Grid grid2(3, 3, false);
    
    ASSERT(grid1.equals(grid2), "Grilles vides devraient être égales");
    
    grid1.setCellState(1, 1, std::make_unique<AliveState>());
    ASSERT(!grid1.equals(grid2), "Grilles différentes ne devraient pas être égales");
    
    grid2.setCellState(1, 1, std::make_unique<AliveState>());
    ASSERT(grid1.equals(grid2), "Grilles identiques devraient être égales");
}

// ============================================================================
// Tests de la simulation
// ============================================================================

void testBlinkerOscillator() {
    // Le Blinker est un oscillateur de période 2
    // Configuration initiale:
    //   . . .       . O .
    //   O O O  ->   . O .  -> retour à l'état initial
    //   . . .       . O .
    
    Grid grid(5, 5, false);
    grid.setCellState(1, 2, std::make_unique<AliveState>());
    grid.setCellState(2, 2, std::make_unique<AliveState>());
    grid.setCellState(3, 2, std::make_unique<AliveState>());
    
    Grid initialGrid = grid;
    
    auto rule = std::make_unique<ClassicRule>();
    Game game(std::make_unique<Grid>(grid), std::move(rule), 0);
    
    // Après 1 itération: blinker vertical
    game.step();
    ASSERT(game.getGrid().getCell(2, 1).isAlive(), "Blinker: (2,1) devrait être vivant après 1 iter");
    ASSERT(game.getGrid().getCell(2, 2).isAlive(), "Blinker: (2,2) devrait être vivant après 1 iter");
    ASSERT(game.getGrid().getCell(2, 3).isAlive(), "Blinker: (2,3) devrait être vivant après 1 iter");
    ASSERT(!game.getGrid().getCell(1, 2).isAlive(), "Blinker: (1,2) devrait être mort après 1 iter");
    ASSERT(!game.getGrid().getCell(3, 2).isAlive(), "Blinker: (3,2) devrait être mort après 1 iter");
    
    // Après 2 itérations: retour à l'état initial
    game.step();
    ASSERT(game.getGrid().equals(initialGrid), "Blinker devrait revenir à l'état initial après 2 iter");
}

void testBlockStable() {
    // Le Block est une structure stable (still life)
    //   O O
    //   O O
    
    Grid grid(5, 5, false);
    grid.setCellState(1, 1, std::make_unique<AliveState>());
    grid.setCellState(2, 1, std::make_unique<AliveState>());
    grid.setCellState(1, 2, std::make_unique<AliveState>());
    grid.setCellState(2, 2, std::make_unique<AliveState>());
    
    Grid initialGrid = grid;
    
    auto rule = std::make_unique<ClassicRule>();
    Game game(std::make_unique<Grid>(grid), std::move(rule), 0);
    
    // Après plusieurs itérations, devrait rester identique
    for (int i = 0; i < 5; ++i) {
        game.step();
        ASSERT(game.getGrid().equals(initialGrid), "Block devrait rester stable");
    }
}

void testGliderMovement() {
    // Le Glider se déplace en diagonale
    // Configuration initiale (dans un coin):
    //   . O .
    //   . . O
    //   O O O
    
    Grid grid(10, 10, false);
    grid.setCellState(1, 0, std::make_unique<AliveState>());
    grid.setCellState(2, 1, std::make_unique<AliveState>());
    grid.setCellState(0, 2, std::make_unique<AliveState>());
    grid.setCellState(1, 2, std::make_unique<AliveState>());
    grid.setCellState(2, 2, std::make_unique<AliveState>());
    
    auto rule = std::make_unique<ClassicRule>();
    Game game(std::make_unique<Grid>(grid), std::move(rule), 0);
    
    // Après 4 itérations, le glider s'est déplacé d'une case en diagonale
    for (int i = 0; i < 4; ++i) {
        game.step();
    }
    
    // Vérifie qu'il y a bien 5 cellules vivantes après 4 itérations
    int aliveCount = 0;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            if (game.getGrid().getCell(x, y).isAlive()) {
                ++aliveCount;
            }
        }
    }
    ASSERT(aliveCount == 5, "Glider devrait toujours avoir 5 cellules vivantes");
}

void testObstacles() {
    // Les obstacles ne changent pas d'état
    Grid grid(5, 5, false);
    grid.setCellState(2, 2, std::make_unique<ObstacleAliveState>());
    
    // Entoure l'obstacle de cellules vivantes (forcerait normalement sa mort)
    grid.setCellState(1, 1, std::make_unique<AliveState>());
    grid.setCellState(2, 1, std::make_unique<AliveState>());
    grid.setCellState(3, 1, std::make_unique<AliveState>());
    grid.setCellState(1, 2, std::make_unique<AliveState>());
    grid.setCellState(3, 2, std::make_unique<AliveState>());
    grid.setCellState(1, 3, std::make_unique<AliveState>());
    grid.setCellState(2, 3, std::make_unique<AliveState>());
    grid.setCellState(3, 3, std::make_unique<AliveState>());
    
    auto rule = std::make_unique<ClassicRule>();
    Game game(std::make_unique<Grid>(grid), std::move(rule), 0);
    
    game.step();
    
    // L'obstacle devrait toujours être vivant malgré la surpopulation
    ASSERT(game.getGrid().getCell(2, 2).isAlive(), "Obstacle devrait rester vivant");
    ASSERT(game.getGrid().getCell(2, 2).isObstacle(), "Devrait toujours être un obstacle");
}

// ============================================================================
// Tests des patterns
// ============================================================================

void testPatterns() {
    // Vérifie que les patterns existent et ont une taille valide
    auto glider = Patterns::glider();
    ASSERT(!glider.empty(), "Glider pattern ne devrait pas être vide");
    ASSERT(glider.size() == 3, "Glider devrait avoir 3 lignes");
    ASSERT(glider[0].size() == 3, "Glider devrait avoir 3 colonnes");
    
    auto pulsar = Patterns::pulsar();
    ASSERT(!pulsar.empty(), "Pulsar pattern ne devrait pas être vide");
    ASSERT(pulsar.size() == 13, "Pulsar devrait avoir 13 lignes");
    
    auto patterns = Patterns::listPatterns();
    ASSERT(patterns.size() >= 9, "Devrait y avoir au moins 9 patterns");
}

void testPlacePattern() {
    Grid grid(10, 10, false);
    
    auto glider = Patterns::glider();
    grid.placePattern(glider, 2, 2);
    
    // Vérifie le placement du glider
    ASSERT(grid.getCell(3, 2).isAlive(), "Glider mal placé");
    ASSERT(grid.getCell(4, 3).isAlive(), "Glider mal placé");
    ASSERT(grid.getCell(2, 4).isAlive(), "Glider mal placé");
    ASSERT(grid.getCell(3, 4).isAlive(), "Glider mal placé");
    ASSERT(grid.getCell(4, 4).isAlive(), "Glider mal placé");
}

// ============================================================================
// Test de vérification de grille attendue
// ============================================================================

/**
 * @brief Vérifie l'état de la grille après un nombre arbitraire d'itérations
 * 
 * Fonction requise par le cahier des charges pour les tests unitaires.
 * 
 * @param initialGrid Grille initiale
 * @param expectedGrid Grille attendue après les itérations
 * @param iterations Nombre d'itérations à exécuter
 * @return true si la grille finale correspond à la grille attendue
 */
bool verifyGridAfterIterations(const Grid& initialGrid, const Grid& expectedGrid, int iterations) {
    auto rule = std::make_unique<ClassicRule>();
    auto grid = std::make_unique<Grid>(initialGrid);
    Game game(std::move(grid), std::move(rule), iterations);
    
    for (int i = 0; i < iterations; ++i) {
        game.step();
    }
    
    return game.getGrid().equals(expectedGrid);
}

void testVerifyGridFunction() {
    // Test avec un blinker
    Grid initial(5, 5, false);
    initial.setCellState(1, 2, std::make_unique<AliveState>());
    initial.setCellState(2, 2, std::make_unique<AliveState>());
    initial.setCellState(3, 2, std::make_unique<AliveState>());
    
    // Après 2 itérations, devrait revenir à l'état initial
    ASSERT(verifyGridAfterIterations(initial, initial, 2), 
           "Blinker devrait revenir à l'état initial après 2 itérations");
    
    // Grille attendue après 1 itération
    Grid expected(5, 5, false);
    expected.setCellState(2, 1, std::make_unique<AliveState>());
    expected.setCellState(2, 2, std::make_unique<AliveState>());
    expected.setCellState(2, 3, std::make_unique<AliveState>());
    
    ASSERT(verifyGridAfterIterations(initial, expected, 1),
           "Blinker après 1 itération devrait être vertical");
}

// ============================================================================
// Programme principal des tests
// ============================================================================

int main() {
    std::cout << "============================================\n";
    std::cout << "    Tests Unitaires - Jeu de la Vie\n";
    std::cout << "============================================\n\n";
    
    // Tests des cellules
    std::cout << "--- Tests des cellules ---\n";
    RUN_TEST(testCellStates);
    RUN_TEST(testCell);
    
    // Tests des règles
    std::cout << "\n--- Tests des règles ---\n";
    RUN_TEST(testClassicRule);
    
    // Tests de la grille
    std::cout << "\n--- Tests de la grille ---\n";
    RUN_TEST(testGridCreation);
    RUN_TEST(testGridNeighbors);
    RUN_TEST(testGridToroidal);
    RUN_TEST(testGridEquality);
    
    // Tests de la simulation
    std::cout << "\n--- Tests de la simulation ---\n";
    RUN_TEST(testBlinkerOscillator);
    RUN_TEST(testBlockStable);
    RUN_TEST(testGliderMovement);
    RUN_TEST(testObstacles);
    
    // Tests des patterns
    std::cout << "\n--- Tests des patterns ---\n";
    RUN_TEST(testPatterns);
    RUN_TEST(testPlacePattern);
    
    // Test de la fonction de vérification
    std::cout << "\n--- Tests de vérification de grille ---\n";
    RUN_TEST(testVerifyGridFunction);
    
    // Résumé
    std::cout << "\n============================================\n";
    std::cout << "Résultat: " << testsPassed << "/" << testsRun << " tests réussis\n";
    std::cout << "============================================\n";
    
    return (testsPassed == testsRun) ? 0 : 1;
}


