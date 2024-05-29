#ifndef ANIMATED_CHARACTER_H
#define ANIMATED_CHARACTER_H

#include <gui/AnimatedSprite.h>

namespace GUI {

enum MoveDirection {
    Up,
    Down,
    Left,
    Right
};

int constexpr kCharacterSheetOffsetX { 0 };
int constexpr kCharacterSheetOffsetY { 0 };

int constexpr kCharacterWidth { 64 };
int constexpr kCharacterHeight { 64 };

const sf::IntRect kCharacterTextureRect { sf::IntRect(kCharacterSheetOffsetX, kCharacterSheetOffsetY, kCharacterWidth, kCharacterHeight) };

uint32_t constexpr kCharacterUpMoveRow { 8u };
uint32_t constexpr kCharacterLeftMoveRow { 9u };
uint32_t constexpr kCharacterDownMoveRow { 10u };
uint32_t constexpr kCharacterRightMoveRow { 11u };

uint32_t constexpr kCharacterViewWidthScale { 16u }; // Width of viewable map from character's perspective; Cells
uint32_t constexpr kCharacterViewHeightScale { 9u }; // Height of viewable map from character's perspective; Cells

float constexpr kCharacterMoveDelay_ms { 150.0 }; // How long the game waits between successive character movements
uint16_t constexpr kCharacterMoveCycle { 9u }; // How many frames there are in the movecycle

class AnimatedCharacter : public AnimatedSprite {
public:
    // Empty constructor (will require call to fully-defined constructor to actually populate)
    AnimatedCharacter() {}
    AnimatedCharacter(const std::string& spriteSheetFilePath, const sf::Vector2f& cellSize, const sf::FloatRect& mapBoundingBox, 
                const uint16_t& moveCycleSize = kCharacterMoveCycle) : 
                            AnimatedSprite(spriteSheetFilePath, kCharacterTextureRect, cellSize, moveCycleSize), 
                            m_mapBoundingBox(mapBoundingBox) {}

    sf::View getView() const;
    sf::View createView();

    void processMoveOneDirection(const uint32_t& spriteMoveRow,
                const sf::Vector2f& movement);
    bool processKeyPressToMove(const sf::Event& keyPressEvent);

private:
    // Stores a camera view centered on the character
    sf::View m_charView;
    // Stores the bounding box of the playing map
    sf::FloatRect m_mapBoundingBox;
};
}

#endif // ANIMATED_CHARACTER_H