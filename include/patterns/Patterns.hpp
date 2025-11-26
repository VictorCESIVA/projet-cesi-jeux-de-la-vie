#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include <vector>
#include <map>
#include <string>

/**
 * @brief Collection de patterns prédéfinis pour le Jeu de la Vie
 * 
 * Contient des structures connues comme les oscillateurs, les vaisseaux, etc.
 */
namespace Patterns {
    
    using Pattern = std::vector<std::vector<bool>>;
    
    /**
     * @brief Glider - Vaisseau simple
     */
    inline Pattern glider() {
        return {
            {false, true, false},
            {false, false, true},
            {true, true, true}
        };
    }
    
    /**
     * @brief Blinker - Oscillateur période 2
     */
    inline Pattern blinker() {
        return {
            {true, true, true}
        };
    }
    
    /**
     * @brief Block - Structure stable
     */
    inline Pattern block() {
        return {
            {true, true},
            {true, true}
        };
    }
    
    /**
     * @brief Beehive - Structure stable
     */
    inline Pattern beehive() {
        return {
            {false, true, true, false},
            {true, false, false, true},
            {false, true, true, false}
        };
    }
    
    /**
     * @brief Toad - Oscillateur période 2
     */
    inline Pattern toad() {
        return {
            {false, true, true, true},
            {true, true, true, false}
        };
    }
    
    /**
     * @brief Beacon - Oscillateur période 2
     */
    inline Pattern beacon() {
        return {
            {true, true, false, false},
            {true, false, false, false},
            {false, false, false, true},
            {false, false, true, true}
        };
    }
    
    /**
     * @brief Pulsar - Oscillateur période 3
     */
    inline Pattern pulsar() {
        return {
            {false, false, true, true, true, false, false, false, true, true, true, false, false},
            {false, false, false, false, false, false, false, false, false, false, false, false, false},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {false, false, true, true, true, false, false, false, true, true, true, false, false},
            {false, false, false, false, false, false, false, false, false, false, false, false, false},
            {false, false, true, true, true, false, false, false, true, true, true, false, false},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {true, false, false, false, false, true, false, true, false, false, false, false, true},
            {false, false, false, false, false, false, false, false, false, false, false, false, false},
            {false, false, true, true, true, false, false, false, true, true, true, false, false}
        };
    }
    
    /**
     * @brief Lightweight spaceship (LWSS)
     */
    inline Pattern lwss() {
        return {
            {false, true, false, false, true},
            {true, false, false, false, false},
            {true, false, false, false, true},
            {true, true, true, true, false}
        };
    }
    
    /**
     * @brief Pentadecathlon - Oscillateur période 15
     */
    inline Pattern pentadecathlon() {
        return {
            {false, false, true, false, false, false, false, true, false, false},
            {true, true, false, true, true, true, true, false, true, true},
            {false, false, true, false, false, false, false, true, false, false}
        };
    }
    
    /**
     * @brief Retourne un pattern par son nom
     * @param name Nom du pattern
     * @return Pattern correspondant ou pattern vide
     */
    inline Pattern getByName(const std::string& name) {
        static const std::map<std::string, Pattern(*)()> patterns = {
            {"glider", glider},
            {"blinker", blinker},
            {"block", block},
            {"beehive", beehive},
            {"toad", toad},
            {"beacon", beacon},
            {"pulsar", pulsar},
            {"lwss", lwss},
            {"pentadecathlon", pentadecathlon}
        };
        
        auto it = patterns.find(name);
        if (it != patterns.end()) {
            return it->second();
        }
        return {};
    }
    
    /**
     * @brief Liste tous les noms de patterns disponibles
     * @return Liste des noms
     */
    inline std::vector<std::string> listPatterns() {
        return {"glider", "blinker", "block", "beehive", "toad", "beacon", "pulsar", "lwss", "pentadecathlon"};
    }
}

#endif // PATTERNS_HPP


