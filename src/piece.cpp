class Piece {
public:
  char symbol;
  bool white;
  Piece(bool w, char s): white(w), symbol(s) {}
};

class Rook: public Piece {
public:
  Rook(bool w): Piece(w, w?'R':'r') {}
};

class Knight: public Piece {
public:
  Knight(bool w): Piece(w, w?'N':'n') {}
};

class Bishop: public Piece {
public:
  Bishop(bool w): Piece(w, w?'B':'b') {}
};

class King: public Piece {
public:
  King(bool w): Piece(w, w?'K':'k') {}
};

class Queen: public Piece {
public:
  Queen(bool w): Piece(w, w?'Q':'q') {}
};

class Pawn: public Piece {
public:
  Pawn(bool w): Piece(w, w?'P':'p') {}
};
