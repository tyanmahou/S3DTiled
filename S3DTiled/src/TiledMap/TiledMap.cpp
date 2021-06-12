
#include "../Parser/TiledMapReader.hpp"
#include "CTiledMap.hpp"

#include <S3DTiled/TiledMap.hpp>
#include <S3DTiled/Layer.hpp>
#include <S3DTiled/Property.hpp>

#include <Siv3D/Color.hpp>
#include <Siv3D/Optional.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/Rectangle.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/TextureRegion.hpp>

namespace s3dTiled
{
    using namespace s3d;

	TiledMap::TiledMap(s3d::FilePathView path, TiledMapFileType fileType)
	{
		this->open(path, fileType);
	}

	bool TiledMap::open(s3d::FilePathView path, TiledMapFileType fileType)
	{
		TiledMapReader reader(path, fileType);
		if (!reader) {
			return false;
		}
		this->pImpl = reader.getTiledMap();
		return true;
	}

	const Size& TiledMap::getMapSize() const
	{
		return pImpl->m_mapSize;
	}

	const Size& TiledMap::getTileSize() const
	{
		return pImpl->m_tileSize;
	}

	Optional<Property> TiledMap::getProperty(const String& key) const
	{
		if (this->pImpl->m_props.find(key) != this->pImpl->m_props.end()) {
			return this->pImpl->m_props.at(key);
		}
		return none;
	}

	Optional<Layer> TiledMap::getLayer(const String& name) const
	{
		if (pImpl->m_layerCache.find(name) == pImpl->m_layerCache.end()) {
			return none;
		}
		return pImpl->m_layers[pImpl->m_layerCache.at(name)];
	}

	TextureRegion TiledMap::getTileTexture(GId gId, double timeSec) const
	{
		return pImpl->getTileTexture(gId, timeSec);
	}

	const Properties& TiledMap::getTileProperties(GId gId) const
	{
		return pImpl->getTileProperties(gId);
	}

	Optional<Property> TiledMap::getTileProperty(GId gId, const String& key) const
	{
		return pImpl->getTileProperty(gId, key);
	}

	bool TiledMap::drawLayer(const String& name, const Rect& rect, double timeSec) const
	{
		auto&& layer = this->getLayer(name);
		if (!layer) {

			return false;
		}
		return layer->draw(*this, rect, timeSec);
	}

	void TiledMap::draw(const Rect& rect, double timeSec) const
	{
		pImpl->m_backGroundColor.then([&](Color bgColor) {
			rect.draw(bgColor);
		});

		for (const auto& layer : pImpl->m_layers) {
			layer.draw(*this, rect, timeSec);
		}
	}

	void TiledMap::draw(double timeSec) const
	{
		this->draw(pImpl->getRect(), timeSec);
	}

	TiledMap::operator bool() const
	{
		return static_cast<bool>(pImpl);
	}
	const s3d::Texture& TiledMap::loadTexture(const s3d::FilePath& imagePath) const
	{
		return pImpl->loadTexture(imagePath);
	}

	const s3d::Optional<s3d::Color>& TiledMap::getBackGroundColor() const
	{
		return pImpl->getBackGroundColor();
	}
	const s3d::Array<std::pair<GId, TileSet>>& TiledMap::getTileSets() const
	{
		return pImpl->getTileSets();
	}
} // namespace s3dTiled
