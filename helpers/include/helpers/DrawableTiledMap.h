#ifndef DRAWABLE_TILE_MAP_H
#define DRAWABLE_TILE_MAP_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

namespace GUI {

const sf::Color kGridOutlineColor { sf::Color::White };
const sf::Color kGridBackgroundColor { sf::Color::Transparent };
float constexpr kGridOutlineThickness { 1.0 };

const sf::Vector2f kTextureTileSize(32.0, 32.0);

class DrawableTiledMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const sf::Vector2f& cellSize, const sf::Vector2f& pixelOffset, 
                const sf::Vector2i& mapSize_cells,
                const std::vector<uint32_t>& tileList, const std::string& tileTextureFilePath);

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Apply the entity's transform -- combine it with what was passed by the caller
        states.transform *= getTransform();

        // Apply the tileset texture
        states.texture = &m_mapTexture;

        // Draw the textured vertex array as the base map
        target.draw(m_texturedGrid, states);
        // Draw the base grid to visualize grid-based movement system
        for (auto& cellRow : m_baseGrid) {
            for (auto& cell : cellRow) {
                target.draw(cell);
            }
        }
    }

    std::vector<std::vector<sf::RectangleShape>> m_baseGrid;
    sf::VertexArray m_texturedGrid;
    sf::Texture m_mapTexture;
};

}

#endif // DRAWABLE_TILE_MAP_H