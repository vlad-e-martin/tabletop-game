#include <gui/AnimatedSpell.h>

void GUI::AnimatedSpell::startSpellCasting(const sf::Vector2f& originPosition, const sf::Vector2f& targetPosition) {
    m_sprite.setPosition(originPosition);
    
    const sf::Vector2f directionVector(targetPosition.x - originPosition.x, 
                targetPosition.y - originPosition.y);
    const double spellPathLength = GUI::MathHelpers::calcVectorLength(directionVector);
    // Calculate how many steps to animate as fireball travels to target
    const double avgStepSize = (m_spellStepSize.x + m_spellStepSize.y) * 0.5;
    if (spellPathLength > avgStepSize) {
        m_numStepsToTarget = std::floor(spellPathLength / avgStepSize);
        m_movementVector = sf::Vector2f(directionVector.x / static_cast<float>(m_numStepsToTarget), 
                    directionVector.y / static_cast<float>(m_numStepsToTarget));
    }
    else {
        m_numStepsToTarget = 1u;
        m_movementVector = directionVector;
    }
    
    // Calculate angle of approach fireball takes from wizard --> spell target
    const double azimuth_deg = GUI::MathHelpers::calcAngleBetweenVectors_deg(m_movementVector, kDownVector);

    // Reset the fireball sprite to point downwards initially (we calculate azimuth relative to "down" direction)
    resetSpriteTextureRect(kFireballDownRow);
    // Apply the appropriate rotation to the fireball, so that it looks like it's flying in the right direction
    m_sprite.rotate(-azimuth_deg);
}

bool GUI::AnimatedSpell::processSpellMove() {
    // Don't process any movement unless a sufficient amount of time has past
    if (m_clock.getElapsedTime().asMilliseconds() <= kFireballMoveDelay_ms) {
        return false;
    }
    // Restart clock, to avoid ultra-fast movement
    m_clock.restart();

    moveSprite(m_movementVector, kFireballDownRow);

    return true;
}