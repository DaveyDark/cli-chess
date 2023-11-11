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

    bool playing = true, invalid = false;
    std::string move;
    while(playing){
      board.display();
      if(invalid) {
        std::cout << "\033[41m!INVALID MOVE! \033[0m" << std::endl;
        invalid = false;
      }
      if(whitesTurn) {
        std::cout << board.WHITE_BACKGROUND_COLOR << board.BLACK_COLOR_CODE << "White";
      }
      else {
        std::cout << board.BLACK_BACKGROUND_COLOR << board.WHITE_COLOR_CODE << "Black";
      }
      std::cout << ">> ";
      std::cin >> move;
      bool turn = (notation == 1)?this->processChessNotation(move, whitesTurn) : this->processAlgebraicNotation(move, whitesTurn);
      if(!turn) invalid = true;
      else whitesTurn = !whitesTurn;
    }
  }
  bool processChessNotation(std::string inp, bool white) {
    return true;
  }
  bool processAlgebraicNotation(std::string inp, bool white) {
    if(inp.length() != 4) return false;
    int s1 = inp[0] - 'a';
    int s0 = 8 - (inp[1] - '0');
    int d1 = inp[2] - 'a';
    int d0 = 8 - (inp[3] - '0');

    if(s0 < 0 || s0 > 7 || s1 < 0 || s1 > 7 ||d0 < 0 || d0 > 7 || d1 < 0 || d1 > 7) return false;

    return this->board.move(s0,s1,d0,d1, white);
  }
};

int main() {
  Chess chess;
  chess.play();
}
