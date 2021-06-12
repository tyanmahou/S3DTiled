#include <S3DTiled/TileSet.hpp>
#include "CTileSet.hpp"
#include "../Parser/TileSetReader.hpp"

namespace s3dTiled
{
    TileSet::TileSet(std::shared_ptr<CTileSet> tileSet):
        pImpl(tileSet)
    {}
    TileSet::TileSet(s3d::FilePathView path, TileSetFileType fileType)
    {
        this->open(path, fileType);
    }
    bool TileSet::open(s3d::FilePathView path, TileSetFileType fileType)
    {
        TileSetReader reader(path, fileType);
        if (!reader) {
            return false;
        }
        this->pImpl = reader.getTileSet();
        return true;
    }
    s3d::uint32 TileSet::getTileCount() const
    {
        return pImpl->getTileCount();
    }
    const Tile& TileSet::find(TileId tileId) const
    {
        return pImpl->find(tileId);
    }
    const Tile& TileSet::getTile(TileId tileId, double timeSec) const
    {
        return pImpl->getTile(tileId, timeSec);
    }
    const Properties& TileSet::getProperties(TileId tileId) const
    {
        return pImpl->getProperties(tileId);
    }
    s3d::Optional<Property> TileSet::getProperty(TileId tileId, const s3d::String& key) const
    {
        return pImpl->getProperty(tileId, key);
    }
    s3d::Array<Tile> TileSet::getTiles() const
    {
        return pImpl->getTiles();
    }
} // namespace s3dTiled