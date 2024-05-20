#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML/Graphics.hpp>

#include <helpers/MathHelpers.h>

#include <iostream>
#include <string>

namespace GUI {

class AnimatedSprite {
public:
    // Empty constructor (will require call to fully-defined constructor to actually populate)
    AnimatedSprite() {}

    /// @brief Construct an animated sprite of any type (character, monster, or spell)
    /// @param spriteSheetFilePath File path to the spritesheet (to set its texture)
    /// @param spriteSheetElementBox Bounding box around a single element within the larger spritesheet
    /// @param cellSize Size of a cell on the playing map; Pixels
    /// @param moveCycleSize Number of frames to cycle through to complete movement animation
    AnimatedSprite(const std::string& spriteSheetFilePath, const sf::IntRect& spriteSheetElementBox,
                const sf::Vector2f& cellSize, const uint16_t& moveCycleSize);

    /// @brief Loads the sprite's texture from a spritesheet
    /// @param spriteSheetFilePath File path to the spritesheet
    void loadSprite(const std::string& spriteSheetFilePath);
    

    sf::Sprite& getSprite();
    /// @brief Calculate the sprite's cell position (assuming the game map's top-left corner is precisely (0,0))
    /// @return Sprite's position; Cells (x,y)
    const sf::Vector2i getSpriteCellPosition() const;
    const sf::Vector2f getSpritePosition() const;

    void setPosition(const sf::Vector2f& position);
    void setPosition(const double& x, const double& y);

    sf::Vector2f calcRequiredCellPadding() const;

    void resetSpriteTextureRect(const uint32_t& spriteSheetRow);
    void moveSprite(const sf::Vector2f& movement, const uint32_t& spriteSheetRow);

protected:
    // Sprite parameters
    sf::Texture m_spriteTexture;
    sf::Sprite m_sprite;
    sf::IntRect m_spriteSheetElementBox;
    
    // Map cell size
    sf::Vector2f m_cellSize;

    // Movement parameters
    uint16_t m_moveCycleSize;
    uint16_t m_moveCycleInd;

    sf::Clock m_clock;
};
}

#endif // ANIMATED_SPRITE_H