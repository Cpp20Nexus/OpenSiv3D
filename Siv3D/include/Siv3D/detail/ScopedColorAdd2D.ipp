﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once

namespace s3d
{
	inline ScopedColorAdd2D::ScopedColorAdd2D(const ColorF& color)
		: m_oldColor{ Graphics2D::GetColorAdd() }
	{
		Graphics2D::Internal::SetColorAdd(color.toFloat4());
	}

	inline ScopedColorAdd2D::ScopedColorAdd2D(const double rgb, const double a)
		: ScopedColorAdd2D{ ColorF{rgb, a} }
	{

	}

	inline ScopedColorAdd2D::ScopedColorAdd2D(const double r, const double g, const double b, const double a)
		: ScopedColorAdd2D{ ColorF{ r, g, b, a } }
	{

	}

	inline ScopedColorAdd2D::ScopedColorAdd2D(ScopedColorAdd2D&& other) noexcept
	{
		m_oldColor = other.m_oldColor;
		other.clear();
	}

	inline ScopedColorAdd2D::~ScopedColorAdd2D()
	{
		m_oldColor.then(Graphics2D::Internal::SetColorAdd);
	}

	inline void ScopedColorAdd2D::clear() noexcept
	{
		m_oldColor.reset();
	}
}
