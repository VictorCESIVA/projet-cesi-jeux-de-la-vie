#include "gui/GUIRenderer.hpp"
#include <sstream>
#include <iomanip>

GUIRenderer::GUIRenderer(sf::RenderWindow& window, int cellSize)
    : m_window(window)
    , m_cellSize(cellSize)
    , m_aliveColor(sf::Color(46, 204, 113))        // Vert émeraude
    , m_deadColor(sf::Color(44, 62, 80))           // Bleu foncé
    , m_obstacleAliveColor(sf::Color(230, 126, 34)) // Orange
    , m_obstacleDeadColor(sf::Color(127, 140, 141)) // Gris
    , m_gridColor(sf::Color(52, 73, 94))           // Bleu gris
    , m_fontLoaded(false)
    , m_showGrid(true)
    , m_offsetX(200)   // Espace pour le panneau latéral
    , m_offsetY(50)    // Espace pour le panneau info
    , m_panelWidth(200)
{
    // Tente de charger une police système
    const std::vector<std::string> fontPaths = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };
    
    for (const auto& path : fontPaths) {
        if (m_font.loadFromFile(path)) {
            m_fontLoaded = true;
            break;
        }
    }
}

void GUIRenderer::render(const Grid& grid) {
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();
    
    // Dessine les cellules
    sf::RectangleShape cell(sf::Vector2f(static_cast<float>(m_cellSize - 1), 
                                         static_cast<float>(m_cellSize - 1)));
    
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            const Cell& c = grid.getCell(x, y);
            
            // Détermine la couleur
            sf::Color color;
            if (c.isObstacle()) {
                color = c.isAlive() ? m_obstacleAliveColor : m_obstacleDeadColor;
            } else {
                color = c.isAlive() ? m_aliveColor : m_deadColor;
            }
            
            cell.setFillColor(color);
            cell.setPosition(
                static_cast<float>(m_offsetX + x * m_cellSize),
                static_cast<float>(m_offsetY + y * m_cellSize)
            );
            m_window.draw(cell);
        }
    }
    
    // Dessine la grille si activée
    if (m_showGrid && m_cellSize > 4) {
        sf::VertexArray lines(sf::Lines);
        
        // Lignes verticales
        for (int x = 0; x <= gridWidth; ++x) {
            float xPos = static_cast<float>(m_offsetX + x * m_cellSize);
            lines.append(sf::Vertex(sf::Vector2f(xPos, static_cast<float>(m_offsetY)), m_gridColor));
            lines.append(sf::Vertex(sf::Vector2f(xPos, static_cast<float>(m_offsetY + gridHeight * m_cellSize)), m_gridColor));
        }
        
        // Lignes horizontales
        for (int y = 0; y <= gridHeight; ++y) {
            float yPos = static_cast<float>(m_offsetY + y * m_cellSize);
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(m_offsetX), yPos), m_gridColor));
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(m_offsetX + gridWidth * m_cellSize), yPos), m_gridColor));
        }
        
        m_window.draw(lines);
    }
}

void GUIRenderer::renderInfoPanel(int iteration, bool running, int speed, bool toroidal, bool parallel,
                                   int aliveCells, bool stable, int gridWidth, int gridHeight) {
    // Fond du panneau supérieur
    sf::RectangleShape panel(sf::Vector2f(static_cast<float>(m_window.getSize().x), 
                                          static_cast<float>(m_offsetY)));
    panel.setFillColor(sf::Color(30, 30, 30));
    m_window.draw(panel);
    
    if (!m_fontLoaded) return;
    
    // Texte d'information
    std::ostringstream info;
    info << "Gen: " << iteration;
    info << "  |  Vivantes: " << aliveCells;
    info << "  |  Grille: " << gridWidth << "x" << gridHeight;
    info << "  |  " << (running ? "EN COURS" : "PAUSE");
    if (stable && iteration > 0) info << "  |  STABLE";
    
    sf::Text text;
    text.setFont(m_font);
    text.setString(info.str());
    text.setCharacterSize(13);
    text.setFillColor(running ? sf::Color(46, 204, 113) : sf::Color::White);
    text.setPosition(m_offsetX + 10.f, 8.f);
    m_window.draw(text);
    
    // Indicateurs d'options
    std::ostringstream opts;
    if (toroidal) opts << "[Torique] ";
    if (parallel) opts << "[Parallele] ";
    opts << "Vitesse: " << speed << "ms  |  Zoom: " << m_cellSize << "px";
    
    sf::Text optsText;
    optsText.setFont(m_font);
    optsText.setString(opts.str());
    optsText.setCharacterSize(11);
    optsText.setFillColor(sf::Color(100, 180, 100));
    optsText.setPosition(m_offsetX + 10.f, 28.f);
    m_window.draw(optsText);
    
    // Instructions rapides
    sf::Text quickHelp;
    quickHelp.setFont(m_font);
    quickHelp.setString("[ESPACE] Play  [[] Taille  [H] Aide");
    quickHelp.setCharacterSize(10);
    quickHelp.setFillColor(sf::Color(120, 120, 120));
    quickHelp.setPosition(static_cast<float>(m_window.getSize().x - 220), 18.f);
    m_window.draw(quickHelp);
}

