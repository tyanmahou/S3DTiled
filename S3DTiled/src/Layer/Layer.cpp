#include <S3DTiled/TiledMap.hpp>
#include <S3DTiled/Layer.hpp>

#include <Siv3D/TextureRegion.hpp>
#include <Siv3D/TexturedQuad.hpp>
#include <Siv3D/Math.hpp>
#include <Siv3D/Mat3x2.hpp>


namespace s3dTiled
{
    using namespace s3d;

	Layer::Layer(std::shared_ptr<LayerBase> layer) :
		pImpl(layer)
	{}

	bool Layer::draw(const TiledMap& map, const Rect& rect, double timeSec) const
	{
		return pImpl->draw(map, rect, timeSec);
	}

	LayerType Layer::getType() const
	{
		if (!this->pImpl) {
			return LayerType::None;
		}
		return this->pImpl->getType();
	}

    s3d::uint32 Layer::getId() const
    {
        return this->pImpl->getId();
    }

	const String& Layer::getName() const
	{
		return this->pImpl->getName();
	}

	Optional<Property> Layer::getProperty(const String& key) const
	{
		return pImpl->getProperty(key);
	}

	void Layer::setVisible(bool visible)
	{
		pImpl->setVisible(visible);
	}

	bool Layer::getVisible() const
	{
		return pImpl->getVisible();
	}

	bool Layer::then(std::function<void(const ImageLayer&)> callback) const
	{
		if (pImpl->getType() != LayerType::ImageLayer) {
			return false;
		}
		if (auto layer = dynamic_cast<ImageLayer*>(pImpl.get())) {
			callback(*layer);
			return true;
		}
		return false;
	}

	bool Layer::then(std::function<void(const TileLayer&)> callback) const
	{
		if (pImpl->getType() != LayerType::TileLayer) {
			return false;
		}
		if (auto layer = dynamic_cast<TileLayer*>(pImpl.get())) {
			callback(*layer);
			return true;
		}
		return false;
	}

	bool Layer::then(std::function<void(const ObjectGroup&)> callback) const
	{
		if (pImpl->getType() != LayerType::ObjectGroup) {
			return false;
		}
		if (auto layer = dynamic_cast<ObjectGroup*>(pImpl.get())) {
			callback(*layer);
			return true;
		}
		return false;
	}
	bool Layer::then(std::function<void(const GroupLayer&)> callback) const
	{
		if (pImpl->getType() != LayerType::GroupLayer) {
			return false;
		}
		if (auto layer = dynamic_cast<GroupLayer*>(pImpl.get())) {
			callback(*layer);
			return true;
		}
		return false;
	}
	// LayerBase

	void LayerBase::setVisible(bool visible)
	{
		this->m_visible = visible;
	}
	bool LayerBase::getVisible() const
	{
		return m_visible;
	}
	void LayerBase::setProps(Properties&& props)
	{
		this->m_props = std::move(props);
	}
	Optional<Property> LayerBase::getProperty(const String& key) const
	{
		if (this->m_props.find(key) != this->m_props.end()) {
			return this->m_props.at(key);
		}
		return none;
	}
    void LayerBase::setId(s3d::uint32 id)
    {
		m_id = id;
	}
    s3d::uint32 LayerBase::getId() const
    {
		return m_id;
    }
	void LayerBase::setName(const String& name)
	{
		this->m_name = name;
	}
	const String& LayerBase::getName() const
	{
		return m_name;
	}
	void LayerBase::setOffset(const Vec2& offset)
	{
		this->m_offset = offset;
	}

	const Vec2& LayerBase::getOffset() const
	{
		return m_offset;
	}

	// ImageLayer

	void ImageLayer::setImagePath(const s3d::FilePath& image)
	{
		m_image = image;
	}

	const s3d::FilePath& ImageLayer::getImagePath() const
	{
		return m_image;
	}

	s3d::Texture ImageLayer::createTexture() const
	{
		return s3d::Texture(m_image);
	}

	bool ImageLayer::draw(const TiledMap& map, const Rect& rect, [[maybe_unused]]double timeSec) const
	{
		if (!m_visible) {
			return false;
		}
		const auto& texture = map.loadTexture(m_image);
		if (!RectF(m_offset, texture.size()).intersects(rect)) {
			// 範囲外
			return false;
		}
		texture.draw(m_offset);

		return true;
	}

