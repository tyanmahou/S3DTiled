#include "CTiledMap.hpp"

namespace s3dTiled
{
	CTiledMap::CTiledMap(const s3d::Size& mapSize, const s3d::Size& tileSize) :
		m_mapSize(mapSize),
		m_tileSize(tileSize)
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

	void CTiledMap::addTileSet(std::unique_ptr<TileSetBase>&& tileSet)
	{
		m_tiledSets.addTileSet(std::move(tileSet));
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
	const TiledSets& CTiledMap::getTiledSets() const
	{
		return m_tiledSets;
	}
    const s3d::Texture& CTiledMap::loadTexture(const s3d::FilePath& imagePath)
    {
		if (m_textures.find(imagePath) != m_textures.end()) {
			return m_textures[imagePath];
		}
		return m_textures[imagePath] = s3d::Texture(imagePath);
    }
    s3d::Array<Tile> CTiledMap::getTiles() const
    {
		return m_tiledSets.getTiles();
    }
	s3d::Array<AnimationFrame> CTiledMap::getAnimationFrames() const
	{
		return m_tiledSets.getAnimationFrames();
	}
} // namespace s3dTiled