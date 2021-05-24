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
# include <Siv3D/AssetHandleManager/AssetHandleManager.hpp>
# include "IAudio.hpp"
# include "AudioData.hpp"
# include "AudioBus.hpp"

namespace s3d
{
	class CAudio final : public ISiv3DAudio
	{
	public:

		CAudio();

		~CAudio();

		void init() override;

		Audio::IDType create(Wave&& wave, const Optional<AudioLoopTiming>& loop) override;

		Audio::IDType create(Wave&& wave, const Duration& loopBegin, const Duration& loopEnd) override;

		Audio::IDType createStreamingNonLoop(FilePathView path) override;

		Audio::IDType createStreamingLoop(FilePathView path, uint64 loopBegin) override;

		void release(Audio::IDType handleID) override;

		bool isStreaming(Audio::IDType handleID) override;

		uint32 sampleRate(Audio::IDType handleID) override;

		size_t samples(Audio::IDType handleID) override;

		int64 samplesPlayed(Audio::IDType handleID) override;

		bool isActive(Audio::IDType handleID) override;

		bool isPlaying(Audio::IDType handleID) override;

		bool isPaused(Audio::IDType handleID) override;

		bool isLoop(Audio::IDType handleID) override;

		AudioLoopTiming getLoopTiming(Audio::IDType handleID) override;

		void setLoop(Audio::IDType handleID, bool loop) override;

		void setLoopPoint(Audio::IDType handleID, const Duration& loopBegin) override;

		void play(Audio::IDType handleID, size_t busIndex) override;

		void play(Audio::IDType handleID, size_t busIndex, const Duration& duration) override;

		void pause(Audio::IDType handleID) override;

		void pause(Audio::IDType handleID, const Duration& duration) override;

		void stop(Audio::IDType handleID) override;

		void stop(Audio::IDType handleID, const Duration& duration) override;


		double posSec(Audio::IDType handleID) override;

		void seekTo(Audio::IDType handleID, const Duration& pos) override;

		double getVolume(Audio::IDType handleID) override;

		void setVolume(Audio::IDType handleID, double volume) override;

		void fadeVolume(Audio::IDType handleID, double volume, const Duration& time) override;

		double getPan(Audio::IDType handleID) override;

		void setPan(Audio::IDType handleID, double pan) override;

		void fadePan(Audio::IDType handleID, double pan, const Duration& time) override;

		double getSpeed(Audio::IDType handleID) override;

		void setSpeed(Audio::IDType handleID, double speed) override;

		void fadeSpeed(Audio::IDType handleID, double speed, const Duration& time) override;


		size_t getActiveVoiceCount() override;

		void fadeGlobalVolume(double volume, const Duration& time) override;

		void getGlobalSamples(Array<float>& samples) override;

		void getGlobalFFT(Array<float>& result) override;

		void getBusSamples(size_t busIndex, Array<float>& samples) override;

		void getBusFFT(size_t busIndex, Array<float>& result) override;

		SoLoud::Bus& getBus(size_t busIndex) override;

	private:

		std::unique_ptr<SoLoud::Soloud> m_soloud;
		
		std::array<std::unique_ptr<AudioBus>, Audio::MaxBusCount> m_buses;

		AssetHandleManager<Audio::IDType, AudioData> m_audios{ U"Audio" };
	};
}
