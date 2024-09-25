#include "examples.h"


enum ID : int32_t
{
    PLAYER = -1,
    EMPTY = 0,
    ENGINE = 1
};

struct BoardInfo
{
    int eval = 0;
    int move = -1;
};

static constexpr kl::Int2 SIZES = { 3, 901 };
static const kl::Color PLAYER_COLOR = kl::colors::ORANGE;
static const kl::Color ENGINE_COLOR = kl::colors::GREEN;

static bool has_empty(const std::vector<int>& board)
{
    for (auto& piece : board) {
        if (piece == ID::EMPTY) {
            return true;
        }
    }
    return false;
}

static int evaluate(const std::vector<int>& board)
{
    for (int y = 0; y < SIZES.x; y++) {
        int sum = 0;
        for (int x = 0; x < SIZES.x; x++) {
            sum += board[y * SIZES.x + x];
        }

        if (sum == ID::PLAYER * SIZES.x) {
            return ID::PLAYER;
        }
        if (sum == ID::ENGINE * SIZES.x) {
            return ID::ENGINE;
        }
    }

    for (int x = 0; x < SIZES.x; x++) {
        int sum = 0;
        for (int y = 0; y < SIZES.x; y++) {
            sum += board[y * SIZES.x + x];
        }

        if (sum == ID::PLAYER * SIZES.x) {
            return ID::PLAYER;
        }
        if (sum == ID::ENGINE * SIZES.x) {
            return ID::ENGINE;
        }
    }

    {
        int sum = 0;
        for (int i = 0; i < SIZES.x; i++) {
            sum += board[i * SIZES.x + i];
        }

        if (sum == ID::PLAYER * SIZES.x) {
            return ID::PLAYER;
        }
        if (sum == ID::ENGINE * SIZES.x) {
            return ID::ENGINE;
        }
    }

    {
        int sum = 0;
        for (int i = 0; i < SIZES.x; i++) {
            sum += board[i * SIZES.x + (SIZES.x - 1 - i)];
        }

        if (sum == ID::PLAYER * SIZES.x) {
            return ID::PLAYER;
        }
        if (sum == ID::ENGINE * SIZES.x) {
            return ID::ENGINE;
        }
    }

    return ID::EMPTY;
}

static BoardInfo find_best(const std::vector<int>& board, const bool playersTurn, const int depth, int alpha, int beta)
{
    if (const int current_eval = evaluate(board); current_eval != ID::EMPTY || !has_empty(board)) {
        return BoardInfo(current_eval, -1);
    }

    if (!playersTurn) {
        BoardInfo max_info(-1, -1);
        for (int i = 0; i < (int) board.size(); i++) {
            if (board[i] == ID::EMPTY) {
                if (max_info.move == -1) {
                    max_info.move = i;
                }

                std::vector<int> future_board = board;
                future_board[i] = ID::ENGINE;

                const int future_eval = find_best(future_board, true, depth + 1, alpha, beta).eval;
                if (future_eval > max_info.eval) {
                    max_info.eval = future_eval;
                    max_info.move = i;
                }

                alpha = std::max(alpha, future_eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return max_info;
    }

    BoardInfo min_info(1, -1);
    for (int i = 0; i < (int) board.size(); i++) {
        if (board[i] == ID::EMPTY) {
            if (min_info.move == -1) {
                min_info.move = i;
            }

            std::vector<int> future_board = board;
            future_board[i] = ID::PLAYER;

            const int future_eval = find_best(future_board, false, depth + 1, alpha, beta).eval;
            if (future_eval < min_info.eval) {
                min_info.eval = future_eval;
                min_info.move = i;
            }

            beta = std::min(beta, future_eval);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return min_info;
}

int examples::tic_tac_main(const int argc, const char** argv)
{
    std::vector<int> board(SIZES.x * SIZES.x);
    bool players_turn = kl::random::gen_bool();
    const bool player_was_first = players_turn;

    const int square_size = SIZES.y / SIZES.x;
    const int line_offset = square_size / 10;
    const int circle_offset = square_size / 2;

    kl::Window window{ "Tic Engine" };
    kl::Image frame;

    window.resize({ SIZES.y, SIZES.y });
    frame.resize(window.size());

    window.set_resizeable(false);

    while (window.process()) {
        if (const int eval = evaluate(board)) {
            window.set_title((eval == ID::PLAYER) ? "Player wins!" : "Engine wins!");
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
                    const kl::Int2 pos = window.mouse.position() / square_size;
                    if (const int index = pos.y * SIZES.x + pos.x; board[index] == ID::EMPTY) {
                        board[index] = ID::PLAYER;
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
            board[find_best(board, false, 0, ID::PLAYER, ID::ENGINE).move] = ID::ENGINE;
            players_turn = true;
        }

        frame.fill(kl::colors::GRAY);

        for (kl::Int2 pos; pos.y < SIZES.y; pos.y += square_size) {
            for (pos.x = 0; pos.x < SIZES.y; pos.x += square_size) {
                frame.draw_rectangle(pos, pos + kl::Int2(square_size), kl::colors::WHITE);
            }
        }

        for (kl::Int2 pos; pos.y < SIZES.x; pos.y++) {
            for (pos.x = 0; pos.x < SIZES.x; pos.x++) {
                const int pos_id = board[pos.y * SIZES.x + pos.x];

                if (player_was_first) {
                    if (pos_id == ID::PLAYER) {
                        frame.draw_line(pos * square_size + kl::Int2(line_offset),
                            (pos + kl::Int2(1, 1)) * square_size - kl::Int2(line_offset), PLAYER_COLOR);
                        frame.draw_line((pos + kl::Int2(0, 1)) * square_size + kl::Int2(line_offset, -line_offset),
                            (pos + kl::Int2(1, 0)) * square_size + kl::Int2(-line_offset, line_offset), PLAYER_COLOR);
                    }
                    else if (pos_id == ID::ENGINE) {
                        frame.draw_circle(pos * square_size + kl::Int2(circle_offset),
                            pos * square_size + kl::Int2(circle_offset, line_offset), ENGINE_COLOR);
                    }
                }
                else {
                    if (pos_id == ID::ENGINE) {
                        frame.draw_line(pos * square_size + kl::Int2(line_offset),
                            (pos + kl::Int2(1, 1)) * square_size - kl::Int2(line_offset), ENGINE_COLOR);
                        frame.draw_line((pos + kl::Int2(0, 1)) * square_size + kl::Int2(line_offset, -line_offset),
                            (pos + kl::Int2(1, 0)) * square_size + kl::Int2(-line_offset, line_offset), ENGINE_COLOR);
                    }
                    else if (pos_id == ID::PLAYER) {
                        frame.draw_circle(pos * square_size + kl::Int2(circle_offset),
                            pos * square_size + kl::Int2(circle_offset, line_offset), PLAYER_COLOR);
                    }
                }
            }
        }

        window.draw_image(frame);
    }
    return 0;
}
