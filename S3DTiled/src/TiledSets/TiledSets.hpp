#pragma once

#include<memory>
#include <S3DTiled/TiledTypes.hpp>

namespace s3dTiled
{
	class TileSetBase;

	class TiledSets
	{
		class CTiledSets;
		std::shared_ptr<CTiledSets> pImpl;
	public:
		TiledSets();

		void addTileSet(std::unique_ptr<TileSetBase>&& tileSet) const;
		s3d::TextureRegion getTile(GId gId) const;

		s3d::Optional<TiledProperty> getProperty(GId gId, const s3d::String& key) const;
	};
} // namespace s3dTiled