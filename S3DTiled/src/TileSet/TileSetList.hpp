#pragma once

#include<memory>
#include <Siv3D/Array.hpp>

#include <S3DTiled/TileSet.hpp>

namespace s3dTiled
{
	class TileSetList
	{
		class Impl;
		std::shared_ptr<Impl> pImpl;
	public:
		TileSetList();

		void add(GId firstGId, const TileSet& tileSet) const;

		const Tile& getTile(GId gId, double timeSec) const;

		const Properties& getProperties(GId gId) const;
		s3d::Optional<Property> getProperty(GId gId, const s3d::String& key) const;

		const s3d::Array<std::pair<GId, TileSet>>& getTileSets() const;
	};
} // namespace s3dTiled