#include <S3DTiled/Animation.hpp>

namespace s3dTiled
{
    void Animation::addFrame(TileId tileId, Duration duration)
    {
        m_frames.emplace_back(tileId, duration);
		m_maxDuration += duration;
    }
    TileId Animation::getTileId(double timeSec) const
    {
		if (m_maxDuration == 0) {
			return 0;
		}
		s3d::int32 currentDuration = static_cast<s3d::int32>(timeSec * 1000.0) % m_maxDuration;

		s3d::int32 sumDuration = 0;
		for (const auto& frame : m_frames) {
			if (currentDuration < sumDuration + frame.second) {
				return frame.first;
			}
			sumDuration += frame.second;
		}

		return 0;
    }
} // namespace s3dTiled