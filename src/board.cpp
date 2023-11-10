#include <iostream>
#include "piece.cpp"

class Board {
public:
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
    std::cout << std::endl;
    std::cout << "\033[35m /-----------------\\" << std::endl;
    for(int i = 0; i < 8; i++) {
      std::cout << "\033[35m | ";
      for(int j = 0; j < 8; j++) {
        if(pieces[i][j]) {
          if(pieces[i][j]->white) {
            std::cout << "\033[36m";
          } else {
            std::cout << "\033[32m";
          }
          std::cout << pieces[i][j]->symbol << " ";
        }
        else std::cout << "\033[90m- ";
      }
      std::cout << "\033[35m|" << std::endl;
    }
    std::cout << " \\-----------------/" << std::endl;
    std::cout << std::endl;
  }
};
