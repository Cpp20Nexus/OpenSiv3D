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

# include "CScript.hpp"
# include <Siv3D/Error.hpp>
# include <Siv3D/EngineLog.hpp>
# include <Siv3D/Unicode.hpp>
# include <Siv3D/FileSystem.hpp>
# include <Siv3D/Logger.hpp>
# include "angelscript/scriptarray.h"
# include "angelscript/scriptgrid.h"
# include "angelscript/scriptstdstring.h"
# include "Bind/ScriptBind.hpp"

namespace s3d
{
	namespace detail
	{
		[[nodiscard]]
		inline constexpr StringView GetMessageType(const AngelScript::asEMsgType msgType) noexcept
		{
			constexpr std::array<StringView, 3> types =
			{
				U"error"_sv, U"warning"_sv, U"info"_sv
			};

			return types[msgType];
		}

		static void MessageCallback(const AngelScript::asSMessageInfo* msg, void* pMessageArray)
		{
			const StringView type = GetMessageType(msg->type);
			const String section = Unicode::Widen(msg->section);
			const String message = Unicode::Widen(msg->message);

			const String fullMessage = (U"[{}] {}({}): "_fmt(type, FileSystem::FileName(section), msg->row) + message);
			const String logMessage = (U"{}({}): {}: "_fmt(section, msg->row, type) + message);
			Logger(logMessage);

			Array<String>* messageArray = static_cast<Array<String>*>(pMessageArray);
			messageArray->push_back(fullMessage);
		}
	}

	CScript::CScript() {}

	CScript::~CScript()
	{
		LOG_SCOPED_TRACE(U"CScript::~CScript()");

		shutdown();
	}

	void CScript::init()
	{
		LOG_SCOPED_TRACE(U"CScript::init()");

		if (m_engine = AngelScript::asCreateScriptEngine(ANGELSCRIPT_VERSION); 
			(not m_engine))
		{
			throw EngineError{ U"asCreateScriptEngine() failed" };
		}

		if (m_engine->SetMessageCallback(asFUNCTION(detail::MessageCallback), &m_messages, AngelScript::asCALL_CDECL) < 0)
		{
			throw EngineError{ U"SetMessageCallback() failed" };
		}
		m_engine->SetEngineProperty(AngelScript::asEP_REQUIRE_ENUM_SCOPE, 1);
		m_engine->SetEngineProperty(AngelScript::asEP_DISALLOW_EMPTY_LIST_ELEMENTS, 1);
		m_engine->SetEngineProperty(AngelScript::asEP_ALLOW_UNSAFE_REFERENCES, 1);

		{
			AngelScript::RegisterScriptArray(m_engine);
			AngelScript::RegisterScriptGrid(m_engine);
			RegisterTypes(m_engine);
			AngelScript::RegisterStdString(m_engine);
		}

		{
			auto nullScript = std::make_unique<ScriptData>(ScriptData::Null{}, m_engine);

			if (not nullScript->isInitialized())
			{
				throw EngineError{ U"Failed to create nullScript" };
			}

			m_scripts.setNullData(std::move(nullScript));
		}

		m_shutDown = false;
	}

	void CScript::shutdown()
	{
		if (m_shutDown)
		{
			return;
		}

		LOG_SCOPED_TRACE(U"CScript::shutdown()");

		m_scripts.destroy();

		m_engine->ShutDownAndRelease();

		m_shutDown = true;
	}

	Script::IDType CScript::createFromCode(const StringView code, const ScriptCompileOption compileOption)
	{
		if (not code)
		{
			return Script::IDType::NullAsset();
		}

		{
			auto script = std::make_unique<ScriptData>(ScriptData::Code{}, code, m_engine, compileOption);

			if (not script->isInitialized())
			{
				return Script::IDType::NullAsset();
			}

			const auto id = m_scripts.add(std::move(script));

			m_scripts[id]->setScriptID(id.value());

			return id;
		}
	}

	Script::IDType CScript::createFromFile(const FilePathView path, const ScriptCompileOption compileOption)
	{
		if (not path)
		{
			return Script::IDType::NullAsset();
		}

		{
			auto script = std::make_unique<ScriptData>(ScriptData::File{}, path, m_engine, compileOption);

			if (not script->isInitialized())
			{
				return Script::IDType::NullAsset();
			}

			const auto id = m_scripts.add(std::move(script));

			m_scripts[id]->setScriptID(id.value());

			return id;
		}
	}

	void CScript::release(const Script::IDType handleID)
	{
		m_scripts.erase(handleID);
	}

	bool CScript::compiled(const Script::IDType handleID)
	{
		return m_scripts[handleID]->compileSucceeded();
	}

	bool CScript::reload(const Script::IDType handleID, const ScriptCompileOption compileOption)
	{
		return m_scripts[handleID]->reload(compileOption, handleID.value());
	}
	
	const std::shared_ptr<ScriptModule>& CScript::getModule(const Script::IDType handleID)
	{
		return m_scripts[handleID]->getModule();
	}
	
	AngelScript::asIScriptFunction* CScript::getFunction(const Script::IDType handleID, const StringView decl)
	{
		return m_scripts[handleID]->getFunction(decl);
	}

	Array<String> CScript::getFunctionDeclarations(const Script::IDType handleID, const IncludeParamNames includeParamNames)
	{
		return m_scripts[handleID]->getFunctionDeclarations(includeParamNames);
	}

	const FilePath& CScript::path(const Script::IDType handleID)
	{
		return m_scripts[handleID]->path();
	}

	Array<String> CScript::retrieveMessages_internal()
	{
		Array<String> messages = std::move(m_messages);

		m_messages.clear();

		return messages;
	}

	const Array<String>& CScript::getMessages(const Script::IDType handleID)
	{
		return m_scripts[handleID]->getMessages();
	}

	void CScript::setSystemUpdateCallback(const Script::IDType handleID, const std::function<bool(void)>& callback)
	{
		return m_scripts[handleID]->setSystemUpdateCallback(callback);
	}

	AngelScript::asIScriptEngine* CScript::getEngine()
	{
		return m_engine;
	}
}
