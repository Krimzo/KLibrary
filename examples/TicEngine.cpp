#include "KrimzLib.h"


const int BOARD_LEN = 3;
const int FRAME_SIZE = 900;

const int ID_PLAYER = -1;
const int ID_EMPTY = 0;
const int ID_ENGINE = 1;

const kl::color COLOR_PLAYER = kl::colors::orange;
const kl::color COLOR_ENGINE = kl::colors::green;

bool HasEmpty(const std::vector<int>& board) {
	for (int i = 0; i < board.size(); i++) {
		if (board[i] == ID_EMPTY) {
			return true;
		}
	}
	return false;
}

int Evaluate(const std::vector<int>& board) {
	// Check rows
	for (int y = 0; y < BOARD_LEN; y++) {
		// Check
		int sum = 0;
		for (int x = 0; x < BOARD_LEN; x++) {
			sum += board[size_t(y * BOARD_LEN + x)];
		}

		// Player wins
		if (sum == ID_PLAYER * BOARD_LEN) {
			return ID_PLAYER;
		}
		// Engine wins
		else if (sum == ID_ENGINE * BOARD_LEN) {
			return ID_ENGINE;
		}
	}

	// Check cols
	for (int x = 0; x < BOARD_LEN; x++) {
		// Check
		int sum = 0;
		for (int y = 0; y < BOARD_LEN; y++) {
			sum += board[size_t(y * BOARD_LEN + x)];
		}

		// Player wins
		if (sum == ID_PLAYER * BOARD_LEN) {
			return ID_PLAYER;
		}
		// Engine wins
		else if (sum == ID_ENGINE * BOARD_LEN) {
			return ID_ENGINE;
		}
	}

	// Main diagonal
	{
		// Check
		int sum = 0;
		for (int i = 0; i < BOARD_LEN; i++) {
			sum += board[size_t(i * BOARD_LEN + i)];
		}

		// X wins
		if (sum == ID_PLAYER * BOARD_LEN) {
			return ID_PLAYER;
		}
		// O wins
		else if (sum == ID_ENGINE * BOARD_LEN) {
			return ID_ENGINE;
		}
	}
	
	// Second diagonal
	{
		// Check
		int sum = 0;
		for (int i = 0; i < BOARD_LEN; i++) {
			sum += board[size_t(i * BOARD_LEN + (BOARD_LEN - 1 - i))];
		}

		// X wins
		if (sum == ID_PLAYER * BOARD_LEN) {
			return ID_PLAYER;
		}
		// O wins
		else if (sum == ID_ENGINE * BOARD_LEN) {
			return ID_ENGINE;
		}
	}

	// Draw
	return ID_EMPTY;
}

struct BoardInfo {
	int eval = 0;
	int move = -1;

	BoardInfo() {
		eval = 0;
		move = -1;
	}
	BoardInfo(int eval, int move) {
		this->eval = eval;
		this->move = move;
	}
};

BoardInfo FindBest(const std::vector<int>& board, bool playersTurn, int depth, int alpha, int beta) {
	// Game finished check
	const int currEval = Evaluate(board);
	if (currEval != ID_EMPTY || !HasEmpty(board)) {
		return BoardInfo(currEval, -1);
	}

	// Turn select
	if (!playersTurn) {
		BoardInfo maxInfo(-1, -1);
		for (int i = 0; i < board.size(); i++) {
			if (board[i] == ID_EMPTY) {
				// First move setup
				if (maxInfo.move == -1) {
					maxInfo.move = i;
				}

				// Copy board and play the move
				std::vector<int> futureBoard = board;
				futureBoard[i] = ID_ENGINE;

				// Find best move for future board
				const int futureEval = FindBest(futureBoard, true, depth + 1, alpha, beta).eval;
				if (futureEval > maxInfo.eval) {
					maxInfo.eval = futureEval;
					maxInfo.move = i;
				}

				// Alpha beta pruning
				alpha = max(alpha, futureEval);
				if (beta <= alpha) {
					break;
				}
			}
		}
		return maxInfo;
	}
	else {
		BoardInfo minInfo(1, -1);
		for (int i = 0; i < board.size(); i++) {
			if (board[i] == ID_EMPTY) {
				// First move setup
				if (minInfo.move == -1) {
					minInfo.move = i;
				}

				// Copy board and play the move
				std::vector<int> futureBoard = board;
				futureBoard[i] = ID_PLAYER;

				// Find best move for future board
				const int futureEval = FindBest(futureBoard, false, depth + 1, alpha, beta).eval;
				if (futureEval < minInfo.eval) {
					minInfo.eval = futureEval;
					minInfo.move = i;
				}

				// Alpha beat pruning
				beta = min(beta, futureEval);
				if (beta <= alpha) {
					break;
				}
			}
		}
		return minInfo;
	}
}

