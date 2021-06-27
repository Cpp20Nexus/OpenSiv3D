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

# include "D3D11Renderer3DCommand.hpp"

namespace s3d
{
	D3D11Renderer3DCommandManager::D3D11Renderer3DCommandManager()
	{
		m_vsSamplerStates.fill(Array<SamplerState>{ SamplerState::Default3D });
		m_psSamplerStates.fill(Array<SamplerState>{ SamplerState::Default3D });
		m_vsTextures.fill(Array<Texture::IDType>{ Texture::IDType::InvalidValue()});
		m_psTextures.fill(Array<Texture::IDType>{ Texture::IDType::InvalidValue()});
		m_meshes = { Mesh::IDType::InvalidValue() };

		reset();
	}

	void D3D11Renderer3DCommandManager::reset()
	{
		// clear commands
		{
			m_commands.clear();
			m_changes.clear();
		}

		// clear buffers
		{
			m_draws.clear();
			m_drawLocalToWorlds.clear();
			m_drawDiffuses.clear();

		//	m_nullDraws.clear();
			m_blendStates		= { m_blendStates.back() };
			m_rasterizerStates	= { m_rasterizerStates.back() };
			m_depthStencilStates = { m_depthStencilStates.back() };

			for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
			{
				m_vsSamplerStates[i] = { m_vsSamplerStates[i].back() };
			}

			for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
			{
				m_psSamplerStates[i] = { m_psSamplerStates[i].back() };
			}

			m_scissorRects			= { m_scissorRects.back() };
			m_viewports				= { m_viewports.back() };
		//	m_sdfParams				= { m_sdfParams.back() };
			m_RTs					= { m_RTs.back() };

			m_VSs					= { VertexShader::IDType::InvalidValue() };
			m_PSs					= { PixelShader::IDType::InvalidValue() };
			m_cameraTransforms		= { m_cameraTransforms.back() };
			m_constants.clear();
			m_constantBufferCommands.clear();
		}

		// clear reserves
		{
			m_reservedVSs.clear();
			m_reservedPSs.clear();
			m_reservedTextures.clear();
			m_reservedMeshes.clear();
		}

		// Begin a new frame
		{
		//	m_commands.emplace_back(D3D11Renderer2DCommandType::SetBuffers, 0);
		//	m_commands.emplace_back(D3D11Renderer2DCommandType::UpdateBuffers, 0);

			m_commands.emplace_back(D3D11Renderer3DCommandType::BlendState, 0);
			m_currentBlendState = m_blendStates.front();

			m_commands.emplace_back(D3D11Renderer3DCommandType::RasterizerState, 0);
			m_currentRasterizerState = m_rasterizerStates.front();

			m_commands.emplace_back(D3D11Renderer3DCommandType::DepthStencilState, 0);
			m_currentDepthStencilState = m_depthStencilStates.front();

			for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
			{
				const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSSamplerState0) + i);
				m_commands.emplace_back(command, 0);
				m_currentVSSamplerStates[i] = m_currentVSSamplerStates.front();
			}

