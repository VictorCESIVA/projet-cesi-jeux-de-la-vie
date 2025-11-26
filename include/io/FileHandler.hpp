#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <string>
#include <memory>
#include <filesystem>
#include "../grid/Grid.hpp"

/**
 * @brief Classe gérant la lecture/écriture des fichiers
 * 
 * Responsable de charger les configurations initiales et de sauvegarder
 * les états de la grille au format spécifié.
 */
class FileHandler {
public:
    /**
     * @brief Charge une grille depuis un fichier
     * @param filepath Chemin du fichier
     * @param toroidal Mode torique
     * @return Grille chargée
     * @throw std::runtime_error si le fichier est invalide
     */
    static std::unique_ptr<Grid> loadFromFile(const std::string& filepath, bool toroidal = false);
    
    /**
     * @brief Sauvegarde une grille dans un fichier
     * @param grid Grille à sauvegarder
     * @param filepath Chemin du fichier
     */
    static void saveToFile(const Grid& grid, const std::string& filepath);
    
    /**
     * @brief Crée le dossier de sortie pour les itérations
     * @param inputFilepath Chemin du fichier d'entrée
     * @return Chemin du dossier de sortie
     */
    static std::string createOutputDirectory(const std::string& inputFilepath);
    
    /**
     * @brief Génère le nom de fichier pour une itération
     * @param outputDir Dossier de sortie
     * @param iteration Numéro d'itération
     * @return Chemin complet du fichier
     */
    static std::string getIterationFilename(const std::string& outputDir, int iteration);
    
    /**
     * @brief Extrait le nom de base d'un fichier (sans extension)
     * @param filepath Chemin du fichier
     * @return Nom de base
     */
    static std::string getBaseName(const std::string& filepath);
};

#endif // FILEHANDLER_HPP


