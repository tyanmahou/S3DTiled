#pragma once

#include<memory>

#include<S3DTiled/Types.hpp>

#include<Siv3D/Fwd.hpp>
#include<Siv3D/Optional.hpp>

namespace s3dTiled
{
	class CTileSet;

	/// <summary>
	/// TileSet読み込み
	/// </summary>
	class TileSetReader
	{
	private:

		class CTileSetReader;

		std::shared_ptr<CTileSetReader> pImpl;

	public:
		TileSetReader();

		explicit TileSetReader(s3d::FilePathView path, TileSetFileType fileType = TileSetFileType::Unspecified);

		bool open(s3d::FilePathView path, TileSetFileType fileType = TileSetFileType::Unspecified);

		explicit operator bool() const;

		std::shared_ptr<CTileSet> getTileSet() const;
	};
} // namespace s3dTiled

