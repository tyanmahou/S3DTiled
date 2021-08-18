#pragma once
#include <Siv3D/Grid.hpp>
#include <Siv3D/HashTable.hpp>

namespace s3dTiled
{
    template<class Type>
    class Chunk
    {
    public:
        Chunk() = default;
        Chunk(s3d::int32 chunkSizeX, s3d::int32 chunkSizeY) :
            Chunk(s3d::Size{ chunkSizeX, chunkSizeY })
        {}
        Chunk(const s3d::Size& chunkSize):
            m_chunkSize(chunkSize),
            m_startIndex(0, 0),
            m_endIndex(chunkSize)
        {}

        const Type& operator()(s3d::int32 y, s3d::int32 x) const
        {
            const s3d::int32 baseY = y >= 0 ? y / m_chunkSize.y : (y - m_chunkSize.y + 1) / m_chunkSize.y;
            const s3d::int32 baseX = x >= 0 ? x / m_chunkSize.x : (x - m_chunkSize.x + 1) / m_chunkSize.x;
            if (!m_chunks.contains(baseX)) {
                return notfound;
            }
            if (!m_chunks.at(baseX).contains(baseY)) {
                return notfound;
            }
            const auto& grid = m_chunks.at(baseX).at(baseY);
            auto fixY = static_cast<size_t>(y - baseY * m_chunkSize.y);
            auto fixX = static_cast<size_t>(x - baseX * m_chunkSize.x);
            if (!grid.inBounds(fixY, fixX)) {
                return notfound;
            }
            return grid[fixY][fixX];
        }
        const s3d::Size& chunkSize() const
        {
            return m_chunkSize;
        }
        const s3d::Size& startIndex() const
        {
            return m_startIndex;
        }
        const s3d::Size& endIndex() const
        {
            return m_endIndex;
        }
        s3d::Size size() const
        {
            return m_endIndex - m_startIndex;
        }
        s3d::int32 width() const
        {
            return m_endIndex.x - m_startIndex.x;
        }
        s3d::Size height() const
        {
            return m_endIndex.y - m_startIndex.y;
        }
        auto begin() const
        {
            return m_chunks.begin();
        }

        auto end() const
        {
            return m_chunks.end();
        }

    public:
        void push(const s3d::Point& pos, s3d::Grid<Type>&& grid)
        {
            if (pos.x < m_startIndex.x) {
                m_startIndex.x = pos.x;
            } else if (pos.x + m_chunkSize.x > m_endIndex.x) {
                m_endIndex.x = pos.x + m_chunkSize.x;
            }
            if (pos.y < m_startIndex.y) {
                m_startIndex.y = pos.y;
            } else if (pos.y + m_chunkSize.y > m_endIndex.y) {
                m_endIndex.y = pos.y + m_chunkSize.y;
            }
            m_chunks[pos.x / m_chunkSize.x][pos.y / m_chunkSize.y] = std::move(grid);
        }
    private:
        s3d::Size m_chunkSize;
        s3d::Size m_startIndex;
        s3d::Size m_endIndex;

        s3d::HashTable<s3d::int32, s3d::HashTable<s3d::int32, s3d::Grid<Type>>> m_chunks;

        inline static constexpr Type notfound{};
    };
} // namespace s3dTiled