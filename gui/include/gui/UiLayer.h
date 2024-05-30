#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace GUI {

    uint32_t constexpr kMenuFontSize { 34u };
    uint32_t constexpr kMessageFontSize { 26u };


    const sf::Vector2f kActionIconTextureSize(48.0, 48.0);
    const sf::Vector2f kActionIconMapSize(75.0, 75.0);
    float constexpr kActionIconBuffer { 25.0 };
    const sf::Color kActionBarBackgroundColor(0u, 0u, 0u, 17u * 5); // 17 * 15 = 225 (max opacity) 
    
    const sf::Vector2f kActionMessageMapSize(500.0, 75.0);
    float constexpr kMessageDividerBuffer { 15.0 };
    const sf::Color kActionMessageBackgroundColor(0u, 0u, 0u, 17u * 12); // 17 * 15 = 225 (max opacity)

    class UiLayer : public sf::Drawable, public sf::Transformable {
    public:
        UiLayer(const std::string& actionIconFilePath, const std::string& messageDividerFilePath, 
                    const std::string& gameFontFilePath) {
            if (!m_font.loadFromFile(gameFontFilePath)) {
                throw std::runtime_error("ERROR: UiLayer::UiLayer(): Failed to load game's font file!");
            }

            loadActionIcon(actionIconFilePath);
            loadMessageDivider(messageDividerFilePath);
        }

        void loadActionIcon(const std::string& actionIconFilePath) {
            // Attempt to load the tileset from file
            if (actionIconFilePath.empty() || !m_actionIconTexture.loadFromFile(actionIconFilePath)) {
                throw std::runtime_error("ERROR: UiLayer::loadActionIcon(): Failed to load action icon texture file!");
            }
        }

        void loadMessageDivider(const std::string& messageDividerFilePath) {
            // Attempt to load the tileset from file
            if (messageDividerFilePath.empty() || !m_messageDividerTexture.loadFromFile(messageDividerFilePath)) {
                throw std::runtime_error("ERROR: UiLayer::loadMessageDivider(): Failed to load message divider texture file!");
            }
        }

        // Getters
        sf::View getView() const {
            return m_uiOverlayView;
        }
        int16_t getIndOfActionIconClicked(const sf::Vector2f& mousePosition) const {
            // Mouse click not inside the action bar
            if (!m_actionIcons.getBounds().contains(mousePosition)) {
                return -1;
            }

            // Measure mouse position relative to the top-left corner of first action icon
            const auto mouseRelativeX = mousePosition.x - m_actionIcons.getBounds().left;

            float currentIcon_leftEdge, currentIcon_rightEdge, iconIndF;
            for (uint16_t iconInd = 0; iconInd < m_numActionIcons; iconInd++) {
                iconIndF = static_cast<float>(iconInd);
                currentIcon_leftEdge = iconIndF * (kActionIconMapSize.x + kActionIconBuffer);
                currentIcon_rightEdge = (iconIndF + 1.0) * kActionIconMapSize.x + iconIndF * kActionIconBuffer;
                // Mouse click inside an action icon
                if (mouseRelativeX >= currentIcon_leftEdge && mouseRelativeX <= currentIcon_rightEdge) {
                    return iconInd;
                }
            }
            // Mouse click inside the action bar, but in a buffer space
            return -1;
        }

        // Setters
        void setView(const sf::View& newView) {
            m_uiOverlayView = newView;
        }

        /// @brief Construct UI layer (currently only contains action bar)
        /// @param numActionIcons Number of action icons to store in the bar
        void initializeUiOverlay(const uint16_t& numActionIcons) {
            m_numActionIcons = numActionIcons;
            createActionBar();
        }

        void createActionMessage(const std::string& actionMessage) {
            // Create transculent background for the action message
            m_actionMessageBackground = sf::RectangleShape(kActionMessageMapSize);
            const sf::Vector2f uiCenterPosition = m_uiOverlayView.getCenter();
            m_actionMessageBackground.setPosition(uiCenterPosition.x - 0.5 * kActionMessageMapSize.x, uiCenterPosition.y - kActionMessageMapSize.y);
            m_actionMessageBackground.setFillColor(kActionMessageBackgroundColor);
            // Write out the action message itself
            m_actionMessage = sf::Text(actionMessage, m_font, kMessageFontSize);
            const float messageWidth = m_actionMessage.getGlobalBounds().width;
            const float messageHeight = m_actionMessage.getGlobalBounds().height;
            const float messageLeftBuffer = 0.5 * (m_actionMessageBackground.getSize().x - messageWidth);
            const float messageTopBuffer = 0.5 * (m_actionMessageBackground.getSize().y - messageHeight);
            m_actionMessage.setPosition(m_actionMessageBackground.getPosition().x + messageLeftBuffer, 
                        m_actionMessageBackground.getPosition().y + messageTopBuffer);
            // Create left divider for the action message
            m_messageDivider = sf::RectangleShape(sf::Vector2f(messageLeftBuffer - kMessageDividerBuffer, messageTopBuffer));
            m_messageDivider.setPosition(m_actionMessageBackground.getPosition().x, 
                        m_actionMessageBackground.getPosition().y + messageTopBuffer);
            m_messageDivider.setTexture(&m_messageDividerTexture, true);
        }

        void drawActionMessage(sf::RenderTarget& target) {
            target.draw(m_actionMessageBackground);
            target.draw(m_actionMessage);
            target.draw(m_messageDivider);
        }

    private:
        /// @brief Create the action bar and its translucent background
        void createActionBar();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // Apply the entity's transform -- combine it with what was passed by the caller
            states.transform *= getTransform();

            // Apply the action icon texture
            states.texture = &m_actionIconTexture;
            
            // Draw the action bar background 
            target.draw(m_actionBarBackground);
            // Draw the action bar icons themselves
            target.draw(m_actionIcons, states);
        }

        sf::View m_uiOverlayView;
        // Action bar
        uint16_t m_numActionIcons;
        sf::RectangleShape m_actionBarBackground;
        sf::Texture m_actionIconTexture;
        sf::VertexArray m_actionIcons;
        // On-screen message related to action
        sf::Texture m_messageDividerTexture;
        sf::RectangleShape m_actionMessageBackground;
        sf::Text m_actionMessage;
        sf::RectangleShape m_messageDivider;
        // In-game header text
        sf::Font m_font;
        sf::Text m_headerText;
    };
}

#endif // UI_LAYOUT_H