#pragma once

#include "Types.hpp"
#include <Siv3D/Array.hpp>

namespace s3dTiled
{
    /// <summary>
    /// アニメーション
    /// </summary>
    class Animation
    {
    public:
        using Duration = s3d::int32;
    public:
        Animation() = default;

        void addFrame(TileId tileId, Duration duration);
        TileId getTileId(double timeSec) const;

        const s3d::Array<std::pair<TileId, Duration>>& getFrames() const
        {
            return m_frames;
        }

        bool isEmpty() const
        {
            return m_frames.isEmpty();
        }
    private:
        s3d::Array<std::pair<TileId, Duration>> m_frames;
        Duration m_maxDuration = 0;
    };
} // namespace s3dTiled