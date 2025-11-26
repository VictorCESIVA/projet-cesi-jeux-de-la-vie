#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>
#include <functional>
#include "../grid/Grid.hpp"
#include "../rules/Rule.hpp"

/**
 * @brief Classe principale gérant le jeu de la vie
 * 
 * Orchestre la simulation en coordonnant la grille et les règles.
 * Fournit les méthodes pour exécuter et contrôler la simulation.
 */
class Game {
private:
    std::unique_ptr<Grid> m_grid;          ///< Grille de cellules
    std::unique_ptr<Rule> m_rule;          ///< Règles du jeu
    int m_iteration;                        ///< Numéro d'itération actuel
    int m_maxIterations;                    ///< Nombre maximum d'itérations (0 = infini)
    bool m_running;                         ///< État de la simulation
    bool m_parallel;                        ///< Mode parallèle activé
    Grid m_previousGrid;                    ///< Grille précédente (pour détecter la stabilité)

public:
    /**
     * @brief Constructeur
     * @param grid Grille initiale
     * @param rule Règles du jeu
     * @param maxIterations Nombre max d'itérations (0 = infini)
     */
    Game(std::unique_ptr<Grid> grid, std::unique_ptr<Rule> rule, int maxIterations = 0);
    
    ~Game() = default;

    // Getters
    const Grid& getGrid() const { return *m_grid; }
    Grid& getGrid() { return *m_grid; }
    int getIteration() const { return m_iteration; }
    int getMaxIterations() const { return m_maxIterations; }
    bool isRunning() const { return m_running; }
    const Rule& getRule() const { return *m_rule; }
    
    /**
     * @brief Compte le nombre de cellules vivantes
     * @return Nombre de cellules vivantes
     */
    int countAliveCells() const;
    
    /**
     * @brief Réinitialise le compteur d'itérations
     */
    void resetIteration() { m_iteration = 0; }
    
    // Setters
    void setMaxIterations(int max) { m_maxIterations = max; }
    void setParallel(bool parallel) { m_parallel = parallel; }
    
    /**
     * @brief Exécute une itération
     * @return true si la simulation continue, false si terminée
     */
    bool step();
    
    /**
     * @brief Vérifie si la simulation est terminée
     * @return true si terminée
     */
    bool isFinished() const;
    
    /**
     * @brief Vérifie si l'automate est stable (plus d'évolution)
     * @return true si stable
     */
    bool isStable() const;
    
    /**
     * @brief Démarre la simulation
     */
    void start() { m_running = true; }
    
    /**
     * @brief Met en pause la simulation
     */
    void pause() { m_running = false; }
    
    /**
     * @brief Bascule l'état de la simulation
     */
    void togglePause() { m_running = !m_running; }
    
    /**
     * @brief Réinitialise la simulation
     * @param grid Nouvelle grille
     */
    void reset(std::unique_ptr<Grid> grid);
    
    /**
     * @brief Change les règles du jeu
     * @param rule Nouvelles règles
     */
    void setRule(std::unique_ptr<Rule> rule);
};

#endif // GAME_HPP

