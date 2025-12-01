# Jeu de la Vie - Conway's Game of Life

Implémentation orientée objet du célèbre automate cellulaire de John Conway en C++17.

## Table des matières

- [Présentation](#présentation)
- [Architecture](#architecture)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Format des fichiers](#format-des-fichiers)
- [Tests unitaires](#tests-unitaires)
- [Extensions](#extensions)

## Présentation

Ce projet implémente le Jeu de la Vie en respectant les principes SOLID de la programmation orientée objet :

- **Single Responsibility** : Chaque classe a une responsabilité unique
- **Open/Closed** : Les règles et états sont extensibles sans modification du code existant
- **Liskov Substitution** : Les sous-classes peuvent remplacer leurs classes parentes
- **Interface Segregation** : Interfaces spécifiques et légères
- **Dependency Inversion** : Les modules dépendent d'abstractions

### Règles du jeu

- Une cellule morte avec exactement 3 voisins vivants devient vivante
- Une cellule vivante avec 2 ou 3 voisins vivants reste vivante, sinon elle meurt

## Architecture

```
projet-cesi-jeux-de-la-vie/
├── CMakeLists.txt              # Configuration CMake
├── README.md                   # Documentation
├── include/                    # En-têtes
│   ├── cell/                   # Classes de cellules
│   │   ├── Cell.hpp
│   │   ├── CellState.hpp       # Classe abstraite d'état
│   │   ├── AliveState.hpp      # État vivant
│   │   ├── DeadState.hpp       # État mort
│   │   ├── ObstacleAliveState.hpp
│   │   └── ObstacleDeadState.hpp
│   ├── grid/
│   │   └── Grid.hpp            # Grille de cellules
│   ├── rules/
│   │   ├── Rule.hpp            # Classe abstraite de règle
│   │   └── ClassicRule.hpp     # Règles classiques de Conway
│   ├── game/
│   │   └── Game.hpp            # Orchestrateur du jeu
│   ├── io/
│   │   └── FileHandler.hpp     # Lecture/écriture fichiers
│   ├── patterns/
│   │   └── Patterns.hpp        # Patterns prédéfinis
│   └── gui/
│       ├── GUIRenderer.hpp     # Rendu graphique
│       └── GUIController.hpp   # Contrôleur interface
├── src/                        # Implémentations
│   ├── cell/
│   ├── grid/
│   ├── rules/
│   ├── game/
│   ├── io/
│   ├── gui/
│   ├── tests/
│   │   └── test_main.cpp       # Tests unitaires
│   ├── main_console.cpp        # Point d'entrée console
│   └── main_gui.cpp            # Point d'entrée graphique
└── examples/                   # Fichiers d'exemple
    ├── glider.txt
    ├── blinker.txt
    ├── pulsar.txt
    ├── gosper_gun.txt
    └── obstacles.txt
```

### Diagramme de classes simplifié

```
CellState (abstract)
├── AliveState
├── DeadState
├── ObstacleAliveState
└── ObstacleDeadState

Rule (abstract)
└── ClassicRule

Cell ──────> CellState
Grid ──────> Cell[][]
Game ──────> Grid
     ──────> Rule

GUIController ──> Game
              ──> GUIRenderer
```

## Compilation

### Prérequis

- CMake 3.16+
- Compilateur C++17 (GCC 8+, Clang 7+, MSVC 2019+)
- SFML 2.5+ (pour le mode graphique)

### Instructions Windows (avec Visual Studio)

```powershell
# Créer le dossier de build
mkdir build
cd build

# Configurer avec CMake
cmake .. -G "Visual Studio 17 2022"

# Compiler
cmake --build . --config Release
```

### Instructions Linux/macOS

```bash
# Installer SFML (Ubuntu/Debian)
sudo apt install libsfml-dev

# Créer le dossier de build
mkdir build && cd build

# Configurer et compiler
cmake ..
make -j4
```

### Options de compilation

- `BUILD_GUI=ON/OFF` : Active/désactive la compilation de l'interface graphique
- `BUILD_TESTS=ON/OFF` : Active/désactive la compilation des tests

```bash
cmake .. -DBUILD_GUI=OFF  # Compile sans interface graphique
```

## Utilisation

### Mode Console

```bash
# Usage basique
./gameoflife_console examples/glider.txt

# Avec options
./gameoflife_console examples/glider.txt -n 50 -t -v

# Options disponibles :
#   -n <nombre>  : Nombre d'itérations (défaut: 100)
#   -t           : Mode torique (bords connectés)
#   -p           : Calcul parallèle
#   -v           : Mode verbeux
#   -h           : Aide
```

Les résultats sont sauvegardés dans `<nom_fichier>_out/`.

### Mode Graphique

```bash
# Usage basique
./gameoflife_gui examples/pulsar.txt

# Avec options
./gameoflife_gui examples/pulsar.txt -s 15 -t

# Options disponibles :
#   -s <taille>  : Taille des cellules en pixels (défaut: 10)
#   -w <largeur> : Largeur de la fenêtre
#   -h <hauteur> : Hauteur de la fenêtre
#   -t           : Mode torique
#   -n <nombre>  : Nombre max d'itérations
```

#### Contrôles dans l'interface

| Touche | Action |
|--------|--------|
| ESPACE | Pause/Reprendre |
| S | Avancer d'une itération |
| R | Réinitialiser |
| H | Afficher l'aide |
| G | Afficher/masquer la grille |
| T | Activer/désactiver mode torique |
| P | Activer calcul parallèle |
| +/- | Ajuster la vitesse |
| 1-9 | Sélectionner un pattern |
| ←/→ | Changer de pattern |
| Clic gauche | Placer le pattern sélectionné |
| Shift+Clic | Activer une cellule |
| Clic droit | Désactiver une cellule |
| ESC | Quitter |

## Format des fichiers

### Format d'entrée

```
<hauteur> <largeur>
<matrice de valeurs>
```

Valeurs possibles :
- `0` : Cellule morte
- `1` : Cellule vivante
- `2` : Obstacle mort (extension)
- `3` : Obstacle vivant (extension)

### Exemple

```
5 10
0 0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0 0
0 1 1 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
```

## Tests unitaires

```bash
# Exécuter les tests
./gameoflife_tests
```

Les tests vérifient :
- Les états des cellules
- Les règles de transition
- La création et manipulation de la grille
- Le mode torique
- Les structures connues (Blinker, Block, Glider)
- Les obstacles
- Les patterns prédéfinis
- La fonction de vérification de grille attendue

## Extensions

### 1. Grille torique

Les cellules aux extrémités sont voisines :
- Colonne 0 ↔ Colonne N-1
- Ligne 0 ↔ Ligne N-1

Activation : option `-t` ou touche `T` dans l'interface.

### 2. Cellules obstacles

Les obstacles ne changent jamais d'état mais comptent dans le voisinage.

Format fichier :
- `2` : Obstacle mort
- `3` : Obstacle vivant

### 3. Patterns pré-programmés

Patterns disponibles :
1. Glider (vaisseau)
2. Blinker (oscillateur)
3. Block (stable)
4. Beehive (stable)
5. Toad (oscillateur)
6. Beacon (oscillateur)
7. Pulsar (oscillateur période 3)
8. LWSS (vaisseau)
9. Pentadecathlon (oscillateur période 15)

### 4. Parallélisation

La mise à jour des cellules peut être parallélisée sur plusieurs threads.

Activation : option `-p` ou touche `P` dans l'interface.

## Auteurs

Projet réalisé dans le cadre du module POO - CESI

## Licence

Ce projet est sous licence MIT.



