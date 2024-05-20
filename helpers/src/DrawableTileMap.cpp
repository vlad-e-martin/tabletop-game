#include <helpers/DrawableTiledMap.h>

bool GUI::DrawableTiledMap::load(const sf::Vector2f& cellSize, const sf::Vector2f& pixelOffset, 
            const sf::Vector2i& mapSize_cells,
            const std::vector<uint32_t>& tileList, const std::string& tileTextureFilePath) {
    // Attempt to load the tileset from file
    if (tileTextureFilePath.empty() || !m_mapTexture.loadFromFile(tileTextureFilePath)) {
        std::cerr << "ERROR: DrawableTiledMap::load(): Could not find tileset at " << tileTextureFilePath 
                    << "!" << std::endl;
        return false;
    }
    if (mapSize_cells.x * mapSize_cells.y != tileList.size()) {
        std::cerr << "ERROR: DrawableTiledMap::load(): The given map (" << tileList.size() << 
                    " tiles) does not have the expected number of tiles (" << mapSize_cells.x * mapSize_cells.y << 
                    ")!" << std::endl;
        return false;
    }

    m_texturedGrid.setPrimitiveType(sf::PrimitiveType::Triangles);
    // 1 tile = 2 triangles = 6 vertices
    m_texturedGrid.resize(mapSize_cells.x * mapSize_cells.y * 6);

    uint32_t mapInd = 0, tileInd = 0, vertexInd = 0;
    uint32_t mapX0, mapX1, mapY0, mapY1;
    uint32_t tileX0, tileX1, tileY0, tileY1;
    uint32_t numTilesInTilesetRow = m_mapTexture.getSize().x / kTextureTileSize.x;
    
    // Each map cell is represented by two triangles
    for (uint32_t rowInd = 0; rowInd < mapSize_cells.y; ++rowInd) {
        std::vector<sf::RectangleShape> currentCellRow;
        for (uint32_t colInd = 0; colInd < mapSize_cells.x; ++colInd) {
            // Cell index in the drawn map
            mapInd = rowInd * mapSize_cells.x + colInd;
            // Tile index in the tileset
            tileInd = tileList[mapInd];

            // Figure out which tile in the tilset we are processing
            int tileX = tileInd % numTilesInTilesetRow;
            int tileY = tileInd / numTilesInTilesetRow;

            /*
            x-----x
            | \   |
            |  \  |
            |   \ |
            x-----x
            */
            // Define the 6 triangle vertices on the game map
            mapX0 = pixelOffset.x + colInd * cellSize.x, mapX1 = pixelOffset.x + (colInd + 1) * cellSize.x;
            mapY0 = pixelOffset.y + rowInd * cellSize.y, mapY1 = pixelOffset.y + (rowInd + 1) * cellSize.y;
            // Map cell
            m_texturedGrid[vertexInd].position = sf::Vector2f(mapX0, mapY0);
            m_texturedGrid[vertexInd + 1].position = sf::Vector2f(mapX1, mapY0);
            m_texturedGrid[vertexInd + 2].position = sf::Vector2f(mapX1, mapY1);
            m_texturedGrid[vertexInd + 3].position = sf::Vector2f(mapX0, mapY0);
            m_texturedGrid[vertexInd + 4].position = sf::Vector2f(mapX0, mapY1);
            m_texturedGrid[vertexInd + 5].position = sf::Vector2f(mapX1, mapY1);

            // Define the base grid
            sf::RectangleShape currentCell(sf::Vector2f(cellSize.x, cellSize.y));
            currentCell.setPosition(m_texturedGrid[vertexInd].position);
            currentCell.setOutlineColor(kGridOutlineColor);
            currentCell.setFillColor(kGridBackgroundColor);
            currentCell.setOutlineThickness(kGridOutlineThickness);
            currentCellRow.push_back(currentCell);

            // Define the texture coordinates used by the matching triangle vertices
            tileX0 = tileX * kTextureTileSize.x;
            tileX1 = (tileX + 1) * kTextureTileSize.x;
            tileY0 = tileY * kTextureTileSize.y;
            tileY1 = (tileY + 1) * kTextureTileSize.y;
            // Texture rectangle
            m_texturedGrid[vertexInd].texCoords = sf::Vector2f(tileX0, tileY0);
            m_texturedGrid[vertexInd + 1].texCoords = sf::Vector2f(tileX1, tileY0);
            m_texturedGrid[vertexInd + 2].texCoords = sf::Vector2f(tileX1, tileY1);
            m_texturedGrid[vertexInd + 3].texCoords = sf::Vector2f(tileX0, tileY0);
            m_texturedGrid[vertexInd + 4].texCoords = sf::Vector2f(tileX0, tileY1);
            m_texturedGrid[vertexInd + 5].texCoords = sf::Vector2f(tileX1, tileY1);
        
            // Increment by 6, to populate the vertices representing the next tile
            vertexInd += 6u;
        }
        m_baseGrid.push_back(currentCellRow);
    }

    return true;
}