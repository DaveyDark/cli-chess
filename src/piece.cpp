#include <vector>
#include <utility>

// This class is the base for all pieces on the board. It stores the symbol of the piece and
// the color(weather it is white or not)
// The constructor initializes these values
//
// Also there are virtual functions defined to get the Movement Range and Capturing Range of a piece
// Since for most pieces these are the same, the default implementation of these functions calls another
// helper function getRange() which is pure virtual
//
// Lastly there is a function validateMoves() that takes a range of moves and removes invalid moves from it
class Piece {
public:
  char symbol;
  bool white;
  Piece(bool w, char s): white(w), symbol(s) {}
  virtual ~Piece() = default;
  virtual std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) = 0;
  virtual std::vector<std::vector<std::pair<int,int>>> getMovementRange(std::pair<int,int> pos) {
    return this->getRange(pos);
  }
  virtual std::vector<std::vector<std::pair<int,int>>> getCaptureRange(std::pair<int,int> pos) {
    return this->getRange(pos);
  }
  std::vector<std::vector<std::pair<int, int>>> validateMoves(std::vector<std::pair<int, int>> &moves) {
    std::vector<std::vector<std::pair<int, int>>> validMoves;
    for (const auto &move : moves) {
      if (move.first >= 0 && move.first < 8 && move.second >= 0 && move.second < 8) {
        validMoves.push_back({move});
      }
    }
    return validMoves;
  }
};

// The Rook class additionally keeps track of weather it has moved yet or not
// Rook's implementation of range checks all 4 directions from the given position and adds them to the range
class Rook: public Piece {
public:
  Rook(bool w): Piece(w, w?'R':'r') {}
  bool moved = false;

  std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) {
    std::vector<std::pair<int,int>> up,down,left,right;

    for(int i = pos.first-1; i >= 0; i--) up.push_back(std::pair(i, pos.second));
    for(int i = pos.first+1; i < 8; i++) down.push_back(std::pair(i, pos.second));
    for(int i = pos.second-1; i >= 0; i--) left.push_back(std::pair(pos.first, i));
    for(int i = pos.second+1; i < 8; i++) right.push_back(std::pair(pos.first, i));

    return {up,down,left,right};
  }
};

// Knight's implementation of range checks all of the given coordinates the knight can move to
// and then validates them with validateMoves() and returns them
class Knight: public Piece {
public:
  Knight(bool w): Piece(w, w?'N':'n') {}

  std::vector<std::vector<std::pair<int, int>>> getRange(std::pair<int, int> pos) {
    std::vector<std::pair<int, int>> moves = {
      {pos.first - 2, pos.second - 1}, {pos.first - 2, pos.second + 1},
      {pos.first - 1, pos.second - 2}, {pos.first - 1, pos.second + 2},
      {pos.first + 1, pos.second - 2}, {pos.first + 1, pos.second + 2},
      {pos.first + 2, pos.second - 1}, {pos.first + 2, pos.second + 1}};

    return this->validateMoves(moves);
  }
};

// Bishop's implementation of range checks all of the diagonal directions from the given position
class Bishop: public Piece {
public:
  Bishop(bool w): Piece(w, w?'B':'b') {}
  std::vector<std::vector<std::pair<int, int>>> getRange(std::pair<int, int> pos) {
    std::vector<std::pair<int, int>> upLeft, upRight, downLeft, downRight;

    for (int i = 1; pos.first - i >= 0 && pos.second - i >= 0; i++)
      upLeft.push_back(std::pair(pos.first - i, pos.second - i));
    for (int i = 1; pos.first - i >= 0 && pos.second + i < 8; i++)
      upRight.push_back(std::pair(pos.first - i, pos.second + i));
    for (int i = 1; pos.first + i < 8 && pos.second - i >= 0; i++)
      downLeft.push_back(std::pair(pos.first + i, pos.second - i));
    for (int i = 1; pos.first + i < 8 && pos.second + i < 8; i++)
      downRight.push_back(std::pair(pos.first + i, pos.second + i));

    return {upLeft, upRight, downRight, downLeft};
  }
};

