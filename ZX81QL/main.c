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

    char basic_output[114700] = "COULD NOT LOAD BASIC FILE";

    struct string_buffer code_buffer;
    code_buffer.buffer = basic_output;
    code_buffer.buffer_length = sizeof(basic_output);
    
    char screen_output[800] = "NO SCREEN FOUND";
    struct string_buffer screen_buffer;
    screen_buffer.buffer = screen_output;
    screen_buffer.buffer_length = sizeof(screen_output);
    
    FILE *in = fopen(argv[1], "r");
    
    if(NULL != in) {
        get_basic_listing(code_buffer,in);
        get_screen(screen_buffer, in);
        fclose(in);
    }
    
    printf("BASIC:\n%s\nSCREEN:\n%s\n",basic_output,screen_output);
    
    return 0;
}
