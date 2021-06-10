#pragma once

#include <memory>

#include "Tile.hpp"
#include "Types.hpp"

namespace s3dTiled
{
    class CTileSet;

    /// <summary>
    /// タイルセット
    /// </summary>
    class TileSet
    {
    public:
        TileSet() = default;

        TileSet(std::shared_ptr<CTileSet> tileSet);

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="path">開くファイル</param>
        /// <param name="fileType"></param>
        TileSet(s3d::FilePathView path, TileSetFileType fileType = TileSetFileType::Unspecified);

        /// <summary>
        /// タイルセットを開く
        /// </summary>
        /// <param name="path"></param>
        /// <param name="fileType"></param>
        bool open(s3d::FilePathView path, TileSetFileType fileType = TileSetFileType::Unspecified);

        /// <summary>
        /// タイル数
        /// </summary>
        /// <returns></returns>
        s3d::uint32 getTileCount() const;

        /// <summary>
        /// タイルを探す
        /// </summary>
        /// <param name="tileId"></param>
        /// <returns></returns>
        const Tile& find(TileId tileId) const;

        /// <summary>
        /// タイルを取得
        /// </summary>
        /// <param name="tileId"></param>
        /// <param name="timeMillisec">現在時間[s]</param>
        /// <returns></returns>
        const Tile& getTile(TileId tileId, double timeSec) const;

        const Properties& getProperties(TileId tileId) const;

        s3d::Optional<Property> getProperty(TileId tileId, const s3d::String& key) const;

    private:
        std::shared_ptr<CTileSet> pImpl;
    };
} // namespace s3dTiled