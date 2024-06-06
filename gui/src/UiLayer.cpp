#include <gui/UiLayer.h>

void GUI::UiLayer::createActionBar(const std::vector<std::shared_ptr<AnimatedSpell>>& spellPtrList) {
    const float numF = static_cast<float>(m_numActionIcons);
    const sf::Vector2f actionBarSize(numF * kActionIconMapSize.x + (numF - 1.0) * kActionIconBuffer, kActionIconMapSize.y);
    const sf::Vector2f uiCenterPosition = m_uiOverlayView.getCenter();
    const float xOffset = uiCenterPosition.x - 0.5 * actionBarSize.x;
    const float yOffset = uiCenterPosition.y + 0.5 * m_uiOverlayView.getSize().y - actionBarSize.y - kActionIconBuffer;

    // Action bar background
    m_actionBarBackground = sf::RectangleShape(sf::Vector2f(actionBarSize.x + kActionIconBuffer, actionBarSize.y + kActionIconBuffer));
    m_actionBarBackground.setPosition(sf::Vector2f(xOffset - 0.5 * kActionIconBuffer, yOffset - 0.5 * kActionIconBuffer));
    m_actionBarBackground.setFillColor(kActionBarBackgroundColor);

    m_actionIcons.setPrimitiveType(sf::PrimitiveType::Quads);
    // 1 tile = 1 quad = 4 vertices
    m_actionIcons.resize(m_numActionIcons * 4);
    float iconIndF, xBuffer, yBuffer;
    uint32_t vertexInd = 0u, textureInd = 0u;
    m_actionIconArtList = {};
    std::cout << "Creating " << m_numActionIcons << " action icons in the action bar!" << std::endl;
    for (uint16_t iconInd = 0; iconInd < m_numActionIcons; iconInd++) {
        iconIndF = static_cast<float>(iconInd);
        xBuffer = iconIndF * (kActionIconMapSize.x + kActionIconBuffer);

        // Action icon art
        std::cout << "Setting texture for action icon #" << iconInd << std::endl;
        const sf::IntRect iconArtBox = spellPtrList[iconInd]->getActionIconArtTextureBox();
        const sf::Vector2f iconArtBoxSize(iconArtBox.width, iconArtBox.height);
        sf::RectangleShape currentActionIconArt(iconArtBoxSize);
        currentActionIconArt.setTexture(&spellPtrList[iconInd]->getActionIconArt());
        currentActionIconArt.setTextureRect(iconArtBox);
        currentActionIconArt.setOrigin(0.5 * iconArtBox.width, 0.5 * iconArtBox.height);
        currentActionIconArt.setPosition(xOffset + xBuffer + 0.5 * kActionIconMapSize.x, yOffset + 0.5 * kActionIconMapSize.y);
        m_actionIconArtList.push_back(currentActionIconArt);

        // Map action bar
        m_actionIcons[vertexInd].position = sf::Vector2f(xOffset + xBuffer, yOffset);
        m_actionIcons[vertexInd + 1].position = sf::Vector2f(xOffset + xBuffer, kActionIconMapSize.y + yOffset);
        m_actionIcons[vertexInd + 2].position = sf::Vector2f(xOffset + kActionIconMapSize.x + xBuffer, kActionIconMapSize.y + yOffset);
        m_actionIcons[vertexInd + 3].position = sf::Vector2f(xOffset + kActionIconMapSize.x + xBuffer, yOffset);

        // Texture action bar
        m_actionIcons[vertexInd + 0].texCoords = sf::Vector2f(0.0, 0.0);
        m_actionIcons[vertexInd + 1].texCoords = sf::Vector2f(0.0, kActionIconTextureSize.y);
        m_actionIcons[vertexInd + 2].texCoords = kActionIconTextureSize;
        m_actionIcons[vertexInd + 3].texCoords = sf::Vector2f(kActionIconTextureSize.x, 0.0);

        // Increment by 4, to populate the vertices representing the next tile
        vertexInd += 4u;
    }
}

