#pragma once

#include<Siv3D/Vector2D.hpp>
#include<Siv3D/String.hpp>
#include<Siv3D/Optional.hpp>

#include "Types.hpp"
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
		Properties props;
		s3d::Optional<GId> gId;
		bool isMirrored = false;
		bool isFlipped = false;
		void setProps(Properties&& props);
		s3d::Optional<Property> getProperty(const s3d::String& key) const;

		s3d::RectF toRectF() const;

		s3d::Optional<Property> operator[](const s3d::String& key) const;
	};
} // namespace s3dTiled
