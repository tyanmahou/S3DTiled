#include "TileSetReader.hpp"
#include "TiledParser.hpp"
#include "FileUtil.hpp"

namespace s3dTiled
{
    using namespace s3d;
	class TileSetReader::CTileSetReader
	{
		friend TileSetReader;
	private:
		std::shared_ptr<CTileSet> m_tileSet;

		bool open(s3d::FilePathView path, TileSetFileType fileType)
		{
			auto ext = FileUtil::Extension(path);
			if (fileType == TileSetFileType::Tsx || ext == U"tsx") {
				m_tileSet = ParseTsx(path);
			}

			return static_cast<bool>(m_tileSet);
		}
	public:
		CTileSetReader() = default;

		CTileSetReader(s3d::FilePathView path, TileSetFileType fileType)
		{
			this->open(path, fileType);
		}
	};

    TileSetReader::TileSetReader() :
        pImpl(std::make_shared<CTileSetReader>())
    {}
    TileSetReader::TileSetReader(s3d::FilePathView path, TileSetFileType fileType):
		pImpl(std::make_shared<CTileSetReader>(path, fileType))
    {}
    bool TileSetReader::open(s3d::FilePathView path, TileSetFileType fileType)
    {
        return pImpl->open(path, fileType);
    }
    TileSetReader::operator bool() const
    {
		return static_cast<bool>(pImpl->m_tileSet);
	}
    std::shared_ptr<CTileSet> TileSetReader::getTileSet() const
    {
		return pImpl->m_tileSet;
    }
}

