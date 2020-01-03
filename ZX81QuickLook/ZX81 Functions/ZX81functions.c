//
//  ZX81functions.c
//  ZX81QuickLook
//
//  Created by Sebastien Boisvert on 2019-09-29.
//  Copyright © 2019 Sebastien Boisvert. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include "ZX81-Character-Table.h"
#include "ZX81functions.h"

#include "Third Party/p2raw.c"


// PEEKs the value of a 2-byte word (2nd byte MSB)
unsigned int getWord(FILE *in) {
    return (fgetc(in) + (fgetc(in) << 8));
}

// reads in a single byte
unsigned int getByte(FILE *in) {
    return fgetc(in);
}


/// Given a FILE input, checks the file to make sure it is not invalid/corrupt,
/// and assings various values into a ZX81_Program struct, which are the offsets
/// within the file (useful for seeking).
/// @param in FILE object for the BASIC program
/// @return a ZX81_Program struct object
struct ZX81_Program checkFile(FILE *in) {
    
    // rewind the file
    fseek(in, 0, SEEK_SET);
    
    // set basic settings of the struct
    struct ZX81_Program program;
    program.PROGRAM = (PROGRAM_START - FILE_START); // first byte of basic
    program.status = valid; // assume valid unless otherwise noted
    
    // get info on the file for its length
    struct stat fileStat;
    int fd = fileno(in);
    fstat(fd,&fileStat);
    
    // make sure we can read up to program start
    if(fileStat.st_size < program.PROGRAM) {
        program.status = invalid;
        return program;
    } else {
        program.fileSize = fileStat.st_size;
    }
    
    // read in some of the system variables
    
    fseek(in, 3, SEEK_CUR); // skip VERSN & E_PPC
    
    unsigned int DFILE = getWord(in); // D_FILE, which marks end of program
    program.D_FILE = (DFILE - FILE_START);
    
    fseek(in,2, SEEK_CUR); // skip up to VARS
    
    unsigned int VARS = getWord(in); // VARS, indicating end of D_FILE
    program.VARS = (VARS - FILE_START);
    
    fseek(in,2, SEEK_CUR); // skip to E_LINE
    
    unsigned int ELINE = getWord(in); // should be last byte of BASIC in file
    program.E_LINE = (ELINE - FILE_START);
    
    // make sure the values are sane for the file size
    if((ELINE - FILE_START) > fileStat.st_size) {
        program.status = corrupt;
    }
    
    return program;
}

/* Append to buffer printf style; takes a string_buffer struct as the buffer
 to 'print' into, with other parameters being the usual as any printf statement;
 ensures the buffer isn't overrun, and returns the amount of characters that got
 printed into the buffer. */
int buffer_print(struct string_buffer buffer, const char* restrict format, ...) {
    unsigned long length = strlen(buffer.buffer); // current length of string in buffer
    va_list aptr;
    int ret;
    
    va_start(aptr, format);
    // append to end of the current string in the buffer
    ret = vsnprintf(buffer.buffer+length, buffer.buffer_length-length, format, aptr);
    va_end(aptr);
    
    return(ret);
}

// outputs the line number, followed by a space
void output_line_number(struct string_buffer codeBuffer, unsigned int lineNumber) {
    
    /* the possible characters to display. Initially the first character
     is a space, which will be replaced by '0' once we start printing
     out actual numbers; this will pad out the left side with spaces. */
    char numberChars[] =" 123456789ABCDEFG";
    
    /* we'll start the divider at 1000 then divide by ten each iteration to
     get 1000's, 100's, 10's & 1's */
    int divider = 1000;
    int value;
    
    while(divider > 0) {
        
        // get digit value
        value = lineNumber / divider;
        
        // ensure sane value
        if(value > 16) {
            value = 16;
        }
        
        /* check if we'll have something printable, or we're down to single
         digit (might be a 0 numbered line) */
        if(value || divider < 10) {
            numberChars[0] = '0'; // change space to '0'
        }
        
        
        // output character
        buffer_print(codeBuffer, "%c",numberChars[value]);
        
        // prepare for next loop
        lineNumber = lineNumber % divider;
        divider = divider / 10;
    }
    
    /* space after line number; we could also output the 'current line' cursor
     here as appropritate if wanted */
    buffer_print(codeBuffer, " ");
    
}

