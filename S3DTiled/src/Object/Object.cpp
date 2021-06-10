#include <S3DTiled/Object.hpp>

namespace s3dTiled
{
	void Object::setProps(TiledProperties&& _props)
	{
		this->props = std::move(_props);
	}

	s3d::Optional<Property> Object::getProperty(const s3d::String& key) const
	{
		if (props.find(key) != props.end()) {
			return props.at(key);
		}
		return s3d::none;
	}

	s3d::RectF Object::toRectF() const
	{
		return std::get<s3d::RectF>(shape).movedBy(pos);
	}
} // namespace s3dTiled