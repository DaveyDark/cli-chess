#include <iostream>
#include "piece.cpp"

class Board {
public:
  // Escape codes constants
  const std::string WHITE_COLOR_CODE = "\033[37m";
  const std::string BLACK_COLOR_CODE = "\033[30m";
  const std::string WHITE_BACKGROUND_COLOR = "\033[47m";
  const std::string BLACK_BACKGROUND_COLOR = "\033[40m";
  const std::string GRAY_TEXT_COLOR = "\033[90m";
  const std::string BACKGROUND_PRIMARY_COLOR = "\033[0;38;2;194;178;128;48;2;194;178;128m";
  const std::string BACKGROUND_SECONDARY_COLOR = "\033[0;38;2;0;128;0;48;2;0;128;0m";
  const std::string BACKGROUND_BORDER_COLOR = "\033[48;2;75;20;5m";
  const std::string CLEAR_SCREEN = "\033c";

  Piece* pieces[8][8];

  Board() {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) pieces[i][j] = nullptr;
    pieces[0][0] = new Rook(false);
    pieces[0][1] = new Knight(false);
    pieces[0][2] = new Bishop(false);
    pieces[0][3] = new Queen(false);
    pieces[0][4] = new King(false);
    pieces[0][5] = new Bishop(false);
    pieces[0][6] = new Knight(false);
    pieces[0][7] = new Rook(false);
    for (int i = 0; i < 8; i++) pieces[1][i] = new Pawn(false);

    pieces[7][0] = new Rook(true);
    pieces[7][1] = new Knight(true);
    pieces[7][2] = new Bishop(true);
    pieces[7][3] = new Queen(true);
    pieces[7][4] = new King(true);
    pieces[7][5] = new Bishop(true);
    pieces[7][6] = new Knight(true);
    pieces[7][7] = new Rook(true);
    for (int i = 0; i < 8; i++) pieces[6][i] = new Pawn(true);
  }

  void display() {
    std::cout << CLEAR_SCREEN << std::endl;
    std::cout << BACKGROUND_BORDER_COLOR << GRAY_TEXT_COLOR << "   a  b  c  d  e  f  g  h   " << std::endl;
    for(int i = 0; i < 8; i++) {
      std::cout << GRAY_TEXT_COLOR << 8 - i << " " ;
      for(int j = 0; j < 8; j++) {
        if((i+j)%2 == 0) std::cout << BACKGROUND_PRIMARY_COLOR;
        else std::cout << BACKGROUND_SECONDARY_COLOR;
        if(pieces[i][j]) {
          if(pieces[i][j]->white) {
            std::cout << WHITE_COLOR_CODE;
          } else {
            std::cout << BLACK_COLOR_CODE;
          }
          std::cout << " " << pieces[i][j]->symbol << " ";
        }
        else std::cout << "   ";
      }
      std::cout << BACKGROUND_BORDER_COLOR;
      std::cout << "  " << std::endl;
    }
    std::cout << "                            \033[0m" << std::endl;
    std::cout << std::endl;
  }

  Piece* move(int s0, int s1, int d0, int d1, bool white) {
    if(!this->pieces[s0][s1]) return nullptr;
    if(this->pieces[s0][s1]->white != white) return nullptr;

    std::vector<std::vector<std::pair<int,int>>> range;
    Piece* ret;

    if(this->pieces[d0][d1]) {
      //capture move
      if(this->pieces[d0][d1]->white == white) return nullptr;
      range = this->pieces[s0][s1]->getCaptureRange({s0,s1});
      ret = this->pieces[d0][d1];
    } else {
      //normal move
      ret = this->pieces[s0][s1];
      range = this->pieces[s0][s1]->getMovementRange({s0,s1});
    }
    for(const auto direction: range) {
      for(auto pos: direction) {
        if(pos.first == d0 && pos.second == d1) {
          this->pieces[d0][d1] = this->pieces[s0][s1];
          this->pieces[s0][s1] = nullptr;
          return ret;
        }
        if(this->pieces[pos.first][pos.second]) break;
      }
    }
    return nullptr;
  }
};