/* Processes a BASIC line; the file pointer is at the byte right after the line
 number. No length limit is applied - the line is output as a single line,
 terminated by a '\n'; it is assumed the consumer will wrap the lines as
 needed. */
void process_basic_line(struct string_buffer codeBuffer, FILE *in) {
    
    /* flag to keep track of the need to print a space for tokens; initially
     set to suppress space when printing a line; some tokens will dictate if
     the leading space for a subsequent token should be supressed */
    bool suppressSpace = true;

    
    /* read in the line number and output it (line number stored differently
     than words in ZX81 BASIC) */
    unsigned int line_number = (fgetc(in) << 8) + fgetc(in);
    output_line_number(codeBuffer, line_number);
    
    // get the length of the rest of the BASIC line
    unsigned int length = getWord(in);
    
    // iterate over the bytes up to the end-of-line marker
    int linePosition = 0; // stores offset within the line itself
    while(linePosition < length) {
        
        unsigned int byte = getByte(in);
        linePosition++;
        
        if(0x7e == byte) {
            /* if number marker, skip the 5 next bytes, but only up to
             the end of the line */

            int numberToSeek = 5;
            
            if(linePosition + 5 > length) {
                numberToSeek = (linePosition + 5 - length);
            }
            
            fseek(in, numberToSeek, SEEK_CUR);
            linePosition += numberToSeek;
            
        } else if (0x76 == byte) {
            
            /* newline encountered, seek to end of line (either
             we're in the middle of a line, or at the end of it) */
            fseek(in, length-linePosition, SEEK_CUR);
            
            // done with this line, break loop
            break;
            
        } else {
            
            // printable character, output it based on table
            
            // get charater(s) from table
            const char *characters = zx81_table[byte];
            
            // for tokens, check if we need to print the leading space
            if(byte & 1<<6) { // if token
                if(!suppressSpace) { // if not already suppressing space
                    if( !(byte & 1<<7)
                       // if not high token (bit 7 is zero)
                       || ((byte >= 0xC0 && byte <= 0xD7) ||
                        strpbrk(characters,"<>*\"")) )
                        // or a token that does not print a leading space
                    {
                        suppressSpace = true; // suppress the space
                    }
                }
                
                if(!suppressSpace) {
                    buffer_print(codeBuffer, " "); // print leading space for token
                }
            }
            
            // output the character/token
            buffer_print(codeBuffer,"%s",characters);
            
            // assume we'll be allowing future token leading space
            suppressSpace = false;
            
            // for tokens, check if we need to print a trailing space
            if(byte & 1<<6) { // if token
                if(byte & 1<<7 && !strpbrk(characters,"<>*\"")) {
                    /* if a token that has a trailing space, print it out and
                     suppress the leading space for any subsequent token */
                    buffer_print(codeBuffer, " ");
                    suppressSpace = true;
                }
            }
        }
    }
    
    // output LF at the end of the BASIC line
    buffer_print(codeBuffer, "\n");
}

/* Produces the BASIC listing. First parameter is a string_buffer struct to
output the listing to, and second is the FILE input of the program file. */
void get_basic_listing(struct string_buffer codeBuffer, FILE *in) {
    
    fpos_t position; // holds current seek position in the file
    
    // 'clear' the buffer
    codeBuffer.buffer[0] = '\0';

    struct ZX81_Program program_file = checkFile(in);
    
    // check file is valid
    if(invalid == program_file.status) {
        buffer_print(codeBuffer, "INVALID BASIC FILE");
    } else if (corrupt == program_file.status) {
        buffer_print(codeBuffer, "CORRUPT BASIC FILE");
    }
    
    if(valid != program_file.status) {
        return;
    }
    
    // go to the first byte of BASIC
    fseek(in, program_file.PROGRAM, SEEK_SET);
    
    // read in BASIC lines until we've reached the end of the program
    while( (!fgetpos(in, &position)) &&
          (position < (program_file.D_FILE - 1)) ) {
        process_basic_line(codeBuffer, in);
    }
}


