#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

#include "ZX81functions.h"

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
    @autoreleasepool
    {
        /* The line buffer for output of basic/screen lines. ZX81 BASIC cannot be
         larger than 16KB technically, but this assumes a (theoritical) 16KB statement
         filled with the largest token (6 characters + 1 space each), with a 4-digit
         line number + space + end character (0x76) for total output of
         (5 + (16384*7) + 1), which isn't really possible for a ZX81 BASIC program but
         this makes it safe to assume as possible maximum (rounded up). */
        char basic_output[114700] = "COULD NOT LOAD BASIC FILE";

        struct string_buffer codeBuffer;
        codeBuffer.buffer = basic_output;
        codeBuffer.buffer_length = sizeof(basic_output);
        
        /* Screen buffer is always maximum 792 bytes. */
        char screen_output[800];
        struct string_buffer screenBuffer;
        screenBuffer.buffer = screen_output;
        screenBuffer.buffer_length = sizeof(screen_output);
        
        struct ZX81_Program program;
        NSUInteger fileSize = 0;
        
        // process the BASIC .p file
        char filename[1025];
        if(CFURLGetFileSystemRepresentation(url, true, (UInt8 *)filename,
                                            sizeof(filename)-1)) {
            
            FILE *basic_file = fopen(filename, "r");
            program = checkFile(basic_file);
            
            if(valid == program.status) {
                get_basic_listing(codeBuffer,basic_file);
                get_screen(screenBuffer,basic_file);
                fileSize = program.fileSize;
            }
            
            fclose(basic_file);
            
        } else {
            
            return noErr; // don't output anything if can't open file
        }
        
        // Load a CSS stylesheet to attach to the HTML
        NSBundle *bundle = [NSBundle bundleWithIdentifier:@"com.sebastienboisvert.ZX81QuickLook"];
        NSURL *cssFile = [bundle URLForResource:@"source" withExtension:@"css"];
        NSURL *htmlFile = [bundle URLForResource:@"source" withExtension:@"html"];
        NSURL *fontFile = [bundle URLForResource:@"zx81_vdu" withExtension:@"ttf"];
        
        NSString *htmlString = [NSString stringWithContentsOfURL:htmlFile
                                                        encoding:NSUTF8StringEncoding
                                                           error:nil];
        NSString *basicString = [NSString stringWithCString:basic_output
                                                   encoding:NSASCIIStringEncoding];
        NSString *screenString = [NSString stringWithCString:screen_output
                                                    encoding:NSASCIIStringEncoding];
        
 
        /* escape any characters (like '<', '>', etc.) in the BASIC code string
           for HTML insertion */
        basicString = [[NSXMLNode textWithStringValue:basicString] XMLString];
        screenString = [[NSXMLNode textWithStringValue:screenString] XMLString];

        // replace tokens with strings in HTML template
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"BASIC_HERE"
                                                           withString:basicString];
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"SCREEN_HERE"
                                                           withString:screenString];
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"PROGRAM_NAME"
                                                           withString:[(__bridge NSURL*)url lastPathComponent]];
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"PROGRAM_SIZE"
                                              withString:[NSString stringWithFormat:@"%lu bytes",(unsigned long)fileSize]];

        
        // check if the Screen button needs to be hidden
        NSString *screenButtonStyle = @"inline-block";
        if(!strlen(screen_output)) {
            screenButtonStyle = @"none";
        }

        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"SCREEN_BUTTON_DISPLAY"
                                                           withString:screenButtonStyle];

        // create data resources
        NSData *htmlData = [htmlString dataUsingEncoding:NSUTF8StringEncoding];
        NSData *cssData = [NSData dataWithContentsOfURL:cssFile];
        NSData *fontData = [NSData dataWithContentsOfURL:fontFile];
        
        // Put metadata and attachment in a dictionary
        NSDictionary *properties = @{ // properties for the HTML data
            (__bridge NSString *)kQLPreviewPropertyTextEncodingNameKey : @"UTF-8",
            (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/html",
            // properties for attaching the CSS stylesheet & font
            (__bridge NSString *)kQLPreviewPropertyAttachmentsKey : @{
                @"source.css" : @{
                    (__bridge NSString *)kQLPreviewPropertyTextEncodingNameKey : @"UTF-8",
                    (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/css",
                    (__bridge NSString *)kQLPreviewPropertyAttachmentDataKey: cssData,
                },
                @"zx81_vdu.ttf" : @{
                    (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"font/ttf",
                    (__bridge NSString *)kQLPreviewPropertyAttachmentDataKey: fontData,
                }
            },
        };
        
        // Pass preview data and metadata/attachment dictionary to QuickLook
        QLPreviewRequestSetDataRepresentation(preview,
                                              (__bridge CFDataRef)htmlData,
                                              kUTTypeHTML,
                                              (__bridge CFDictionaryRef)properties);
    }
    
    return noErr;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
