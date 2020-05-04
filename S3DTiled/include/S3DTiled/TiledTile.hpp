#pragma once
#include <compare>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/String.hpp>

#include "TiledTypes.hpp"

namespace s3dTiled
{
	struct TiledTile
	{
		GId gId;
		s3d::String image;
		s3d::Vec2 offset;
		s3d::Vec2 size;

		auto operator <=> (const TiledTile& other) const
		{
			return gId <=> other.gId;
		}
	};
} // namespace s3dTiled