#pragma once

#include <memory>
#include <Siv3D/Array.hpp>

#include "AnimationFrame.hpp"
#include "Tile.hpp"
#include "Types.hpp"

namespace s3dTiled
{
	class TiledLayer;
	class CTiledMap;

	class TiledMap
	{
		std::shared_ptr<CTiledMap> pImpl;
	public:
		TiledMap() = default;

		TiledMap(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		bool open(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		const s3d::Size& getMapSize() const;
		const s3d::Size& getTileSize() const;

		s3d::Optional<Property> getProperty(const s3d::String& key) const;

		s3d::Optional<TiledLayer> getLayer(const s3d::String& name)const;

		s3d::TextureRegion getTile(GId gId) const;
		s3d::Optional<Property> getTileProperty(GId gId, const s3d::String& key) const;

		void draw(const s3d::Rect& rect) const;
		void draw() const;
		bool drawLayer(const s3d::String& name, const s3d::Rect& rect) const;

		operator bool() const;

		const s3d::Texture& loadTexture(const s3d::FilePath& imagePath) const;

		s3d::Array<Tile> getTiles()const;
		s3d::Array<AnimationFrame> getAnimationFrames()const;

		const s3d::Optional<s3d::Color>& getBackGroundColor()const;
	};
} // namespace s3dTiled