// King's implementation of range checks all of the given coordinates the knight can move to
// and then validates them with validateMoves() and returns them
// It also tracks weather it has moved yet or not
class King: public Piece {
public:
  bool moved = false;
  King(bool w): Piece(w, w?'K':'k') {}
  std::vector<std::vector<std::pair<int, int>>> getRange(std::pair<int, int> pos) {
    std::vector<std::pair<int, int>> moves = {
      {pos.first - 1, pos.second - 1}, {pos.first - 1, pos.second},
      {pos.first - 1, pos.second + 1}, {pos.first, pos.second - 1},
      {pos.first, pos.second + 1},     {pos.first + 1, pos.second - 1},
      {pos.first + 1, pos.second},     {pos.first + 1, pos.second + 1}};

    return this->validateMoves(moves);
  }
};

// Queen's implementation of range checks all 8 directions and returns it
class Queen: public Piece {
public:
  Queen(bool w): Piece(w, w?'Q':'q') {}

  std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) {
    std::vector<std::pair<int,int>> up,down,left,right;

    for(int i = pos.first-1; i >= 0; i--) up.push_back(std::pair(i, pos.second));
    for(int i = pos.first+1; i < 8; i++) down.push_back(std::pair(i, pos.second));
    for(int i = pos.second-1; i >= 0; i--) left.push_back(std::pair(pos.first, i));
    for(int i = pos.second+1; i < 8; i++) right.push_back(std::pair(pos.first, i));

    std::vector<std::pair<int, int>> upLeft, upRight, downLeft, downRight;

    for (int i = 1; pos.first - i >= 0 && pos.second - i >= 0; i++)
      upLeft.push_back(std::pair(pos.first - i, pos.second - i));
    for (int i = 1; pos.first - i >= 0 && pos.second + i < 8; i++)
      upRight.push_back(std::pair(pos.first - i, pos.second + i));
    for (int i = 1; pos.first + i < 8 && pos.second - i >= 0; i++)
      downLeft.push_back(std::pair(pos.first + i, pos.second - i));
    for (int i = 1; pos.first + i < 8 && pos.second + i < 8; i++)
      downRight.push_back(std::pair(pos.first + i, pos.second + i));

    return {upLeft, upRight, downRight, downLeft, up,down,left,right};
  }
};

// Pawn has different capture range and movement range so their functions are different
// If the pawn has moved, it can move 1 square forward for 2 squares forward if it hasn't
// And it can capture diagonally
// Additionally it tracks weather an En Passant is possible on the pawn in current turn
class Pawn: public Piece {
public:
  bool moved = false;
  bool enPassantAble = false;
  Pawn(bool w): Piece(w, w?'P':'p') {}

  std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) {
    return this->getMovementRange(pos);
  }

  std::vector<std::vector<std::pair<int,int>>> getMovementRange(std::pair<int,int> pos) {
    std::vector<std::vector<std::pair<int,int>>> range;
    if(this->white) {
      if(moved) {
        return {{std::pair(pos.first-1, pos.second)}};
      } else {
        return {{std::pair(pos.first-1, pos.second), std::pair(pos.first-2, pos.second)}};
      }
    } else {
      if(moved) {
        return {{std::pair(pos.first+1, pos.second)}};
      } else {
        return {{std::pair(pos.first+1, pos.second), std::pair(pos.first+2, pos.second)}};
      }
    }
  }

  std::vector<std::vector<std::pair<int,int>>> getCaptureRange(std::pair<int,int> pos) {
    if(this->white) {
      std::vector<std::pair<int, int>> moves = 
        {{pos.first - 1, pos.second - 1}, {pos.first - 1, pos.second + 1}};

      return this->validateMoves(moves);
    } else {
      std::vector<std::pair<int, int>> moves = 
        {{pos.first + 1, pos.second - 1}, {pos.first + 1, pos.second + 1}};

      return this->validateMoves(moves);
    }
  }
};
