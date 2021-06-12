#pragma once

#include <memory>
#include <Siv3D/Fwd.hpp>
#include <Siv3D/XMLReader.hpp>

namespace s3dTiled
{
	class CTileSet;

	std::shared_ptr<CTileSet> ParseTsx(s3d::FilePathView path);

	std::shared_ptr<CTileSet> ParseTsx(const s3d::XMLElement& xml, const FilePath& parentPath);

} // namespace s3dTiled