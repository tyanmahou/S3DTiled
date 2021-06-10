#pragma once

#include <Siv3D/HashTable.hpp>
#include <S3DTiled/Tile.hpp>

namespace s3dTiled
{
    class CTileSet
    {
    public:
        virtual ~CTileSet() = default;

        s3d::uint32 getTileCount() const
        {
            return m_tileCount;
        }
        void setTileCount(s3d::uint32 count)
        {
            m_tileCount = count;
        }

        s3d::uint32 getColumns() const
        {
            return m_columns;
        }
        void setColumns(s3d::uint32 columns)
        {
            m_columns = columns;
        }

        void add(Tile&& tile)
        {
            m_tiles.emplace(tile.tileId, std::move(tile));
        }
        
        Tile& find(TileId tileId)
        {
            return m_tiles[tileId];
        }

        TileId getTileId(TileId tileId, double timeSec) const;

        const Tile& getTile(TileId tileId, double timeSec) const;

        const Properties& getProperties(TileId tileId);

        s3d::Optional<Property> getProperty(TileId tileId, const s3d::String& key);

        s3d::Array<Tile> getTiles() const;
    private:
        s3d::HashTable<TileId, Tile> m_tiles;
        s3d::uint32 m_tileCount;
        s3d::uint32 m_columns;
    };
} // namespace s3dTiled