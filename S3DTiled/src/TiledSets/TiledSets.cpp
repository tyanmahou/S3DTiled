#include "TiledSets.hpp"
#include "TileSetBase.hpp"

#include <Siv3D/TextureRegion.hpp>

namespace s3dTiled
{
    using namespace s3d;

	class TiledSets::CTiledSets
	{
		friend class TiledSets;

		Array<std::unique_ptr<TileSetBase>> m_tileSets;
		std::unordered_map<GId, uint32> m_gIdTileIndexMap; // gIdからtileセットへの検索をO(1)にします

	public:
		void addTileSet(std::unique_ptr<TileSetBase>&& tileSet)
		{
			m_tileSets.push_back(std::move(tileSet));

			// gIdからtilesetへのマッピングを作成
			const auto& last = m_tileSets.back();
			for (TileId id = 0; id < last->getTileCount(); ++id) {
				m_gIdTileIndexMap[last->getFirstGId() + id] = static_cast<s3d::uint32>(m_tileSets.size()) - 1;
			}
		}

		TileSetBase* getTileSet(GId gId) const
		{
			return m_tileSets[m_gIdTileIndexMap.at(gId)].get();
		}

	};

	TiledSets::TiledSets():
		pImpl(std::make_shared<CTiledSets>())
	{}

	TextureRegion TiledSets::getTile(GId gId) const
	{
		return pImpl->getTileSet(gId)->getTextureRegion(gId);
	}

	Optional<TiledProperty> TiledSets::getProperty(GId gId, const String& key) const
	{
		return pImpl->getTileSet(gId)->getProperty(gId, key);
	}

	void TiledSets::addTileSet(std::unique_ptr<TileSetBase>&& tileSet) const
	{
		pImpl->addTileSet(std::move(tileSet));
	}
} // namespace s3dTiled