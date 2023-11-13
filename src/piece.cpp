#include <vector>
#include <utility>

class Piece {
public:
  char symbol;
  bool white;
  Piece(bool w, char s): white(w), symbol(s) {}
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

class Rook: public Piece {
public:
  Rook(bool w): Piece(w, w?'R':'r') {}

  std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) {
    std::vector<std::pair<int,int>> up,down,left,right;

    for(int i = pos.first-1; i >= 0; i--) up.push_back(std::pair(i, pos.second));
    for(int i = pos.first+1; i < 8; i++) down.push_back(std::pair(i, pos.second));
    for(int i = pos.second-1; i >= 0; i--) left.push_back(std::pair(pos.first, i));
    for(int i = pos.second+1; i < 8; i++) right.push_back(std::pair(pos.first, i));

    return {up,down,left,right};
  }
};

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

class King: public Piece {
public:
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

class Queen: public Piece {
public:
  Queen(bool w): Piece(w, w?'Q':'q') {}

  std::vector<std::vector<std::pair<int,int>>> getRange(std::pair<int,int> pos) {
    std::vector<std::pair<int,int>> up,down,left,right;

    for(int i = 0; i < pos.first; i++) up.push_back(std::pair(i, pos.second));
    for(int i = pos.first+1; i < 8; i++) down.push_back(std::pair(i, pos.second));
    for(int i = 0; i < pos.second; i++) left.push_back(std::pair(pos.first, i));
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

class Pawn: public Piece {
private:
  bool moved = false;
public:
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
        this->moved = true;
        return {{std::pair(pos.first-1, pos.second), std::pair(pos.first-2, pos.second)}};
      }
    } else {
      if(moved) {
        return {{std::pair(pos.first+1, pos.second)}};
      } else {
        this->moved = true;
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
