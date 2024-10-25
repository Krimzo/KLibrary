#include "examples.h"


enum SudokuPiece : uint16_t
{
	ZERO  = 0b0000000000000000,
	ONE   = 0b0000000000000001,
	TWO   = 0b0000000000000010,
	THREE = 0b0000000000000100,
	FOUR  = 0b0000000000001000,
	FIVE  = 0b0000000000010000,
	SIX   = 0b0000000000100000,
	SEVEN = 0b0000000001000000,
	EIGHT = 0b0000000010000000,
	NINE  = 0b0000000100000000,

	_ALL = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE,
};

static constexpr SudokuPiece convert_char(char value)
{
	switch (value)
	{
	case '1': return ONE;
	case '2': return TWO;
	case '3': return THREE;
	case '4': return FOUR;
	case '5': return FIVE;
	case '6': return SIX;
	case '7': return SEVEN;
	case '8': return EIGHT;
	case '9': return NINE;
	}
	return ZERO;
}

static constexpr char convert_piece(SudokuPiece value)
{
	switch (value)
	{
	case   ONE: return '1';
	case   TWO: return '2';
	case THREE: return '3';
	case  FOUR: return '4';
	case  FIVE: return '5';
	case   SIX: return '6';
	case SEVEN: return '7';
	case EIGHT: return '8';
	case  NINE: return '9';
	}
	return '0';
}

struct Index
{
	int value = 0;

	constexpr Index(int value)
		: value(value)
	{}

	constexpr Index(int x, int y)
		: value(x + y * 9)
	{}

	constexpr operator int() const
	{
		return value;
	}

	constexpr operator kl::Int2() const
	{
		return { x(), y() };
	}

	constexpr int x() const
	{
		return value % 9;
	}

	constexpr int y() const
	{
		return value / 9;
	}

	static friend std::ostream& operator<<(std::ostream& stream, const Index& index)
	{
		return stream << "(" << index.x() << ", " << index.y() << ")";
	}
};

struct Possible : std::vector<SudokuPiece>
{
	std::bitset<9> data;

	constexpr Possible()
	{}

	constexpr void insert(SudokuPiece piece)
	{
		int index = convert_piece(piece) - '0' - 1;
		if (index < 0)
			return;

		if (!data[index]) {
			data[index] = true;
			push_back(piece);
		}
	}
};

struct Sudoku
{
	SudokuPiece board[81] = {};
	bool defaults[81] = {};

	constexpr Sudoku()
	{}

	constexpr Sudoku(const std::string_view& board_data)
	{
		if (board_data.size() != 81)
			throw std::exception("Board data size is not 81");

		for (int i = 0; i < 81; i++) {
			board[i] = convert_char(board_data[i]);
			if (board[i] != ZERO)
				defaults[i] = true;
		}
	}

	constexpr SudokuPiece& operator[](Index index)
	{
		return board[index];
	}

	constexpr SudokuPiece operator[](Index index) const
	{
		return board[index];
	}

	constexpr bool is_solved() const
	{
		for (int i = 0; i < 9; i++) {
			uint16_t row = 0;
			uint16_t col = 0;
			for (int j = 0; j < 9; j++) {
				row |= (*this)[{ i, j }];
				col |= (*this)[{ j, i }];
			}
			if (row != _ALL || col != _ALL)
				return false;
		}
		for (int i : { 0, 3, 6, 27, 30, 33, 54, 57, 60 }) {
			const uint16_t block =
				(*this)[i + 0] |
				(*this)[i + 1] |
				(*this)[i + 2] |
				(*this)[i + 9] |
				(*this)[i + 10] |
				(*this)[i + 11] |
				(*this)[i + 18] |
				(*this)[i + 19] |
				(*this)[i + 20];
			if (block != _ALL)
				return false;
		}
		return true;
	}

	constexpr Possible get_possible(Index index) const
	{
		Possible possible{};
		for (SudokuPiece p = ONE; p <= NINE; p = SudokuPiece(p << 1)) {
			const int x = index.x();
			const int y = index.y();
			bool found_same = false;
			for (int i = 0; i < 9; i++) {
				if (p & (*this)[{ i, y }] || p & (*this)[{ x, i }]) {
					found_same = true;
					break;
				}
			}
			if (found_same)
				continue;

			const int sqr_index = Index{ (x / 3) * 3, (y / 3) * 3 };
			found_same =
				p & (*this)[sqr_index + 0] ||
				p & (*this)[sqr_index + 1] ||
				p & (*this)[sqr_index + 2] ||
				p & (*this)[sqr_index + 9] ||
				p & (*this)[sqr_index + 10] ||
				p & (*this)[sqr_index + 11] ||
				p & (*this)[sqr_index + 18] ||
				p & (*this)[sqr_index + 19] ||
				p & (*this)[sqr_index + 20];
			if (found_same)
				continue;

			possible.insert(p);
		}
		return possible;
	}
};

float display_wait_time();

