#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include <gui/AnimatedCharacter.h>
#include <gui/AnimatedSpell.h>
#include <gui/MapLayer.h>

#include <algorithm>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace GUI {
    enum CharType {
        Wizard,
        Fighter,
        Cleric
    };

    enum NpcType {
        Orc,
        Goblin,
        Wolf
    };

    enum SpellType {
        Fireball,
        Heal
    };

    const std::map<CharType, std::string> kCharSpriteSheetFilePathMap {
        {CharType::Wizard, "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\assets\\characters\\custom_wizard\\spritesheet.png"}
    };

    const std::map<SpellType, std::string> kSpellSpriteSheetFilePathMap {
        { SpellType::Fireball, "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\assets\\spells\\fireball.png" }
    };

    const std::map<NpcType, std::string> kNpcSpriteSheetFilePathMap {
        { NpcType::Goblin, "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\assets\\characters\\LPC_Goblin\\goblinsword.png" }
    };

    const std::map<NpcType, sf::IntRect> kNpcSingleElementBoxMap {
        { NpcType::Goblin, sf::IntRect(0u, 0u, 64u, 64u) }
    };
    
    const std::map<NpcType, uint32_t> kNpcMoveCycleMap {
        { NpcType::Goblin, 7u }
    };

    const std::string kCsvMapFilePath { "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\maps\\sampleMap.csv" };

    // Game window parameters
    uint32_t constexpr kWindowWidth { 1920u };
    uint32_t constexpr kWindowHeight { 1080u };
    uint32_t constexpr kFrameRateLimit { 144u };

    // Game design parameters
    std::string kFontPath { "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\assets\\fonts\\knight-vision-font\\KnightVision-p7Ezy.ttf" };
    uint32_t kFontSize { 50u };

    // Map parameters
    const sf::Vector2f kPixelOffset(100, 200); // Offset of the map from top-left corner of the window; Pixels
    const sf::Vector2f kCellSize(75, 75); // Dimensions of a cell; Pixels

    class GameEnvironment {
    public:
        GameEnvironment() : m_gameWindow(sf::RenderWindow( {kWindowWidth, kWindowHeight}, "Tabletop Simulator", sf::Style::Default)),
                            m_mapLayer(kCellSize, kPixelOffset) {
            m_gameWindow.setFramerateLimit(kFrameRateLimit);

            if (!m_font.loadFromFile(kFontPath)) {
                throw std::runtime_error("ERROR: GameEnvironment::GameEnvironment(): Failed to load game's font file!");
            }
            m_headerText = sf::Text("Tabletop Simulator", m_font, kFontSize);
            m_headerText.setStyle(sf::Text::Bold | sf::Text::Underlined);

            // Use the premade map for testing purposes
            if (!m_mapLayer.loadMap(kCsvMapFilePath)) {
                throw std::runtime_error("ERROR: GameEnvironment::GameEnvironment(): Failed to load the premade map!");
            }

            m_mapBoundingBox = m_mapLayer.getMapRect();
        }

        sf::Vector2f convertCellToPixelPosition(const uint16_t& cellX, const uint16_t& cellY, 
                    const sf::Vector2f& spritePadding) {
            // Convert cell position to pixel position
            const float pixelX = (static_cast<float>(cellX) + 0.5) * kCellSize.x;
            const float pixelY = (static_cast<float>(cellY) + 0.5) * kCellSize.y;
            // Apply game map pixel offsets & padding to ensure the sprite is placed in the center of the cell
            const float x = m_mapBoundingBox.left + pixelX;
            const float y = m_mapBoundingBox.top + pixelY;

            return sf::Vector2f(x, y);
        }

        void initializeCharPlacement(const uint16_t& cellX, const uint16_t& cellY) {
            // Update the sprite's rendered position
            m_characterPtr->setPosition(convertCellToPixelPosition(cellX, cellY, 
                        m_characterPtr->calcRequiredCellPadding()));
        }

        void initializeNpcPlacement() {
            // Generate a list of random, available, and unique cell positions
            auto availableCellPositionList = calcRandomAvailableCellPositionList(m_npcPtrList.size());

            sf::Vector2i npcCellPosition;
            float pixelX, pixelY, npcX, npcY;
            sf::Vector2f npcPosition, npcPadding;
            for (uint32_t npcInd = 0; npcInd < m_npcPtrList.size(); ++npcInd) {
                npcCellPosition = availableCellPositionList[npcInd];
                // Set the NPC's position
                m_npcPtrList[npcInd]->setPosition(convertCellToPixelPosition(npcCellPosition.x, npcCellPosition.y, 
                            m_npcPtrList[npcInd]->calcRequiredCellPadding()));
            }
        }

        bool validateNewPosition(const sf::Vector2i& cellPositionDest, const std::vector<sf::Vector2i>& existingCellPositionList) {
            // Ensure that all randomly generated cell positions are unique
            return !std::any_of(existingCellPositionList.begin(), existingCellPositionList.end(), 
                        [&cellPositionDest](const sf::Vector2i& usedPosition) {
                            return (cellPositionDest.x == usedPosition.x && cellPositionDest.y == usedPosition.y);
                        });
        }

        std::vector<sf::Vector2i> calcRandomAvailableCellPositionList(const uint32_t& numPositions) {
            std::vector<sf::Vector2i> uniqueCellPositionList = {};
            std::vector<sf::Vector2i> usedCellPositionList;

            uint64_t numIterations = 0;
            uint32_t cellIndX, cellIndY;
            sf::Vector2i newCellPosition;
            while (uniqueCellPositionList.size() < numPositions) {
                // If we've generated more than 100x the number of positions requested, assume it's impossible
                if (numIterations > numPositions * 100u) {
                    std::ostringstream oStrStream;
                    oStrStream << "ERROR: GameEnvironment::calcRandomAvailableCellPositionList(): " << 
                                "Failed to generate " << numPositions << 
                                " unique positions, could not find an unoccupied space!" << std::endl;
                    throw std::runtime_error(oStrStream.str());
                }

                cellIndX = rand() % m_mapLayer.getMapSize_cells().x; 
                cellIndY = rand() % m_mapLayer.getMapSize_cells().y; 
                newCellPosition = sf::Vector2i(cellIndX, cellIndY);

                // Ensure that all randomly generated cell positions are unique
                usedCellPositionList = uniqueCellPositionList;
                usedCellPositionList.push_back(m_characterPtr->getSpriteCellPosition());
                if (!validateNewPosition(newCellPosition, usedCellPositionList)) {
                    numIterations++;
                    continue;
                }
                // We know it's unique, add it to the list
                uniqueCellPositionList.push_back(newCellPosition);
                numIterations++;
            }
            // Return list of N requested unique random cell positions
            return uniqueCellPositionList;
        }

        void processUserInput() {
            sf::Event event;
            // Process newly-detected events
            while (m_gameWindow.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed) {
                    m_gameWindow.close();
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    // Avoid spell casting until current spell has reached its target
                    if (m_spellPtr == nullptr) {
                        m_spellPtr = std::make_unique<GUI::AnimatedSpell>(kSpellSpriteSheetFilePathMap.at(SpellType::Fireball), 
                                    kCellSize);
                        // Spell starts from the character who casted it
                        // Spell will end in the center of the cell containing the mouse click
                        const auto clickPosition_pixels = m_gameWindow.mapPixelToCoords(sf::Mouse::getPosition(m_gameWindow));
                        const auto targetX_cells = std::floor((clickPosition_pixels.x - m_mapBoundingBox.left) / kCellSize.x);
                        const auto targetY_cells = std::floor((clickPosition_pixels.y - m_mapBoundingBox.top) / kCellSize.y);
                        const sf::Vector2f targetPosition((targetX_cells + 0.5) * kCellSize.x + m_mapBoundingBox.left, 
                                    (targetY_cells + 0.5) * kCellSize.y + m_mapBoundingBox.top);
                        // Initialize the casting process, setting the spell's velocity (direction & speed)
                        m_spellPtr->startSpellCasting(m_characterPtr->getSpritePosition(), targetPosition);
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    // Movement keys: 
                    // W, Up = Up
                    // A, Left = Left
                    // S, Down = Down
                    // D, Right = Right
                    if (!m_characterPtr->processKeyPressToMove(event) && 
                                event.key.code == sf::Keyboard::Key::Escape) {
                        m_gameWindow.close();
                    }
                }
            }
        }

        void processTurn() {
            // Make sure to update any sprites on the map according to user input
            processUserInput();

            // Clear old content from window
            m_gameWindow.clear();

            // Draw the in-game header text
            m_gameWindow.draw(m_headerText);
            // Draw the base terrain map
            m_gameWindow.draw(m_mapLayer.getTiledMap());

            // Update view so that camera moves with the character
            m_gameWindow.setView(m_characterPtr->createView());
            // Draw the character
            m_gameWindow.draw(m_characterPtr->getSprite());
            // Draw the NPCs
            for (auto& npcPtr : m_npcPtrList) {
                m_gameWindow.draw(npcPtr->getSprite());
            }

            // Draw a spell if it is being cast
            if (m_spellPtr != nullptr) {
                if (m_spellStepCount < m_spellPtr->getNumStepsToTarget()) {
                    // Only increment when the spell moves
                    if (m_spellPtr->processSpellMove()) {
                        m_spellStepCount++;
                    }
                    m_gameWindow.draw(m_spellPtr->getSprite());
                } else {
                    // Reset spell casting trackers
                    m_spellPtr.reset();
                    m_spellStepCount = 0;
                }
            }

            // Display the new content on the window
            m_gameWindow.display();
        }

        void beginGameCycle(const CharType& charType, const std::vector<NpcType>& npcTypeList) {
            // Construct character
            m_characterPtr = std::make_unique<AnimatedCharacter>(kCharSpriteSheetFilePathMap.at(charType), kCellSize, m_mapBoundingBox);
            // Construct NPCs
            for (const auto& npcType : npcTypeList) {
                m_npcPtrList.push_back(std::make_unique<AnimatedSprite>(kNpcSpriteSheetFilePathMap.at(npcType),
                            kNpcSingleElementBoxMap.at(npcType), kCellSize, kNpcMoveCycleMap.at(npcType)));
            }
            
            // Place the character of the 6th row, 11th column
            initializeCharPlacement(0u, 0u);
            // Place the NPCs
            initializeNpcPlacement();

            while (m_gameWindow.isOpen()) {
                processTurn();
            }
        }

    private:
        // Window containing entire game
        sf::RenderWindow m_gameWindow;
        
        // Sprites on the map
        std::unique_ptr<AnimatedCharacter> m_characterPtr;
        std::vector<std::unique_ptr<AnimatedSprite>> m_npcPtrList;
        std::unique_ptr<AnimatedSpell> m_spellPtr = nullptr;
        
        // Movement trackers
        uint32_t m_spellStepCount;

        // Game map
        MapLayer m_mapLayer;
        sf::FloatRect m_mapBoundingBox;

        // In-game header text
        sf::Font m_font;
        sf::Text m_headerText;
    };
}

#endif // GAME_ENVIRONMENT_H