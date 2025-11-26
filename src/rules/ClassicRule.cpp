#include "rules/ClassicRule.hpp"
#include "cell/AliveState.hpp"
#include "cell/DeadState.hpp"

std::unique_ptr<CellState> ClassicRule::computeNextState(bool currentlyAlive, int aliveNeighbors) const {
    // Règles classiques de Conway:
    // - Une cellule morte avec exactement 3 voisins vivants devient vivante
    // - Une cellule vivante avec 2 ou 3 voisins vivants reste vivante
    // - Dans tous les autres cas, la cellule meurt ou reste morte
    
    if (currentlyAlive) {
        // Cellule vivante : survit avec 2 ou 3 voisins
        if (aliveNeighbors == 2 || aliveNeighbors == 3) {
            return std::make_unique<AliveState>();
        } else {
            return std::make_unique<DeadState>();
        }
    } else {
        // Cellule morte : naît avec exactement 3 voisins
        if (aliveNeighbors == 3) {
            return std::make_unique<AliveState>();
        } else {
            return std::make_unique<DeadState>();
        }
    }
}


