#pragma once
#include <Siv3D/String.hpp>

namespace s3dTiled::FileUtil
{
	s3d::FilePath FixRelativePath(s3d::FilePathView path);

	s3d::FilePath ParentPath(s3d::FilePathView path);
}