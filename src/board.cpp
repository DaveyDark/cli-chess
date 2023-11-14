#include <cctype>
#include <cstdlib>
#include <iostream>
#include "piece.cpp"

class Board {
private:
  Piece* pieces[8][8];
  std::pair<int,int> locatePiece(char symbol, bool white) {
    std::pair<int,int> *pos;
    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(!this->pieces[i][j]) continue;
        if(this->pieces[i][j]->white == white && tolower(this->pieces[i][j]->symbol) == symbol){
          pos = new std::pair(i,j);
        }
      }
    }
    if(pos) return *pos;
    return {-1,-1};
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

  void promote(std::pair<int,int> pos) {
    if(tolower(this->pieces[pos.first][pos.second]->symbol) != 'p') return;
    if(this->pieces[pos.first][pos.second]->white && pos.first != 1) return;
    else if(!this->pieces[pos.first][pos.second]->white && pos.first != 6) return;

    char choice = 'a';
    Piece *newPiece;
    while(choice != 'q' && choice != 'r' && choice != 'b' && choice != 'n') {
      std::cout << "Enter piece to promote pawn to:" << std::endl;
      std::cout << "q - Queen" << std::endl;
      std::cout << "r - Rook" << std::endl;
      std::cout << "b - Bishop" << std::endl;
      std::cout << "n - Knight" << std::endl;
      std::cout << ">> ";
      std::cin >> choice;

      switch(choice) {
        case 'q':
          newPiece = new Queen(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'r':
          newPiece = new Rook(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'b':
          newPiece = new Bishop(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'n':
          newPiece = new Knight(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        default:
          std::cout << "Invalid choice" << std::endl;
      }
    }
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
          } 
          if(this->pieces[i][j]->white) {
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

  bool tryMove(int s0, int s1, int d0, int d1, bool white) {
    if(!this->pieces[s0][s1]) return false;
    if(this->pieces[s0][s1]->white != white) return false;

    if(tolower(this->pieces[s0][s1]->symbol) == 'p') {
      if((this->pieces[s0][s1]->white && s0 == 3) || (!this->pieces[s0][s1]->white && s0 == 4)) {
        if(tolower(this->pieces[s0][d1]->symbol) == 'p') {
          Pawn* pawn = (Pawn*)this->pieces[s0][d1];
          if(pawn->enPassantAble && !this->pieces[d0][d1]) {
            return true;
          }
        }
      }
    }

    std::vector<std::vector<std::pair<int,int>>> range;
    Piece* ret;
    bool capture = false;

    if(this->pieces[d0][d1]) {
      //capture move
      capture = true;
      if(this->pieces[d0][d1]->white == white) return false;
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
      bool check = this->isChecked(white);
      this->pieces[s0][s1] = this->pieces[d0][d1];
      if(capture) this->pieces[d0][d1] = ret;
      else this->pieces[d0][d1] = nullptr;
      if(check) return false;
      else return true;
    }
    return false;
  }

  Piece* move(int s0, int s1, int d0, int d1, bool white) {
    if (this->tryMove(s0, s1, d0, d1, white)) {
      if(tolower(this->pieces[s0][s1]->symbol) == 'p') {
        Pawn* pawn = (Pawn*)this->pieces[s0][s1];
        if(!pawn->moved) {
          pawn->moved = true;
          pawn->enPassantAble = true;
        }

        if(!this->pieces[d0][d1] && tolower(this->pieces[s0][d1]->symbol) == 'p' && s0 != d0) {
          // en passant
          Pawn* pwn = (Pawn*)this->pieces[s0][d1];
          if(pwn->white != white && pwn->enPassantAble) {
            Piece* ret = this->pieces[s0][d1];
            this->pieces[d0][d1] = this->pieces[s0][s1];
            this->pieces[s0][s1] = nullptr;
            this->pieces[s0][d1] = nullptr;
            return ret;
          }
        }

        this->promote({s0,s1});
      }

      for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
          if(this->pieces[i][j] && tolower(this->pieces[i][j]->symbol) == 'p') {
            Pawn* pawn = (Pawn*)this->pieces[i][j];
            if(pawn->white != white && pawn->enPassantAble) {
              pawn->enPassantAble = false;
            }
          }
        }
      }
      
      if(this->pieces[d0][d1]) {
        //capture
        Piece* ret = this->pieces[d0][d1];
        this->pieces[d0][d1] = this->pieces[s0][s1];
        this->pieces[s0][s1] = nullptr;
        return ret;
      } else {
        this->pieces[d0][d1] = this->pieces[s0][s1];
        this->pieces[s0][s1] = nullptr;
        return this->pieces[d0][d1];
      }
    }
    return nullptr;
  }

  bool isChecked(bool white) {
    std::pair<int,int> king = this->locatePiece('k',white);

    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(!this->pieces[i][j]) continue;
        if(this->pieces[i][j]->white != white){
          if(this->isInRange(king, this->pieces[i][j]->getCaptureRange({i,j}))) {
            return true;
          }
        }
      }
    }

    return false;
  }

  bool isCheckMate(bool white) {
    std::pair<int,int> king = this->locatePiece('k',white);
    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(this->pieces[i][j] && this->pieces[i][j]->white == white) {
          std::vector<std::vector<std::pair<int,int>>> range = this->pieces[i][j]->getMovementRange({i,j});
          std::vector<std::pair<int,int>> possibleMoves;
          for(const auto direction: range) {
            for(auto pos: direction) {
              possibleMoves.push_back(pos);
              if(this->pieces[pos.first][pos.second]) break;
            }
          }
          if(tolower(this->pieces[i][j]->symbol) == 'p') {
            for(const auto direction: this->pieces[i][j]->getCaptureRange({i,j})) {
              for(auto pos: direction) {
                possibleMoves.push_back(pos);
                if(this->pieces[pos.first][pos.second]) break;
              }
            }
          }
          for(const auto move: possibleMoves) {
            if(this->tryMove(i, j, move.first, move.second, white)) return false;
          }
        }
      }
    }
    return true;
  }
};
