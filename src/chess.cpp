#include "board.cpp"
#include <regex>
#include <unordered_map>

class Chess {
private:
  Board board;
  std::unordered_map<std::string, int> stateHistory;
  bool whitesTurn = true;
  int notation = 0;
  std::vector<Piece*> capturedWhite;
  std::vector<Piece*> capturedBlack;

  // Chess Notation Regex
  std::regex CN_PAWN_MOVE = std::regex("[a-h][1-8]");
  std::regex CN_PAWN_PROMOTION = std::regex("[a-h][1-8]=[BQRN]");
  std::regex CN_PAWN_CAPTURE = std::regex("[a-h]x[a-h][1-8]");
  std::regex CN_PAWN_CAPTURE_PROMOTE = std::regex("[a-h]x[a-h][1-8]=[BQRN]");
  std::regex CN_PIECE_MOVE = std::regex("[BRNKQ][a-h][1-8]");
  std::regex CN_PIECE_MOVE_AMBIG = std::regex("[BRNKQ][a-h][a-h][1-8]");
  std::regex CN_PIECE_MOVE_AMBIG_RANK = std::regex("[BRNKQ][1-8][a-h][1-8]");
  std::regex CN_PIECE_CAPTURE = std::regex("[BRNKQ]x[a-h][1-8]");
  std::regex CN_PIECE_CAPTURE_AMBIG = std::regex("[BRNKQ][a-h]x[a-h][1-8]");
  std::regex CN_PIECE_CAPTURE_AMBIG_RANK = std::regex("[BRNKQ][1-8]x[a-h][1-8]");
  std::regex CN_CASTLE_KINGSIDE = std::regex("O-O");
  std::regex CN_CASTLE_QUEENSIDE = std::regex("O-O-O");
  
public:
  void play() {
    while(notation < 1 || notation > 2){
      std::cout << "Please choose an input method: " << std::endl;
      std::cout << "1) Chess notation (ex:- 'Ng4' moves knight to g4)" << std::endl;
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
    if(std::regex_match(inp, CN_PAWN_MOVE)) {
      // Pawn move
      int file = inp[0] - 'a';
      int rank = 8 - (inp[1] - '0');
      int pawnRank = (white)?rank+1:rank-1;
      if(pawnRank > 7 || pawnRank < 0 || !this->board.pieces[pawnRank][file] || tolower(this->board.pieces[pawnRank][file]->symbol) != 'p') {
        pawnRank = (white)?pawnRank+1:pawnRank-1;
        if(pawnRank > 7 || pawnRank < 0 || !this->board.pieces[pawnRank][file] || tolower(this->board.pieces[pawnRank][file]->symbol) != 'p') return false;
      }
      return this->board.move(pawnRank, file, rank, file, white);
    } else if (std::regex_match(inp, CN_PAWN_CAPTURE)) {
      // Pawn Capture
      std::pair<int,int> target = {8 - (inp[3]-'0'), inp[2] - 'a'};
      int sourceFile = inp[0] - 'a';
      int sourceRank = (white)?target.first+1:target.first-1;
      if(this->board.pieces[target.first][target.second]) {
        if(this->board.pieces[target.first][target.second]->white == white)return false;
      } else {
        int t = (white)?target.first+1:target.first-1;
        if(!this->board.pieces[t][target.second] || 
          this->board.pieces[t][target.second]->white == white ||
          tolower(this->board.pieces[t][target.second]->symbol) != 'p')return false;
      }
      if(!this->board.pieces[sourceRank][sourceFile] ||
        tolower(this->board.pieces[sourceRank][sourceFile]->symbol) != 'p' || 
        this->board.pieces[sourceRank][sourceFile]->white != white)
          return false;
      return this->board.move(sourceRank,sourceFile,target.first,target.second,white);
    } else if (std::regex_match(inp, CN_PAWN_CAPTURE_PROMOTE)) {
      // Pawn capture + promotion
      std::pair<int,int> target = {8 - (inp[3]-'0'), inp[2] - 'a'};
      int sourceFile = inp[0] - 'a';
      int sourceRank = (white)?target.first+1:target.first-1;
      char promotion = inp[5];
      if(!this->board.pieces[target.first][target.second] || this->board.pieces[target.first][target.second]->white == white)return false;
      if(!this->board.pieces[sourceRank][sourceFile] ||
        tolower(this->board.pieces[sourceRank][sourceFile]->symbol) != 'p' || 
        this->board.pieces[sourceRank][sourceFile]->white != white)
          return false;
      return this->board.move(sourceRank,sourceFile,target.first,target.second,white, promotion);
    } else if (std::regex_match(inp, CN_PAWN_PROMOTION)) {
      // Pawn Promotion
      int file = inp[0] - 'a';
      int rank = 8 - (inp[1] - '0');
      int pawnRank = (white)?rank+1:rank-1;
      char promotion = inp[3];
      if(!this->board.pieces[pawnRank][file] || tolower(this->board.pieces[pawnRank][file]->symbol) != 'p') return false;
      return this->board.move(pawnRank, file, rank, file, white, promotion);
    } else if (std::regex_match(inp, CN_PIECE_MOVE)) {
      // Piece Move
      int d0 = 8 - (inp[2] - '0');
      int d1 = inp[1] - 'a';
      if(this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
          if(this->board.pieces[i][j] && this->board.pieces[i][j]->white == white && toupper(this->board.pieces[i][j]->symbol) == piece){
            if(this->board.move(i,j,d0,d1,white)) return true;
          }
        }
      }
    } else if (std::regex_match(inp, CN_PIECE_MOVE_AMBIG)) {
      // Ambiguous Piece Move
      int d0 = 8 - (inp[3] - '0');
      int d1 = inp[2] - 'a';
      int s1 = inp[1] - 'a';
      if(this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        if(this->board.pieces[i][s1] && this->board.pieces[i][s1]->white == white && toupper(this->board.pieces[i][s1]->symbol) == piece){
          if(this->board.move(i,s1,d0,d1,white)) return true;
        }
      }
    } else if (std::regex_match(inp, CN_PIECE_MOVE_AMBIG_RANK)) {
      // Ambiguous Piece Move by Rank
      int d0 = 8 - (inp[3] - '0');
      int d1 = inp[2] - 'a';
      int s0 = 8 - (inp[1] - '0');
      if(this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        if(this->board.pieces[s0][i] && this->board.pieces[s0][i]->white == white && toupper(this->board.pieces[s0][i]->symbol) == piece){
          if(this->board.move(s0,i,d0,d1,white)) return true;
        }
      }
    } else if (std::regex_match(inp, CN_PIECE_CAPTURE)) {
      // Piece Capture
      int d0 = 8 - (inp[3] - '0');
      int d1 = inp[2] - 'a';
      if(!this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
          if(this->board.pieces[i][j] && this->board.pieces[i][j]->white == white && toupper(this->board.pieces[i][j]->symbol) == piece){
            if(this->board.move(i,j,d0,d1,white)) return true;
          }
        }
      }
    } else if (std::regex_match(inp, CN_PIECE_CAPTURE_AMBIG)) {
      // Ambiguous Piece Capture
      int d0 = 8 - (inp[4] - '0');
      int d1 = inp[3] - 'a';
      int s1 = inp[1] - 'a';
      if(!this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        if(this->board.pieces[i][s1] && this->board.pieces[i][s1]->white == white && toupper(this->board.pieces[i][s1]->symbol) == piece){
          if(this->board.move(i,s1,d0,d1,white)) return true;
        }
      }
    } else if (std::regex_match(inp, CN_PIECE_CAPTURE_AMBIG_RANK)) {
      // Ambiguous Piece Capture by Rank
      int d0 = 8 - (inp[4] - '0');
      int d1 = inp[3] - 'a';
      int s0 = 8 - (inp[1] - '0');
      if(!this->board.pieces[d0][d1]) return false;
      char piece = inp[0];
      for (int i=0; i<8; i++) {
        if(this->board.pieces[s0][i] && this->board.pieces[s0][i]->white == white && toupper(this->board.pieces[s0][i]->symbol) == piece){
          if(this->board.move(s0,i,d0,d1,white)) return true;
        }
      }
    } else if (std::regex_match(inp, CN_CASTLE_KINGSIDE)) {
      // Kingside Castle
      int s0,s1;
      for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
          if(this->board.pieces[i][j] && this->board.pieces[i][j]->white == white && toupper(this->board.pieces[i][j]->symbol) == 'K'){
            s0 = i;
            s1 = j;
            break;
          }
        }
      }
      int d1 = s1+2;
      return this->board.move(s0,s1,s0,d1,white);
    } else if (std::regex_match(inp, CN_CASTLE_QUEENSIDE)) {
      // Queenside Castle
      int s0,s1;
      for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
          if(this->board.pieces[i][j] && this->board.pieces[i][j]->white == white && toupper(this->board.pieces[i][j]->symbol) == 'K'){
            s0 = i;
            s1 = j;
            break;
          }
        }
      }
      int d1 = s1-2;
      return this->board.move(s0,s1,s0,d1,white);
    } else return false;

    return false;
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
