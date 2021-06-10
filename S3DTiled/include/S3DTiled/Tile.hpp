#pragma once
#include <compare>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/String.hpp>

#include "Types.hpp"
#include "Animation.hpp"
#include "Property.hpp"

namespace s3dTiled
{
	struct Tile
	{
		TileId tileId;
		s3d::String image;
		s3d::Vec2 offset;
		s3d::Vec2 size;

		Animation animation;
		Properties props;

		auto operator <=> (const Tile& other) const
		{
			return tileId <=> other.tileId;
		}
	};
} // namespace s3dTiled