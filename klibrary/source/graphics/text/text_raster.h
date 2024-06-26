#pragma once

#include "apis/apis.h"


namespace kl {
	inline constexpr UINT GPU_BUFFER_COUNT = 2;
}

namespace kl {
	struct Text
	{
		TextFormat format{};
		kl::Float4 color{ 1.0f };
		kl::Float2 position{};
		kl::Float2 rect_size{};
		std::wstring data{};
	};
}

namespace kl {
	class TextRaster
	{
	protected:
		ComPtr<ID2D1Factory> m_d2d1_factory{};
		ComPtr<IDWriteFactory> m_write_factory{};
		ComPtr<ID2D1RenderTarget> m_d2d1_targets[GPU_BUFFER_COUNT] = {};

	public:
		std::vector<Text> text_data{};

		// Init
		TextRaster();

		// Copy
		TextRaster(const TextRaster&) = delete;
		TextRaster(TextRaster&&) = delete;

		void operator=(const TextRaster&) = delete;
		void operator=(TextRaster&&) = delete;

		// Create
		TextFormat create_text_format(
			const std::wstring_view& font_family,
			DWRITE_FONT_WEIGHT font_weight,
			DWRITE_FONT_STYLE font_style,
			float font_size,
			const std::wstring_view& locale = L"en-us"
		) const;

		// Draw
		void draw_text(UINT target_index) const;
	};
}
