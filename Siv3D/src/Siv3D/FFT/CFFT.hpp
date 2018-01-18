﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2018 Ryo Suzuki
//	Copyright (c) 2016-2018 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <array>
# include "IFFT.hpp"
# include "../../ThirdParty/pffft/pffft.h"

namespace s3d
{
	class CFFT : public ISiv3DFFT
	{
	private:

		std::array<PFFFT_Setup*, 7> m_setups;

		float* m_inoutBuffer = nullptr;

		float* m_workBuffer = nullptr;

		void doFFT(FFTResult& result, uint32 samplingRate, FFTSampleLength samplingLength);

	public:

		CFFT();

		~CFFT() override;

		bool init() override;

		void fft(FFTResult& result, const Wave& wave, const uint32 pos, const FFTSampleLength sampleLength) override;

		void fft(FFTResult& result, const float* input, size_t size, uint32 samplingRate, FFTSampleLength sampleLength) override;
	};
}