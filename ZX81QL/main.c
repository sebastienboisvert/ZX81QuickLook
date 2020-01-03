//
//  main.c
//  ZX81QL
//
//  Created by Sebastien on 2019-10-20.
//  Copyright © 2019 Sebastien Boisvert. All rights reserved.
//

#include <stdio.h>

#include "ZX81functions.h"

int main(int argc, const char * argv[]) {

    FILE *in = fopen(argv[1], "r");

    if(NULL != in) {
        
        char basic_output[114700] = "COULD NOT LOAD BASIC FILE";

        struct ZX81_Program program;
        
        struct string_buffer code_buffer;
        code_buffer.buffer = basic_output;
        code_buffer.buffer_length = sizeof(basic_output);
        
        char screen_output[800] = "NO SCREEN FOUND";
        struct string_buffer screen_buffer;
        screen_buffer.buffer = screen_output;
        screen_buffer.buffer_length = sizeof(screen_output);
        
        program = checkFile(in);
    
        if(valid == program.status) {
            printf("Program: %0X\n",FILE_START+program.PROGRAM);
            printf("DFILE: %0.0X\n",FILE_START+program.D_FILE);
            printf("VARS: %0X\n",FILE_START+program.VARS);
            printf("ELINE: %0X\n\n",FILE_START+program.E_LINE);
            
            get_basic_listing(code_buffer,in);
            get_screen(screen_buffer, in);

            printf("BASIC:\n%s\nSCREEN:\n%s\n",basic_output,screen_output);

        }
        else {
            printf("Invalid BASIC file.");
        }
        
        fclose(in);
    }
    else {
        if(argv[1]) {
            printf("Could not open file %s.\n",argv[1]);
        } else {
            printf("No file provided.\n");
        }
    }
    
    return 0;
}
