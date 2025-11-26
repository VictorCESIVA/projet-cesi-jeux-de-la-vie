#ifndef GUIRENDERER_HPP
#define GUIRENDERER_HPP

#include <SFML/Graphics.hpp>
#include "../grid/Grid.hpp"
#include <string>

/**
 * @brief Classe responsable du rendu graphique de la grille
 * 
 * Gère l'affichage de la grille avec SFML.
 * Séparation des responsabilités : uniquement le rendu visuel.
 */
class GUIRenderer {
private:
    sf::RenderWindow& m_window;        ///< Fenêtre SFML
    int m_cellSize;                     ///< Taille d'une cellule en pixels
    sf::Color m_aliveColor;             ///< Couleur des cellules vivantes
    sf::Color m_deadColor;              ///< Couleur des cellules mortes
    sf::Color m_obstacleAliveColor;     ///< Couleur des obstacles vivants
    sf::Color m_obstacleDeadColor;      ///< Couleur des obstacles morts
    sf::Color m_gridColor;              ///< Couleur de la grille
    sf::Font m_font;                    ///< Police pour le texte
    bool m_fontLoaded;                  ///< Indique si la police est chargée
    bool m_showGrid;                    ///< Afficher les lignes de grille
    int m_offsetX;                      ///< Décalage X pour le panneau latéral
    int m_offsetY;                      ///< Décalage Y pour le panneau info
    int m_panelWidth;                   ///< Largeur du panneau latéral

public:
    /**
     * @brief Constructeur
     * @param window Fenêtre SFML
     * @param cellSize Taille des cellules en pixels
     */
    GUIRenderer(sf::RenderWindow& window, int cellSize = 10);
    
    /**
     * @brief Dessine la grille complète
     * @param grid Grille à dessiner
     */
    void render(const Grid& grid);
    
    /**
     * @brief Dessine le panneau d'information en haut
     * @param iteration Numéro d'itération
     * @param running État de la simulation
     * @param speed Vitesse (ms entre itérations)
     * @param toroidal Mode torique actif
     * @param parallel Mode parallèle actif
     * @param aliveCells Nombre de cellules vivantes
     * @param stable L'automate est stable
     */
    void renderInfoPanel(int iteration, bool running, int speed, bool toroidal, bool parallel, 
                         int aliveCells, bool stable);
    
    /**
     * @brief Dessine le panneau latéral avec les options
     * @param selectedPattern Pattern actuellement sélectionné
     * @param toroidal Mode torique
     * @param parallel Mode parallèle
     */
    void renderSidePanel(const std::string& selectedPattern, bool toroidal, bool parallel);
    
    /**
     * @brief Dessine les instructions (aide)
     */
    void renderHelp();
    
    /**
     * @brief Convertit une position écran en position grille
     * @param screenX Position X écran
     * @param screenY Position Y écran
     * @param gridX Position X grille (sortie)
     * @param gridY Position Y grille (sortie)
     * @return true si la position est valide
     */
    bool screenToGrid(int screenX, int screenY, int& gridX, int& gridY, const Grid& grid) const;
    
    /**
     * @brief Vérifie si un clic est dans le panneau latéral
     * @param screenX Position X
     * @param screenY Position Y
     * @return Index du bouton cliqué (-1 si aucun)
     */
    int getSidePanelButtonAt(int screenX, int screenY) const;
    
    // Setters pour la personnalisation
    void setCellSize(int size) { m_cellSize = size; }
    void setShowGrid(bool show) { m_showGrid = show; }
    void toggleShowGrid() { m_showGrid = !m_showGrid; }
    void setAliveColor(sf::Color color) { m_aliveColor = color; }
    void setDeadColor(sf::Color color) { m_deadColor = color; }
    
    int getCellSize() const { return m_cellSize; }
    bool isShowGrid() const { return m_showGrid; }
    int getOffsetY() const { return m_offsetY; }
    int getPanelWidth() const { return m_panelWidth; }
};

#endif // GUIRENDERER_HPP
