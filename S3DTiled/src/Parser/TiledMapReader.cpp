#include "TiledMapReader.hpp"
#include "TiledParser.hpp"
#include "FileUtil.hpp"

namespace s3dTiled
{
    using namespace s3d;

	class TiledMapReader::CTiledMapReader
	{
		friend TiledMapReader;
	private:
		std::shared_ptr<CTiledMap> m_map;

		bool open(s3d::FilePathView path, TiledMapFileType fileType)
		{
			auto ext = FileUtil::Extension(path);
			if (fileType == TiledMapFileType::Tmx || ext == U"tmx") {
				m_map = ParseTmx(path);
			} else if (fileType == TiledMapFileType::Json || ext == U"json") {
				// TODO(@tyanmahou): Json Parse
			}
			return static_cast<bool>(m_map);
		}
	public:
		CTiledMapReader() = default;

		CTiledMapReader(s3d::FilePathView path, TiledMapFileType fileType)
		{
			this->open(path, fileType);
		}
	};

	TiledMapReader::TiledMapReader():
		pImpl(std::make_shared<CTiledMapReader>())
	{}

	TiledMapReader::TiledMapReader(s3d::FilePathView path, TiledMapFileType fileType) :
		pImpl(std::make_shared<CTiledMapReader>(path, fileType))
	{}

	bool TiledMapReader::open(s3d::FilePathView path, TiledMapFileType fileType)
	{
		return pImpl->open(path, fileType);
	}

	TiledMapReader::operator bool() const
	{
		return static_cast<bool>(pImpl->m_map);
	}
	std::shared_ptr<CTiledMap> TiledMapReader::getTiledMap() const
	{
		return pImpl->m_map;
	}
} // namespace s3dTiled
