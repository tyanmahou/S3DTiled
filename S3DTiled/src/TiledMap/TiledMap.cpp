
#include "../Parser/TiledReader.hpp"
#include "CTiledMap.hpp"

#include <S3DTiled/TiledMap.hpp>
#include <S3DTiled/TiledLayer.hpp>
#include <S3DTiled/TiledProperty.hpp>

#include <Siv3D/Color.hpp>
#include <Siv3D/Optional.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/Rectangle.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/TextureRegion.hpp>

namespace s3dTiled
{
    using namespace s3d;

	TiledMap::TiledMap(s3d::FilePathView path, TiledFileType fileType)
	{
		TiledReader reader(path, fileType);
		if (!reader) {
			return;
		}
		this->pImpl = reader.getTiledMap();
	}

	bool TiledMap::open(s3d::FilePathView path, TiledFileType fileType)
	{
		TiledReader reader(path, fileType);
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

	Optional<TiledProperty> TiledMap::getProperty(const String& key) const
	{
		if (this->pImpl->m_props.find(key) != this->pImpl->m_props.end()) {
			return this->pImpl->m_props.at(key);
		}
		return none;
	}

	Optional<TiledLayer> TiledMap::getLayer(const String& name) const
	{
		if (pImpl->m_layerCache.find(name) == pImpl->m_layerCache.end()) {
			return none;
		}
		return pImpl->m_layers[pImpl->m_layerCache.at(name)];
	}

	TextureRegion TiledMap::getTile(GId gId) const
	{
		return pImpl->m_tiledSets.getTile(gId, *this);
	}

	Optional<TiledProperty> TiledMap::getTileProperty(GId gId, const String& key) const
	{
		return pImpl->m_tiledSets.getProperty(gId, key);
	}

	bool TiledMap::drawLayer(const String& name, const Rect& rect) const
	{
		auto&& layer = this->getLayer(name);
		if (!layer) {

			return false;
		}
		return layer->draw(*this, rect);
	}

	void TiledMap::draw(const Rect& rect) const
	{
		pImpl->m_backGroundColor.then([&](Color bgColor) {
			rect.draw(bgColor);
		});

		for (const auto& layer : pImpl->m_layers) {
			layer.draw(*this, rect);
		}
	}

	void TiledMap::draw() const
	{
		this->draw(pImpl->getRect());
	}

	TiledMap::operator bool() const
	{
		return static_cast<bool>(pImpl);
	}
	const s3d::Texture& TiledMap::loadTexture(const s3d::FilePath& imagePath) const
	{
		return pImpl->loadTexture(imagePath);
	}
	s3d::Array<TiledTile> TiledMap::getTiles() const
	{
		return pImpl->getTiles();
	}
	s3d::Array<TiledAnimationFrame> TiledMap::getAnimationFrames() const
	{
		return pImpl->getAnimationFrames();
	}
} // namespace s3dTiled