static constexpr std::optional<Sudoku> solve(Sudoku sudoku, Sudoku& displayer, int depth = 0)
{
	while (true) {
		int solve_count = 0;
		for (int i = 0; i < 81; i++) {
			if (sudoku[{i}] != ZERO)
				continue;

			const auto possible = sudoku.get_possible(i);
			if (possible.size() == 0)
				return std::nullopt;

			if (possible.size() == 1) {
				sudoku[{i}] = possible.front();
				solve_count += 1;

				displayer = sudoku;
				kl::time::wait(display_wait_time());
			}
		}
		if (solve_count == 0)
			break;
	}
	for (int i = 0; i < 81; i++) {
		if (sudoku[{i}] != ZERO)
			continue;

		bool some_worked = false;
		for (SudokuPiece possible : sudoku.get_possible(i)) {
			Sudoku possible_sudoku = sudoku;
			possible_sudoku[{i}] = possible;

			kl::print("Guessing ", convert_piece(possible), " at index ", Index(i), " and depth ", depth);
			displayer = possible_sudoku;
			kl::time::wait(display_wait_time());

			if (auto opt_solved = solve(possible_sudoku, displayer, depth + 1)) {
				some_worked = true;
				if (opt_solved->is_solved())
					return opt_solved;
			}
		}
		if (!some_worked)
			return std::nullopt;
	}
	return { sudoku };
}

struct SudokuApp
{
	kl::Window window{ "Sudoku Solver" };
	kl::GPU gpu{ window };

	SudokuApp()
	{
		window.set_dark_mode(true);
		window.on_resize.emplace_back([&](kl::Int2 size)
		{
			gpu.resize_internal(size);
			gpu.set_viewport_size(size);
		});
		window.resize({ 800, 800 });

		gpu.bind_blend_state(gpu.create_blend_state(true));
		gpu.bind_shaders(gpu.create_shaders(R"(
struct VS_DATA
{
	float4 position : SV_Position;
	float4 color : VS_Color;
};

VS_DATA v_shader(
	const float3 position : KL_Position,
	const float3 color : KL_Normal,
	const float2 uv : KL_UV
)
{
	VS_DATA data;
	data.position = float4(position, 1.0f);
	data.color = float4(color, uv.x);
	return data;
}

float4 p_shader(const VS_DATA data) : SV_Target
{
	return data.color;
}
)"));

		std::vector<kl::Vertex> vertex_data;
		for (int i = 0; i < 4; i++) {
			float coord = -1 + i * (2.0f / 3.0f);
			for (float j : { -1.0f, 1.0f })
				vertex_data.emplace_back(kl::Float3{ coord, j, 0.0f }, kl::Float3{ kl::colors::WHITE }, kl::Float2{ 0.9f });
			for (float j : { -1.0f, 1.0f })
				vertex_data.emplace_back(kl::Float3{ j, coord, 0.0f }, kl::Float3{ kl::colors::WHITE }, kl::Float2{ 0.9f });
		}
		for (int i = 0; i < 10; i++) {
			float coord = -1 + i * (2.0f / 9.0f);
			for (float j : { -1.0f, 1.0f })
				vertex_data.emplace_back(kl::Float3{ coord, j, 0.0f }, kl::Float3{ kl::colors::WHITE }, kl::Float2{ 0.3f });
			for (float j : { -1.0f, 1.0f })
				vertex_data.emplace_back(kl::Float3{ j, coord, 0.0f }, kl::Float3{ kl::colors::WHITE }, kl::Float2{ 0.3f });
		}
		m_mesh = gpu.create_vertex_buffer(vertex_data);
	}

	bool update()
	{
		gpu.clear_internal(kl::colors::GRAY);
		gpu.draw(m_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		gpu.draw_text();
		gpu.swap_buffers(true);
		return window.process();
	}

	void reload(const Sudoku& sudoku)
	{
		const float font_size = (window.height() / 9.0f);
		const kl::Float2 rect_size = window.size() / 9;

		const float font_dips = window.pixels_to_dips(font_size);
		kl::TextFormat format = gpu.create_text_format(L"JetBrains Mono NL", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, font_dips);
		format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		gpu.text_data.clear();
		for (int i = 0; i < 81; i++) {
			if (sudoku.board[i] == ZERO)
				continue;

			kl::Text text;
			text.format = format;
			text.color = sudoku.defaults[i] ? kl::colors::CYAN : kl::colors::WHEAT;
			text.position = { rect_size.x * (i % 9), rect_size.y * (i / 9) };
			text.rect_size = rect_size;
			text.data = std::wstring(1, convert_piece(sudoku.board[i]));
			gpu.text_data.push_back(text);
		}
	}

private:
	kl::dx::Buffer m_mesh;
};

float display_wait_time()
{
	return 0.0025f;
}

int examples::sudoku_solver_main(int argc, const char** argv)
{
	Sudoku sudoku
	{
		//"9  "" 63""   "
		//" 6 ""   "" 7 "
		//"  8""   ""9  "
		//"4  ""   ""  3"
		//" 5 ""  7""   "
		//"2  "" 8 ""  1"
		//"  9""   ""8 5"
		//" 7 ""   "" 6 "
		//"   ""3 4""   "

		//"900""063""000"
		//"001""200""500"
		//"702""000""030"
		//"000""306""050"
		//"003""407""600"
		//"070""508""000"
		//"080""000""105"
		//"007""001""200"
		//"000""840""009"

		"000""406""000"
		"012""030""500"
		"000""000""070"
		"800""004""007"
		"060""700""050"
		"500""080""009"
		"003""000""020"
		"000""090""010"
		"000""305""000"
	};
	std::thread thread{ [&sudoku]
	{
		SudokuApp app;
		while (app.update()) {
			app.reload(sudoku);
		}
	} };

	if (auto solved_opt = solve(sudoku, sudoku)) {
		sudoku = solved_opt.value();
		kl::print("Done");
	}
	else {
		kl::print("No solution found");
	}

	if (thread.joinable()) {
		thread.join();
	}
	return 0;
}

