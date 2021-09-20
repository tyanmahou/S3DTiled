#pragma once

#include "../TileSet/TileSetList.hpp"

#include <S3DTiled/Layer.hpp>
#include <S3DTiled/Property.hpp>

#include <Siv3D/Optional.hpp>
#include <Siv3D/Color.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/RectF.hpp>
#include <Siv3D/Array.hpp>

namespace s3dTiled
{
	class CTiledMap;
	class TileSetBase;

	class CTiledMap
	{
		friend class TiledMap;
	private:
		TileSetList m_tileSet;
		Properties m_props;

		s3d::Size m_mapSize;
		s3d::Size m_tileSize;
		bool m_isInfinite = false;

		s3d::Optional<s3d::Color> m_backGroundColor;
		// TODO(@tyanmahou): orientation
		// TODO(@tyanmahou): renderorder

		s3d::Array<Layer> m_layers;
		s3d::HashTable<s3d::String, std::size_t> m_layerCache; // レイヤーへの名前検索をO(1)にする

		s3d::HashTable<s3d::FilePath, s3d::Texture> m_textures;
	public:
		CTiledMap(const s3d::Size& mapSize, const s3d::Size& tileSize, bool isInfinite);

		s3d::Rect getRect() const;
		bool isInfinite() const { return m_isInfinite; }

		const s3d::Optional<s3d::Color>& getBackGroundColor()const;
		void setBackGroundColor(const s3d::Color& color);
		void addTileSet(GId firstGId, const TileSet& tileSet);
		void setProps(Properties&& props);
		void addLayer(const Layer& layer);

		s3d::TextureRegion getTileTexture(GId gId, double timeSec);
		const Properties& getTileProperties(GId gId) const;
		s3d::Optional<Property> getTileProperty(GId gId, const s3d::String& key) const;

		const s3d::Texture& loadTexture(const s3d::FilePath& imagePath);

		const s3d::Array<std::pair<GId, TileSet>>& getTileSets() const;
	};
} // namespace s3dTiled