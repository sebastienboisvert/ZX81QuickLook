//
//  ZX81-Character-Table.h
//  ZX81QuickLook
//
//  Created by Sebastien on 2019-09-29.
//  Copyright © 2019 Sebastien Boisvert. All rights reserved.
//
    
#ifndef ZX81_Character_Table_h
#define ZX81_Character_Table_h

/*
 
 The character map for use with the ZX81 VDU font.
 
 The ZX81 VDU font was designed by Matthew Eagles and available
 at many font sites under the Creative Commons license:
 https://creativecommons.org/licenses/by-sa/3.0/
 
 */

static const char* zx81_table[256] =
{
  " "      /* 0x00  space                   */,
  "\xf1"   /* 0x01  top-left block          */,
  "\xf2"   /* 0x02  top-right block         */,
  "\xf3"   /* 0x03  top bar                 */,
  "\xf4"   /* 0x04  bottom-left block       */,
  "\xf5"   /* 0x05  left bar                */,
  "\xf6"   /* 0x06  right-left cross        */,
  "\xf7"   /* 0x07  top-left corner         */,
  "\xf8"   /* 0x08  gray space              */,
  "\xf9"   /* 0x09  bottom gray bar         */,
  "\xfa"   /* 0x0A  top gray bar            */,
  "\""     /* 0x0B  "                       */,
  "\xa3"   /* 0x0C  £                       */,
  "$"      /* 0x0D  $                       */,
  ":"      /* 0x0E  :                       */,
  "?"      /* 0x0F  ?                       */,
  "("      /* 0x10  (                       */,
  ")"      /* 0x11  )                       */,
  ">"      /* 0x12  >                       */,
  "<"      /* 0x13  <                       */,
  "="      /* 0x14  =                       */,
  "+"      /* 0x15  +                       */,
  "-"      /* 0x16  -                       */,
  "*"      /* 0x17  *                       */,
  "/"      /* 0x18  /                       */,
  ";"      /* 0x19  ;                       */,
  ","      /* 0x1A  ,                       */,
  "."      /* 0x1B  .                       */,
  "0"      /* 0x1C  0                       */,
  "1"      /* 0x1D  1                       */,
  "2"      /* 0x1E  2                       */,
  "3"      /* 0x1F  3                       */,
  "4"      /* 0x20  4                       */,
  "5"      /* 0x21  5                       */,
  "6"      /* 0x22  6                       */,
  "7"      /* 0x23  7                       */,
  "8"      /* 0x24  8                       */,
  "9"      /* 0x25  9                       */,
  "A"      /* 0x26  A                       */,
  "B"      /* 0x27  B                       */,
  "C"      /* 0x28  C                       */,
  "D"      /* 0x29  D                       */,
  "E"      /* 0x2A  E                       */,
  "F"      /* 0x2B  F                       */,
  "G"      /* 0x2C  G                       */,
  "H"      /* 0x2D  H                       */,
  "I"      /* 0x2E  I                       */,
  "J"      /* 0x2F  J                       */,
  "K"      /* 0x30  K                       */,
  "L"      /* 0x31  L                       */,
  "M"      /* 0x32  M                       */,
  "N"      /* 0x33  N                       */,
  "O"      /* 0x34  O                       */,
  "P"      /* 0x35  P                       */,
  "Q"      /* 0x36  Q                       */,
  "R"      /* 0x37  R                       */,
  "S"      /* 0x38  S                       */,
  "T"      /* 0x39  T                       */,
  "U"      /* 0x3A  U                       */,
  "V"      /* 0x3B  V                       */,
  "W"      /* 0x3C  W                       */,
  "X"      /* 0x3D  X                       */,
  "Y"      /* 0x3E  Y                       */,
  "Z"      /* 0x3F  Z                       */,
  "RND"    /* 0x40  RND                     */,
  "INKEY$" /* 0x41  INKEY$                  */,
  "PI"     /* 0x42  PI                      */,
  "?"      /* 0x43  not used                */,
  "?"      /* 0x44  not used                */,
  "?"      /* 0x45  not used                */,
  "?"      /* 0x46  not used                */,
  "?"      /* 0x47  not used                */,
  "?"      /* 0x48  not used                */,
  "?"      /* 0x49  not used                */,
  "?"      /* 0x4A  not used                */,
  "?"      /* 0x4B  not used                */,
  "?"      /* 0x4C  not used                */,
  "?"      /* 0x4D  not used                */,
  "?"      /* 0x4E  not used                */,
  "?"      /* 0x4F  not used                */,
  "?"      /* 0x50  not used                */,
  "?"      /* 0x51  not used                */,
  "?"      /* 0x52  not used                */,
  "?"      /* 0x53  not used                */,
  "?"      /* 0x54  not used                */,
  "?"      /* 0x55  not used                */,
  "?"      /* 0x56  not used                */,
  "?"      /* 0x57  not used                */,
  "?"      /* 0x58  not used                */,
  "?"      /* 0x59  not used                */,
  "?"      /* 0x5A  not used                */,
  "?"      /* 0x5B  not used                */,
  "?"      /* 0x5C  not used                */,
  "?"      /* 0x5D  not used                */,
  "?"      /* 0x5E  not used                */,
  "?"      /* 0x5F  not used                */,
  "?"      /* 0x60  not used                */,
  "?"      /* 0x61  not used                */,
  "?"      /* 0x62  not used                */,
  "?"      /* 0x63  not used                */,
  "?"      /* 0x64  not used                */,
  "?"      /* 0x65  not used                */,
  "?"      /* 0x66  not used                */,
  "?"      /* 0x67  not used                */,
  "?"      /* 0x68  not used                */,
  "?"      /* 0x69  not used                */,
  "?"      /* 0x6A  not used                */,
  "?"      /* 0x6B  not used                */,
  "?"      /* 0x6C  not used                */,
  "?"      /* 0x6D  not used                */,
  "?"      /* 0x6E  not used                */,
  "?"      /* 0x6F  not used                */,
  "?"      /* 0x70  cursor up               */,
  "?"      /* 0x71  cursor down             */,
  "?"      /* 0x72  cursor left             */,
  "?"      /* 0x73  cursor right            */,
  "?"      /* 0x74  graphics                */,
  "?"      /* 0x75  edit                    */,
  "?"      /* 0x76  newline                 */,
  "?"      /* 0x77  rubput                  */,
  "?"      /* 0x78  K/L mode                */,
  "?"      /* 0x79  function                */,
  "?"      /* 0x7A  not used                */,
  "?"      /* 0x7B  not used                */,
  "?"      /* 0x7C  not used                */,
  "?"      /* 0x7D  not used                */,
  "?"      /* 0x7E  number                  */,
  "?"      /* 0x7F  cursor                  */,
  "\xb0"   /* 0x80  inverse space           */,
  "\xb1"   /* 0x81  bottom-right corner     */,
  "\xb2"   /* 0x82  bottom-left corner      */,
  "\xb3"   /* 0x83  bottom bar              */,
  "\xb4"   /* 0x84  top-right corner        */,
  "\xb5"   /* 0x85  right bar               */,
  "\xb6"   /* 0x86  left-right cross        */,
  "\xb7"   /* 0x87  bottom-right block      */,
  "\xb8"   /* 0x88  inverse gray space      */,
  "\xb9"   /* 0x89  inverse bottom gray bar */,
  "\xba"   /* 0x8A  inverse top gray bar    */,
  "\xbb"   /* 0x8B  inverse "               */,
  "\xbc"   /* 0x8C  inverse £               */,
  "\xbd"   /* 0x8D  inverse $               */,
  "\xbe"   /* 0x8E  inverse :               */,
  "\xbf"   /* 0x8F  inverse ?               */,
  "\xc0"   /* 0x90  inverse (               */,
  "\xc1"   /* 0x91  inverse )               */,
  "\xc2"   /* 0x92  inverse >               */,
  "\xc3"   /* 0x93  inverse <               */,
  "\xc4"   /* 0x94  inverse =               */,
  "\xc5"   /* 0x95  inverse +               */,
  "\xc6"   /* 0x96  inverse -               */,
  "\xc7"   /* 0x97  inverse *               */,
  "\xc8"   /* 0x98  inverse /               */,
  "\xc9"   /* 0x99  inverse ;               */,
  "\xca"   /* 0x9A  inverse ,               */,
  "\xcb"   /* 0x9B  inverse .               */,
  "\xcc"   /* 0x9C  inverse 0               */,
  "\xcd"   /* 0x9D  inverse 1               */,
  "\xce"   /* 0x9E  inverse 2               */,
  "\xcf"   /* 0x9F  inverse 3               */,
  "\xd0"   /* 0xA0  inverse 4               */,
  "\xd1"   /* 0xA1  inverse 5               */,
  "\xd2"   /* 0xA2  inverse 6               */,
  "\xd3"   /* 0xA3  inverse 7               */,
  "\xd4"   /* 0xA4  inverse 8               */,
  "\xd5"   /* 0xA5  inverse 9               */,
  "a"      /* 0xA6  inverse A               */,
  "b"      /* 0xA7  inverse B               */,
  "c"      /* 0xA8  inverse C               */,
  "d"      /* 0xA9  inverse D               */,
  "e"      /* 0xAA  inverse E               */,
  "f"      /* 0xAB  inverse F               */,
  "g"      /* 0xAC  inverse G               */,
  "h"      /* 0xAD  inverse H               */,
  "i"      /* 0xAE  inverse I               */,
  "j"      /* 0xAF  inverse J               */,
  "k"      /* 0xB0  inverse K               */,
  "l"      /* 0xB1  inverse L               */,
  "m"      /* 0xB2  inverse M               */,
  "n"      /* 0xB3  inverse N               */,
  "o"      /* 0xB4  inverse O               */,
  "p"      /* 0xB5  inverse P               */,
  "q"      /* 0xB6  inverse Q               */,
  "r"      /* 0xB7  inverse R               */,
  "s"      /* 0xB8  inverse S               */,
  "t"      /* 0xB9  inverse T               */,
  "u"      /* 0xBA  inverse U               */,
  "v"      /* 0xBB  inverse V               */,
  "w"      /* 0xBC  inverse W               */,
  "x"      /* 0xBD  inverse X               */,
  "y"      /* 0xBE  inverse Y               */,
  "z"      /* 0xBF  inverse Z               */,
  "\"\""   /* 0xC0  ""                      */,
  "AT"     /* 0xC1  AT                      */,
  "TAB"    /* 0xC2  TAB                     */,
  "?"      /* 0xC3  not used                */,
  "CODE"   /* 0xC4  CODE                    */,
  "VAL"    /* 0xC5  VAL                     */,
  "LEN"    /* 0xC6  LEN                     */,
  "SIN"    /* 0xC7  SIN                     */,
  "COS"    /* 0xC8  COS                     */,
  "TAN"    /* 0xC9  TAN                     */,
  "ASN"    /* 0xCA  ASN                     */,
  "ACS"    /* 0xCB  ACS                     */,
  "ATN"    /* 0xCC  ATN                     */,
  "LN"     /* 0xCD  LN                      */,
  "EXP"    /* 0xCE  EXP                     */,
  "INT"    /* 0xCF  INT                     */,
  "SQR"    /* 0xD0  SQR                     */,
  "SGN"    /* 0xD1  SGN                     */,
  "ABS"    /* 0xD2  ABS                     */,
  "PEEK"   /* 0xD3  PEEK                    */,
  "USR"    /* 0xD4  USR                     */,
  "STR$"   /* 0xD5  STR$                    */,
  "CHR$"   /* 0xD6  CHR$                    */,
  "NOT"    /* 0xD7  NOT                     */,
  "**"     /* 0xD8  **                      */,
  "OR"     /* 0xD9  OR                      */,
  "AND"    /* 0xDA  AND                     */,
  "<="     /* 0xDB  <=                      */,
  ">="     /* 0xDC  >=                      */,
  "<>"     /* 0xDD  <>                      */,
  "THEN"   /* 0xDE  THEN                    */,
  "TO"     /* 0xDF  TO                      */,
  "STEP"   /* 0xE0  STEP                    */,
  "LPRINT" /* 0xE1  LPRINT                  */,
  "LLIST"  /* 0xE2  LLIST                   */,
  "STOP"   /* 0xE3  STOP                    */,
  "SLOW"   /* 0xE4  SLOW                    */,
  "FAST"   /* 0xE5  FAST                    */,
  "NEW"    /* 0xE6  NEW                     */,
  "SCROLL" /* 0xE7  SCROLL                  */,
  "CONT"   /* 0xE8  CONT                    */,
  "DIM"    /* 0xE9  DIM                     */,
  "REM"    /* 0xEA  REM                     */,
  "FOR"    /* 0xEB  FOR                     */,
  "GOTO"   /* 0xEC  GOTO                    */,
  "GOSUB"  /* 0xED  GOSUB                   */,
  "INPUT"  /* 0xEE  INPUT                   */,
  "LOAD"   /* 0xEF  LOAD                    */,
  "LIST"   /* 0xF0  LIST                    */,
  "LET"    /* 0xF1  LET                     */,
  "PAUSE"  /* 0xF2  PAUSE                   */,
  "NEXT"   /* 0xF3  NEXT                    */,
  "POKE"   /* 0xF4  POKE                    */,
  "PRINT"  /* 0xF5  PRINT                   */,
  "PLOT"   /* 0xF6  PLOT                    */,
  "RUN"    /* 0xF7  RUN                     */,
  "SAVE"   /* 0xF8  SAVE                    */,
  "RAND"   /* 0xF9  RAND                    */,
  "IF"     /* 0xFA  IF                      */,
  "CLS"    /* 0xFB  CLS                     */,
  "UNPLOT" /* 0xFC  UNPLOT                  */,
  "CLEAR"  /* 0xFD  CLEAR                   */,
  "RETURN" /* 0xFE  RETURN                  */,
  "COPY"   /* 0xFF  COPY                    */,
};

#endif /* ZX81_Character_Table_h */