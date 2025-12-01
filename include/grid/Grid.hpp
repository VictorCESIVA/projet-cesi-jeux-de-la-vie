#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <memory>
#include <functional>
#include "../cell/Cell.hpp"
#include "../rules/Rule.hpp"

/**
 * @brief Classe représentant la grille de cellules
 * 
 * Gère une grille 2D de cellules avec support optionnel pour la topologie torique.
 * Peut être parallélisée pour la mise à jour des cellules.
 */
class Grid {
private:
    int m_width;                           ///< Largeur de la grille
    int m_height;                          ///< Hauteur de la grille
    bool m_toroidal;                       ///< Mode torique activé
    std::vector<std::vector<std::unique_ptr<Cell>>> m_cells; ///< Matrice de cellules

    /**
     * @brief Normalise une coordonnée pour le mode torique
     * @param coord Coordonnée à normaliser
     * @param max Valeur maximale (width ou height)
     * @return Coordonnée normalisée
     */
    int normalizeCoord(int coord, int max) const;

public:
    /**
     * @brief Constructeur
     * @param width Largeur de la grille
     * @param height Hauteur de la grille
     * @param toroidal Active le mode torique
     */
    Grid(int width, int height, bool toroidal = false);
    
    /**
     * @brief Constructeur de copie
     */
    Grid(const Grid& other);
    
    /**
     * @brief Opérateur d'affectation
     */
    Grid& operator=(const Grid& other);
    
    ~Grid() = default;

    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    bool isToroidal() const { return m_toroidal; }
    
    /**
     * @brief Accède à une cellule
     * @param x Position X
     * @param y Position Y
     * @return Référence à la cellule
     */
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    /**
     * @brief Définit l'état d'une cellule
     * @param x Position X
     * @param y Position Y
     * @param state Nouvel état
     */
    void setCellState(int x, int y, std::unique_ptr<CellState> state);
    
    /**
     * @brief Compte les voisins vivants d'une cellule
     * @param x Position X
     * @param y Position Y
     * @return Nombre de voisins vivants
     */
    int countAliveNeighbors(int x, int y) const;
    
    /**
     * @brief Met à jour la grille selon les règles données
     * @param rule Règles à appliquer
     * @param parallel Utiliser le calcul parallèle
     */
    void update(const Rule& rule, bool parallel = false);
    
    /**
     * @brief Vérifie si une position est valide
     * @param x Position X
     * @param y Position Y
     * @return true si la position est valide
     */
    bool isValidPosition(int x, int y) const;
    
    /**
     * @brief Compare deux grilles
     * @param other Autre grille
     * @return true si les grilles sont identiques
     */
    bool equals(const Grid& other) const;
    
    /**
     * @brief Active/désactive le mode torique
     * @param toroidal État du mode torique
     */
    void setToroidal(bool toroidal) { m_toroidal = toroidal; }
    
    /**
     * @brief Retourne une représentation textuelle de la grille
     * @return Chaîne représentant la grille
     */
    std::string toString() const;
    
    /**
     * @brief Place un pattern sur la grille
     * @param pattern Matrice du pattern (true = vivant)
     * @param startX Position X de départ
     * @param startY Position Y de départ
     */
    void placePattern(const std::vector<std::vector<bool>>& pattern, int startX, int startY);
};

#endif // GRID_HPP



