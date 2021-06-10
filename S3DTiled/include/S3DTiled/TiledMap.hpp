#pragma once

#include <memory>
#include <Siv3D/Array.hpp>
#include <Siv3D/Scene.hpp>

#include "Types.hpp"

namespace s3dTiled
{
	class Layer;
	class TileSet;
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

		s3d::Optional<Layer> getLayer(const s3d::String& name)const;

		s3d::TextureRegion getTileTexture(GId gId, double timeSec = s3d::Scene::Time()) const;
		s3d::Optional<Property> getTileProperty(GId gId, const s3d::String& key) const;

		void draw(const s3d::Rect& rect, double timeSec = s3d::Scene::Time()) const;
		void draw(double timeSec = s3d::Scene::Time()) const;
		bool drawLayer(const s3d::String& name, const s3d::Rect& rect, double timeSec = s3d::Scene::Time()) const;

		operator bool() const;

		const s3d::Texture& loadTexture(const s3d::FilePath& imagePath) const;

		const s3d::Optional<s3d::Color>& getBackGroundColor()const;

		const s3d::Array<std::pair<GId, TileSet>>& getTileSets() const;
	};
} // namespace s3dTiled