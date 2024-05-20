#ifndef ANIMATED_SPELL_H
#define ANIMATED_SPELL_H

#include <gui/AnimatedSprite.h>

namespace GUI {
int constexpr kFireballSheetOffsetX { 0 };
int constexpr kFireballSheetOffsetY { 0 };

int constexpr kFireballWidth { 64 };
int constexpr kFireballHeight { 64 };

const sf::IntRect kFireballTextureRect { sf::IntRect(kFireballSheetOffsetX, kFireballSheetOffsetY, kFireballWidth, kFireballHeight) };

uint32_t constexpr kFireballLeftRow { 0u };
uint32_t constexpr kFireballUpLeftRow { 1u };
uint32_t constexpr kFireballUpRow { 2u };
uint32_t constexpr kFireballUpRightRow { 3u };
uint32_t constexpr kFireballRightRow { 4u };
uint32_t constexpr kFireballDownRightRow { 5u };
uint32_t constexpr kFireballDownRow { 6u };
uint32_t constexpr kFireballDownLeftRow { 7u };

float constexpr kFireballMoveDelay_ms { 30.0 }; // How long the game waits between successive fireball movements
uint16_t constexpr kFireballMoveCycle { 9u }; // How many frames there are in the movecycle

const sf::Vector2f kDownVector(0.0, 1.0);

class AnimatedSpell : public AnimatedSprite {
public:
    AnimatedSpell(const std::string& spriteSheetFilePath, const sf::Vector2f& cellSize, const uint16_t& moveCycleSize = kFireballMoveCycle) : 
                AnimatedSprite(spriteSheetFilePath, kFireballTextureRect, cellSize, moveCycleSize), 
                m_spellStepSize(sf::Vector2f(cellSize.x / 3.0, cellSize.y / 3.0)) {}

    uint32_t getNumStepsToTarget() const {
        return m_numStepsToTarget;
    }

    void startSpellCasting(const sf::Vector2f& originPosition, const sf::Vector2f& targetPosition);
    bool processSpellMove();

private:
    uint32_t m_numStepsToTarget;
    sf::Vector2f m_spellStepSize;
    sf::Vector2f m_movementVector;
};
} // namespace GUI


#endif // ANIMATED_SPELL_H