#include "KrimzLib.h"


static constexpr int BOARD_SIZE = 3;
static constexpr int FRAME_SIZE = 900;

static constexpr int ID_PLAYER = -1;
static constexpr int ID_EMPTY = 0;
static constexpr int ID_ENGINE = 1;

static const kl::color COLOR_PLAYER = kl::colors::orange;
static const kl::color COLOR_ENGINE = kl::colors::green;

bool HasEmpty(const std::vector<int>& board)
{
	for (auto& piece : board)
	{
		if (piece == ID_EMPTY)
		{
			return true;
		}
	}
	return false;
}

int Evaluate(const std::vector<int>& board)
{
	for (int y = 0; y < BOARD_SIZE; y++)
	{
		int sum = 0;
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			sum += board[uint64(y * BOARD_SIZE + x)];
		}

		if (sum == ID_PLAYER * BOARD_SIZE)
		{
			return ID_PLAYER;
		}
		else if (sum == ID_ENGINE * BOARD_SIZE)
		{
			return ID_ENGINE;
		}
	}

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		int sum = 0;
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			sum += board[uint64(y * BOARD_SIZE + x)];
		}

		if (sum == ID_PLAYER * BOARD_SIZE)
		{
			return ID_PLAYER;
		}
		else if (sum == ID_ENGINE * BOARD_SIZE)
		{
			return ID_ENGINE;
		}
	}

	{
		int sum = 0;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			sum += board[uint64(i * BOARD_SIZE + i)];
		}

		if (sum == ID_PLAYER * BOARD_SIZE)
		{
			return ID_PLAYER;
		}
		else if (sum == ID_ENGINE * BOARD_SIZE)
		{
			return ID_ENGINE;
		}
	}

	{
		int sum = 0;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			sum += board[uint64(i * BOARD_SIZE + (BOARD_SIZE - 1 - i))];
		}

		if (sum == ID_PLAYER * BOARD_SIZE)
		{
			return ID_PLAYER;
		}
		else if (sum == ID_ENGINE * BOARD_SIZE)
		{
			return ID_ENGINE;
		}
	}

	return ID_EMPTY;
}

struct BoardInfo
{
	int eval;
	int move;

	BoardInfo() : eval(0), move(-1)
	{
	}
	BoardInfo(int eval, int move) : eval(eval), move(move)
	{
	}
};

BoardInfo FindBest(const std::vector<int>& board, bool playersTurn, int depth, int alpha, int beta)
{
	const int currEval = Evaluate(board);
	if (currEval != ID_EMPTY || !HasEmpty(board))
	{
		return BoardInfo(currEval, -1);
	}

	if (!playersTurn)
	{
		BoardInfo maxInfo(-1, -1);
		for (int i = 0; i < board.size(); i++)
		{
			if (board[i] == ID_EMPTY)
			{
				if (maxInfo.move == -1)
				{
					maxInfo.move = i;
				}

				std::vector<int> futureBoard = board;
				futureBoard[i] = ID_ENGINE;

				const int futureEval = FindBest(futureBoard, true, depth + 1, alpha, beta).eval;
				if (futureEval > maxInfo.eval)
				{
					maxInfo.eval = futureEval;
					maxInfo.move = i;
				}

				alpha = max(alpha, futureEval);
				if (beta <= alpha)
				{
					break;
				}
			}
		}
		return maxInfo;
	}
	else
	{
		BoardInfo minInfo(1, -1);
		for (int i = 0; i < board.size(); i++)
		{
			if (board[i] == ID_EMPTY)
			{
				if (minInfo.move == -1)
				{
					minInfo.move = i;
				}

				std::vector<int> futureBoard = board;
				futureBoard[i] = ID_PLAYER;

				const int futureEval = FindBest(futureBoard, false, depth + 1, alpha, beta).eval;
				if (futureEval < minInfo.eval)
				{
					minInfo.eval = futureEval;
					minInfo.move = i;
				}

				beta = min(beta, futureEval);
				if (beta <= alpha)
				{
					break;
				}
			}
		}
		return minInfo;
	}
}

