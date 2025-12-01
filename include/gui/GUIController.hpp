#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include "../game/Game.hpp"
#include "../patterns/Patterns.hpp"
#include "GUIRenderer.hpp"
#include <memory>

/**
 * @brief Contrôleur de l'interface graphique
 * 
 * Gère les interactions utilisateur et coordonne le jeu et le rendu.
 * Séparation des responsabilités : logique de contrôle uniquement.
 */
class GUIController {
private:
    sf::RenderWindow m_window;         ///< Fenêtre SFML
    Game& m_game;                       ///< Référence au jeu
    GUIRenderer m_renderer;             ///< Renderer graphique
    sf::Clock m_clock;                  ///< Horloge pour le timing
    int m_updateInterval;               ///< Intervalle entre mises à jour (ms)
    bool m_showHelp;                    ///< Afficher l'aide
    std::string m_selectedPattern;      ///< Pattern sélectionné
    int m_patternIndex;                 ///< Index du pattern actuel
    bool m_parallel;                    ///< Mode parallèle
    std::unique_ptr<Grid> m_initialGrid; ///< Copie de la grille initiale pour reset
    std::string m_saveFilename;         ///< Nom du fichier pour sauvegarde
    int m_saveCounter;                  ///< Compteur pour les sauvegardes

public:
    /**
     * @brief Constructeur
     * @param game Référence au jeu
     * @param width Largeur de la fenêtre
     * @param height Hauteur de la fenêtre
     * @param cellSize Taille des cellules
     */
    GUIController(Game& game, int width, int height, int cellSize = 10);
    
    /**
     * @brief Boucle principale
     */
    void run();

private:
    /**
     * @brief Gère les événements utilisateur
     */
    void handleEvents();
    
    /**
     * @brief Met à jour la simulation
     */
    void update();
    
    /**
     * @brief Dessine le contenu
     */
    void render();
    
    /**
     * @brief Gère les entrées clavier
     * @param event Événement clavier
     */
    void handleKeyPress(const sf::Event& event);
    
    /**
     * @brief Gère les clics souris
     * @param event Événement souris
     */
    void handleMouseClick(const sf::Event& event);
    
    /**
     * @brief Place le pattern sélectionné
     * @param gridX Position X sur la grille
     * @param gridY Position Y sur la grille
     */
    void placeSelectedPattern(int gridX, int gridY);
    
    /**
     * @brief Cycle vers le pattern suivant
     */
    void nextPattern();
    
    /**
     * @brief Cycle vers le pattern précédent
     */
    void previousPattern();
    
    /**
     * @brief Efface toute la grille
     */
    void clearGrid();
    
    /**
     * @brief Réinitialise la grille à l'état initial
     */
    void resetGrid();
    
    /**
     * @brief Sauvegarde l'état actuel dans un fichier
     */
    void saveCurrentState();
    
    /**
     * @brief Gère la molette de souris (zoom)
     * @param event Événement souris
     */
    void handleMouseWheel(const sf::Event& event);
    
    /**
     * @brief Redimensionne la grille
     * @param newWidth Nouvelle largeur
     * @param newHeight Nouvelle hauteur
     */
    void resizeGrid(int newWidth, int newHeight);
    
    /**
     * @brief Augmente la taille de la grille
     * @param amount Nombre de cellules à ajouter
     */
    void increaseGridSize(int amount = 10);
    
    /**
     * @brief Diminue la taille de la grille
     * @param amount Nombre de cellules à retirer
     */
    void decreaseGridSize(int amount = 10);
};

#endif // GUICONTROLLER_HPP
