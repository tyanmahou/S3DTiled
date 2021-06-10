#include <S3DTiled/TileSet.hpp>
#include "CTileSet.hpp"

namespace s3dTiled
{
    TileSet::TileSet(std::shared_ptr<CTileSet> tileSet):
        pImpl(tileSet)
    {}
    TileSet::TileSet(s3d::FilePathView path, TileSetFileType fileType)
    {}
    bool TileSet::open(s3d::FilePathView path, TileSetFileType fileType)
    {
        return false;
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
} // namespace s3dTiled