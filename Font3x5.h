/*
 * File:        Font3x5.h
 * Author:      Luke de Munk
 * 
 * 3x5 font bitmaps.
 */
#define FONT_3X5        0
#define FONT_3X5_ROWS   5
#define FONT_3X5_COLS   3
#define FONT_3X5_SIZE   87

/* Standard ascii 3x5 font */
const uint8_t Font3x5[] = {
    0x00, 0x00, 0x00,   // (space)
    0x00, 0x17, 0x00,   // !
    0x19, 0x04, 0x13,   // %
    0x00, 0x01, 0x00,   // '
    0x0E, 0x11, 0x00,   // (
    0x00, 0x11, 0x0E,   // )
    0x04, 0x0E, 0x04,   // +
    0x00, 0x18, 0x00,   // ,
    0x04, 0x04, 0x04,   // -
    0x00, 0x10, 0x00,   // .
    0x10, 0x0E, 0x01,   // /
    0x1F, 0x11, 0x1F,   // 0
    0x00, 0x1F, 0x00,   // 1
    0x1D, 0x15, 0x17,   // 2
    0x11, 0x15, 0x1F,   // 3
    0x07, 0x04, 0x1F,   // 4
    0x17, 0x15, 0x1D,   // 5
    0x1F, 0x15, 0x1D,   // 6
    0x01, 0x01, 0x1F,   // 7
    0x1F, 0x15, 0x1F,   // 8
    0x17, 0x15, 0x1F,   // 9
    0x00, 0x0A, 0x00,   // :
    0x00, 0x1A, 0x00,   // ;
    0x04, 0x0A, 0x11,   // <
    0x0A, 0x0A, 0x0A,   // =
    0x11, 0x0A, 0x04,   // >
    0x00, 0x15, 0x02,   // ?
    0x1F, 0x05, 0x1F,   // A
    0x1F, 0x15, 0x1B,   // B
    0x1F, 0x11, 0x11,   // C
    0x1F, 0x11, 0x0E,   // D
    0x1F, 0x15, 0x11,   // E
    0x1F, 0x05, 0x01,   // F
    0x1F, 0x11, 0x1D,   // G
    0x1F, 0x04, 0x1F,   // H
    0x00, 0x1F, 0x00,   // I
    0x10, 0x10, 0x1F,   // J
    0x1F, 0x04, 0x1B,   // K
    0x1F, 0x10, 0x10,   // L
    0x1F, 0x06, 0x1F,   // M
    0x1F, 0x01, 0x1F,   // N
    0x1F, 0x11, 0x1F,   // O
    0x1F, 0x05, 0x07,   // P
    0x1F, 0x31, 0x1F,   // Q
    0x1F, 0x05, 0x1B,   // R
    0x17, 0x15, 0x1D,   // S
    0x01, 0x1F, 0x01,   // T
    0x1F, 0x10, 0x1F,   // U
    0x0F, 0x10, 0x0F,   // V
    0x1F, 0x0C, 0x1F,   // W
    0x1B, 0x04, 0x1B,   // X
    0x07, 0x1C, 0x07,   // Y
    0x19, 0x15, 0x13,   // Z
    0x1F, 0x11, 0x00,   // [
    0x00, 0x11, 0x1F,   // ]
    0x02, 0x01, 0x02,   // ^
    0x10, 0x10, 0x10,   // _
    0x00, 0x01, 0x02,   // `
    0x0C, 0x12, 0x1E,   // a
    0x1F, 0x12, 0x0C,   // b
    0x1E, 0x12, 0x12,   // c
    0x0C, 0x12, 0x1F,   // d
    0x0C, 0x1A, 0x14,   // e
    0x04, 0x1F, 0x05,   // f
    0x2E, 0x2A, 0x1E,   // g
    0x1F, 0x02, 0x1C,   // h
    0x00, 0x1D, 0x00,   // i
    0x20, 0x1D, 0x00,   // j
    0x1F, 0x04, 0x1A,   // k
    0x01, 0x1F, 0x00,   // l
    0x1E, 0x04, 0x1E,   // m
    0x1E, 0x02, 0x1E,   // n
    0x1E, 0x12, 0x1E,   // o
    0x3E, 0x12, 0x0C,   // p
    0x0C, 0x12, 0x3E,   // q
    0x1E, 0x02, 0x06,   // r
    0x14, 0x12, 0x0A,   // s
    0x02, 0x0F, 0x12,   // t
    0x1E, 0x10, 0x1E,   // u
    0x0E, 0x10, 0x0E,   // v
    0x1E, 0x08, 0x1E,   // w
    0x1A, 0x04, 0x1A,   // x
    0x2E, 0x28, 0x1E,   // y
    0x1A, 0x12, 0x16,   // z
    0x04, 0x1B, 0x11,   // {
    0x00, 0x1F, 0x00,   // |
    0x11, 0x1B, 0x04,   // }
};

const unsigned char FontToIndex3x5[FONT_3X5_SIZE+1] = " !%'()+,-./0123456789:;<=>?ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}";
