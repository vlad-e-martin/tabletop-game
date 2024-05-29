#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace GUI {

    const sf::Vector2f kActionIconTextureSize(48.0, 48.0);
    const sf::Vector2f kActionIconMapSize(75.0, 75.0);
    float const kActionIconBuffer { 25.0 };
    const sf::Color kActionBarBackgroundColor(0u, 0u, 0u, 17u * 5); // 15 * 17 = 225 (max opacity) 

    class UiLayer : public sf::Drawable, public sf::Transformable {
    public:
        UiLayer(const std::string& actionIconFilePath) {
            loadActionIcon(actionIconFilePath);
        }

        void loadActionIcon(const std::string& actionIconFilePath) {
            // Attempt to load the tileset from file
            if (actionIconFilePath.empty() || !m_actionIconTexture.loadFromFile(actionIconFilePath)) {
                throw std::runtime_error("ERROR: AnimatedSprite::AnimatedSprite(): Failed to load character spritesheet!");
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

    private:
        /// @brief Create the action bar and its translucent background
        void createActionBar();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // Apply the entity's transform -- combine it with what was passed by the caller
            states.transform *= getTransform();

            // Apply the action icon texture
            states.texture = &m_actionIconTexture;
            
            // Draw the action bar background 
            target.draw(m_actionBarBackground, states);
            // Draw the action bar icons themselves
            target.draw(m_actionIcons, states);
        }

        sf::Texture m_actionIconTexture;
        sf::VertexArray m_actionIcons;
        sf::RectangleShape m_actionBarBackground;
        sf::View m_uiOverlayView;
        uint16_t m_numActionIcons;
    };
}

#endif // UI_LAYOUT_H