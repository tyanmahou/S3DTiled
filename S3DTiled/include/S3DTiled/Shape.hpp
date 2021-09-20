#pragma once

#include<variant>

#include <Siv3D/RectF.hpp>
#include <Siv3D/Ellipse.hpp>
#include <Siv3D/LineString.hpp>
#include <Siv3D/Polygon.hpp>

namespace s3dTiled
{
	using Shape = std::variant<
		s3d::RectF,
		s3d::Ellipse,
		s3d::Polygon,
		s3d::LineString
	>;
} // namespace s3dTiled