/// Produces the screen output.
/// If the screen doesn't have any real characters to display (nothing besides
/// 0x00 and 0x76), the resulting output buffer will be an empty (null-terminated)
/// string.
/// @param screenBuffer string_buffer struct to output the screen display to
/// @param basic_file FILE input of the BASIC program file
void get_screen(struct string_buffer screenBuffer, FILE *basic_file) {
    
    struct ZX81_Program program_file = checkFile(basic_file);
    
    // check file is valid
    if(invalid == program_file.status) {
        buffer_print(screenBuffer, "INVALID BASIC FILE");
    } else if (corrupt == program_file.status) {
        buffer_print(screenBuffer, "CORRUPT BASIC FILE");
    }
    
    if(valid != program_file.status) {
        return;
    }

    unsigned int position = 0; // holds current offset
    
    // get the first byte of display (1 byte past D_FILE)
    unsigned int DFILE_start = program_file.D_FILE +1;
    unsigned int display_length = program_file.VARS - DFILE_start;
    
    fseek(basic_file, DFILE_start, SEEK_SET);
    
    // 'clear' the buffer
    screenBuffer.buffer[0] = '\0';
    
    /* Read in the screen data, replacing any 0x76 with a newline.
     
     The saved screen may be collapsed, so we simply iterate through each byte
     until we've iterated through its entire length */
    while(position < display_length)  {
        
        int byte = getc(basic_file);
        position++;
        
        if(0x76 == byte) {
            buffer_print(screenBuffer, "\n");
        } else {
            const char *character = zx81_table[byte];
            buffer_print(screenBuffer, "%s", character);
        }
    }

    /* finally, check if there's actual data (other than space & newline
        characters */
    if(strspn(screenBuffer.buffer," \n")) {
        // clear the buffer to indicate no content
        screenBuffer.buffer[0] = '\0';
    }
}


