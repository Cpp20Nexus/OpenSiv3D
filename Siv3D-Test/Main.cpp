﻿
# include <Siv3D.hpp>
# include "Test/Siv3DTest.hpp"
using namespace s3d;
using namespace s3d::literals;
using namespace std::literals;

void F(ByteArrayView view)
{
	const uint8* ptr = view.data();
	const size_t size = view.size_bytes();
	for (const uint8 byte : view)
	{
		byte;
	}
}

void F2(ByteArrayView view)
{
	Log(L"a");
}

void F2(const FilePath& path)
{
	Log(L"b");
}


void K()
{
	const int32 n = 100;
	int32 na[5] = { 11,22,33 };
	const Array<int32> v = { 111,222,333 };
	const String s = L"Siv3D";
	F(n);
	F(na);
	F(v);
	F(s);
	F(Vec2(20, 20));
	F(Date::Today());

	F2(FilePath(L"ABC"));
}

void Main()
{	
	Compression::Compress(ByteArray(L"Example/LICENSE.txt").getView()).save(L"c1.zst");
	Compression::CompressFile(L"Example/LICENSE.txt").save(L"c2.zst");
	Compression::CompressFileToFile(L"Example/LICENSE.txt", L"c4.zst");
}