void GUIRenderer::renderSidePanel(const std::string& selectedPattern, bool toroidal, bool parallel) {
    // Fond du panneau latéral
    sf::RectangleShape panel(sf::Vector2f(static_cast<float>(m_panelWidth), 
                                          static_cast<float>(m_window.getSize().y)));
    panel.setFillColor(sf::Color(25, 25, 35));
    m_window.draw(panel);
    
    if (!m_fontLoaded) return;
    
    float yPos = 60.f;
    float xMargin = 10.f;
    
    // Titre
    sf::Text title;
    title.setFont(m_font);
    title.setString("JEU DE LA VIE");
    title.setCharacterSize(16);
    title.setFillColor(sf::Color(46, 204, 113));
    title.setStyle(sf::Text::Bold);
    title.setPosition(xMargin, yPos);
    m_window.draw(title);
    yPos += 35.f;
    
    // Section Contrôles
    sf::Text sectionCtrl;
    sectionCtrl.setFont(m_font);
    sectionCtrl.setString("CONTROLES");
    sectionCtrl.setCharacterSize(12);
    sectionCtrl.setFillColor(sf::Color(100, 100, 100));
    sectionCtrl.setPosition(xMargin, yPos);
    m_window.draw(sectionCtrl);
    yPos += 20.f;
    
    // Boutons de contrôle (représentation visuelle)
    std::vector<std::pair<std::string, std::string>> controls = {
        {"[ESPACE]", "Play/Pause"},
        {"[S]", "1 iteration"},
        {"[C]", "Effacer grille"},
        {"[R]", "Reinitialiser"},
        {"[+/-]", "Vitesse"},
    };
    
    for (const auto& ctrl : controls) {
        sf::Text keyText;
        keyText.setFont(m_font);
        keyText.setString(ctrl.first);
        keyText.setCharacterSize(11);
        keyText.setFillColor(sf::Color(150, 200, 150));
        keyText.setPosition(xMargin, yPos);
        m_window.draw(keyText);
        
        sf::Text descText;
        descText.setFont(m_font);
        descText.setString(ctrl.second);
        descText.setCharacterSize(11);
        descText.setFillColor(sf::Color(180, 180, 180));
        descText.setPosition(xMargin + 60.f, yPos);
        m_window.draw(descText);
        yPos += 18.f;
    }
    
    yPos += 15.f;
    
    // Section Options
    sf::Text sectionOpt;
    sectionOpt.setFont(m_font);
    sectionOpt.setString("OPTIONS");
    sectionOpt.setCharacterSize(12);
    sectionOpt.setFillColor(sf::Color(100, 100, 100));
    sectionOpt.setPosition(xMargin, yPos);
    m_window.draw(sectionOpt);
    yPos += 20.f;
    
    // Option Torique
    sf::RectangleShape toggleBg(sf::Vector2f(14.f, 14.f));
    toggleBg.setPosition(xMargin, yPos);
    toggleBg.setFillColor(toroidal ? sf::Color(46, 204, 113) : sf::Color(60, 60, 60));
    toggleBg.setOutlineColor(sf::Color(100, 100, 100));
    toggleBg.setOutlineThickness(1.f);
    m_window.draw(toggleBg);
    
    sf::Text toroidalText;
    toroidalText.setFont(m_font);
    toroidalText.setString("[T] Mode Torique");
    toroidalText.setCharacterSize(11);
    toroidalText.setFillColor(sf::Color(180, 180, 180));
    toroidalText.setPosition(xMargin + 22.f, yPos);
    m_window.draw(toroidalText);
    yPos += 22.f;
    
    // Option Parallèle
    sf::RectangleShape toggleBg2(sf::Vector2f(14.f, 14.f));
    toggleBg2.setPosition(xMargin, yPos);
    toggleBg2.setFillColor(parallel ? sf::Color(46, 204, 113) : sf::Color(60, 60, 60));
    toggleBg2.setOutlineColor(sf::Color(100, 100, 100));
    toggleBg2.setOutlineThickness(1.f);
    m_window.draw(toggleBg2);
    
    sf::Text parallelText;
    parallelText.setFont(m_font);
    parallelText.setString("[P] Parallele");
    parallelText.setCharacterSize(11);
    parallelText.setFillColor(sf::Color(180, 180, 180));
    parallelText.setPosition(xMargin + 22.f, yPos);
    m_window.draw(parallelText);
    yPos += 22.f;
    
    // Option Grille
    sf::RectangleShape toggleBg3(sf::Vector2f(14.f, 14.f));
    toggleBg3.setPosition(xMargin, yPos);
    toggleBg3.setFillColor(m_showGrid ? sf::Color(46, 204, 113) : sf::Color(60, 60, 60));
    toggleBg3.setOutlineColor(sf::Color(100, 100, 100));
    toggleBg3.setOutlineThickness(1.f);
    m_window.draw(toggleBg3);
    
    sf::Text gridText;
    gridText.setFont(m_font);
    gridText.setString("[G] Afficher grille");
    gridText.setCharacterSize(11);
    gridText.setFillColor(sf::Color(180, 180, 180));
    gridText.setPosition(xMargin + 22.f, yPos);
    m_window.draw(gridText);
    yPos += 30.f;
    
    // Section Patterns
    sf::Text sectionPat;
    sectionPat.setFont(m_font);
    sectionPat.setString("PATTERNS [1-9]");
    sectionPat.setCharacterSize(12);
    sectionPat.setFillColor(sf::Color(100, 100, 100));
    sectionPat.setPosition(xMargin, yPos);
    m_window.draw(sectionPat);
    yPos += 20.f;
    
    std::vector<std::pair<std::string, std::string>> patterns = {
        {"1", "Glider"},
        {"2", "Blinker"},
        {"3", "Block"},
        {"4", "Beehive"},
        {"5", "Toad"},
        {"6", "Beacon"},
        {"7", "Pulsar"},
        {"8", "LWSS"},
        {"9", "Pentadeca"},
    };
    
    for (const auto& pat : patterns) {
        bool isSelected = (pat.second == selectedPattern || 
                          (selectedPattern == "glider" && pat.second == "Glider") ||
                          (selectedPattern == "blinker" && pat.second == "Blinker") ||
                          (selectedPattern == "block" && pat.second == "Block") ||
                          (selectedPattern == "beehive" && pat.second == "Beehive") ||
                          (selectedPattern == "toad" && pat.second == "Toad") ||
                          (selectedPattern == "beacon" && pat.second == "Beacon") ||
                          (selectedPattern == "pulsar" && pat.second == "Pulsar") ||
                          (selectedPattern == "lwss" && pat.second == "LWSS") ||
                          (selectedPattern == "pentadecathlon" && pat.second == "Pentadeca"));
        
        sf::Text numText;
        numText.setFont(m_font);
        numText.setString("[" + pat.first + "]");
        numText.setCharacterSize(11);
        numText.setFillColor(isSelected ? sf::Color(46, 204, 113) : sf::Color(100, 150, 100));
        numText.setPosition(xMargin, yPos);
        m_window.draw(numText);
        
        sf::Text patText;
        patText.setFont(m_font);
        patText.setString(pat.second);
        patText.setCharacterSize(11);
        patText.setFillColor(isSelected ? sf::Color::White : sf::Color(150, 150, 150));
        if (isSelected) patText.setStyle(sf::Text::Bold);
        patText.setPosition(xMargin + 30.f, yPos);
        m_window.draw(patText);
        yPos += 18.f;
    }
    
    yPos += 15.f;
    
    // Instructions souris
    sf::Text mouseTitle;
    mouseTitle.setFont(m_font);
    mouseTitle.setString("SOURIS");
    mouseTitle.setCharacterSize(12);
    mouseTitle.setFillColor(sf::Color(100, 100, 100));
    mouseTitle.setPosition(xMargin, yPos);
    m_window.draw(mouseTitle);
    yPos += 20.f;
    
    sf::Text mouseInfo;
    mouseInfo.setFont(m_font);
    mouseInfo.setString("Clic G: Placer pattern\nShift+Clic: 1 cellule\nCtrl+Clic: Obstacle\nClic D: Supprimer\nMolette: Zoom");
    mouseInfo.setCharacterSize(10);
    mouseInfo.setFillColor(sf::Color(150, 150, 150));
    mouseInfo.setPosition(xMargin, yPos);
    m_window.draw(mouseInfo);
    yPos += 70.f;
    
    // Section Grille
    sf::Text gridSection;
    gridSection.setFont(m_font);
    gridSection.setString("TAILLE GRILLE");
    gridSection.setCharacterSize(12);
    gridSection.setFillColor(sf::Color(100, 100, 100));
    gridSection.setPosition(xMargin, yPos);
    m_window.draw(gridSection);
    yPos += 20.f;
    
    sf::Text gridInfo;
    gridInfo.setFont(m_font);
    gridInfo.setString("[ ] Reduire/Agrandir\nCtrl+Fleches: W/H");
    gridInfo.setCharacterSize(10);
    gridInfo.setFillColor(sf::Color(150, 150, 150));
    gridInfo.setPosition(xMargin, yPos);
    m_window.draw(gridInfo);
    yPos += 35.f;
    
    // Section Fichier
    sf::Text fileSection;
    fileSection.setFont(m_font);
    fileSection.setString("FICHIER");
    fileSection.setCharacterSize(12);
    fileSection.setFillColor(sf::Color(100, 100, 100));
    fileSection.setPosition(xMargin, yPos);
    m_window.draw(fileSection);
    yPos += 20.f;
    
    sf::Text saveInfo;
    saveInfo.setFont(m_font);
    saveInfo.setString("[F5] Sauvegarder");
    saveInfo.setCharacterSize(11);
    saveInfo.setFillColor(sf::Color(150, 150, 150));
    saveInfo.setPosition(xMargin, yPos);
    m_window.draw(saveInfo);
}

