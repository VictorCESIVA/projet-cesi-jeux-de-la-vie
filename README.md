# 🎮 Jeu de la Vie - Conway's Game of Life

Implémentation orientée objet du célèbre automate cellulaire de John Conway en **C++17** avec interface graphique **SFML**.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## 📋 Table des matières

- [Présentation](#-présentation)
- [Fonctionnalités](#-fonctionnalités)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Architecture](#-architecture)
- [Tests](#-tests)
- [Captures d'écran](#-captures-décran)

## 🎯 Présentation

Ce projet implémente le **Jeu de la Vie** en respectant les principes **SOLID** de la programmation orientée objet :

- **S**ingle Responsibility : Chaque classe a une responsabilité unique
- **O**pen/Closed : Les règles et états sont extensibles sans modification
- **L**iskov Substitution : Les sous-classes remplacent leurs classes parentes
- **I**nterface Segregation : Interfaces spécifiques et légères
- **D**ependency Inversion : Les modules dépendent d'abstractions

### Règles de Conway
- Une cellule **morte** avec exactement **3 voisins vivants** → devient **vivante**
- Une cellule **vivante** avec **2 ou 3 voisins vivants** → reste **vivante**
- Sinon → la cellule **meurt** ou reste morte

## ✨ Fonctionnalités

### Mode Console
- ✅ Lecture de fichiers de configuration
- ✅ Génération des n premières itérations
- ✅ Export automatique dans des fichiers de sortie
- ✅ Mode verbeux avec affichage en temps réel

### Mode Graphique (SFML)
- ✅ Interface intuitive avec panneau de contrôle
- ✅ Grille vide par défaut (lancement sans fichier)
- ✅ Statistiques en temps réel (cellules vivantes, génération)
- ✅ Zoom avec la molette de souris
- ✅ Redimensionnement dynamique de la grille
- ✅ Sauvegarde de l'état actuel (F5)
- ✅ 9 patterns prédéfinis (Glider, Pulsar, LWSS...)
- ✅ Placement interactif de cellules et patterns

### Extensions (Bonus)
- ✅ **Grille torique** : Les bords sont connectés
- ✅ **Cellules obstacles** : États fixes qui ne changent pas
- ✅ **Patterns pré-programmés** : Placement avec touches 1-9
- ✅ **Calcul parallèle** : Multi-threading pour grandes grilles

## 🔧 Installation

### Prérequis

- **CMake** 3.16+
- **Compilateur C++17** (GCC 8+, Clang 7+, MSVC 2019+)
- **SFML** 2.5+ (pour le mode graphique)

### Windows (avec MSYS2/MinGW)

```powershell
# 1. Installer MSYS2 depuis https://www.msys2.org/

# 2. Dans le terminal MSYS2, installer les dépendances :
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml

# 3. Ajouter MinGW au PATH (PowerShell) :
$env:Path = "C:\msys64\mingw64\bin;" + $env:Path

# 4. Compiler le projet :
cd projet-cesi-jeux-de-la-vie
mkdir build; cd build
cmake .. -G "MinGW Makefiles"
mingw32-make -j4
```

### Linux (Ubuntu/Debian)

```bash
# 1. Installer les dépendances
sudo apt update
sudo apt install build-essential cmake libsfml-dev

# 2. Compiler
mkdir build && cd build
cmake ..
make -j4
```

### macOS

```bash
# 1. Installer les dépendances
brew install cmake sfml

# 2. Compiler
mkdir build && cd build
cmake ..
make -j4
```

## 🚀 Utilisation

### Mode Graphique (recommandé)

```bash
# Lancer avec une grille vide (50x50 par défaut)
./gameoflife_gui

# Lancer avec une grille personnalisée
./gameoflife_gui -g 80x60

# Charger un fichier existant
./gameoflife_gui examples/pulsar.txt

# Options disponibles
./gameoflife_gui --help
```

### Contrôles de l'interface

| Touche | Action |
|--------|--------|
| `ESPACE` | Démarrer / Pause |
| `S` | Avancer d'une génération |
| `C` | Effacer la grille |
| `R` | Réinitialiser |
| `+` / `-` | Ajuster la vitesse |
| `T` | Mode torique ON/OFF |
| `P` | Calcul parallèle ON/OFF |
| `G` | Afficher/masquer la grille |
| `[` / `]` | Réduire/Agrandir la grille |
| `Ctrl+Flèches` | Ajuster largeur/hauteur |
| `1-9` | Sélectionner un pattern |
| `←` / `→` | Changer de pattern |
| `F5` | Sauvegarder l'état |
| `H` | Afficher l'aide |
| `ESC` | Quitter |

| Souris | Action |
|--------|--------|
| Clic gauche | Placer le pattern sélectionné |
| Shift + Clic | Activer une cellule |
| Ctrl + Clic | Créer un obstacle |
| Clic droit | Supprimer une cellule |
| Molette | Zoom +/- |

### Mode Console

```bash
# Exécuter 100 itérations
./gameoflife_console examples/glider.txt -n 100

# Mode verbeux avec grille torique
./gameoflife_console examples/pulsar.txt -n 50 -t -v

# Options
./gameoflife_console --help
```

### Format des fichiers

```
<hauteur> <largeur>
<matrice de valeurs>
```

**Valeurs possibles :**
- `0` : Cellule morte
- `1` : Cellule vivante
- `2` : Obstacle mort (ne change pas)
- `3` : Obstacle vivant (ne change pas)

**Exemple (Glider) :**
```
5 10
0 0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0 0
0 1 1 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
```

## 🏗 Architecture

```
projet-cesi-jeux-de-la-vie/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── cell/
│   │   ├── Cell.hpp
│   │   ├── CellState.hpp          # Classe abstraite (polymorphisme)
│   │   ├── AliveState.hpp
│   │   ├── DeadState.hpp
│   │   ├── ObstacleAliveState.hpp
│   │   └── ObstacleDeadState.hpp
│   ├── grid/
│   │   └── Grid.hpp               # Grille avec support torique
│   ├── rules/
│   │   ├── Rule.hpp               # Classe abstraite des règles
│   │   └── ClassicRule.hpp        # Règles de Conway
│   ├── game/
│   │   └── Game.hpp               # Orchestrateur
│   ├── io/
│   │   └── FileHandler.hpp        # Lecture/écriture fichiers
│   ├── patterns/
│   │   └── Patterns.hpp           # Patterns prédéfinis
│   └── gui/
│       ├── GUIRenderer.hpp        # Rendu graphique
│       └── GUIController.hpp      # Contrôleur interface
├── src/
│   ├── cell/
│   ├── grid/
│   ├── rules/
│   ├── game/
│   ├── io/
│   ├── gui/
│   ├── tests/
│   │   └── test_main.cpp          # Tests unitaires
│   ├── main_console.cpp
│   └── main_gui.cpp
└── examples/
    ├── glider.txt
    ├── blinker.txt
    ├── pulsar.txt
    ├── gosper_gun.txt
    └── obstacles.txt
```

### Diagramme de classes simplifié

```
┌─────────────────┐
│   CellState     │ (abstract)
├─────────────────┤
│ + isAlive()     │
│ + isObstacle()  │
│ + clone()       │
└────────┬────────┘
         │
    ┌────┴────┬──────────┬──────────────┐
    ▼         ▼          ▼              ▼
┌───────┐ ┌───────┐ ┌──────────┐ ┌──────────┐
│ Alive │ │ Dead  │ │ Obstacle │ │ Obstacle │
│ State │ │ State │ │  Alive   │ │   Dead   │
└───────┘ └───────┘ └──────────┘ └──────────┘

┌─────────────────┐
│      Rule       │ (abstract)
├─────────────────┤
│ + computeNext() │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  ClassicRule    │
└─────────────────┘

┌──────┐     ┌──────┐     ┌──────┐
│ Game │────▶│ Grid │────▶│ Cell │────▶ CellState
└──────┘     └──────┘     └──────┘
    │
    ▼
  Rule
```

## 🧪 Tests

```bash
# Exécuter les tests unitaires
./gameoflife_tests
```

Les tests vérifient :
- ✅ États des cellules (Alive, Dead, Obstacles)
- ✅ Règles de transition de Conway
- ✅ Comptage des voisins
- ✅ Mode torique
- ✅ Structures connues (Blinker, Block, Glider)
- ✅ Fonctionnement des obstacles
- ✅ Patterns prédéfinis

## 📸 Patterns disponibles

| # | Nom | Type | Description |
|---|-----|------|-------------|
| 1 | Glider | Vaisseau | Se déplace en diagonale |
| 2 | Blinker | Oscillateur (période 2) | Ligne qui oscille |
| 3 | Block | Stable | Carré 2x2 fixe |
| 4 | Beehive | Stable | Structure en ruche |
| 5 | Toad | Oscillateur (période 2) | Double ligne décalée |
| 6 | Beacon | Oscillateur (période 2) | Deux blocs clignotants |
| 7 | Pulsar | Oscillateur (période 3) | Grande structure symétrique |
| 8 | LWSS | Vaisseau | Lightweight Spaceship |
| 9 | Pentadecathlon | Oscillateur (période 15) | Structure complexe |

## 👥 Auteurs

Projet réalisé dans le cadre du module **POO** - CESI

## 📦 Créer une release

Pour créer un dossier release avec tous les exécutables et DLLs :

```powershell
# Compiler le projet
mkdir build; cd build
cmake .. -G "MinGW Makefiles"
mingw32-make -j4

# Le dossier release/ contiendra :
# - gameoflife_console.exe
# - gameoflife_gui.exe  
# - gameoflife_tests.exe
# - Toutes les DLLs nécessaires
# - Les fichiers d'exemple
```

## 📄 Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.
