#pragma once

#include <unordered_map>

#include <Siv3D/Fwd.hpp>

namespace s3dTiled
{
	using GId = s3d::uint32;
	using TileId = s3d::uint32;

	class TiledProperty;
	using TiledProperties = std::unordered_map<s3d::String, TiledProperty>;

	/// <summary>
	/// Tiledのファイル形式
	/// </summary>
	enum class TiledFileType
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