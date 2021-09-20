#pragma once

#include <unordered_map>

#include <Siv3D/Fwd.hpp>
#include <Siv3D/HashTable.hpp>
#include <Siv3D/Optional.hpp>

namespace s3dTiled
{
	using GId = s3d::uint32;
	using TileId = s3d::uint32;

	class Property;
	using Properties = s3d::HashTable<s3d::String, Property>;

	/// <summary>
	/// Tiledのファイル形式
	/// </summary>
	enum class TiledMapFileType
	{
		Unspecified,
		Tmx,
		Json
	};

	enum class TileSetFileType
	{
		Unspecified,
		Tsx,
	};
} // namespace s3dTiled