#include <gui/AnimatedSprite.h>

GUI::AnimatedSprite::AnimatedSprite(const std::string& spriteSheetFilePath, const sf::IntRect& spriteSheetElementBox,
                const sf::Vector2f& cellSize, const uint16_t& moveCycleSize) : 
                            m_spriteSheetElementBox(spriteSheetElementBox), 
                            m_cellSize(cellSize), m_moveCycleSize(moveCycleSize) {
    m_moveCycleInd = 0;
    // Load sprite-sheet to populate texture & sprite
    loadSprite(spriteSheetFilePath);
}

void GUI::AnimatedSprite::loadSprite(const std::string& spriteSheetFilePath) {
    // Attempt to load the tileset from file
    if (spriteSheetFilePath.empty() || !m_spriteTexture.loadFromFile(spriteSheetFilePath)) {
        throw std::runtime_error("ERROR: AnimatedSprite::AnimatedSprite(): Failed to load character spritesheet!");
    }

    m_sprite = sf::Sprite(m_spriteTexture);
    m_sprite.setTextureRect(m_spriteSheetElementBox);
    // Ensure that the sprite's origin is the center (middle) of the sprite
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0, m_sprite.getLocalBounds().height / 2.0);
}

sf::Sprite& GUI::AnimatedSprite::getSprite() {
    return m_sprite;
}

const sf::Vector2i GUI::AnimatedSprite::getSpriteCellPosition() const {
    return sf::Vector2i(std::floor(m_sprite.getPosition().x / m_cellSize.x), 
                std::floor(m_sprite.getPosition().y / m_cellSize.y));
}
const sf::Vector2f GUI::AnimatedSprite::getSpritePosition() const {
    return m_sprite.getPosition();
}

void GUI::AnimatedSprite::setPosition(const sf::Vector2f& position) {
    m_sprite.setPosition(position);
}

void GUI::AnimatedSprite::setPosition(const double& x, const double& y) {
    m_sprite.setPosition(x, y);
} 

sf::Vector2f GUI::AnimatedSprite::calcRequiredCellPadding() const {
    sf::FloatRect boundingBox = m_sprite.getGlobalBounds();
    // Padding required to center the character in the middle of its current cell
    return sf::Vector2f(m_cellSize.x - boundingBox.width, 
                m_cellSize.y - boundingBox.height);
}

void GUI::AnimatedSprite::resetSpriteTextureRect(const uint32_t& spriteSheetRow) {
    // Reset the move cycle as requested
    m_moveCycleInd = 0;
    // Change to the next sprite, making it appear to move
    m_sprite.setTextureRect(sf::IntRect(m_spriteSheetElementBox.left, 
                m_spriteSheetElementBox.top + spriteSheetRow * m_spriteSheetElementBox.height, 
                m_spriteSheetElementBox.width, m_spriteSheetElementBox.height));
}

void GUI::AnimatedSprite::moveSprite(const sf::Vector2f& movement, const uint32_t& spriteSheetRow) {
    // Reset the move cycle if necessary
    if (m_moveCycleInd >= m_moveCycleSize) {
        m_moveCycleInd = 0;
    }
    // Change to the next sprite, making it appear to move
    m_sprite.setTextureRect(sf::IntRect(m_spriteSheetElementBox.left + m_spriteSheetElementBox.width * m_moveCycleInd, 
                m_spriteSheetElementBox.top + spriteSheetRow * m_spriteSheetElementBox.height, 
                m_spriteSheetElementBox.width, m_spriteSheetElementBox.height));

    m_sprite.move(movement);
    m_moveCycleInd++;
}