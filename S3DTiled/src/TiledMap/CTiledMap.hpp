#pragma once

#include "../TiledSets/TiledSets.hpp"

#include <S3DTiled/TiledLayer.hpp>
#include <S3DTiled/Property.hpp>

#include <Siv3D/Optional.hpp>
#include <Siv3D/Color.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/Rectangle.hpp>
#include <Siv3D/Array.hpp>

namespace s3dTiled
{
	class CTiledMap;
	class TileSetBase;

	class CTiledMap
	{
		friend class TiledMap;
	private:
		TiledSets m_tiledSets;
		Properties m_props;

		s3d::Size m_mapSize;
		s3d::Size m_tileSize;

		s3d::Optional<s3d::Color> m_backGroundColor;
		// TODO(@tyanmahou): orientation
		// TODO(@tyanmahou): renderorder

		s3d::Array<TiledLayer> m_layers;
		std::unordered_map<s3d::String, std::size_t> m_layerCache; // レイヤーへの名前検索をO(1)にする

		std::unordered_map<s3d::FilePath, s3d::Texture> m_textures;
	public:
		CTiledMap(const s3d::Size& mapSize, const s3d::Size& tileSize);

		s3d::Rect getRect() const;

		const s3d::Optional<s3d::Color>& getBackGroundColor()const;
		void setBackGroundColor(const s3d::Color& color);
		void addTileSet(std::unique_ptr<TileSetBase>&& tileSet);
		void setProps(Properties&& props);
		void addLayer(const TiledLayer& layer);

		const TiledSets& getTiledSets() const;

		const s3d::Texture& loadTexture(const s3d::FilePath& imagePath);
		s3d::Array<Tile> getTiles()const;
		s3d::Array<AnimationFrame> getAnimationFrames()const;
	};
} // namespace s3dTiled