int main() {
	kl::console::hide();

	std::vector<int> board(BOARD_LEN * BOARD_LEN);
	bool playersTurn = kl::random::BOOL();

	static const bool playerWasFirst = playersTurn;

	const int squareSize = FRAME_SIZE / BOARD_LEN;
	const int lineOffs = squareSize / 10;
	const int circlOffs = squareSize / 2;

	kl::window win;
	kl::image frame(kl::int2(FRAME_SIZE), kl::colors::gray);

	win.update = [&]() {
		// Eval check
		const int eval = Evaluate(board);
		if (eval) {
			win.setTitle((eval == ID_PLAYER) ? "Player wins!" : "Engine wins!");
			win.update = []() {};
			return;
		}
		else if (!HasEmpty(board)) {
			win.setTitle("Draw!");
			win.update = []() {};
			return;
		}

		// Logic/input
		if (playersTurn) {
			static bool wasDown = false;
			if (win.mouse.lmb) {
				if (!wasDown) {
					const kl::int2 pos = win.mouse.position / kl::int2(squareSize);
					const int ind = pos.y * BOARD_LEN + pos.x;
					if (board[ind] == ID_EMPTY) {
						board[ind] = ID_PLAYER;
						playersTurn = false;
					}
				}
				wasDown = true;
			}
			else {
				wasDown = false;
			}
		}
		else {
			board[FindBest(board, false, 0, ID_PLAYER, ID_ENGINE).move] = ID_ENGINE;
			playersTurn = true;
		}

		// Clearing the image
		frame.fill(kl::colors::gray);

		// Drawing the squares
		for (kl::int2 pos = 0; pos.y < FRAME_SIZE; pos.y += squareSize) {
			for (pos.x = 0; pos.x < FRAME_SIZE; pos.x += squareSize) {
				frame.drawRectangle(pos, pos + kl::int2(squareSize), kl::colors::white);
			}
		}

		// Drawing the data
		for (kl::int2 pos = 0; pos.y < BOARD_LEN; pos.y++) {
			for (pos.x = 0; pos.x < BOARD_LEN; pos.x++) {
				const int posID = board[size_t(pos.y * BOARD_LEN + pos.x)];
				if (playerWasFirst) {
					if (posID == ID_PLAYER) {
						frame.drawLine(pos * squareSize + kl::int2(lineOffs), (pos + 1) * squareSize - kl::int2(lineOffs), COLOR_PLAYER);
						frame.drawLine((pos + kl::int2(0, 1)) * squareSize + kl::int2(lineOffs, -lineOffs), (pos + kl::int2(1, 0)) * squareSize + kl::int2(-lineOffs, lineOffs), COLOR_PLAYER);
					}
					else if (posID == ID_ENGINE) {
						frame.drawCircle(pos * squareSize + kl::int2(circlOffs), pos * squareSize + kl::int2(circlOffs, lineOffs), COLOR_ENGINE);
					}
				}
				else {
					if (posID == ID_ENGINE) {
						frame.drawLine(pos * squareSize + kl::int2(lineOffs), (pos + 1) * squareSize - kl::int2(lineOffs), COLOR_ENGINE);
						frame.drawLine((pos + kl::int2(0, 1)) * squareSize + kl::int2(lineOffs, -lineOffs), (pos + kl::int2(1, 0)) * squareSize + kl::int2(-lineOffs, lineOffs), COLOR_ENGINE);
					}
					else if(posID == ID_PLAYER) {
						frame.drawCircle(pos * squareSize + kl::int2(circlOffs), pos * squareSize + kl::int2(circlOffs, lineOffs), COLOR_PLAYER);
					}
				}
			}
		}

		// Image draw
		win.drawImage(frame);
	};
	
	win.startNew(kl::int2(FRAME_SIZE + 1), "TicEngine", false, false);
}
