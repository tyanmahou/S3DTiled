#include <S3DTiled/Property.hpp>

namespace s3dTiled
{
    using namespace s3d;

	Property::Property() :
		m_type(Type::None)
	{
		memset(&m_storage, 0, sizeof(m_storage));
	}

	Property::Property(const Property & other) :
		m_type(Type::None)
	{
		*this = other;
	}
	Property::Property(Property && other)noexcept :
		m_type(Type::None)
	{
		*this = std::move(other);
	}
	Property::Property(bool b):
		m_type(Type::Bool)
	{
		m_storage.b = b;
	}
	Property::Property(const s3d::Color& col):
		m_type(Type::Color)
	{
		m_storage.col = col;
	}
	Property::Property(double f):
		m_type(Type::Float)
	{
		m_storage.f = f;
	}
	Property::Property(s3d::int32 i):
		m_type(Type::Int)
	{
		m_storage.i = i;
	}
	Property::Property(const s3d::String& str):
		m_type(Type::String)
	{
		m_storage.str = new String(str);
	}
	Property::Property(const s3d::String::value_type* rawStr):
		Property(String(rawStr))
	{}
	Property::~Property()
	{
		this->clear();
	}
	bool Property::toBool() const
	{
		return this->m_storage.b;
	}
	Property::operator bool() const
	{
		return this->toBool();
	}
	Color Property::toColor() const
	{
		return this->m_storage.col;
	}
	Property::operator s3d::Color() const
	{
		return this->toColor();
	}
	double Property::toFloat() const
	{
		return this->m_storage.f;
	}
	Property::operator double() const
	{
		return this->toFloat();
	}
	int32 Property::toInt() const
	{
		return this->m_storage.i;
	}
	Property::operator s3d::int32() const
	{
		return this->toInt();
	}
	const String& Property::toString() const
	{
		return *this->m_storage.str;
	}
	Property::operator s3d::String() const
	{
		return this->toString();
	}
	Property::operator s3d::StringView() const
	{
		return this->toString();
	}
	Property::Type Property::getType() const
	{
		return m_type;
	}
	void Property::clear()
	{
		switch (this->m_type)
		{
		case Type::Bool:
			this->m_storage.b = false;
			break;
		case Type::Color:
			this->m_storage.col = Color();
			break;
		case Type::Float:
			this->m_storage.f = 0.0;
			break;
		case Type::Int:
			this->m_storage.i = 0;
			break;
		case Type::String:
			delete this->m_storage.str;
			this->m_storage.str = nullptr;
			break;
		default:
			break;
		}
		this->m_type = Type::None;
	}
	bool Property::isNone() const
	{
		return m_type == Type::None;
	}
	void Property::reset(Type type)
	{
		if (m_type == type) {
			return;
		}
		this->clear();
		switch (type)
		{
		case s3dTiled::Property::Type::String:
			m_storage.str = new String();
			break;
		default:
			break;
		}
		m_type = type;
	}
	Property& Property::operator=(const Property & other)
	{
		if (this == &other) {
			return *this;
		}
		this->reset(other.m_type);

		switch (other.m_type)
		{
		case Type::Bool:
			this->m_storage.b = other.m_storage.b;
			break;
		case Type::Color:
			this->m_storage.col = other.m_storage.col;
			break;
		case Type::Float:
			this->m_storage.f = other.m_storage.f;
			break;
		case Type::Int:
			this->m_storage.i = other.m_storage.i;
			break;
		case Type::String:
			if (this->m_storage.str == nullptr) {
				this->m_storage.str = new String();
			}
			*this->m_storage.str = *other.m_storage.str;
			break;
		default:
			break;
		}
		return *this;
	}
	Property& Property::operator=(Property && other) noexcept
	{
		if (this == &other) {
			return *this;
		}
		this->clear();
		switch (other.m_type)
		{
		case Type::Bool:
			this->m_storage.b = other.m_storage.b;
			break;
		case Type::Color:
			this->m_storage.col = other.m_storage.col;
			break;
		case Type::Float:
			this->m_storage.f = other.m_storage.f;
			break;
		case Type::Int:
			this->m_storage.i = other.m_storage.i;
			break;
		case Type::String:
			this->m_storage.str = other.m_storage.str;
			break;
		default:
			break;
		}
		this->m_type = other.m_type;
		other.m_type = Type::None;
		memset(&other.m_storage, 0, sizeof(other.m_storage));

		return *this;
	}
} // namespace s3dTiled