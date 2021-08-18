#include "CTiledMap.hpp"
#include <Siv3D/TextureRegion.hpp>

namespace s3dTiled
{
	CTiledMap::CTiledMap(const s3d::Size& mapSize, const s3d::Size& tileSize, bool isInfinite) :
		m_mapSize(mapSize),
		m_tileSize(tileSize),
		m_isInfinite(isInfinite)
	{}

	s3d::Rect CTiledMap::getRect() const
	{
		return { m_tileSize.x * m_mapSize.x, m_tileSize.y * m_mapSize.y };
	}

	const s3d::Optional<s3d::Color>& CTiledMap::getBackGroundColor() const
	{
		return m_backGroundColor;
	}

	void CTiledMap::setBackGroundColor(const s3d::Color& color)
	{
		m_backGroundColor = color;
	}

	void CTiledMap::addTileSet(GId firstGId, const TileSet& tileSet)
	{
		m_tileSet.add(firstGId, tileSet);
	}

	void CTiledMap::setProps(Properties&& props)
	{
		m_props = std::move(props);
	}

	void CTiledMap::addLayer(const Layer& layer)
	{
		std::size_t index = m_layers.size();
		m_layerCache[layer.getName()] = index;

		m_layers.push_back(layer);
	}
	s3d::TextureRegion CTiledMap::getTileTexture(GId gId, double timeSec)
	{
		const auto& tile = m_tileSet.getTile(gId, timeSec);
		const auto& tex = this->loadTexture(tile.image);
		return tex(tile.offset, tile.size);
	}
	const Properties& CTiledMap::getTileProperties(GId gId) const
	{
		return m_tileSet.getProperties(gId);
	}
	s3d::Optional<Property> CTiledMap::getTileProperty(GId gId, const s3d::String& key) const
	{
		return m_tileSet.getProperty(gId, key);
	}
	const s3d::Texture& CTiledMap::loadTexture(const s3d::FilePath& imagePath)
    {
		if (m_textures.find(imagePath) != m_textures.end()) {
			return m_textures[imagePath];
		}
		return m_textures[imagePath] = s3d::Texture(imagePath);
    }
	const s3d::Array<std::pair<GId, TileSet>>& CTiledMap::getTileSets() const
	{
		return m_tileSet.getTileSets();
	}
} // namespace s3dTiled