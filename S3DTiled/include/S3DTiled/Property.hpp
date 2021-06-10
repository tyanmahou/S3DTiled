#pragma once

#include<Siv3D/Fwd.hpp>
#include<Siv3D/Color.hpp>

namespace s3dTiled
{
		/// <summary>
		/// Tiledのプロパティ
		/// </summary>
		class Property
		{
		public:
			enum class Type
			{
				Bool,
				Color,
				Float,
				Int,
				String,
				None,
			};
		private:
			Type m_type;
			union
			{
				bool b;
				s3d::Color col;
				double f;
				s3d::int32 i;
				s3d::String* str;
			}m_storage;

			void reset(Type type);
		public:
			Property();
			Property(const Property& other);
			Property(Property&& other) noexcept;

			Property(bool b);
			Property(const s3d::Color& col);
			Property(double f);
			Property(s3d::int32 i);
			Property(const s3d::String& str);

			~Property();

			bool toBool() const;
			operator bool() const;

			s3d::Color toColor() const;
			operator s3d::Color() const;

			double toFloat() const;
			operator double() const;

			s3d::int32 toInt() const;
			operator s3d::int32() const;

			const s3d::String& toString() const;
			operator s3d::String() const;

			Type getType()const;

			void clear();

			Property& operator =(const Property& other);
			Property& operator =(Property&& other) noexcept;
		};
} // namespace s3dTiled