	LayerType ImageLayer::getType() const
	{
		return LayerType::ImageLayer;
	}

	// TileLayer

	void TileLayer::setGrid(Grid<GId>&& grid)
	{
		this->m_gIds = std::move(grid);
	}

	const Grid<GId>& TileLayer::getGrid() const
	{
		return m_gIds;
	}

	bool TileLayer::draw(const TiledMap& map, const Rect& rect, double timeSec) const
	{
		if (!m_visible) {
			return false;
		}
		Size tileSize = map.getTileSize();
		Size mapSize = map.getMapSize();

		GId xStart = static_cast<GId>(Max(0, (rect.x - rect.w)/ tileSize.x));
		GId yStart = static_cast<GId>(Max(0, (rect.y - rect.h) / tileSize.y));

		GId xEnd = Min<GId>(mapSize.x, xStart + static_cast<GId>(rect.w * 3 / tileSize.x));
		GId yEnd = Min<GId>(mapSize.y, yStart + static_cast<GId>(rect.h * 3/ tileSize.y));
		for (GId y = yStart; y < yEnd; ++y) {
			for (GId x = xStart; x < xEnd; ++x) {
				GId gId = m_gIds[y][x];
				if (gId <= 0) {
					continue;
				}
				auto&& texture = map.getTileTexture(gId, timeSec);
				Vec2 pos = { x * tileSize.x, y * tileSize.y };
				// 座標の調整
				pos.y -= (texture.size.y - tileSize.y);

				RectF drawRegion{ pos, texture.size };
				if (!rect.intersects(drawRegion)) {
					continue;
				}
				texture.draw(pos);
			}
		}

		return true;
	}

	LayerType TileLayer::getType() const
	{
		return LayerType::TileLayer;
	}

	// ObjectGroup

	void ObjectGroup::addObject(Object&& obj)
	{
		this->m_objects.push_back(std::move(obj));
	}

	const Array<Object>& ObjectGroup::getObjects() const
	{
		return m_objects;
	}

	bool ObjectGroup::draw(const TiledMap& map, const Rect& rect, double timeSec) const
	{
		if (!m_visible) {
			return false;
		}
		for (auto&& obj : m_objects) {
			if (obj.gId) {
				auto&& texture = map.getTileTexture(obj.gId.value(), timeSec)
					.mirrored(obj.isMirrored)
					.flipped(obj.isFliped);

				// 座標の調整
				Vec2 pos = obj.pos;
				Vec2 offset{ texture.size.x / 2, -+texture.size.y / 2 };
				pos += offset;

				double rad = Math::ToRadians(obj.rotation);
				pos = Mat3x2::Rotate(rad, obj.pos).transform(pos);

				auto&& textureQuad = texture.rotated(rad);
				auto drawRegion = textureQuad.quad.movedBy(pos - textureQuad.center);
				if (!rect.intersects(drawRegion)) {
					// 範囲外
					continue;
				}
				textureQuad.drawAt(pos);
			}
		}
		return true;
	}

	LayerType ObjectGroup::getType() const
	{
		return LayerType::ObjectGroup;
	}
	void GroupLayer::addLayer(const Layer& layer)
	{
		std::size_t index = m_layers.size();
		m_layerCache[layer.getName()] = index;

		m_layers.push_back(layer);
	}
	s3d::Optional<Layer> GroupLayer::getLayer(const s3d::String& name) const
	{
		if (m_layerCache.find(name) == m_layerCache.end()) {
			return none;
		}
		return m_layers[m_layerCache.at(name)];
	}
	const Array<Layer>& GroupLayer::getLayers() const
	{
		return m_layers;
	}
	bool GroupLayer::draw(const TiledMap& map, const Rect& rect, double timeSec) const
	{
		for (auto&& layer: m_layers) {
			layer.draw(map, rect, timeSec);
		}
		return true;
	}
	LayerType GroupLayer::getType() const
	{
		return LayerType::GroupLayer;
	}
} // namespace s3dTiled