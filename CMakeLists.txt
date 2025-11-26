cmake_minimum_required(VERSION 3.16)
project(GameOfLife VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options de compilation
option(BUILD_GUI "Build with SFML GUI support" ON)
option(BUILD_TESTS "Build unit tests" ON)

# Recherche de threads pour la parallélisation
find_package(Threads REQUIRED)

# ============================================================================
# Configuration SFML
# ============================================================================
# Option 1: SFML dans MSYS2/MinGW
if(EXISTS "C:/msys64/mingw64/lib/cmake/SFML")
    set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML")
    message(STATUS "SFML found in MSYS2/MinGW64")
# Option 2: SFML dans le dossier external/ du projet
elseif(EXISTS "${CMAKE_SOURCE_DIR}/external/SFML-2.6.1")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-2.6.1/lib/cmake/SFML")
    message(STATUS "SFML found in external/SFML-2.6.1")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/external/SFML")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML/lib/cmake/SFML")
    message(STATUS "SFML found in external/SFML")
endif()

# Option 3: Variable d'environnement SFML_ROOT
if(DEFINED ENV{SFML_ROOT})
    set(SFML_DIR "$ENV{SFML_ROOT}/lib/cmake/SFML")
    message(STATUS "SFML found via SFML_ROOT: $ENV{SFML_ROOT}")
endif()

# Sources communes
set(CORE_SOURCES
    src/cell/Cell.cpp
    src/cell/CellState.cpp
    src/cell/AliveState.cpp
    src/cell/DeadState.cpp
    src/cell/ObstacleAliveState.cpp
    src/cell/ObstacleDeadState.cpp
    src/grid/Grid.cpp
    src/rules/Rule.cpp
    src/rules/ClassicRule.cpp
    src/game/Game.cpp
    src/io/FileHandler.cpp
)

# Bibliothèque core
add_library(GameOfLifeCore STATIC ${CORE_SOURCES})
target_include_directories(GameOfLifeCore PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(GameOfLifeCore PUBLIC Threads::Threads)

# Exécutable mode console
add_executable(gameoflife_console src/main_console.cpp)
target_link_libraries(gameoflife_console PRIVATE GameOfLifeCore)

# Mode graphique avec SFML
if(BUILD_GUI)
    find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
    if(SFML_FOUND)
        set(GUI_SOURCES
            src/gui/GUIRenderer.cpp
            src/gui/GUIController.cpp
        )
        add_executable(gameoflife_gui src/main_gui.cpp ${GUI_SOURCES})
        target_link_libraries(gameoflife_gui PRIVATE GameOfLifeCore sfml-graphics sfml-window sfml-system)
        message(STATUS "SFML found - Building GUI version")
        
        # Note: Sur MSYS2/MinGW, les DLLs sont dans le PATH donc pas besoin de les copier
        # Pour une distribution standalone, copiez manuellement les DLLs de C:/msys64/mingw64/bin/
    else()
        message(WARNING "SFML not found - GUI version will not be built")
        message(WARNING "To use SFML, either:")
        message(WARNING "  1. Place SFML in: ${CMAKE_SOURCE_DIR}/external/SFML-2.6.1/")
        message(WARNING "  2. Set SFML_ROOT environment variable")
        message(WARNING "  3. Install SFML system-wide")
    endif()
endif()

# Tests unitaires
if(BUILD_TESTS)
    add_executable(gameoflife_tests src/tests/test_main.cpp)
    target_link_libraries(gameoflife_tests PRIVATE GameOfLifeCore)
endif()

# Copie des fichiers d'exemple
file(COPY ${CMAKE_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})

