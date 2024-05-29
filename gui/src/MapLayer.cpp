#include "gui/MapLayer.h"

#include <sstream>

std::vector<uint32_t> GUI::MapLayer::loadMapFromCsv(const std::string& mapFilePath) {
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

bool GUI::MapLayer::loadMap(const std::string& mapFilePath) {
    const auto mapTileList = loadMapFromCsv(mapFilePath);
    // Create a map from the Terrain tileset
    return m_tiledMap.load(m_cellSize, m_pixelOffset, m_mapSize_cells, 
                mapTileList, kTerrainTilesetPath);
}