#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H
 
#include <cstdint>
#include <stdexcept>
#include <vector>

#include <helpers/DrawableTiledMap.h>
#include <helpers/CsvReader.h>

namespace GUI {
    const uint32_t kTilsetNumColumns { static_cast<uint32_t>(kTextureTileSize.x) };
    const uint32_t kGrass { 5 * kTilsetNumColumns + 21};
    const uint32_t kFlowers { 11 * kTilsetNumColumns + 21};
    const uint32_t kFerns { 25 * kTilsetNumColumns + 2 };
    
    const uint32_t kPathLeft { 29 * kTilsetNumColumns + 3 };
    const uint32_t kPath { 29 * kTilsetNumColumns + 4 };
    const uint32_t kPathRight { 29 * kTilsetNumColumns + 5 };

    const std::map<uint32_t, uint32_t> kCsvToTileIndices {
        {0u, kGrass},
        {1u, kFlowers},
        {2u, kFerns},
        {3u, kPath}
    };

    const std::string kTerrainTilesetPath { "C:\\Users\\vladi\\Documents\\Code\\C++\\tabletop-game\\gui\\assets\\tilesets\\lpc_atlas\\terrain_atlas.png" };

    class MapLayer {
    public:
        MapLayer(const sf::Vector2f& cellSize, const sf::Vector2f& pixelOffset) : 
                    m_cellSize(cellSize), m_pixelOffset(pixelOffset) {}

        std::vector<uint32_t> loadMapFromCsv(const std::string& mapFilePath) {
            GUI::CsvReader csvReader(mapFilePath);

            const auto csvIndexMatrix = csvReader.readCsv_uint32();

            std::vector<uint32_t> tileIndexList;
            uint32_t numColumns = csvIndexMatrix[0].size();
            for (const auto& csvIndexRow : csvIndexMatrix) {
                if (csvIndexRow.size() != numColumns) {
                    std::ostringstream oStrStream;
                    oStrStream << "ERROR: MapLayer::loadMapFromCsv(): " 
                                << "Mismatching number of columns (expected: " 
                                << numColumns << ", received: " << csvIndexRow.size() << ")!";
                    throw std::out_of_range(oStrStream.str());
                }
                for (const uint32_t& csvIndex : csvIndexRow) {
                    // Convert CSV index to tile index
                    tileIndexList.push_back(kCsvToTileIndices.at(csvIndex));
                }
            }

            // Extract map size (in cells) from the CSV content matrix
            m_mapSize_cells = sf::Vector2i(csvIndexMatrix[0].size(), csvIndexMatrix.size());

            return tileIndexList;
        }

        // TODO: Create a map creation view which will enable us to create larger or smaller game maps
        bool loadMap(const std::string& mapFilePath) {
            const auto mapTileList = loadMapFromCsv(mapFilePath);
            // Create a map from the Terrain tileset
            return m_tiledMap.load(m_cellSize, m_pixelOffset, m_mapSize_cells, 
                        mapTileList, kTerrainTilesetPath);
        }

        const DrawableTiledMap& getTiledMap() const {
            return m_tiledMap;
        }

        sf::FloatRect getMapRect() const {
            return sf::FloatRect(m_pixelOffset.x, m_pixelOffset.y, 
                        m_mapSize_cells.x * m_cellSize.x, m_mapSize_cells.y * m_cellSize.y);
        }

        sf::Vector2i getMapSize_cells() const {
            return sf::Vector2i(m_mapSize_cells);
        }

        sf::Vector2f getMapSize_pixels() const {
            return sf::Vector2f(m_mapSize_cells.x * m_cellSize.x, m_mapSize_cells.y * m_cellSize.y);
        }
        
    private:
        DrawableTiledMap m_tiledMap;
        sf::Vector2f m_cellSize;
        sf::Vector2f m_pixelOffset;
        sf::Vector2i m_mapSize_cells;
    };
}

#endif // MAP_GENERATOR_H