#ifndef CELLSTATE_HPP
#define CELLSTATE_HPP

#include <memory>

// Forward declaration
class Cell;

/**
 * @brief Classe abstraite représentant l'état d'une cellule
 * 
 * Utilise le pattern State pour gérer les différents états des cellules.
 * Permet le polymorphisme d'héritage pour les états vivant/mort/obstacle.
 */
class CellState {
public:
    virtual ~CellState() = default;
    
    /**
     * @brief Indique si la cellule est vivante
     * @return true si vivante, false sinon
     */
    virtual bool isAlive() const = 0;
    
    /**
     * @brief Indique si la cellule est un obstacle (état fixe)
     * @return true si obstacle, false sinon
     */
    virtual bool isObstacle() const = 0;
    
    /**
     * @brief Crée une copie de l'état
     * @return Pointeur unique vers la copie
     */
    virtual std::unique_ptr<CellState> clone() const = 0;
    
    /**
     * @brief Retourne le caractère représentant l'état pour l'affichage console
     * @return Caractère de représentation
     */
    virtual char toChar() const = 0;
    
    /**
     * @brief Retourne la valeur numérique pour l'export fichier
     * @return 1 si vivant, 0 si mort
     */
    virtual int toInt() const = 0;
};

#endif // CELLSTATE_HPP



