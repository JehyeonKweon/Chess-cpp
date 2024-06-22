#include <iostream>
#include <vector>
#include <algorithm>

enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
enum Color { WHITE, BLACK, NONE };

struct Piece {
    PieceType type;
    Color color;
};

struct Position {
    int x, y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};
class Board {
    public:
        Board() {
            board.resize(8, std::vector<Piece>(8, {EMPTY, NONE}));
            setupBoard();
        }

        Piece getPieceAt(Position pos) const {
            return board[pos.x][pos.y];
        }

        void setPieceAt(Position pos, Piece piece) {
            board[pos.x][pos.y] = piece;
        }

        void displayBoard() const {
            // Column labels
            std::cout << "  a b c d e f g h" << std::endl;
            std::cout << " +----------------+" << std::endl;
            
            for (int i = 7; i >= 0; --i) {
                // Row labels
                std::cout << (i + 1) << "|";

                for (int j = 0; j < 8; ++j) {
                    std::cout << pieceToChar(board[i][j]) << " ";
                }
                std::cout << "|" << (i + 1) << std::endl;
            }

            std::cout << " +----------------+" << std::endl;
            // Column labels
            std::cout << "  a b c d e f g h" << std::endl;
        }

        bool enermyKingOnBoard(Color turn) const {
            Color check = (turn == WHITE) ? BLACK : WHITE;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    Position pos = {i, j};
                    Piece piece = getPieceAt(pos);
                    if (piece.type == KING && piece.color == check) {
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        std::vector<std::vector<Piece>> board;

        char pieceToChar(Piece piece) const {
            if (piece.color == WHITE) {
                switch (piece.type) {
                    case KING: return 'K';
                    case QUEEN: return 'Q';
                    case ROOK: return 'R';
                    case BISHOP: return 'B';
                    case KNIGHT: return 'N';
                    case PAWN: return 'P';
                    default: return '-';
                }
            } else if (piece.color == BLACK) {
                switch (piece.type) {
                    case KING: return 'k';
                    case QUEEN: return 'q';
                    case ROOK: return 'r';
                    case BISHOP: return 'b';
                    case KNIGHT: return 'n';
                    case PAWN: return 'p';
                    default: return '-';
                }
            } else {
                return '-';
            }
        }

        void setupBoard() {
            board[0][0] = {ROOK, WHITE};
            board[0][1] = {KNIGHT, WHITE};
            board[0][2] = {BISHOP, WHITE};
            board[0][3] = {QUEEN, WHITE};
            board[0][4] = {KING, WHITE};
            board[0][5] = {BISHOP, WHITE};
            board[0][6] = {KNIGHT, WHITE};
            board[0][7] = {ROOK, WHITE};
            for (int i = 0; i < 8; ++i) {
                board[1][i] = {PAWN, WHITE};
            }

            board[7][0] = {ROOK, BLACK};
            board[7][1] = {KNIGHT, BLACK};
            board[7][2] = {BISHOP, BLACK};
            board[7][3] = {QUEEN, BLACK};
            board[7][4] = {KING, BLACK};
            board[7][5] = {BISHOP, BLACK};
            board[7][6] = {KNIGHT, BLACK};
            board[7][7] = {ROOK, BLACK};
            for (int i = 0; i < 8; ++i) {
                board[6][i] = {PAWN, BLACK};
            }
        }

};

bool isValidPosition(Position pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

bool isOpponentPiece(Piece piece, Color color) {
    return piece.color != NONE && piece.color != color;
}

bool isEmptyOrOpponent(Position pos, const Board& board, Color color) {
    Piece piece = board.getPieceAt(pos);
    return piece.color == NONE || isOpponentPiece(piece, color);
}


std::vector<Position> getKingMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves;
    std::vector<Position> offsets = {{1, 0}, {1, 1}, {1, -1}, {0, 1}, {0, -1}, {-1, 0}, {-1, 1}, {-1, -1}};
    for (Position offset : offsets) {
        Position newPos = {pos.x + offset.x, pos.y + offset.y};
        if (isValidPosition(newPos) && isEmptyOrOpponent(newPos, board, color)) {
            moves.push_back(newPos);
        }
    }
    return moves;
}

std::vector<Position> getRookMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves;
    std::vector<Position> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (Position dir : directions) {
        Position newPos = pos;
        while (true) {
            newPos = {newPos.x + dir.x, newPos.y + dir.y};
            if (!isValidPosition(newPos)) break;
            Piece targetPiece = board.getPieceAt(newPos);
            if (targetPiece.color == NONE) {
                moves.push_back(newPos);
            } else if (isOpponentPiece(targetPiece, color)) {
                moves.push_back(newPos);
                break;
            } else {
                break;
            }
        }
    }
    return moves;
}

std::vector<Position> getBishopMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves;
    std::vector<Position> directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (Position dir : directions) {
        Position newPos = pos;
        while (true) {
            newPos = {newPos.x + dir.x, newPos.y + dir.y};
            if (!isValidPosition(newPos)) break;
            Piece targetPiece = board.getPieceAt(newPos);
            if (targetPiece.color == NONE) {
                moves.push_back(newPos);
            } else if (isOpponentPiece(targetPiece, color)) {
                moves.push_back(newPos);
                break;
            } else {
                break;
            }
        }
    }
    return moves;
}

