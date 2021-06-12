#include "FileUtil.hpp"
#include <stack>
#include <Siv3D/FileSystem.hpp>

namespace
{
	bool IsResourcePath(const s3d::FilePathView path)
	{
		return path.starts_with(U'/');
	}
}
namespace s3dTiled::FileUtil
{
	s3d::FilePath FixRelativePath(s3d::FilePathView path)
	{
		std::stack<s3d::String> dirNameStack;
		s3d::String dirName = U"";
		for (auto&& c : path) {
			if (c == U'/') {
				if (dirName == U".") {

				} else if (dirName == U"..") {
					dirNameStack.pop();
				} else {
					dirNameStack.push(dirName);
				}
				dirName.clear();
			} else {
				dirName += c;
			}
		}
		s3d::String result = dirName;
		while (!dirNameStack.empty()) {
			result = dirNameStack.top() + U"/" + result;
			dirNameStack.pop();
		}
		return result;
	}

    s3d::FilePath ParentPath(s3d::FilePathView path)
    {
		if (::IsResourcePath(path)) {
			return U"/" + s3d::FileSystem::RelativePath(s3d::FileSystem::ParentPath(path.substr(1)));
		} else {
			return s3d::FileSystem::RelativePath(s3d::FileSystem::ParentPath(path));
		}
    }

	s3d::String Extension(s3d::FilePathView path)
	{
		auto basePath = ::IsResourcePath(path) ? path.substr(1) : path;
		return s3d::FileSystem::Extension(basePath);
	}
} // namespace s3dTiled::FileUtil