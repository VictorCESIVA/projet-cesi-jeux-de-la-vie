#ifndef CELL_HPP
#define CELL_HPP

#include <memory>
#include "CellState.hpp"

/**
 * @brief Classe représentant une cellule dans la grille
 * 
 * Chaque cellule possède une position et un état (vivant, mort, obstacle).
 * L'état est géré via le pattern State pour permettre le polymorphisme.
 */
class Cell {
private:
    int m_x;                              ///< Position X dans la grille
    int m_y;                              ///< Position Y dans la grille
    std::unique_ptr<CellState> m_state;   ///< État actuel de la cellule
    std::unique_ptr<CellState> m_nextState; ///< Prochain état (pour mise à jour synchrone)

public:
    /**
     * @brief Constructeur
     * @param x Position X
     * @param y Position Y
     * @param state État initial de la cellule
     */
    Cell(int x, int y, std::unique_ptr<CellState> state);
    
    /**
     * @brief Constructeur de copie
     */
    Cell(const Cell& other);
    
    /**
     * @brief Opérateur d'affectation
     */
    Cell& operator=(const Cell& other);
    
    /**
     * @brief Constructeur de déplacement
     */
    Cell(Cell&& other) noexcept = default;
    
    /**
     * @brief Opérateur de déplacement
     */
    Cell& operator=(Cell&& other) noexcept = default;
    
    ~Cell() = default;

    // Getters
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    bool isAlive() const { return m_state->isAlive(); }
    bool isObstacle() const { return m_state->isObstacle(); }
    const CellState& getState() const { return *m_state; }
    
    /**
     * @brief Définit le prochain état de la cellule
     * @param state Nouvel état
     */
    void setNextState(std::unique_ptr<CellState> state);
    
    /**
     * @brief Applique le prochain état comme état actuel
     */
    void applyNextState();
    
    /**
     * @brief Définit directement l'état de la cellule
     * @param state Nouvel état
     */
    void setState(std::unique_ptr<CellState> state);
    
    /**
     * @brief Retourne le caractère représentant la cellule
     */
    char toChar() const { return m_state->toChar(); }
    
    /**
     * @brief Retourne la valeur numérique de la cellule
     */
    int toInt() const { return m_state->toInt(); }
};

#endif // CELL_HPP


