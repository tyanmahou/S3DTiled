#include "TileSetList.hpp"

#include <Siv3D/Array.hpp>

namespace s3dTiled
{
    using namespace s3d;

    class TileSetList::Impl
    {
    public:
        Impl()
        {}
        void add(GId firstGId, const TileSet& tileSet)
        {
            m_tileSets.push_back(std::make_pair(firstGId, tileSet));

            // gIdからtilesetへのマッピングを作成
            const auto& last = m_tileSets.back();
            for (TileId id = 0; id < last.second.getTileCount(); ++id) {
                m_gIdTileIndexMap[last.first + id] = static_cast<s3d::uint32>(m_tileSets.size()) - 1;
            }
        }
        const std::pair<GId, TileSet>& getTileSet(GId gId) const
        {
            return m_tileSets[m_gIdTileIndexMap.at(gId)];
        }

        const Array<std::pair<GId, TileSet>>& getTileSets() const
        {
            return m_tileSets;
        }
    private:
        Array<std::pair<GId, TileSet>> m_tileSets;
        std::unordered_map<GId, uint32> m_gIdTileIndexMap; // gIdからtileセットへの検索をO(1)にします
    };
    TileSetList::TileSetList():
        pImpl(std::make_shared<Impl>())
    {}
    void TileSetList::add(GId firstGId, const TileSet& tileSet) const
    {
        pImpl->add(firstGId, tileSet);
    }
    const Tile& TileSetList::getTile(GId gId, double timeSec) const
    {
        const auto& [firstGid, tileSet] = pImpl->getTileSet(gId);
        return tileSet.getTile(gId - firstGid, timeSec);
    }
    const Properties& TileSetList::getProperties(GId gId) const
    {
        const auto& [firstGid, tileSet] = pImpl->getTileSet(gId);
        return tileSet.getProperties(gId - firstGid);
    }
    s3d::Optional<Property> TileSetList::getProperty(GId gId, const s3d::String& key) const
    {
        const auto& [firstGid, tileSet] = pImpl->getTileSet(gId);
        return tileSet.getProperty(gId - firstGid, key);
    }
    const Array<std::pair<GId, TileSet>>& TileSetList::getTileSets() const
    {
        return pImpl->getTileSets();
    }
} // namespace s3dTiled