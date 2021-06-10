#pragma once

#include<memory>

#include<S3DTiled/TiledTypes.hpp>

#include<Siv3D/Fwd.hpp>
#include<Siv3D/Optional.hpp>

namespace s3dTiled
{
	class CTiledMap;

	/// <summary>
	/// Tiled読み込み
	/// </summary>
	class TiledReader
	{
	private:

		class CTiledReader;

		std::shared_ptr<CTiledReader> pImpl;

	public:
		TiledReader();

		explicit TiledReader(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		bool open(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		operator bool() const;

		std::shared_ptr<CTiledMap> getTiledMap() const;
	};
} // namespace s3dTiled

