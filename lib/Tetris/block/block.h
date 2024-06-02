#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H

#include <Arduino.h>

#include <board_constants.h>

#include <block_codes/block_codes.h>

class Block {
private:
  void updateCodes(bool color = false, bool shape = false, bool offset = false, bool wall_kick_offset = false);

public:
  const char& block_code;
  short pos_x;
  short pos_y;
  short rotation;

  Block(const char& _block_code, int _pos_x, int _pos_y = 0, int _rotation = 0);

  // Movement and Rotation
  bool safe_rotate(char (*board_matrix)[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH], const bool& direction = ROTATE::CW);
  bool safe_move(char (*board_matrix)[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH], const byte& direction = MOVE::DOWN);
  void drop(char (*board_matrix)[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH]);

  // Board Manipulation
  void remove_block_from_board_matrix(char (*board_matrix)[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH], const bool& use_telegraph = true);
  void add_block_to_board_matrix(char (*board_matrix)[BOARD::HEIGHT][BOARD::WIDTH][BOARD::DEPTH], const bool& use_telegraph = true);

  // Getters (add more if needed)
  char getBlockCode() const { return block_code; }
  int getPosX() const { return pos_x; }
  int getPosY() const { return pos_y; }
  int getRotation() const { return rotation; }
};

class Block_List {
private:
  char block_code_list[BOARD::FUTURE_BLOCKS];

public:
  Block_List();

  char get_next_block_code();
};

#endif // TETRIS_BLOCK_H
