#pragma once

#include <memory>
#include <functional>

#include<Siv3D/Texture.hpp>
#include<Siv3D/Grid.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/Vector2D.hpp>

#include "TiledTypes.hpp"
#include "TiledProperty.hpp"
#include "TiledObject.hpp"

namespace s3dTiled
{
	class TiledMap;

	class TiledLayerBase;
	class ImageLayer;
	class TileLayer;
	class ObjectGroup;
	class GroupLayer;

	enum class LayerType
	{
		ImageLayer,
		TileLayer,
		ObjectGroup,
		GroupLayer,
		None
	};

	/// <summary>
	/// Tiled Layer
	/// </summary>
	class TiledLayer
	{
	private:
		std::shared_ptr<TiledLayerBase> pImpl;
	public:
		TiledLayer() = default;
		explicit TiledLayer(std::shared_ptr<TiledLayerBase> layer);

		/// <summary>
		/// レイヤーの描画
		/// </summary>
		/// <param name="map">タイルマップ</param>
		/// <param name="rect">描画領域</param>
		/// <returns>true: 成功 false:失敗</returns>
		bool draw(const TiledMap& map, const s3d::Rect& rect) const;

		/// <summary>
		/// レイヤータイプを取得
		/// </summary>
		/// <returns></returns>
		LayerType getType() const;

		/// <summary>
		/// Idを取得
		/// </summary>
		/// <returns></returns>
		s3d::uint32 getId() const;

		/// <summary>
		/// レイヤーの名前を取得
		/// </summary>
		/// <returns></returns>
		const s3d::String& getName() const;

		/// <summary>
		/// プロパティを取得
		/// </summary>
		s3d::Optional<TiledProperty> getProperty(const s3d::String& key) const;

		/// <summary>
		/// 表示の有無を切り替える
		/// </summary>
		void setVisible(bool visible);

		/// <summary>
		/// 表示の有無を取得
		/// </summary>
		bool getVisible() const;

		/// <summary>
		/// レイヤータイプが ImageLayerの場合にコールバックを呼ぶ
		/// </summary>
		/// <param name="callback">コールバック</param>
		/// <returns>コールバックが呼ばれたらtrue</returns>
		bool then(std::function<void(const ImageLayer&)> callback) const;

		/// <summary>
		/// レイヤータイプが TileLayer の場合にコールバックを呼ぶ
		/// </summary>
		/// <param name="callback">コールバック</param>
		/// <returns>コールバックが呼ばれたらtrue</returns>
		bool then(std::function<void(const TileLayer&)> callback) const;

		/// <summary>
		/// レイヤータイプが ObjectGroup の場合にコールバックを呼ぶ
		/// </summary>
		/// <param name="callback">コールバック</param>
		/// <returns>コールバックが呼ばれたらtrue</returns>
		bool then(std::function<void(const ObjectGroup&)> callback) const;

		/// <summary>
		/// レイヤータイプが GroupLayer の場合にコールバックを呼ぶ
		/// </summary>
		/// <param name="callback">コールバック</param>
		/// <returns>コールバックが呼ばれたらtrue</returns>
		bool then(std::function<void(const GroupLayer&)> callback) const;
	};

	class TiledLayerBase
	{
	protected:
		s3d::int32 m_id;
		s3d::String m_name;
		s3d::Vec2 m_offset;
		TiledProperties m_props;
		bool m_visible = true;
	public:
		TiledLayerBase() = default;

		virtual ~TiledLayerBase() = default;
		virtual bool draw(const TiledMap& map, const s3d::Rect& rect) const = 0;
		virtual LayerType getType() const = 0;

		void setVisible(bool visible);
		bool getVisible() const;
		void setProps(TiledProperties&& props);
		s3d::Optional<TiledProperty> getProperty(const s3d::String& key) const;

		void setId(s3d::uint32 id);
		s3d::uint32 getId() const;

		void setName(const s3d::String& name);
		const s3d::String& getName() const;

		void setOffset(const s3d::Vec2& offset);
		const s3d::Vec2& getOffset() const;
	};
	/// <summary>
	/// ImageLayer
	/// </summary>
	class ImageLayer : public TiledLayerBase
	{
	private:
		s3d::FilePath m_image;
	public:
		void setImagePath(const s3d::FilePath& image);
		const s3d::FilePath& getImagePath()const;
		s3d::Texture createTexture() const;

		bool draw(const TiledMap& map, const s3d::Rect& rect) const override;
		LayerType getType() const override;
	};

	/// <summary>
	/// TileLayer
	/// </summary>
	class TileLayer : public TiledLayerBase
	{
	private:
		s3d::Grid<GId> m_gIds;
	public:
		TileLayer() = default;
		void setGrid(s3d::Grid<GId>&& grid);
		const s3d::Grid<GId>& getGrid() const;

		bool draw(const TiledMap& map, const s3d::Rect& rect) const override;
		LayerType getType() const override;
	};

	/// <summary>
	/// Object Group
	/// </summary>
	class ObjectGroup : public TiledLayerBase
	{
	private:
		s3d::Array<TiledObject> m_objects;

	public:
		ObjectGroup() = default;

		void addObject(TiledObject&& obj);
		const s3d::Array<TiledObject>& getObjects() const;

		bool draw(const TiledMap& map, const s3d::Rect& rect) const override;

		LayerType getType() const override;
	};

	/// <summary>
	/// GroupLayer
	/// </summary>
	class GroupLayer : public TiledLayerBase
	{
	private:
		s3d::Array<TiledLayer> m_layers;
		std::unordered_map<s3d::String, std::size_t> m_layerCache;
	public:
		GroupLayer() = default;
		void addLayer(const TiledLayer& layer);
		s3d::Optional<TiledLayer> getLayer(const s3d::String& name)const;
		const s3d::Array<TiledLayer>&  getLayers() const;

		bool draw(const TiledMap& map, const s3d::Rect& rect) const override;

		LayerType getType() const override;
	};
} // namespace s3dTiled