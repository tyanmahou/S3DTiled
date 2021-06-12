#pragma once

#include<memory>

#include<S3DTiled/Types.hpp>

#include<Siv3D/Fwd.hpp>
#include<Siv3D/Optional.hpp>

namespace s3dTiled
{
	class CTiledMap;

	/// <summary>
	/// TiledMap読み込み
	/// </summary>
	class TiledMapReader
	{
	private:

		class CTiledMapReader;

		std::shared_ptr<CTiledMapReader> pImpl;

	public:
		TiledMapReader();

		explicit TiledMapReader(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		bool open(s3d::FilePathView path, TiledMapFileType fileType = TiledMapFileType::Unspecified);

		explicit operator bool() const;

		std::shared_ptr<CTiledMap> getTiledMap() const;
	};
} // namespace s3dTiled

