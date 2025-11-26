#ifndef CLASSICRULE_HPP
#define CLASSICRULE_HPP

#include "Rule.hpp"

/**
 * @brief Règles classiques du Jeu de la Vie de Conway
 * 
 * - Une cellule morte avec exactement 3 voisins vivants devient vivante
 * - Une cellule vivante avec 2 ou 3 voisins vivants reste vivante
 * - Dans tous les autres cas, la cellule meurt ou reste morte
 */
class ClassicRule : public Rule {
public:
    ClassicRule() = default;
    ~ClassicRule() override = default;
    
    std::unique_ptr<CellState> computeNextState(bool currentlyAlive, int aliveNeighbors) const override;
    const char* getName() const override { return "Conway's Game of Life"; }
};

#endif // CLASSICRULE_HPP


