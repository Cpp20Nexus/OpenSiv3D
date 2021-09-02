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
# include "Common.hpp"

namespace s3d
{
	enum class LanguageCode : int32
	{
		ArabicSA = 0x401,		// ar_SA

		ChineseCN = 0x804,		// zh_CN

		ChineseHK = 0xc04,		// zh_HK

		ChineseTW = 0x404,		// zh_TW

		EnglishAU = 0xc09,		// en_AU

		EnglishGB = 0x809,		// en_GB

		EnglishUS = 0x409,		// en_US

		FrenchFR = 0x40c,		// fr_FR

		GermanDE = 0x407,		// de_DE

		HindiIN = 0x439,		// hi_IN

		ItalianIT = 0x410,		// it_IT

		Japanese = 0x411,		// ja

		Korean = 0x412,			// ko

		PortugueseBR = 0x416,	// pt_BR

		RussianRU = 0x419,		// ru_RU

		SpanishES = 0x40a,		// es_ES

		Unspecified = 0x000,
	};
}
