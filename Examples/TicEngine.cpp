#include "KrimzLib.h"


enum ID {
	Player = -1,
	Empty = 0,
	Engine = 1
};

static const kl::Int2 Sizes = { 3, 900 };
static const kl::Color COLOR_PLAYER = kl::Colors::Orange;
static const kl::Color COLOR_ENGINE = kl::Colors::Green;

struct BoardInfo {
	int eval = 0;
	int move = -1;
};

static bool HasEmpty(const kl::Vector<int>& board) {
	for (auto& piece : board) {
		if (piece == ID::Empty) {
			return true;
		}
	}
	return false;
}

static int Evaluate(const kl::Vector<int>& board) {
	for (int y = 0; y < Sizes.x; y++) {
		int sum = 0;
		for (int x = 0; x < Sizes.x; x++) {
			sum += board[kl::uint64(y) * Sizes.x + x];
		}

		if (sum == ID::Player * Sizes.x) {
			return ID::Player;
		}
		if (sum == ID::Engine * Sizes.x) {
			return ID::Engine;
		}
	}

	for (int x = 0; x < Sizes.x; x++) {
		int sum = 0;
		for (int y = 0; y < Sizes.x; y++) {
			sum += board[kl::uint64(y) * Sizes.x + x];
		}

		if (sum == ID::Player * Sizes.x) {
			return ID::Player;
		}
		if (sum == ID::Engine * Sizes.x) {
			return ID::Engine;
		}
	}

	{
		int sum = 0;
		for (int i = 0; i < Sizes.x; i++) {
			sum += board[kl::uint64(i) * Sizes.x + i];
		}

		if (sum == ID::Player * Sizes.x) {
			return ID::Player;
		}
		if (sum == ID::Engine * Sizes.x) {
			return ID::Engine;
		}
	}

	{
		int sum = 0;
		for (int i = 0; i < Sizes.x; i++) {
			sum += board[kl::uint64(i) * Sizes.x + (Sizes.x - 1 - i)];
		}

		if (sum == ID::Player * Sizes.x) {
			return ID::Player;
		}
		if (sum == ID::Engine * Sizes.x) {
			return ID::Engine;
		}
	}

	return ID::Empty;
}

static BoardInfo FindBest(const kl::Vector<int>& board, bool playersTurn, int depth, int alpha, int beta) {
	const int currEval = Evaluate(board);
	if (currEval != ID::Empty || !HasEmpty(board)) {
		return BoardInfo(currEval, -1);
	}

	if (!playersTurn) {
		BoardInfo maxInfo(-1, -1);
		for (int i = 0; i < board.size(); i++) {
			if (board[i] == ID::Empty) {
				if (maxInfo.move == -1) {
					maxInfo.move = i;
				}

				kl::Vector<int> futureBoard = board;
				futureBoard[i] = ID::Engine;

				const int futureEval = FindBest(futureBoard, true, depth + 1, alpha, beta).eval;
				if (futureEval > maxInfo.eval) {
					maxInfo.eval = futureEval;
					maxInfo.move = i;
				}

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
			if (board[i] == ID::Empty) {
				if (minInfo.move == -1) {
					minInfo.move = i;
				}

				kl::Vector<int> futureBoard = board;
				futureBoard[i] = ID::Player;

				const int futureEval = FindBest(futureBoard, false, depth + 1, alpha, beta).eval;
				if (futureEval < minInfo.eval) {
					minInfo.eval = futureEval;
					minInfo.move = i;
				}

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
	kl::Vector<int> board(Sizes.x * Sizes.x);
	bool playersTurn = kl::Random::BOOL();
	const bool playerWasFirst = playersTurn;

	const int squareSize = Sizes.y / Sizes.x;
	const int lineOffset = squareSize / 10;
	const int circlOffset = squareSize / 2;

	kl::Window window = { { Sizes.y, Sizes.y }, "Tic Engine" };
	kl::Image frame = { window.getSize(), kl::Colors::Gray };

	while (window.process(false)) {
		const int eval = Evaluate(board);
		if (eval) {
			window.setTitle((eval == ID::Player) ? "Player wins!" : "Engine wins!");
			while (window.process());
			break;
		}
		else if (!HasEmpty(board)) {
			window.setTitle("Draw!");
			while (window.process());
			break;
		}

		if (playersTurn) {
			static bool wasDown = false;
			if (window.mouse.left) {
				if (!wasDown) {
					const kl::Int2 pos = window.mouse.getPosition() / squareSize;
					const int ind = pos.y * Sizes.x + pos.x;
					if (board[ind] == ID::Empty) {
						board[ind] = ID::Player;
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
			board[FindBest(board, false, 0, ID::Player, ID::Engine).move] = ID::Engine;
			playersTurn = true;
		}

		frame.fill(kl::Colors::Gray);

		for (kl::Int2 pos; pos.y < Sizes.y; pos.y += squareSize) {
			for (pos.x = 0; pos.x < Sizes.y; pos.x += squareSize) {
				frame.drawRectangle(pos, pos + kl::Int2(squareSize, squareSize), kl::Colors::White);
			}
		}

		for (kl::Int2 pos; pos.y < Sizes.x; pos.y++) {
			for (pos.x = 0; pos.x < Sizes.x; pos.x++) {
				const int posID = board[kl::uint64(pos.y) * Sizes.x + pos.x];
				if (playerWasFirst) {
					if (posID == ID::Player) {
						frame.drawLine(pos * squareSize + kl::Int2(lineOffset, lineOffset),
							(pos + kl::Int2(1, 1)) * squareSize - kl::Int2(lineOffset, lineOffset), COLOR_PLAYER);
						frame.drawLine((pos + kl::Int2(0, 1)) * squareSize + kl::Int2(lineOffset, -lineOffset),
							(pos + kl::Int2(1, 0)) * squareSize + kl::Int2(-lineOffset, lineOffset), COLOR_PLAYER);
					}
					else if (posID == ID::Engine) {
						frame.drawCircle(pos * squareSize + kl::Int2(circlOffset, circlOffset),
							pos * squareSize + kl::Int2(circlOffset, lineOffset), COLOR_ENGINE);
					}
				}
				else {
					if (posID == ID::Engine) {
						frame.drawLine(pos * squareSize + kl::Int2(lineOffset, lineOffset),
							(pos + kl::Int2(1, 1)) * squareSize - kl::Int2(lineOffset, lineOffset), COLOR_ENGINE);
						frame.drawLine((pos + kl::Int2(0, 1)) * squareSize + kl::Int2(lineOffset, -lineOffset),
							(pos + kl::Int2(1, 0)) * squareSize + kl::Int2(-lineOffset, lineOffset), COLOR_ENGINE);
					}
					else if (posID == ID::Player) {
						frame.drawCircle(pos * squareSize + kl::Int2(circlOffset, circlOffset),
							pos * squareSize + kl::Int2(circlOffset, lineOffset), COLOR_PLAYER);
					}
				}
			}
		}

		window.drawImage(frame);
	}
}