std::vector<Position> getQueenMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves = getRookMoves(pos, board, color);
    std::vector<Position> diagonalMoves = getBishopMoves(pos, board, color);
    moves.insert(moves.end(), diagonalMoves.begin(), diagonalMoves.end());
    return moves;
}

std::vector<Position> getKnightMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves;
    std::vector<Position> offsets = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    for (Position offset : offsets) {
        Position newPos = {pos.x + offset.x, pos.y + offset.y};
        if (isValidPosition(newPos) && isEmptyOrOpponent(newPos, board, color)) {
            moves.push_back(newPos);
        }
    }
    return moves;
}

std::vector<Position> getPawnMoves(Position pos, const Board& board, Color color) {
    std::vector<Position> moves;
    int direction = (color == WHITE) ? 1 : -1;

    Position forward = {pos.x + direction, pos.y};
    if (isValidPosition(forward) && board.getPieceAt(forward).color == NONE) {
        moves.push_back(forward);
        if ((color == WHITE && pos.x == 1) || (color == BLACK && pos.x == 6)) {
            Position doubleForward = {pos.x + 2 * direction, pos.y};
            if (board.getPieceAt(doubleForward).color == NONE) {
                moves.push_back(doubleForward);
            }
        }
    }

    Position captureLeft = {pos.x + direction, pos.y - 1};
    if (isValidPosition(captureLeft) && isOpponentPiece(board.getPieceAt(captureLeft), color)) {
        moves.push_back(captureLeft);
    }

    Position captureRight = {pos.x + direction, pos.y + 1};
    if (isValidPosition(captureRight) && isOpponentPiece(board.getPieceAt(captureRight), color)) {
        moves.push_back(captureRight);
    }

    return moves;
}

class Game {
    public:
        Game() : board() {}

        void play() {
            board.displayBoard();
            while (true) {
                std::string move;
                if (turn == WHITE) {
                    std::cout << "White turn: enter your move (e.g., e2e4): ";
                    std::cin >> move;
                    }
                else {
                    std::cout << "Black turn: enter your move (e.g., e2e4): ";
                    std::cin >> move;
                }
                if (move == "exit") break;

                Position from = {move[1] - '1', move[0] - 'a'};
                Position to = {move[3] - '1', move[2] - 'a'};
                if (isValidMove(from, to)) {
                    makeMove(from, to);
                    board.displayBoard();

                    if (checkWin(turn)) {
                        break;
                    }
                    else {
                        turn = (turn == WHITE) ? BLACK : WHITE;
                    }
                } 
                else {
                    std::cout << "Invalid move. Try again." << std::endl;
                }
            }
            if (checkWin(turn)) {
                std::cout << (turn == WHITE ? "White Win" : "Black Win") << std::endl;
            }
        }

    private:
        Board board;
        Color turn = WHITE;

        bool isValidMove(Position from, Position to) {
            Piece piece = board.getPieceAt(from);
            if (piece.color != turn) {
                return false;
            }

            std::vector<Position> validMoves;
            switch (piece.type) {
                case KING:
                    validMoves = getKingMoves(from, board, piece.color);
                    break;
                case QUEEN:
                    validMoves = getQueenMoves(from, board, piece.color);
                    break;
                case ROOK:
                    validMoves = getRookMoves(from, board, piece.color);
                    break;
                case BISHOP:
                    validMoves = getBishopMoves(from, board, piece.color);
                    break;
                case KNIGHT:
                    validMoves = getKnightMoves(from, board, piece.color);
                    break;
                case PAWN:
                    validMoves = getPawnMoves(from, board, piece.color);
                    break;
                default:
                    return false;
            }

            return std::find(validMoves.begin(), validMoves.end(), to) != validMoves.end();
        }   

        void makeMove(Position from, Position to) {
            Piece piece = board.getPieceAt(from);
            board.setPieceAt(to, piece);
            board.setPieceAt(from,{EMPTY, NONE});
        }

        bool checkWin(Color turn) {
            if (board.enermyKingOnBoard(turn)) {return false;}
            else {return true;}
        }

    };

int main() {
    Game game;
    game.play();
    return 0;
}
