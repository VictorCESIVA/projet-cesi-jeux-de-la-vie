#ifndef RULE_HPP
#define RULE_HPP

#include <memory>
#include "../cell/CellState.hpp"

/**
 * @brief Classe abstraite définissant les règles de calcul d'état
 * 
 * Permet de définir différentes règles de transition d'état pour les cellules.
 * Utilise le pattern Strategy pour permettre l'interchangeabilité des règles.
 */
class Rule {
public:
    virtual ~Rule() = default;
    
    /**
     * @brief Calcule le prochain état d'une cellule
     * @param currentlyAlive État actuel (vivant ou mort)
     * @param aliveNeighbors Nombre de voisins vivants
     * @return Nouvel état de la cellule
     */
    virtual std::unique_ptr<CellState> computeNextState(bool currentlyAlive, int aliveNeighbors) const = 0;
    
    /**
     * @brief Retourne le nom de la règle
     * @return Nom descriptif de la règle
     */
    virtual const char* getName() const = 0;
};

#endif // RULE_HPP



