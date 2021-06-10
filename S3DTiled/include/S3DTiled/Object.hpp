#pragma once

#include<Siv3D/Vector2D.hpp>
#include<Siv3D/String.hpp>
#include<Siv3D/Optional.hpp>

#include "TiledTypes.hpp"
#include "Property.hpp"
#include "Shape.hpp"

namespace s3dTiled
{
	struct Object
	{
		s3d::uint32 id;
		s3d::String name;
		s3d::String type;
		s3d::Vec2 pos;
		double rotation;
		Shape shape;
		TiledProperties props;
		s3d::Optional<GId> gId;
		bool isMirrored = false;
		bool isFliped = false;
		void setProps(TiledProperties&& props);
		s3d::Optional<Property> getProperty(const s3d::String& key) const;

		s3d::RectF toRectF() const;
	};
} // namespace s3dTiled