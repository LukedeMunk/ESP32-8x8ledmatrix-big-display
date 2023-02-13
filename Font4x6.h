/*
 * File:        Font4x6.h
 * Author:      Luke de Munk
 * 
 * 5x7 font bitmaps.
 */
#define FONT_4X6        1
#define FONT_4X6_ROWS   6
#define FONT_4X6_COLS   4
#define FONT_4X6_SIZE   88

/* Standard ascii 4x6 font */
const uint8_t Font4x6[] = {
    0x00, 0x00, 0x00, 0x00, // (space)
    0x00, 0x2F, 0x00, 0x00, // !
    0x31, 0x08, 0x04, 0x23, // %
    0x00, 0x03, 0x00, 0x00, // '
    0x0C, 0x12, 0x21, 0x00, // (
    0x00, 0x21, 0x12, 0x0C, // )
    0x02, 0x07, 0x02, 0x00, // *
    0x0C, 0x1E, 0x1E, 0x0C, // +
    0x00, 0x30, 0x00, 0x00, // ,
    0x0C, 0x0C, 0x0C, 0x0C, // -
    0x00, 0x20, 0x00, 0x00, // .
    0x20, 0x18, 0x06, 0x01, // /
    0x1E, 0x29, 0x25, 0x1E, // 0
    0x22, 0x3F, 0x20, 0x00, // 1
    0x32, 0x29, 0x29, 0x26, // 2
    0x12, 0x21, 0x25, 0x1A, // 3
    0x0C, 0x0A, 0x3F, 0x08, // 4
    0x17, 0x25, 0x25, 0x19, // 5
    0x1E, 0x25, 0x25, 0x18, // 6
    0x01, 0x39, 0x05, 0x03, // 7
    0x1A, 0x25, 0x25, 0x1A, // 8
    0x06, 0x29, 0x29, 0x1E, // 9
    0x00, 0x12, 0x00, 0x00, // :
    0x00, 0x32, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x00, // <
    0x14, 0x14, 0x14, 0x00, // =
    0x22, 0x14, 0x08, 0x00, // >
    0x00, 0x2D, 0x02, 0x00, // ?
    0x3E, 0x09, 0x09, 0x3E, // A
    0x3F, 0x25, 0x25, 0x1A, // B
    0x1E, 0x21, 0x21, 0x12, // C
    0x3F, 0x21, 0x21, 0x1E, // D
    0x3F, 0x25, 0x25, 0x21, // E
    0x3F, 0x05, 0x05, 0x01, // F
    0x1E, 0x21, 0x29, 0x3A, // G
    0x3F, 0x04, 0x04, 0x3F, // H
    0x21, 0x3F, 0x21, 0x00, // I
    0x10, 0x21, 0x21, 0x1F, // J
    0x3F, 0x04, 0x0A, 0x31, // K
    0x3F, 0x20, 0x20, 0x20, // L
    0x3F, 0x02, 0x02, 0x3F, // M
    0x3F, 0x02, 0x04, 0x3F, // N
    0x1E, 0x21, 0x21, 0x1E, // O
    0x3F, 0x09, 0x09, 0x06, // P
    0x1E, 0x21, 0x11, 0x2E, // Q
    0x3F, 0x09, 0x09, 0x36, // R
    0x22, 0x25, 0x25, 0x19, // S
    0x01, 0x3F, 0x01, 0x01, // T
    0x1F, 0x20, 0x20, 0x1F, // U
    0x0F, 0x10, 0x20, 0x1F, // V
    0x3F, 0x10, 0x10, 0x3F, // W
    0x3B, 0x04, 0x04, 0x3B, // X
    0x03, 0x04, 0x38, 0x07, // Y
    0x31, 0x2D, 0x23, 0x21, // Z
    0x3F, 0x21, 0x21, 0x00, // [
    0x00, 0x21, 0x21, 0x3F, // ]
    0x02, 0x01, 0x02, 0x00, // ^
    0x20, 0x20, 0x20, 0x20, // _
    0x00, 0x01, 0x02, 0x00, // `
    0x10, 0x2A, 0x2A, 0x3C, // a
    0x3F, 0x24, 0x24, 0x18, // b
    0x1C, 0x22, 0x22, 0x14, // c
    0x18, 0x24, 0x24, 0x3F, // d
    0x1C, 0x2A, 0x2A, 0x2C, // e
    0x04, 0x7E, 0x05, 0x01, // f
    0x4C, 0x52, 0x52, 0x3E, // g
    0x3F, 0x04, 0x04, 0x38, // h
    0x24, 0x3D, 0x20, 0x00, // i
    0x40, 0x40, 0x44, 0x3D, // j
    0x3F, 0x08, 0x14, 0x22, // k
    0x21, 0x3F, 0x20, 0x00, // l
    0x3E, 0x04, 0x04, 0x3E, // m
    0x3E, 0x04, 0x02, 0x3C, // n
    0x1C, 0x22, 0x22, 0x1C, // o
    0x7E, 0x22, 0x22, 0x1C, // p
    0x1C, 0x22, 0x22, 0x7E, // q
    0x3E, 0x04, 0x02, 0x04, // r
    0x24, 0x2A, 0x2A, 0x12, // s
    0x02, 0x1F, 0x22, 0x20, // t
    0x1E, 0x20, 0x20, 0x1E, // u
    0x0E, 0x10, 0x20, 0x1E, // v
    0x3E, 0x10, 0x10, 0x3E, // w
    0x36, 0x08, 0x08, 0x36, // x
    0x4E, 0x50, 0x50, 0x3E, // y
    0x32, 0x2A, 0x26, 0x22, // z
    0x0C, 0x33, 0x21, 0x00, // {
    0x00, 0x3F, 0x00, 0x00, // |
    0x00, 0x21, 0x33, 0x0C, // }
};

const unsigned char FontToIndex4x6[FONT_4X6_SIZE+1] = " !%'()*+,-./0123456789:;<=>?ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}";