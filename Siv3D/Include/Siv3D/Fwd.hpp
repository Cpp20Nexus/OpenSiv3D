﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2017 Ryo Suzuki
//	Copyright (c) 2016-2017 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include "Platform.hpp"
# include "Types.hpp"

namespace s3d
{
	//////////////////////////////////////////////////////
	//
	//	Uncopyable.hpp
	//
	class Uncopyable;

	//////////////////////////////////////////////////////
	//
	//	AlignedAllocator.hpp
	//
	template <class Type, size_t Alignment> class AlignedAllocator;
	template <class Type> struct DefaultAllocator;

	//////////////////////////////////////////////////////
	//
	//	Array.hpp
	//
	template <class Type, class Allocator> class Array;

	//////////////////////////////////////////////////////
	//
	//	ByteArrayView.hpp
	//
	class ByteArrayView;

	//////////////////////////////////////////////////////
	//
	//	Grid.hpp
	//
	template <class Type, class Allocator> class Grid;

	//////////////////////////////////////////////////////
	//
	//	Optional.hpp
	//
	struct None_t;
	template <class Type> class Optional;

	//////////////////////////////////////////////////////
	//
	//	IteratorRange.hpp
	//
	template <class IteratorType> class IteratorRange;

	//////////////////////////////////////////////////////
	//
	//	String.hpp
	//
	class String;
	using FilePath = String;

	//////////////////////////////////////////////////////
	//
	//	StringView.hpp
	//
	template <class CharType> class BasicStringView;
	using CStringView		= BasicStringView<char>;
	using UTF8StringView	= BasicStringView<char>;
	using StringView		= BasicStringView<wchar>;

	//////////////////////////////////////////////////////
	//
	//	LetterCase.hpp
	//
	enum class LetterCase;

	//////////////////////////////////////////////////////
	//
	//	BigInt.hpp
	//
	class BigInt;

	//////////////////////////////////////////////////////
	//
	//	BigFloat.hpp
	//
	class BigFloat;

	//////////////////////////////////////////////////////
	//
	//	Format.hpp
	//
	struct FormatData;

	//////////////////////////////////////////////////////
	//
	//	Date.hpp
	//
	struct Date;

	//////////////////////////////////////////////////////
	//
	//	DateTime.hpp
	//
	struct DateTime;

	//////////////////////////////////////////////////////
	//
	//	Stopwatch.hpp
	//
	class Stopwatch;

	//////////////////////////////////////////////////////
	//
	//	SpeedStopwatch.hpp
	//
	class SpeedStopwatch;

	//////////////////////////////////////////////////////
	//
	//	Timer.hpp
	//
	class Timer;

	//////////////////////////////////////////////////////
	//
	//	TimeProfiler.hpp
	//
	class TimeProfiler;

	//////////////////////////////////////////////////////
	//
	//	RDTSCClock.hpp
	//
	class RDTSCClock;

	//////////////////////////////////////////////////////
	//
	//	FileSystem.hpp
	//
	enum class OpenMode;
	enum class CopyOption;
	enum class SpecialFolder;

	//////////////////////////////////////////////////////
	//
	//	IReader.hpp
	//
	class IReader;

	//////////////////////////////////////////////////////
	//
	//	IWriter.hpp
	//
	class IWriter;

	//////////////////////////////////////////////////////
	//
	//	MemoryMapping.hpp
	//
	class MemoryMapping;

	//////////////////////////////////////////////////////
	//
	//	WritableMemoryMapping.hpp
	//
	class WritableMemoryMapping;
	enum class MMFOpenMode_if_Exists;
	enum class MMFOpenMode_if_NotFound;

	//////////////////////////////////////////////////////
	//
	//	ByteArray.hpp
	//
	class ByteArray;

	//////////////////////////////////////////////////////
	//
	//	BinaryReader.hpp
	//
	class BinaryReader;

	//////////////////////////////////////////////////////
	//
	//	BinaryWriter.hpp
	//
	class BinaryWriter;

	//////////////////////////////////////////////////////
	//
	//	CharacterEncoding.hpp
	//
	enum class CharacterEncoding;

