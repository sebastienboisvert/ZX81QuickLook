//
//  ZX81functions.h
//  ZX81QuickLook
//
//  Created by Sebastien Boisvert on 2019-09-29.
//  Copyright © 2019 Sebastien Boisvert. All rights reserved.
//

#ifndef ZX81functions_h
#define ZX81functions_h

#include <stdio.h>

// Some system defines

/* the first byte saved from memory (byte 0 of the .P file) in the ZX81 memory;
 also used as the offset within the buffer during calculations */
#define FILE_START 0x4009

/* the first byte of BASIC in ZX81 memory */
#define PROGRAM_START 0x407D


// status enum for the validity of the file
typedef enum : unsigned int {
    valid,
    invalid,
    corrupt,
} ZX81_File_Status;

/* The following struct holds the info about the file; the addresses are
 adjusted for the correct offset within the file. */
struct ZX81_Program {
    unsigned int PROGRAM;   // location of the start of the program
    unsigned int D_FILE;    // location of D_FILE, within the file
    unsigned int VARS;      // location of VARS, within the file
    unsigned int E_LINE;    // location of E_LINE, within the file
    long long fileSize;     // the size, in bytes, of the program file
    ZX81_File_Status status; // if the file is valid, invalid or corrupt
};

/* Struct to hold reference to an allocated string buffer, and its max length.*/
struct string_buffer {
    char * buffer;
    unsigned int buffer_length;
};


/// Checks the file and returns a ZX81_Program struct
/// @param basicFile the FILE pointer to a ZX81 BASIC program file
/// @returns a ZX81_Program struct filled in with the appropriate info
struct ZX81_Program checkFile(FILE *basicFile);


/// Produces a listing into the provided buffer.
/// @param buffer the string buffer to use for listing output
/// @param basicFile the FILE pointer to a ZX81 BASIC program file
void get_basic_listing(struct string_buffer buffer,FILE *basicFile);

/// Produces a screen output into the provided buffer.
/// @param buffer the string buffer to use for screen output
/// @param basicFile the FILE pointer to a ZX81 BASIC program file
void get_screen(struct string_buffer buffer, FILE *basicFile);

void get_save_sound(FILE *basic_file, char **outBuffer, size_t *outBufferSize);
void create_wav_data(FILE *outFile);

#endif /* ZX81functions_h */