			for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
			{
				const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSSamplerState0) + i);
				m_commands.emplace_back(command, 0);
				m_currentPSSamplerStates[i] = m_currentPSSamplerStates.front();
			}

			m_commands.emplace_back(D3D11Renderer3DCommandType::ScissorRect, 0);
			m_currentScissorRect = m_scissorRects.front();

			m_commands.emplace_back(D3D11Renderer3DCommandType::Viewport, 0);
			m_currentViewport = m_viewports.front();

		//	m_commands.emplace_back(D3D11Renderer2DCommandType::SDFParams, 0);
		//	m_currentSDFParams = m_sdfParams.front();

			m_commands.emplace_back(D3D11Renderer3DCommandType::SetRT, 0);
			m_currentRT = m_RTs.front();

			m_commands.emplace_back(D3D11Renderer3DCommandType::SetVS, 0);
			m_currentVS = VertexShader::IDType::InvalidValue();

			m_commands.emplace_back(D3D11Renderer3DCommandType::SetPS, 0);
			m_currentPS = PixelShader::IDType::InvalidValue();

			m_commands.emplace_back(D3D11Renderer3DCommandType::CameraTransform, 0);
			m_currentCameraTransform = m_cameraTransforms.front();

			{
				for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
				{
					const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSTexture0) + i);
					m_vsTextures[i] = { Texture::IDType::InvalidValue() };
					m_commands.emplace_back(command, 0);
				}
				m_currentVSTextures.fill(Texture::IDType::InvalidValue());
			}

			{
				for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
				{
					const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSTexture0) + i);
					m_psTextures[i] = { Texture::IDType::InvalidValue() };
					m_commands.emplace_back(command, 0);
				}
				m_currentPSTextures.fill(Texture::IDType::InvalidValue());
			}

			{
				const auto command = D3D11Renderer3DCommandType::SetMesh;
				m_meshes = { Mesh::IDType::InvalidValue() };
				m_commands.emplace_back(command, 0);
				m_currentMesh = Mesh::IDType::InvalidValue();
			}
		}
	}

	void D3D11Renderer3DCommandManager::flush()
	{
		//if (m_currentDraw.indexCount)
		//{
		//	m_commands.emplace_back(D3D11Renderer2DCommandType::Draw, static_cast<uint32>(m_draws.size()));
		//	m_draws.push_back(m_currentDraw);
		//	m_currentDraw.indexCount = 0;
		//}

		//if (m_changes.has(D3D11Renderer2DCommandType::SetBuffers))
		//{
		//	m_commands.emplace_back(D3D11Renderer2DCommandType::SetBuffers, 0);
		//}

		if (m_changes.has(D3D11Renderer3DCommandType::BlendState))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::BlendState, static_cast<uint32>(m_blendStates.size()));
			m_blendStates.push_back(m_currentBlendState);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::RasterizerState))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::RasterizerState, static_cast<uint32>(m_rasterizerStates.size()));
			m_rasterizerStates.push_back(m_currentRasterizerState);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::DepthStencilState))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::DepthStencilState, static_cast<uint32>(m_depthStencilStates.size()));
			m_depthStencilStates.push_back(m_currentDepthStencilState);
		}

		for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
		{
			const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSSamplerState0) + i);

			if (m_changes.has(command))
			{
				m_commands.emplace_back(command, static_cast<uint32>(m_vsSamplerStates[i].size()));
				m_vsSamplerStates[i].push_back(m_currentVSSamplerStates[i]);
			}
		}

		for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
		{
			const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSSamplerState0) + i);

			if (m_changes.has(command))
			{
				m_commands.emplace_back(command, static_cast<uint32>(m_psSamplerStates[i].size()));
				m_psSamplerStates[i].push_back(m_currentPSSamplerStates[i]);
			}
		}

		if (m_changes.has(D3D11Renderer3DCommandType::ScissorRect))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::ScissorRect, static_cast<uint32>(m_scissorRects.size()));
			m_scissorRects.push_back(m_currentScissorRect);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::Viewport))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::Viewport, static_cast<uint32>(m_viewports.size()));
			m_viewports.push_back(m_currentViewport);
		}

		//if (m_changes.has(D3D11Renderer2DCommandType::SDFParams))
		//{
		//	m_commands.emplace_back(D3D11Renderer2DCommandType::SDFParams, static_cast<uint32>(m_sdfParams.size()));
		//	m_sdfParams.push_back(m_currentSDFParams);
		//}

		if (m_changes.has(D3D11Renderer3DCommandType::SetRT))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::SetRT, static_cast<uint32>(m_RTs.size()));
			m_RTs.push_back(m_currentRT);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::SetVS))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::SetVS, static_cast<uint32>(m_VSs.size()));
			m_VSs.push_back(m_currentVS);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::SetPS))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::SetPS, static_cast<uint32>(m_PSs.size()));
			m_PSs.push_back(m_currentPS);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::CameraTransform))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::CameraTransform, static_cast<uint32>(m_cameraTransforms.size()));
			m_cameraTransforms.push_back(m_currentCameraTransform);
		}

		if (m_changes.has(D3D11Renderer3DCommandType::SetConstantBuffer))
		{
			assert(not m_constantBufferCommands.isEmpty());
			m_commands.emplace_back(D3D11Renderer3DCommandType::SetConstantBuffer, static_cast<uint32>(m_constantBufferCommands.size()) - 1);
		}

		for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
		{
			const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSTexture0) + i);

			if (m_changes.has(command))
			{
				m_commands.emplace_back(command, static_cast<uint32>(m_vsTextures[i].size()));
				m_vsTextures[i].push_back(m_currentVSTextures[i]);
			}
		}

		for (uint32 i = 0; i < SamplerState::MaxSamplerCount; ++i)
		{
			const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSTexture0) + i);

			if (m_changes.has(command))
			{
				m_commands.emplace_back(command, static_cast<uint32>(m_psTextures[i].size()));
				m_psTextures[i].push_back(m_currentPSTextures[i]);
			}
		}

		if (m_changes.has(D3D11Renderer3DCommandType::SetMesh))
		{
			m_commands.emplace_back(D3D11Renderer3DCommandType::SetMesh, static_cast<uint32>(m_meshes.size()));
			m_meshes.push_back(m_currentMesh);
		}

		m_changes.clear();
	}

	const Array<D3D11Renderer3DCommand>& D3D11Renderer3DCommandManager::getCommands() const noexcept
	{
		return m_commands;
	}

	void D3D11Renderer3DCommandManager::pushDraw(const uint32 startIndex, const uint32 indexCount, const Mat4x4* mat, const Float4* color, const uint32 instanceCount)
	{
		// [Siv3D ToDo]
		assert(instanceCount == 1);

		if (m_changes.hasStateChange())
		{
			flush();
		}

		m_commands.emplace_back(D3D11Renderer3DCommandType::Draw, static_cast<uint32>(m_draws.size()));
		m_draws.push_back({ startIndex, indexCount, instanceCount });
		m_drawLocalToWorlds.push_back(*mat);
		m_drawDiffuses.push_back(*color);
		m_changes.set(D3D11Renderer3DCommandType::Draw);
	}

	//void D3D11Renderer2DCommandManager::pushUpdateBuffers(const uint32 batchIndex)
	//{
	//	flush();

	//	m_commands.emplace_back(D3D11Renderer2DCommandType::UpdateBuffers, batchIndex);
	//}

	const D3D11Draw3DCommand& D3D11Renderer3DCommandManager::getDraw(const uint32 index) const noexcept
	{
		return m_draws[index];
	}

	const Mat4x4& D3D11Renderer3DCommandManager::getDrawLocalToWorld(const uint32 index) const noexcept
	{
		return m_drawLocalToWorlds[index];
	}

	const Float4& D3D11Renderer3DCommandManager::getDrawDiffuse(const uint32 index) const noexcept
	{
		return m_drawDiffuses[index];
	}

	//void D3D11Renderer2DCommandManager::pushNullVertices(const uint32 count)
	//{
	//	if (m_changes.hasStateChange())
	//	{
	//		flush();
	//	}

	//	m_commands.emplace_back(D3D11Renderer2DCommandType::DrawNull, static_cast<uint32>(m_nullDraws.size()));
	//	m_nullDraws.push_back(count);
	//	m_changes.set(D3D11Renderer2DCommandType::DrawNull);
	//}

	//uint32 D3D11Renderer2DCommandManager::getNullDraw(const uint32 index) const noexcept
	//{
	//	return m_nullDraws[index];
	//}

	void D3D11Renderer3DCommandManager::pushBlendState(const BlendState& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::BlendState;
		auto& current = m_currentBlendState;
		auto& buffer = m_blendStates;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const BlendState& D3D11Renderer3DCommandManager::getBlendState(const uint32 index) const
	{
		return m_blendStates[index];
	}

	const BlendState& D3D11Renderer3DCommandManager::getCurrentBlendState() const
	{
		return m_currentBlendState;
	}

	void D3D11Renderer3DCommandManager::pushRasterizerState(const RasterizerState& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::RasterizerState;
		auto& current = m_currentRasterizerState;
		auto& buffer = m_rasterizerStates;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}
	
	const RasterizerState& D3D11Renderer3DCommandManager::getRasterizerState(const uint32 index) const
	{
		return m_rasterizerStates[index];
	}

	const RasterizerState& D3D11Renderer3DCommandManager::getCurrentRasterizerState() const
	{
		return m_currentRasterizerState;
	}

	void D3D11Renderer3DCommandManager::pushDepthStencilState(const DepthStencilState& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::DepthStencilState;
		auto& current = m_currentDepthStencilState;
		auto& buffer = m_depthStencilStates;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const DepthStencilState& D3D11Renderer3DCommandManager::getDepthStencilState(const uint32 index) const
	{
		return m_depthStencilStates[index];
	}

	const DepthStencilState& D3D11Renderer3DCommandManager::getCurrentDepthStencilState() const
	{
		return m_currentDepthStencilState;
	}

	void D3D11Renderer3DCommandManager::pushVSSamplerState(const SamplerState& state, const uint32 slot)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSSamplerState0) + slot);
		auto& current = m_currentVSSamplerStates[slot];
		auto& buffer = m_vsSamplerStates[slot];

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const SamplerState& D3D11Renderer3DCommandManager::getVSSamplerState(const uint32 slot, const uint32 index) const
	{
		assert(slot < SamplerState::MaxSamplerCount);

		return m_vsSamplerStates[slot][index];
	}

	const SamplerState& D3D11Renderer3DCommandManager::getVSCurrentSamplerState(const uint32 slot) const
	{
		assert(slot < SamplerState::MaxSamplerCount);

		return m_currentVSSamplerStates[slot];
	}

	void D3D11Renderer3DCommandManager::pushPSSamplerState(const SamplerState& state, const uint32 slot)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSSamplerState0) + slot);
		auto& current = m_currentPSSamplerStates[slot];
		auto& buffer = m_psSamplerStates[slot];

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const SamplerState& D3D11Renderer3DCommandManager::getPSSamplerState(const uint32 slot, const uint32 index) const
	{
		assert(slot < SamplerState::MaxSamplerCount);

		return m_psSamplerStates[slot][index];
	}

	const SamplerState& D3D11Renderer3DCommandManager::getPSCurrentSamplerState(const uint32 slot) const
	{
		assert(slot < SamplerState::MaxSamplerCount);

		return m_currentPSSamplerStates[slot];
	}

	void D3D11Renderer3DCommandManager::pushScissorRect(const Rect& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::ScissorRect;
		auto& current = m_currentScissorRect;
		auto& buffer = m_scissorRects;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const Rect& D3D11Renderer3DCommandManager::getScissorRect(const uint32 index) const
	{
		return m_scissorRects[index];
	}

	const Rect& D3D11Renderer3DCommandManager::getCurrentScissorRect() const
	{
		return m_currentScissorRect;
	}

	void D3D11Renderer3DCommandManager::pushViewport(const Optional<Rect>& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::Viewport;
		auto& current = m_currentViewport;
		auto& buffer = m_viewports;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const Optional<Rect>& D3D11Renderer3DCommandManager::getViewport(const uint32 index) const
	{
		return m_viewports[index];
	}

	const Optional<Rect>& D3D11Renderer3DCommandManager::getCurrentViewport() const
	{
		return m_currentViewport;
	}

	//void D3D11Renderer2DCommandManager::pushSDFParameters(const std::array<Float4, 3>& state)
	//{
	//	constexpr auto command = D3D11Renderer2DCommandType::SDFParams;
	//	auto& current = m_currentSDFParams;
	//	auto& buffer = m_sdfParams;

	//	if (not m_changes.has(command))
	//	{
	//		if (state != current)
	//		{
	//			current = state;
	//			m_changes.set(command);
	//		}
	//	}
	//	else
	//	{
	//		if (state == buffer.back())
	//		{
	//			current = state;
	//			m_changes.clear(command);
	//		}
	//		else
	//		{
	//			current = state;
	//		}
	//	}
	//}

	//const std::array<Float4, 3>& D3D11Renderer2DCommandManager::getSDFParameters(const uint32 index) const
	//{
	//	return m_sdfParams[index];
	//}

	//const std::array<Float4, 3>& D3D11Renderer2DCommandManager::getCurrentSDFParameters() const
	//{
	//	return m_currentSDFParams;
	//}


	void D3D11Renderer3DCommandManager::pushStandardVS(const VertexShader::IDType& id)
	{
		constexpr auto command = D3D11Renderer3DCommandType::SetVS;
		auto& current = m_currentVS;
		auto& buffer = m_VSs;

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
			}
		}
	}

	void D3D11Renderer3DCommandManager::pushCustomVS(const VertexShader& vs)
	{
		const auto id = vs.id();
		constexpr auto command = D3D11Renderer3DCommandType::SetVS;
		auto& current = m_currentVS;
		auto& buffer = m_VSs;

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
				m_reservedVSs.try_emplace(id, vs);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
				m_reservedVSs.try_emplace(id, vs);
			}
		}
	}

	const VertexShader::IDType& D3D11Renderer3DCommandManager::getVS(const uint32 index) const
	{
		return m_VSs[index];
	}

	void D3D11Renderer3DCommandManager::pushStandardPS(const PixelShader::IDType& id)
	{
		constexpr auto command = D3D11Renderer3DCommandType::SetPS;
		auto& current = m_currentPS;
		auto& buffer = m_PSs;

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
			}
		}
	}

	void D3D11Renderer3DCommandManager::pushCustomPS(const PixelShader& ps)
	{
		const auto id = ps.id();
		constexpr auto command = D3D11Renderer3DCommandType::SetPS;
		auto& current = m_currentPS;
		auto& buffer = m_PSs;

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
				m_reservedPSs.try_emplace(id, ps);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
				m_reservedPSs.try_emplace(id, ps);
			}
		}
	}
	
	const PixelShader::IDType& D3D11Renderer3DCommandManager::getPS(const uint32 index) const
	{
		return m_PSs[index];
	}

	void D3D11Renderer3DCommandManager::pushCameraTransform(const Mat4x4& state)
	{
		constexpr auto command = D3D11Renderer3DCommandType::CameraTransform;
		auto& current = m_currentCameraTransform;
		auto& buffer = m_cameraTransforms;

		if (not m_changes.has(command))
		{
			if (state != current)
			{
				current = state;
				m_changes.set(command);
			}
		}
		else
		{
			if (state == buffer.back())
			{
				current = state;
				m_changes.clear(command);
			}
			else
			{
				current = state;
			}
		}
	}

	const Mat4x4& D3D11Renderer3DCommandManager::getCurrentCameraTransform() const
	{
		return m_currentCameraTransform;
	}

	const Mat4x4& D3D11Renderer3DCommandManager::getCameraTransform(const uint32 index) const
	{
		return m_cameraTransforms[index];
	}

	void D3D11Renderer3DCommandManager::pushConstantBuffer(const ShaderStage stage, const uint32 slot, const ConstantBufferBase& buffer, const float* data, const uint32 num_vectors)
	{
		constexpr auto command = D3D11Renderer3DCommandType::SetConstantBuffer;

		flush();
		const __m128* pData = reinterpret_cast<const __m128*>(data);
		const uint32 offset = static_cast<uint32>(m_constants.size());
		m_constants.insert(m_constants.end(), pData, (pData + num_vectors));

		D3D11ConstantBuffer3DCommand cb
		{
			.stage			= stage,
			.slot			= slot,
			.offset			= offset,
			.num_vectors	= num_vectors,
			.cbBase			= buffer
		};

		m_constantBufferCommands.push_back(cb);
		m_changes.set(command);
	}

	D3D11ConstantBuffer3DCommand& D3D11Renderer3DCommandManager::getConstantBuffer(const uint32 index)
	{
		return m_constantBufferCommands[index];
	}

	const __m128* D3D11Renderer3DCommandManager::getConstantBufferPtr(const uint32 offset) const
	{
		return (m_constants.data() + offset);
	}

	void D3D11Renderer3DCommandManager::pushVSTextureUnbind(const uint32 slot)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto id = Texture::IDType::InvalidValue();
		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSTexture0) + slot);
		auto& current = m_currentVSTextures[slot];
		auto& buffer = m_vsTextures[slot];

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
			}
		}
	}

	void D3D11Renderer3DCommandManager::pushVSTexture(const uint32 slot, const Texture& texture)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto id = texture.id();
		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::VSTexture0) + slot);
		auto& current = m_currentVSTextures[slot];
		auto& buffer = m_vsTextures[slot];

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);

				if (m_reservedTextures.find(id) == m_reservedTextures.end())
				{
					m_reservedTextures.emplace(id, texture);
				}
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;

				if (m_reservedTextures.find(id) == m_reservedTextures.end())
				{
					m_reservedTextures.emplace(id, texture);
				}
			}
		}
	}

	const Texture::IDType& D3D11Renderer3DCommandManager::getVSTexture(const uint32 slot, const uint32 index) const
	{
		return m_vsTextures[slot][index];
	}

	const std::array<Texture::IDType, SamplerState::MaxSamplerCount>& D3D11Renderer3DCommandManager::getCurrentVSTextures() const
	{
		return m_currentVSTextures;
	}

	void D3D11Renderer3DCommandManager::pushPSTextureUnbind(const uint32 slot)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto id = Texture::IDType::InvalidValue();
		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSTexture0) + slot);
		auto& current = m_currentPSTextures[slot];
		auto& buffer = m_psTextures[slot];

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;
			}
		}
	}

	void D3D11Renderer3DCommandManager::pushPSTexture(const uint32 slot, const Texture& texture)
	{
		assert(slot < SamplerState::MaxSamplerCount);

		const auto id = texture.id();
		const auto command = ToEnum<D3D11Renderer3DCommandType>(FromEnum(D3D11Renderer3DCommandType::PSTexture0) + slot);
		auto& current = m_currentPSTextures[slot];
		auto& buffer = m_psTextures[slot];

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);

				if (m_reservedTextures.find(id) == m_reservedTextures.end())
				{
					m_reservedTextures.emplace(id, texture);
				}
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;

				if (m_reservedTextures.find(id) == m_reservedTextures.end())
				{
					m_reservedTextures.emplace(id, texture);
				}
			}
		}
	}

	const Texture::IDType& D3D11Renderer3DCommandManager::getPSTexture(const uint32 slot, const uint32 index) const
	{
		return m_psTextures[slot][index];
	}

	const std::array<Texture::IDType, SamplerState::MaxSamplerCount>& D3D11Renderer3DCommandManager::getCurrentPSTextures() const
	{
		return m_currentPSTextures;
	}

	void D3D11Renderer3DCommandManager::pushRT(const Optional<RenderTexture>& rt)
	{
		constexpr auto command = D3D11Renderer3DCommandType::SetRT;
		auto& current = m_currentRT;
		auto& buffer = m_RTs;

		if (!m_changes.has(command))
		{
			if (rt != current)
			{
				current = rt;
				m_changes.set(command);
			}
		}
		else
		{
			if (rt == buffer.back())
			{
				current = rt;
				m_changes.clear(command);
			}
			else
			{
				current = rt;
			}
		}
	}

	const Optional<RenderTexture>& D3D11Renderer3DCommandManager::getRT(const uint32 index) const
	{
		return m_RTs[index];
	}

	const Optional<RenderTexture>& D3D11Renderer3DCommandManager::getCurrentRT() const
	{
		return m_currentRT;
	}

	void D3D11Renderer3DCommandManager::pushMesh(const Mesh& mesh)
	{
		const auto id = mesh.id();
		constexpr auto command = D3D11Renderer3DCommandType::SetMesh;
		auto& current = m_currentMesh;
		auto& buffer = m_meshes;

		if (not m_changes.has(command))
		{
			if (id != current)
			{
				current = id;
				m_changes.set(command);

				if (not m_reservedMeshes.contains(id))
				{
					m_reservedMeshes.emplace(id, mesh);
				}
			}
		}
		else
		{
			if (id == buffer.back())
			{
				current = id;
				m_changes.clear(command);
			}
			else
			{
				current = id;

				if (not m_reservedMeshes.contains(id))
				{
					m_reservedMeshes.emplace(id, mesh);
				}
			}
		}
	}

	const Mesh::IDType& D3D11Renderer3DCommandManager::getMesh(const uint32 index) const
	{
		return m_meshes[index];
	}

	const Mesh::IDType& D3D11Renderer3DCommandManager::getCurrentMesh() const
	{
		return m_currentMesh;
	}
}