void GUIRenderer::renderHelp() {
    if (!m_fontLoaded) return;
    
    // Fond semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(m_window.getSize().x), 
                                            static_cast<float>(m_window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    m_window.draw(overlay);
    
    // Cadre central
    float boxWidth = 500.f;
    float boxHeight = 450.f;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setPosition((m_window.getSize().x - boxWidth) / 2.f, (m_window.getSize().y - boxHeight) / 2.f);
    box.setFillColor(sf::Color(35, 35, 45));
    box.setOutlineColor(sf::Color(46, 204, 113));
    box.setOutlineThickness(2.f);
    m_window.draw(box);
    
    // Texte d'aide
    std::string helpText = 
        "JEU DE LA VIE - AIDE\n"
        "====================\n\n"
        "REGLES DE CONWAY:\n"
        "- Cellule morte + 3 voisins = nait\n"
        "- Cellule vivante + 2-3 voisins = survit\n"
        "- Sinon = meurt\n\n"
        "CONTROLES CLAVIER:\n"
        "  ESPACE      Demarrer/Pause\n"
        "  S           Avancer 1 iteration\n"
        "  C           Effacer la grille\n"
        "  R           Reinitialiser\n"
        "  +/-         Ajuster vitesse\n"
        "  G           Afficher/Masquer grille\n"
        "  T           Mode torique ON/OFF\n"
        "  P           Calcul parallele ON/OFF\n"
        "  [ ]         Reduire/Agrandir grille\n"
        "  Ctrl+Fleches  Taille largeur/hauteur\n"
        "  1-9         Selectionner pattern\n"
        "  F5          Sauvegarder etat\n"
        "  H           Cette aide\n"
        "  ESC         Quitter\n\n"
        "SOURIS:\n"
        "  Clic gauche    Placer pattern\n"
        "  Shift+Clic     Activer 1 cellule\n"
        "  Ctrl+Clic      Creer obstacle\n"
        "  Clic droit     Supprimer cellule\n"
        "  Molette        Zoom +/-\n\n"
        "Appuyez sur H pour fermer";
    
    sf::Text text;
    text.setFont(m_font);
    text.setString(helpText);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition((m_window.getSize().x - boxWidth) / 2.f + 30.f, 
                     (m_window.getSize().y - boxHeight) / 2.f + 20.f);
    m_window.draw(text);
}

bool GUIRenderer::screenToGrid(int screenX, int screenY, int& gridX, int& gridY, const Grid& grid) const {
    if (screenY < m_offsetY || screenX < m_offsetX) return false;
    
    gridX = (screenX - m_offsetX) / m_cellSize;
    gridY = (screenY - m_offsetY) / m_cellSize;
    
    return gridX >= 0 && gridX < grid.getWidth() && gridY >= 0 && gridY < grid.getHeight();
}

int GUIRenderer::getSidePanelButtonAt(int screenX, int screenY) const {
    // Pour l'instant, pas de boutons cliquables dans le panneau
    return -1;
}