int main()
{
	kl::console::hide();

	std::vector<int> board(BOARD_SIZE * BOARD_SIZE);
	bool playersTurn = kl::random::BOOL();

	const bool playerWasFirst = playersTurn;

	const int squareSize = FRAME_SIZE / BOARD_SIZE;
	const int lineOffs = squareSize / 10;
	const int circlOffs = squareSize / 2;

	kl::window window = {};
	kl::image frame({ FRAME_SIZE, FRAME_SIZE }, kl::colors::gray);

	window.update = [&]()
	{
		const int eval = Evaluate(board);
		if (eval)
		{
			window.title((eval == ID_PLAYER) ? "Player wins!" : "Engine wins!");
			window.update = []()
			{};
			return;
		}
		else if (!HasEmpty(board))
		{
			window.title("Draw!");
			window.update = []()
			{};
			return;
		}

		if (playersTurn)
		{
			static bool wasDown = false;
			if (window.mouse.lmb)
			{
				if (!wasDown)
				{
					const kl::int2 pos = window.mouse.position / squareSize;
					const int ind = pos.y * BOARD_SIZE + pos.x;
					if (board[ind] == ID_EMPTY)
					{
						board[ind] = ID_PLAYER;
						playersTurn = false;
					}
				}
				wasDown = true;
			}
			else
			{
				wasDown = false;
			}
		}
		else
		{
			board[FindBest(board, false, 0, ID_PLAYER, ID_ENGINE).move] = ID_ENGINE;
			playersTurn = true;
		}

		frame.fill(kl::colors::gray);

		for (kl::int2 pos; pos.y < FRAME_SIZE; pos.y += squareSize)
		{
			for (pos.x = 0; pos.x < FRAME_SIZE; pos.x += squareSize)
			{
				frame.drawRectangle(pos, pos + kl::int2(squareSize, squareSize), kl::colors::white);
			}
		}

		for (kl::int2 pos; pos.y < BOARD_SIZE; pos.y++)
		{
			for (pos.x = 0; pos.x < BOARD_SIZE; pos.x++)
			{
				const int posID = board[uint64(pos.y * BOARD_SIZE + pos.x)];
				if (playerWasFirst)
				{
					if (posID == ID_PLAYER)
					{
						frame.drawLine(pos * squareSize + kl::int2(lineOffs, lineOffs),
							(pos + kl::int2(1, 1)) * squareSize - kl::int2(lineOffs, lineOffs), COLOR_PLAYER);
						frame.drawLine((pos + kl::int2(0, 1)) * squareSize + kl::int2(lineOffs, -lineOffs),
							(pos + kl::int2(1, 0)) * squareSize + kl::int2(-lineOffs, lineOffs), COLOR_PLAYER);
					}
					else if (posID == ID_ENGINE)
					{
						frame.drawCircle(pos * squareSize + kl::int2(circlOffs, circlOffs),
							pos * squareSize + kl::int2(circlOffs, lineOffs), COLOR_ENGINE);
					}
				}
				else
				{
					if (posID == ID_ENGINE)
					{
						frame.drawLine(pos * squareSize + kl::int2(lineOffs, lineOffs),
							(pos + kl::int2(1, 1)) * squareSize - kl::int2(lineOffs, lineOffs), COLOR_ENGINE);
						frame.drawLine((pos + kl::int2(0, 1)) * squareSize + kl::int2(lineOffs, -lineOffs),
							(pos + kl::int2(1, 0)) * squareSize + kl::int2(-lineOffs, lineOffs), COLOR_ENGINE);
					}
					else if (posID == ID_PLAYER)
					{
						frame.drawCircle(pos * squareSize + kl::int2(circlOffs, circlOffs),
							pos * squareSize + kl::int2(circlOffs, lineOffs), COLOR_PLAYER);
					}
				}
			}
		}

		window.draw(frame);
	};

	window.run({ FRAME_SIZE + 1, FRAME_SIZE + 1 }, "TicEngine", false);
}
