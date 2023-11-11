#include "board.cpp"
#include <cstdio>

class Chess {
private:
  Board board;
  bool whitesTurn = true;
  int notation = 0;
public:
  void play() {
    while(notation < 1 || notation > 2){
      std::cout << "Please choose an input method: " << std::endl;
      std::cout << "1) Chess notation (ex:- 'ng4' moves knight to g4)" << std::endl;
      std::cout << "2) Algebraic Notation (ex:- 'f2g2' moves the piece at f2 to g2)" << std::endl;
      std::cout << ">> ";
      std::cin >> notation;
      if(notation < 1 || notation > 2) {
        std::cout << "Invalid choice." << std::endl;
      }
    }

    bool playing = true;
    std::string move;
    while(playing){
      board.display();
      if(whitesTurn) {
        std::cout << board.WHITE_BACKGROUND_COLOR << board.BLACK_COLOR_CODE << "White";
      }
      else {
        std::cout << board.BLACK_BACKGROUND_COLOR << board.WHITE_COLOR_CODE << "Black";
      }
      std::cout << ">> ";
      std::cin >> move;
    }
  }
};

int main() {
  Chess chess;
  chess.play();
}