/// Generates the sound of the save file as a WAV file buffer. Much of this is
/// taken from the p2raw code.
/// @param basic_file FILE input of the BASIC program file
/// @param outBuffer a pointer to a char* buffer; upon successful processing
/// of the BASIC file, this will contain the WAV data.
/// @param outBufferSize a pointer to a size_t variable; upon successful
/// processing this will contain the size of the buffer
void get_save_sound(FILE *basic_file, char **outBuffer, size_t *outBufferSize) {
    
    unsigned long n, k;
    
    // allocate temporary buffer to write data to
    BYTE *basicFileDataBuffer;
    if ((basicFileDataBuffer = (BYTE *) malloc (MUCH))==NULL) {
        return;
    }
    
    /* create a in-memory FILE object to use with the p2raw functions, which
     work on a FILE object. */
    char *newSoundBuffer;
    size_t newSoundBufferSize;
    FILE *outFile = open_memstream(&newSoundBuffer, &newSoundBufferSize);
    
    if(outFile) {
        
        /* The p2raw functions only produce the raw data, so we need to create
         a WAV file header first. */
        create_wav_data(outFile);

        // rewind to start of BASIC file
        fseek(basic_file, 0, SEEK_SET);
        
        // Create some bit of silence at the start of the WAV file
        memset (basicFileDataBuffer, 0x80, MUCH);
        for (k=0; k < 22; k++) {
            fwrite (basicFileDataBuffer, 1, 100, outFile);
        }
        
        /* ZX81 loading expects a program name, so we'll include one; as the
         filename of the file been processed isn't guaranteed to have only
         printable characters, we'll use a generic name. */
        char *filename = "PRODUCED BY ZX81QUICKLOOK BY SEBASTIEN BOISVERt";
        char convertedFilename [strlen(filename)+1];
        
        // figure out the ZX81 characters to use for output into the converted name
        for (k=0; filename[k]; k++)
        {
            char letter[2] = "\0";
            sprintf(letter, "%c", filename[k]);
            for(int i = 0; i< 0xBF; i++) {
                
                if(!(strcmp(zx81_table[i],letter))) {
                    convertedFilename[k] = i;
                }
            }
        }

        // Output the name; end-of-name marker (lowercased letter) already set
        n = strlen(filename); // use filename, as converted name will have null's
        for (k=0; k < n; k++) {
            RawOut (outFile, convertedFilename[k]);
        }
        
        
        // Now process the BASIC file
        while((n = fread (basicFileDataBuffer, 1, 1000, basic_file))) {
            
            for (k=0; k < n; k++) {
                RawOut (outFile, basicFileDataBuffer[k]);
            }
        }
                
        free (basicFileDataBuffer);
        

        // Flush out the FILE output so buffers are updated
        fflush(outFile);
        
        // this is the current total size of sound file
        int bufferEnd = (int)newSoundBufferSize;

        // Check if audio data length is uneven, and pad as necessary
        if(0 != bufferEnd % 2) {
            char pad = 0x80;
            fwrite(&pad, sizeof(char), 1, outFile);
            bufferEnd++;
        }

        // calcuate the final sizes for chunk and sound data
        int dataSize = (bufferEnd - 44); // the current file size minus header
        int totalSize = (bufferEnd - 8); // current file size minus first 8 bytes

        // write the values at the appropriate spots in the header
        fseek(outFile,4, SEEK_SET);
        fwrite(&totalSize, sizeof(int), 1, outFile);
        fseek(outFile,40, SEEK_SET);
        fwrite(&dataSize, sizeof(int), 1, outFile);
        
        // seek back to end of file before closing out the file.
        fseek(outFile,bufferEnd, SEEK_SET);
        fclose(outFile);
        
        // assign the buffer info to the passed in pointers
        if(outBuffer && outBufferSize) {
            *outBuffer = newSoundBuffer;
            *outBufferSize = newSoundBufferSize;
        }
    }
}


/// Creates the WAVE header for the file header of writing out the raw sound
/// data. Leaves the size data info to be filled in later.
/// @param outFile output file as a FILE pointer
void create_wav_data(FILE *outFile)
{
    // The output of the raw data from p2raw
    short channels = 1;      // mono
    short sampleBits = 8;    // 8 bits
    int sampleRate = 22050;  // 22050 Khz
    
    /* Calculate the values; the sound data and total file size will not be
     known yet so only placeholder values are used.
     
     Format per http://www.topherlee.com/software/pcm-tut-wavformat.html ,
     http://soundfile.sapp.org/doc/WaveFormat/ and
     http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
     
     */
    int rate = (channels*sampleBits*sampleRate)/8;
    short blockSize = (channels*sampleBits)/8;
    int chunkSize = 16; // chuck size for the sample, rate, etc. data
    short audioFormat = 1; // PCM
    int sampleDataSize = 0; // these two to be filled in later
    int totalFileSize = 0;
    
    // create the header
    fwrite("RIFF", sizeof(char), 4,outFile);
    fwrite(&totalFileSize, sizeof(int), 1, outFile);
    fwrite("WAVE", sizeof(char), 4, outFile);
    fwrite("fmt ", sizeof(char), 4, outFile);
    fwrite(&chunkSize, sizeof(int),1,outFile);
    fwrite(&audioFormat, sizeof(short), 1, outFile);
    fwrite(&channels, sizeof(short),1,outFile);
    fwrite(&sampleRate, sizeof(int), 1, outFile);
    fwrite(&rate, sizeof(int), 1, outFile);
    fwrite(&blockSize, sizeof(short), 1, outFile);
    fwrite(&sampleBits, sizeof(short), 1, outFile);
    fwrite("data", sizeof(char), 4, outFile);
    fwrite(&sampleDataSize, sizeof(int), 1, outFile);
}
