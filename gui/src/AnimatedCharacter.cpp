#include <gui/AnimatedCharacter.h>

sf::View GUI::AnimatedCharacter::getView() {
    return m_charView;  
}

sf::View GUI::AnimatedCharacter::createView() {
    // Construct a view centered around the character
    m_charView.setCenter(m_sprite.getPosition());
    m_charView.setSize(sf::Vector2f(kCharacterViewWidthScale * m_cellSize.x, kCharacterViewHeightScale * m_cellSize.y));

    return m_charView;
}

void GUI::AnimatedCharacter::processMoveOneDirection(const uint32_t& spriteMoveRow, 
            const sf::Vector2f& movement) {
    const auto charPosition = m_sprite.getPosition();
    // Don't allow the sprite to leave the map
    // On x-axis...
    bool xCheck = true;
    if (movement.x < 0) {
        // Buffer by 1 cell so that movement off the game map is not allowed
        xCheck = charPosition.x > (m_mapBoundingBox.left + m_cellSize.x);
    }
    else {
        // Buffer by 1 cell so that movement off the game map is not allowed
        xCheck = charPosition.x < (m_mapBoundingBox.left + m_mapBoundingBox.width - m_cellSize.x);
    }
    // On y-axis...
    bool yCheck = true;
    if (movement.x < 0) {
        // Buffer by 1 cell so that movement off the game map is not allowed
        yCheck = charPosition.y > (m_mapBoundingBox.top - m_cellSize.y);
    }
    else {
        // Buffer by 1 cell so that movement off the game map is not allowed
        yCheck = charPosition.y < (m_mapBoundingBox.top + m_mapBoundingBox.height - m_cellSize.y);
    }

    if (xCheck && yCheck) {
        // Move the view first
        m_charView.move(movement);
        // Move the character immediately after
        moveSprite(movement, spriteMoveRow);
    }
}

bool GUI::AnimatedCharacter::processKeyPressToMove(const sf::Event& keyPressEvent) {
    // Don't process any movement unless a sufficient amount of time has past
    if (m_clock.getElapsedTime().asMilliseconds() <= kCharacterMoveDelay_ms) {
        return false;
    }
    // Restart clock, to avoid ultra-fast movement
    m_clock.restart();
    
    const auto keyCode = keyPressEvent.key.code;
    if (keyCode == sf::Keyboard::Key::W || keyCode == sf::Keyboard::Key::Up) {
        processMoveOneDirection(kCharacterUpMoveRow, sf::Vector2f(0, -m_cellSize.y));
    }
    else if (keyCode == sf::Keyboard::Key::S || keyCode == sf::Keyboard::Key::Down) {
        processMoveOneDirection(kCharacterDownMoveRow, sf::Vector2f(0, m_cellSize.y));
    }
    else if (keyCode == sf::Keyboard::Key::A || keyCode == sf::Keyboard::Key::Left) {
        processMoveOneDirection(kCharacterLeftMoveRow, sf::Vector2f(-m_cellSize.x, 0));
    }
    else if (keyCode == sf::Keyboard::Key::D || keyCode == sf::Keyboard::Key::Right) {
        processMoveOneDirection(kCharacterRightMoveRow, sf::Vector2f(m_cellSize.x, 0));
    }
    
    return false;
}