#include "board.cpp"
#include <unordered_map>

class Chess {
private:
  Board board;
  std::unordered_map<std::string, int> stateHistory;
  bool whitesTurn = true;
  int notation = 2;
  std::vector<Piece*> capturedWhite;
  std::vector<Piece*> capturedBlack;
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
      this->board.display();
      std::string state = this->board.getState();
      this->stateHistory[state] += 1;
      if(invalid) {
        std::cout << board.RED_BACKGROUND << "!INVALID MOVE! \033[0m" << std::endl;
        invalid = false;
      }
      if(!this->board.canMove(whitesTurn)) {
        if(this->board.isChecked(whitesTurn)) {
          std::cout << board.RED_BACKGROUND << "!CHECKMATE! \033[0m" << std::endl;
          if(whitesTurn) std::cout << "BLACK WINS" << std::endl;
          else std::cout << "WHITE WINS" << std::endl;
        } else {
          std::cout << board.RED_BACKGROUND << "!DRAW! \033[0m" << std::endl;
          std::cout << "DRAW BY STALEMATE" << std::endl;
        }
        break;
      }
      if(this->board.insufficientMaterial(true) && this->board.insufficientMaterial(false)) {
        std::cout << board.RED_BACKGROUND << "!DRAW! \033[0m" << std::endl;
        std::cout << "DRAW BY INSUFFICIENT MATERIAL" << std::endl;
        break;
      }
      for (const auto& pair : this->stateHistory) {
        if(pair.second > 3) {
          std::cout << board.RED_BACKGROUND << "!DRAW! \033[0m" << std::endl;
          std::cout << "DRAW BY REPETITION" << std::endl;
          playing = false;
          break;
        }
      }
      std::cout << board.BLACK_BACKGROUND_COLOR << board.WHITE_COLOR_CODE;
      for(auto p: this->capturedWhite) {
        std::cout << " [" << p->symbol << "] ";
      }
      std::cout << std::endl;
      std::cout << board.WHITE_BACKGROUND_COLOR << board.BLACK_COLOR_CODE;
      for(auto p: this->capturedBlack) {
        std::cout << " [" << p->symbol << "] ";
      }
      std::cout << std::endl;
      std::cout << std::endl;
      if(whitesTurn) {
        std::cout << board.BLACK_BACKGROUND_COLOR << board.WHITE_COLOR_CODE << "White";
      }
      else {
        std::cout << board.WHITE_BACKGROUND_COLOR << board.BLACK_COLOR_CODE << "Black";
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

    Piece* cap = this->board.move(s0,s1,d0,d1, white);
    if(cap && cap->white != white) {
      if(white) this->capturedBlack.push_back(cap);
      else this->capturedWhite.push_back(cap);
    }
    return cap;
  }
};

int main() {
  Chess chess;
  chess.play();
}
