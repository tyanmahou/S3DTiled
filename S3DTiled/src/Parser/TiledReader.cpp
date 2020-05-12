#include "TiledReader.hpp"
#include "TiledParser.hpp"
#include "../TiledMap/CTiledMap.hpp"

#include <S3DTiled/TiledMap.hpp>

#include <Siv3D/String.hpp>
#include <Siv3D/FileSystem.hpp>

namespace s3dTiled
{
    using namespace s3d;

	class TiledReader::CTiledReader
	{
		friend TiledReader;
	private:
		std::shared_ptr<CTiledMap> m_map;

		bool open(s3d::FilePathView path, TiledFileType fileType)
		{
			if (FileSystem::IsResource(path)) {
				path = path.substr(1);
			}
			if (fileType == TiledFileType::Tmx || FileSystem::Extension(path) == U"tmx") {
				m_map = ParseTmx(path);
			} else if (fileType == TiledFileType::Json || FileSystem::Extension(path) == U"json") {
				// TODO(@tyanmahou): Json Parse
			}
			return static_cast<bool>(m_map);
		}
	public:
		CTiledReader() = default;

		CTiledReader(s3d::FilePathView path, TiledFileType fileType)
		{
			this->open(path, fileType);
		}
	};

	TiledReader::TiledReader():
		pImpl(std::make_shared<CTiledReader>())
	{}

	TiledReader::TiledReader(s3d::FilePathView path, TiledFileType fileType) :
		pImpl(std::make_shared<CTiledReader>(path, fileType))
	{}

	bool TiledReader::open(s3d::FilePathView path, TiledFileType fileType)
	{
		return pImpl->open(path, fileType);
	}

	TiledReader::operator bool() const
	{
		return static_cast<bool>(pImpl->m_map);
	}
	std::shared_ptr<CTiledMap> TiledReader::getTiledMap() const
	{
		return pImpl->m_map;
	}
} // namespace s3dTiled
