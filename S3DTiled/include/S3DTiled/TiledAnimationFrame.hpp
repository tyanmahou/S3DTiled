#pragma once
#include <compare>
#include<Siv3D/Vector2D.hpp>
#include<Siv3D/String.hpp>

#include "TiledTypes.hpp"

namespace s3dTiled
{
	struct TiledAnimationFrame
	{
		GId from;
		GId to;
		s3d::int32 duration;

		auto operator <=>(const TiledAnimationFrame& other) const = default;
	};
} // namespace s3dTiled