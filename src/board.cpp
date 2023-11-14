#include <cctype>
#include <iostream>
#include "piece.cpp"

class Board {
private:
  Piece* pieces[8][8];
  bool isChecked(bool white) {
    int king0, king1;
    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(!this->pieces[i][j]) continue;
        if(this->pieces[i][j]->white == white && tolower(this->pieces[i][j]->symbol) == 'k'){
          king0 = i;
          king1 = j;
        }
      }
    }

    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(!this->pieces[i][j]) continue;
        if(this->pieces[i][j]->white != white){
          if(this->isInRange({king0,king1}, this->pieces[i][j]->getCaptureRange({i,j}))) {
            return true;
          }
        }
      }
    }

    return false;
  }

  bool isInRange(std::pair<int,int> p, std::vector<std::vector<std::pair<int,int>>> range) {
    for(const auto direction: range) {
      for(auto pos: direction) {
        if(pos == p) {
          return true;
        }
        if(this->pieces[pos.first][pos.second]) break;
      }
    }
    return false;
  }

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
  const std::string RED_BACKGROUND = "\033[41m";


  Board() {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) this->pieces[i][j] = nullptr;
    this->pieces[0][0] = new Rook(false);
    this->pieces[0][1] = new Knight(false);
    this->pieces[0][2] = new Bishop(false);
    this->pieces[0][3] = new Queen(false);
    this->pieces[0][4] = new King(false);
    this->pieces[0][5] = new Bishop(false);
    this->pieces[0][6] = new Knight(false);
    this->pieces[0][7] = new Rook(false);
    for (int i = 0; i < 8; i++) this->pieces[1][i] = new Pawn(false);

    this->pieces[7][0] = new Rook(true);
    this->pieces[7][1] = new Knight(true);
    this->pieces[7][2] = new Bishop(true);
    this->pieces[7][3] = new Queen(true);
    this->pieces[7][4] = new King(true);
    this->pieces[7][5] = new Bishop(true);
    this->pieces[7][6] = new Knight(true);
    this->pieces[7][7] = new Rook(true);
    for (int i = 0; i < 8; i++) this->pieces[6][i] = new Pawn(true);
  }

  void display() {
    std::cout << CLEAR_SCREEN << std::endl;
    std::cout << BACKGROUND_BORDER_COLOR << GRAY_TEXT_COLOR << "   a  b  c  d  e  f  g  h   " << std::endl;

    for(int i = 0; i < 8; i++) {
      std::cout << GRAY_TEXT_COLOR << 8 - i << " " ;
      for(int j = 0; j < 8; j++) {
        if((i+j)%2 == 0) std::cout << BACKGROUND_PRIMARY_COLOR;
        else std::cout << BACKGROUND_SECONDARY_COLOR;
        if(this->pieces[i][j]) {
          if(tolower(this->pieces[i][j]->symbol) == 'k' && this->isChecked(this->pieces[i][j]->white)){
            std::cout << RED_BACKGROUND;
          } else if(this->pieces[i][j]->white) {
            std::cout << WHITE_COLOR_CODE;
          } else {
            std::cout << BLACK_COLOR_CODE;
          }
          std::cout << " " << this->pieces[i][j]->symbol << " ";
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
    bool capture = false;

    if(this->pieces[d0][d1]) {
      //capture move
      capture = true;
      if(this->pieces[d0][d1]->white == white) return nullptr;
      range = this->pieces[s0][s1]->getCaptureRange({s0,s1});
      ret = this->pieces[d0][d1];
    } else {
      //normal move
      ret = this->pieces[s0][s1];
      range = this->pieces[s0][s1]->getMovementRange({s0,s1});
    }
    if(this->isInRange({d0,d1}, range)) {
      this->pieces[d0][d1] = this->pieces[s0][s1];
      this->pieces[s0][s1] = nullptr;
      if(this->isChecked(white)) {
        this->pieces[s0][s1] = this->pieces[d0][d1];
        if(capture) this->pieces[d0][d1] = ret;
        else this->pieces[d0][d1] = nullptr;
        return nullptr;
      } else return ret;
    }
    return nullptr;
  }

};
