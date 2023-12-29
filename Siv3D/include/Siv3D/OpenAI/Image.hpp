﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2023 Ryo Suzuki
//	Copyright (c) 2016-2023 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include "../Common.hpp"
# include "../String.hpp"
# include "../Array.hpp"
# include "../Image.hpp"
# include "../AsyncTask.hpp"

namespace s3d
{
	namespace OpenAI
	{
		namespace Image
		{
			/// @brief 256x256 ピクセルの画像サイズ | Image size of 256x256 pixels
			inline constexpr Size ImageSize256{ 256, 256 };

			/// @brief 512x512 ピクセルの画像サイズ | Image size of 512x512 pixels
			inline constexpr Size ImageSize512{ 512, 512 };

			/// @brief 1024x1024 ピクセルの画像サイズ | Image size of 1024x1024 pixels
			inline constexpr Size ImageSize1024{ 1024, 1024 };

			/// @brief 生成できる最大の画像の個数 | Maximum number of images that can be generated
			inline constexpr int32 MaxImageCount = 10;

			/// @brief テキストに基づいて DALL-E モデルを使用して生成された画像を取得します。 | Get an image generated by the DALL-E model based on the given text.
			/// @param apiKey OpenAI API キー | OpenAI API key
			/// @param prompt 画像を説明するテキスト。英語で 1000 文字以下 | Text describing the image, in English and up to 1000 characters
			/// @param size 生成する画像のサイズ。OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024 のいずれか | Size of the generated image. One of OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024
			/// @return 生成された画像。生成に失敗した場合は空の画像 | Generated image. An empty image if the generation fails.
			/// @remark インターネットアクセスが必要です。 | Internet access is required.
			[[nodiscard]]
			s3d::Image Create(StringView apiKey, StringView prompt, const Size& size = OpenAI::Image::ImageSize256);

			/// @brief テキストに基づいて DALL-E モデルを使用して生成された 1 つまたは複数の画像を取得します。 | Get one or more images generated by the DALL-E model based on the given text.
			/// @param apiKey OpenAI API キー | OpenAI API key
			/// @param prompt 画像を説明するテキスト。英語で 1000 文字以下 | Text describing the image, in English and up to 1000 characters
			/// @param n 生成する画像の個数。1 以上 OpenAI::MaxImageCount 以下 | Number of images to generate, between 1 and OpenAI::MaxImageCount
			/// @param size 生成する画像のサイズ。OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024 のいずれか | Size of the generated images. One of OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024
			/// @return 生成された画像の配列。生成に失敗した場合は空の配列 | Array of generated images. An empty array if the generation fails.
			/// @remark インターネットアクセスが必要です。 | Internet access is required.
			[[nodiscard]]
			Array<s3d::Image> Create(StringView apiKey, StringView prompt, int32 n, const Size& size = OpenAI::Image::ImageSize256);

			/// @brief テキストに基づいて DALL-E モデルを使用して画像を生成する非同期タスクを返します。 | Returns an asynchronous task that generates an image using the DALL-E model based on the given text.
			/// @param apiKey OpenAI API キー | OpenAI API key
			/// @param prompt 画像を説明するテキスト。英語で 1000 文字以下 | Text describing the image, in English and up to 1000 characters
			/// @param size 生成する画像のサイズ。OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024 のいずれか | Size of the generated image. One of OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024
			/// @return 生成された画像を返す非同期タスク | Asynchronous task that returns the generated image
			/// @remark 戻り値の task が `task.isReady() == true` になれば `task.get()` で結果を取得できます。 | The result can be retrieved if `task.isReady() == true`.
			/// @remark インターネットアクセスが必要です。 | Internet access is required.
			[[nodiscard]]
			AsyncTask<s3d::Image> CreateAsync(StringView apiKey, StringView prompt, const Size& size = OpenAI::Image::ImageSize256);

			/// @brief テキストに基づいて DALL-E モデルを使用して 1 つまたは複数の画像を生成する非同期タスクを返します。 | Returns an asynchronous task that generates one or more images using the DALL-E model based on the given text.
			/// @param apiKey OpenAI API キー | OpenAI API key
			/// @param prompt 画像を説明するテキスト。英語で 1000 文字以下 | Text describing the image, in English and up to 1000 characters
			/// @param n 生成する画像の個数。1 以上 OpenAI::MaxImageCount 以下 | Number of images to generate, between 1 and OpenAI::MaxImageCount
			/// @param size 生成する画像のサイズ。OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024 のいずれか | Size of the generated image. One of OpenAI::Image::ImageSize256, OpenAI::Image::ImageSize512, OpenAI::Image::ImageSize1024
			/// @return 生成された画像の配列を返す非同期タスク | Asynchronous task that returns an array of generated images
			/// @remark 戻り値の task が `task.isReady() == true` になれば `task.get()` で結果を取得できます。 | The result can be retrieved if `task.isReady() == true`.
			/// @remark インターネットアクセスが必要です。 | Internet access is required.
			[[nodiscard]]
			AsyncTask<Array<s3d::Image>> CreateAsync(StringView apiKey, StringView prompt, int32 n, const Size& size = OpenAI::Image::ImageSize256);
		}
	}
}