#pragma once

#include<memory>
#include <Siv3D/Array.hpp>

#include <S3DTiled/TiledTypes.hpp>
#include <S3DTiled/TiledTile.hpp>
#include <S3DTiled/TiledAnimationFrame.hpp>

namespace s3dTiled
{
	class TileSetBase;
	class TiledMap;

	class TiledSets
	{
		class CTiledSets;
		std::shared_ptr<CTiledSets> pImpl;
	public:
		TiledSets();

		void addTileSet(std::unique_ptr<TileSetBase>&& tileSet) const;
		s3d::TextureRegion getTile(GId gId, const TiledMap& map) const;

		const TiledProperties& getProperties(GId gId) const;
		s3d::Optional<TiledProperty> getProperty(GId gId, const s3d::String& key) const;

		s3d::Array<TiledTile> getTiles()const;
		s3d::Array<TiledAnimationFrame> getAnimationFrames()const;
	};
} // namespace s3dTiled