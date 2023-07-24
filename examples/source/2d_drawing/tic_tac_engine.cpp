#include "examples.h"


enum id
{
    player = -1,
    empty = 0,
    engine = 1
};

struct board_info
{
    int eval = 0;
    int move = -1;
};

static const kl::int2 sizes = { 3, 901 };
static const kl::color player_color = kl::colors::orange;
static const kl::color engine_color = kl::colors::green;

static bool has_empty(const std::vector<int>& board)
{
    for (auto& piece : board) {
        if (piece == id::empty) {
            return true;
        }
    }
    return false;
}

static int evaluate(const std::vector<int>& board)
{
    for (int y = 0; y < sizes.x; y++) {
        int sum = 0;
        for (int x = 0; x < sizes.x; x++) {
            sum += board[y * sizes.x + x];
        }

        if (sum == id::player * sizes.x) {
            return id::player;
        }
        if (sum == id::engine * sizes.x) {
            return id::engine;
        }
    }

    for (int x = 0; x < sizes.x; x++) {
        int sum = 0;
        for (int y = 0; y < sizes.x; y++) {
            sum += board[y * sizes.x + x];
        }

        if (sum == id::player * sizes.x) {
            return id::player;
        }
        if (sum == id::engine * sizes.x) {
            return id::engine;
        }
    }

    {
        int sum = 0;
        for (int i = 0; i < sizes.x; i++) {
            sum += board[i * sizes.x + i];
        }

        if (sum == id::player * sizes.x) {
            return id::player;
        }
        if (sum == id::engine * sizes.x) {
            return id::engine;
        }
    }

    {
        int sum = 0;
        for (int i = 0; i < sizes.x; i++) {
            sum += board[i * sizes.x + (sizes.x - 1 - i)];
        }

        if (sum == id::player * sizes.x) {
            return id::player;
        }
        if (sum == id::engine * sizes.x) {
            return id::engine;
        }
    }

    return id::empty;
}

static board_info find_best(const std::vector<int>& board, const bool playersTurn, const int depth, int alpha, int beta)
{
    if (const int current_eval = evaluate(board); current_eval != id::empty || !has_empty(board)) {
        return board_info(current_eval, -1);
    }

    if (!playersTurn) {
        board_info max_info(-1, -1);
        for (int i = 0; i < int(board.size()); i++) {
            if (board[i] == id::empty) {
                if (max_info.move == -1) {
                    max_info.move = i;
                }

                std::vector<int> future_board = board;
                future_board[i] = id::engine;

                const int future_eval = find_best(future_board, true, depth + 1, alpha, beta).eval;
                if (future_eval > max_info.eval) {
                    max_info.eval = future_eval;
                    max_info.move = i;
                }

                alpha = max(alpha, future_eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return max_info;
    }

    board_info min_info(1, -1);
    for (int i = 0; i < int(board.size()); i++) {
        if (board[i] == id::empty) {
            if (min_info.move == -1) {
                min_info.move = i;
            }

            std::vector<int> future_board = board;
            future_board[i] = id::player;

            const int future_eval = find_best(future_board, false, depth + 1, alpha, beta).eval;
            if (future_eval < min_info.eval) {
                min_info.eval = future_eval;
                min_info.move = i;
            }

            beta = min(beta, future_eval);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return min_info;
}

int examples::tic_tac_main()
{
    std::vector<int> board(sizes.x * sizes.x);
    bool players_turn = kl::random::gen_bool();
    const bool player_was_first = players_turn;

    const int square_size = sizes.y / sizes.x;
    const int line_offset = square_size / 10;
    const int circle_offset = square_size / 2;

    auto window = kl::window({ sizes.y, sizes.y }, "Tic Engine");
    kl::image frame = { window.size() };

    window.set_resizeable(false);

    while (window.process(false)) {
        if (const int eval = evaluate(board)) {
            window.set_title((eval == id::player) ? "Player wins!" : "Engine wins!");
            while (window.process());
            break;
        }

        if (!has_empty(board)) {
            window.set_title("Draw!");
            while (window.process());
            break;
        }

        if (players_turn) {
            static bool was_down = false;
            if (window.mouse.left) {
                if (!was_down) {
                    const kl::int2 pos = window.mouse.position() / square_size;
                    if (const int index = pos.y * sizes.x + pos.x; board[index] == id::empty) {
                        board[index] = id::player;
                        players_turn = false;
                    }
                }
                was_down = true;
            }
            else {
                was_down = false;
            }
        }
        else {
            board[find_best(board, false, 0, id::player, id::engine).move] = id::engine;
            players_turn = true;
        }

        frame.fill(kl::colors::gray);

        for (kl::int2 pos; pos.y < sizes.y; pos.y += square_size) {
            for (pos.x = 0; pos.x < sizes.y; pos.x += square_size) {
                frame.draw_rectangle(pos, pos + kl::int2(square_size), kl::colors::white);
            }
        }

        for (kl::int2 pos; pos.y < sizes.x; pos.y++) {
            for (pos.x = 0; pos.x < sizes.x; pos.x++) {
                const int pos_id = board[pos.y * sizes.x + pos.x];

                if (player_was_first) {
                    if (pos_id == id::player) {
                        frame.draw_line(pos * square_size + kl::int2(line_offset),
                            (pos + kl::int2(1, 1)) * square_size - kl::int2(line_offset), player_color);
                        frame.draw_line((pos + kl::int2(0, 1)) * square_size + kl::int2(line_offset, -line_offset),
                            (pos + kl::int2(1, 0)) * square_size + kl::int2(-line_offset, line_offset), player_color);
                    }
                    else if (pos_id == id::engine) {
                        frame.draw_circle(pos * square_size + kl::int2(circle_offset),
                            pos * square_size + kl::int2(circle_offset, line_offset), engine_color);
                    }
                }
                else {
                    if (pos_id == id::engine) {
                        frame.draw_line(pos * square_size + kl::int2(line_offset),
                            (pos + kl::int2(1, 1)) * square_size - kl::int2(line_offset), engine_color);
                        frame.draw_line((pos + kl::int2(0, 1)) * square_size + kl::int2(line_offset, -line_offset),
                            (pos + kl::int2(1, 0)) * square_size + kl::int2(-line_offset, line_offset), engine_color);
                    }
                    else if (pos_id == id::player) {
                        frame.draw_circle(pos * square_size + kl::int2(circle_offset),
                            pos * square_size + kl::int2(circle_offset, line_offset), player_color);
                    }
                }
            }
        }

        window.draw_image(frame);
    }
    return 0;
}
