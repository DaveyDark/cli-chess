#include <cctype>
#include <cstdlib>
#include <iostream>
#include "piece.cpp"

// Board class defines the chessboard and controls most operations on it
// The board is stores in the form of a 8x8 matrix of Piece pointers
// If there is not piece at a coordinate, the array will have a nullptr there
class Board {
private:
  // Array to store the various conditions for Draw by Insufficient Material to occur
  const int insufficient[4][6] = {{0,0,0,0,1,0},{0,1,0,0,1,0},{0,0,1,0,1,0},{0,2,0,0,1,0}};

  // Function to locate a piece on the board
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

  // Function that checks weather a given position is in the given range
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

  // Function to promote a pawn
  void promote(std::pair<int,int> pos, char promotion) {
    if(tolower(this->pieces[pos.first][pos.second]->symbol) != 'p') return;
    if(this->pieces[pos.first][pos.second]->white && pos.first != 1) return;
    else if(!this->pieces[pos.first][pos.second]->white && pos.first != 6) return;

    char choice = promotion;
    Piece *newPiece;
    while(choice != 'Q' && choice != 'R' && choice != 'B' && choice != 'N') {
      std::cout << "Enter piece to promote pawn to:" << std::endl;
      std::cout << "Q - Queen" << std::endl;
      std::cout << "R - Rook" << std::endl;
      std::cout << "B - Bishop" << std::endl;
      std::cout << "N - Knight" << std::endl;
      std::cout << ">> ";
      std::cin >> choice;
      if(choice != 'Q' && choice != 'R' && choice != 'B' && choice != 'N') std::cout << "INVALID CHOICE" << std::endl;
    }
      switch(choice) {
        case 'Q':
          newPiece = new Queen(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'R':
          newPiece = new Rook(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'B':
          newPiece = new Bishop(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
        case 'N':
          newPiece = new Knight(this->pieces[pos.first][pos.second]->white);
          delete this->pieces[pos.first][pos.second];
          this->pieces[pos.first][pos.second] = newPiece;
          break;
      }
  }

  // Function to try a move and see if it is valid without modifying the board
  bool tryMove(int s0, int s1, int d0, int d1, bool white) {
    if(!this->pieces[s0][s1]) return false;
    if(this->pieces[s0][s1]->white != white) return false;

    // Handle En Passant Attempt
    if(tolower(this->pieces[s0][s1]->symbol) == 'p' && !this->pieces[d0][d1] && this->pieces[s0][d1]) {
      if((this->pieces[s0][s1]->white && s0 == 3) || (!this->pieces[s0][s1]->white && s0 == 4)) {
        if(tolower(this->pieces[s0][d1]->symbol) == 'p') {
          Pawn* pawn = (Pawn*)this->pieces[s0][d1];
          if(pawn->enPassantAble && !this->pieces[d0][d1]) {
            return true;
          }
        }
      }
    }

    // Handle Castling attempt
    if(tolower(this->pieces[s0][s1]->symbol) == 'k' && s0 == d0 && abs(s1-d1) == 2) {
      return this->canCastle(s0, s1, d0, d1, white);
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
    // Check if the move is legal
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

  // Checks if a castle is possible
  bool canCastle(int s0, int s1, int d0, int d1, bool white) {
    if(this->isChecked(white))return false;
    int i,end,rk;
    if(s1 > d1) {
      // queenside castle
      i = 1;
      end = s1;
      rk=0;
      if(white) {
        if(this->pieces[7][0]->symbol != 'R') return false;
      } else {
        if(this->pieces[0][0]->symbol != 'r') return false;
      }
    } else {
      // kingside castle
      i = s1+1;
      end = 7;
      rk=7;
      if(white) {
        if(this->pieces[7][7]->symbol != 'R') return false;
      } else {
        if(this->pieces[0][7]->symbol != 'r') return false;
      }
    }
    // Loop and see if an intermediate position is in check
    for(; i<end; i++) {
      if(this->pieces[s0][i]) return false;
      this->pieces[s0][i] = this->pieces[s0][s1];
      this->pieces[s0][s1] = nullptr;
      bool check = isChecked(white);
      this->pieces[s0][s1] = this->pieces[s0][i];
      this->pieces[s0][i] = nullptr;
      if(check) return false;
    }
    Rook* rook = (Rook*)this->pieces[s0][rk];
    King* king = (King*)this->pieces[s0][s1];
    if(rook->moved || king->moved) return false;
    return true;
  }

public:
  // Board Array
  Piece* pieces[8][8];
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

  // Init board with the approriate pieces at appropriate positions
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

  // Print the board
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

  // Function to move a piece from (s0,s1) to (d0,d1)
  Piece* move(int s0, int s1, int d0, int d1, bool white, char promotion = 'x') {
    if (this->tryMove(s0, s1, d0, d1, white)) {
      // Move is legal, we can perform it
      if(tolower(this->pieces[s0][s1]->symbol) == 'p') {
        Pawn* pawn = (Pawn*)this->pieces[s0][s1];
        // Update En Passant and moved state of the pawn
        if(!pawn->moved) {
          pawn->moved = true;
          pawn->enPassantAble = true;
        }

        if(!this->pieces[d0][d1] && s1 != d1 && tolower(this->pieces[s0][d1]->symbol) == 'p' && s0 != d0) {
          // En passant
          Pawn* pwn = (Pawn*)this->pieces[s0][d1];
          if(pwn->white != white && pwn->enPassantAble) {
            Piece* ret = this->pieces[s0][d1];
            this->pieces[d0][d1] = this->pieces[s0][s1];
            this->pieces[s0][s1] = nullptr;
            this->pieces[s0][d1] = nullptr;
            return ret;
          }
        }

        // Check and handle pawn promotion
        this->promote({s0,s1}, promotion);
      }


      // Check and handle Castling
      if(tolower(this->pieces[s0][s1]->symbol) == 'k' && s0 == d0 && abs(s1-d1) == 2) {
        //castling
        this->pieces[d0][d1] = this->pieces[s0][s1];
        if(s1 > d1) {
          // queenside castle
          this->pieces[s0][d1+1] = this->pieces[s0][0];
          this->pieces[s0][0] = nullptr;
        } else {
          // kingside castle
          this->pieces[s0][d1-1] = this->pieces[s0][7];
          this->pieces[s0][7] = nullptr;
        }
      }

      // Update En Passant states for pawns
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

      // Update moved states for pawn and rook
      if(tolower(this->pieces[s0][s1]->symbol) == 'r') {
        Rook* rook = (Rook*)this->pieces[s0][s1];
        if(!rook->moved) rook->moved = true;
      }
      if(tolower(this->pieces[s0][s1]->symbol) == 'k') {
        King* king = (King*)this->pieces[s0][s1];
        if(!king->moved) king->moved = true;
      }
      
      // Perform move
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

  // Function to check if the king of given color is in check
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

  // Function to check if a legal move is possible
  bool canMove(bool white) {
    // We loop over all pieces of given color then try to perform all possible moves for them and see if they are legel
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
            if(this->tryMove(i, j, move.first, move.second, white)) return true;
          }
        }
      }
    }
    return false;
  }

  // Function to check draw by insufficient material
  bool insufficientMaterial(bool white) {
    // 0 = rook
    // 1 = knight
    // 2 = bishop
    // 3 = king
    // 4 = queen
    // 5 = pawn
    // We will count the pieces then see if it matches the couts for a draw
    int counts[6] = {0,0,0,0,0,0};
    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(!this->pieces[i][j] || this->pieces[i][j]->white != white) continue;
        switch(tolower(this->pieces[i][j]->symbol)) {
          case 'r':
            counts[0]++;
            break;
          case 'n':
            counts[1]++;
            break;
          case 'b':
            counts[2]++;
            break;
          case 'q':
            counts[3]++;
            break;
          case 'k':
            counts[4]++;
            break;
          case 'p':
            counts[5]++;
            break;
        }
      }
    }
    for(auto material: this->insufficient) {
      bool equal = true;
      for(int i=0; i<6; i++) {
        if(material[i] != counts[i]) equal = false;
      }
      if(equal) return true;
    }
    return false;
  }

  // Returns the state of the board in the form of a string
  std::string getState() {
    std::string state;
    for(int i=0; i<8; i++) {
      for(int j=0; j<8; j++) {
        if(this->pieces[i][j]) state += this->pieces[i][j]->symbol;
        else state += ' ';
      }
    }
    return state;
  }
};