	//////////////////////////////////////////////////////
	//
	//	TextReader.hpp
	//
	class TextReader;

	//////////////////////////////////////////////////////
	//
	//	TextWriter.hpp
	//
	class TextWriter;

	//////////////////////////////////////////////////////
	//
	//	MD5.hpp
	//
	struct MD5Hash;

	//////////////////////////////////////////////////////
	//
	//	Logger.hpp
	//
	enum class OutputLevel;
	enum class LogDescription;




	//////////////////////////////////////////////////////
	//
	//	INIReader.hpp
	//
	struct INIKey;
	class INIReader;





	//////////////////////////////////////////////////////
	//
	//	Color.hpp
	//
	struct Color;
	struct ColorF;

	//////////////////////////////////////////////////////
	//
	//	HSV.hpp
	//
	struct HSV;

	//////////////////////////////////////////////////////
	//
	//	PointVector.hpp
	//
	struct Point;
	using Size = Point;
	template <class Type> struct Vector2D;
	template <class Type> struct Vector3D;
	template <class Type> struct Vector4D;
	using Float2	= Vector2D<float>;
	using Vec2		= Vector2D<double>;
	using Float3	= Vector3D<float>;
	using Vec3		= Vector3D<double>;
	using Float4	= Vector4D<float>;
	using Vec4		= Vector4D<double>;

	//////////////////////////////////////////////////////
	//
	//	Circular.hpp
	//
	template <int32 Oclock> struct CircularBase;
	using Circular	= CircularBase<0>;
	using Circular0	= CircularBase<0>;
	using Circular3	= CircularBase<3>;
	using Circular6	= CircularBase<6>;
	using Circular9	= CircularBase<9>;

	//////////////////////////////////////////////////////
	//
	//	Cylindrical.hpp
	//
	struct Cylindrical;

	//////////////////////////////////////////////////////
	//
	//	Spherical.hpp
	//
	struct Spherical;






	//////////////////////////////////////////////////////
	//
	//	Distribution.hpp
	//
	template <class Type> class UniformDistribution;
	template <class Type> class NormalDistribution;

	//////////////////////////////////////////////////////
	//
	//	HardwareRNG.hpp
	//
	class HardwareRNG;

	//////////////////////////////////////////////////////
	//
	//	Xorshift.hpp
	//
	class Xorshift64Star;
	class Xorshift128Plus;
	class Xorshift1024Star;

	//////////////////////////////////////////////////////
	//
	//	MersenneTwister.hpp
	//
	class MT11213b;
	class MT19937;
	class MT19937_64;

	//////////////////////////////////////////////////////
	//
	//	DefaultRNG.hpp
	//
	using DefaultRNGType = MT11213b;




	//////////////////////////////////////////////////////
	//
	//	PerlinNoise.hpp
	//
	class PerlinNoise;



	//////////////////////////////////////////////////////
	//
	//	MathParser.hpp
	//
	class MathParser;




	//////////////////////////////////////////////////////
	//
	//	Image.hpp
	//
	class Image;

	//////////////////////////////////////////////////////
	//
	//	ImageFormat.hpp
	//
	enum class ImageFormat;
	class IImageFormat;


	//////////////////////////////////////////////////////
	//
	//	DragDrop.hpp
	//
	enum class DragItemType;
	struct DragStatus;
	struct DroppedFilePath;
	struct DroppedText;




	//////////////////////////////////////////////////////
	//
	//	Key.hpp
	//
	enum class InputDevice : uint8;
	class Key;

	//////////////////////////////////////////////////////
	//
	//	KeyConjunction.hpp
	//
	class KeyConjunction;

	//////////////////////////////////////////////////////
	//
	//	KeyGroup.hpp
	//
	class KeyGroup;



	//////////////////////////////////////////////////////
	//
	//	Monitor.hpp
	//
	struct Monitor;

	//////////////////////////////////////////////////////
	//
	//	Window.hpp
	//
	struct WindowState;



	//////////////////////////////////////////////////////
	//
	//	Graphics.hpp
	//
	struct DisplayMode;
	struct DisplayOutput;
}
