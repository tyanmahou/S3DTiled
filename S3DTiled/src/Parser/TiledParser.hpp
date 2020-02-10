#pragma once

#include <memory>
#include <Siv3D/Fwd.hpp>

namespace s3dTiled
{
	class CTiledMap;

	std::shared_ptr<CTiledMap> ParseTmx(const s3d::FilePath& path);
} // namespace s3dTiled