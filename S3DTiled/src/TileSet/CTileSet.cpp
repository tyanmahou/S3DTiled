#include "CTileSet.hpp"

namespace s3dTiled
{
    TileId CTileSet::getTileId(TileId tileId, double timeSec) const
    {
        const auto& anim = m_tiles.at(tileId).animation;
        if (!anim.isEmpty()) {
            tileId = anim.getTileId(timeSec);
        }
        return tileId;
    }

    const Tile& CTileSet::getTile(TileId tileId, double timeSec) const
    {
        return m_tiles.at(getTileId(tileId, timeSec));
    }
    const Properties& CTileSet::getProperties(TileId tileId)
    {
        return m_tiles[tileId].props;
    }

    s3d::Optional<Property> CTileSet::getProperty(TileId tileId, const s3d::String& key)
    {
        const auto& props = this->getProperties(tileId);

        if (props.find(key) == props.end()) {
            return s3d::none;
        }
        return props.at(key);
    }
    s3d::Array<Tile> CTileSet::getTiles() const
    {
        s3d::Array<Tile> ret;
        for (const auto& pair : m_tiles) {
            ret.push_back(pair.second);
        }
        return ret.sorted();
    }
} // namespace s3dTiled