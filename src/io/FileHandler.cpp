#include "io/FileHandler.hpp"
#include "cell/AliveState.hpp"
#include "cell/DeadState.hpp"
#include "cell/ObstacleAliveState.hpp"
#include "cell/ObstacleDeadState.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace fs = std::filesystem;

std::unique_ptr<Grid> FileHandler::loadFromFile(const std::string& filepath, bool toroidal) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }
    
    // Lecture de la première ligne : dimensions
    int height, width;
    if (!(file >> height >> width)) {
        throw std::runtime_error("Invalid file format: cannot read dimensions");
    }
    
    if (height <= 0 || width <= 0) {
        throw std::runtime_error("Invalid dimensions: must be positive");
    }
    
    auto grid = std::make_unique<Grid>(width, height, toroidal);
    
    // Lecture de la matrice de cellules
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int value;
            if (!(file >> value)) {
                throw std::runtime_error("Invalid file format: not enough cell data");
            }
            
            std::unique_ptr<CellState> state;
            switch (value) {
                case 0:
                    state = std::make_unique<DeadState>();
                    break;
                case 1:
                    state = std::make_unique<AliveState>();
                    break;
                case 2:
                    // Extension : obstacle mort
                    state = std::make_unique<ObstacleDeadState>();
                    break;
                case 3:
                    // Extension : obstacle vivant
                    state = std::make_unique<ObstacleAliveState>();
                    break;
                default:
                    throw std::runtime_error("Invalid cell value: " + std::to_string(value));
            }
            
            grid->setCellState(x, y, std::move(state));
        }
    }
    
    return grid;
}

void FileHandler::saveToFile(const Grid& grid, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filepath);
    }
    
    // Écrit les dimensions
    file << grid.getHeight() << " " << grid.getWidth() << "\n";
    
    // Écrit la matrice de cellules
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            const Cell& cell = grid.getCell(x, y);
            file << cell.toInt();
            if (x < grid.getWidth() - 1) {
                file << " ";
            }
        }
        file << "\n";
    }
}

std::string FileHandler::createOutputDirectory(const std::string& inputFilepath) {
    std::string baseName = getBaseName(inputFilepath);
    std::string outputDir = baseName + "_out";
    
    // Crée le dossier s'il n'existe pas
    fs::create_directories(outputDir);
    
    return outputDir;
}

std::string FileHandler::getIterationFilename(const std::string& outputDir, int iteration) {
    std::ostringstream oss;
    oss << outputDir << "/iteration_" << iteration << ".txt";
    return oss.str();
}

std::string FileHandler::getBaseName(const std::string& filepath) {
    fs::path path(filepath);
    return path.stem().string();
}


