#pragma once

#include <memory>
#include <Siv3D/Fwd.hpp>

namespace s3dTiled
{
	class CTiledMap;

	std::shared_ptr<CTiledMap> ParseTmx(s3d::FilePathView path);
} // namespace s3dTiled