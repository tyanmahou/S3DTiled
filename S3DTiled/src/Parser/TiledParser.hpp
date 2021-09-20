#pragma once

#include <memory>
#include <Siv3D/Fwd.hpp>
#include <Siv3D/StringView.hpp>

namespace s3dTiled
{
	class CTiledMap;
	class CTileSet;


	std::shared_ptr<CTiledMap> ParseTmx(s3d::FilePathView path);


	std::shared_ptr<CTileSet> ParseTsx(s3d::FilePathView path);

} // namespace s3dTiled