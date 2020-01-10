#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

#include "ZX81functions.h"

#define autoWidthKey    @"autoWidth"
#define fontSizeKey     @"fontSize"
#define screenSizeKey   @"screenSize"
#define waveBarsKey     @"waveBars"
#define progressFreqKey @"progressFreq"
#define windowHeightKey @"windowHeight"
#define windowWidthKey  @"windowWidth"

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview,
                               CFURLRef url, CFStringRef contentTypeUTI,
                               CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview,
                               CFURLRef url, CFStringRef contentTypeUTI,
                               CFDictionaryRef options)
{
    @autoreleasepool
    {
        // Setup defaults for the bundle's domain, and set the default settings
        NSUserDefaults *defaults = [[NSUserDefaults alloc]
                                    initWithSuiteName:@"com.sebastienboisvert.ZX81QuickLook"];
        [defaults registerDefaults:
         @{autoWidthKey:@(NO),   // auto-width
           fontSizeKey:@(15),    // code font size
           screenSizeKey:@(15),  // screen font size
           waveBarsKey:@(400),   // number of bars for soundwave
           progressFreqKey:@(25) // update frequency in ms
         }];
        
        /* The line buffer for output of basic/screen lines. ZX81 BASIC cannot be
         larger than 16KB technically, but this assumes a (theoretical) 16KB statement
         filled with the largest token (6 characters + 1 space each), with a 4-digit
         line number + space + end character (0x76) for total output of
         (5 + (16384*7) + 1), which isn't really possible for a ZX81 BASIC program but
         this makes it safe to assume as possible maximum (rounded up). */
        char basic_output[114700] = "COULD NOT PROCESS BASIC FILE";

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
        NSData *soundData = nil;
        
        // process the BASIC .p file
        char filename[1025];
        if(CFURLGetFileSystemRepresentation(url, true, (UInt8 *)filename,
                                            sizeof(filename)-1)) {
            
            FILE *basic_file = fopen(filename, "r");
            program = checkFile(basic_file);
            
            if(valid == program.status) {
                
                fileSize = program.fileSize;

                // generate BASIC listing and screen preview
                get_basic_listing(codeBuffer,basic_file);
                get_screen(screenBuffer,basic_file);

                // generate the sound
                char *soundBuffer = NULL;
                size_t soundBufferSize = 0;
                get_save_sound(basic_file, &soundBuffer, &soundBufferSize);
                if(NULL != soundBuffer) {
                    soundData = [NSData dataWithBytesNoCopy:soundBuffer
                                                     length:soundBufferSize];
                }                
            }
            
            fclose(basic_file);
            
        } else {
            
            return kQLReturnNoError; // don't output anything if can't open file
        }
        
        // Get resources to attach to the HTML
        NSBundle *bundle = [NSBundle bundleWithIdentifier:@"com.sebastienboisvert.ZX81QuickLook"];
        NSURL *cssFile = [bundle URLForResource:@"source" withExtension:@"css"];
        NSURL *htmlFile = [bundle URLForResource:@"source" withExtension:@"html"];
        NSURL *fontFile = [bundle URLForResource:@"zx81_vdu" withExtension:@"ttf"];
        NSURL *scriptFile = [bundle URLForResource:@"source" withExtension: @"js"];

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

        // get/set the various defaults
        NSUInteger bars = [defaults integerForKey:waveBarsKey];
        bars = MIN(2000,(MAX(bars,400))); // keep range between 400-2000

        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"NUMBER_BARS"
                                                           withString:@(bars).stringValue];
        
        BOOL autoSelected = [defaults boolForKey:autoWidthKey];
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"AUTO_DEFAULT"
                                                           withString:(autoSelected ? @"selected" : @"")];
        
        NSUInteger codeSize = [defaults integerForKey:fontSizeKey];
        codeSize = MIN(50,(MAX(codeSize,10))); // range 10-50
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"CODE_SIZE"
                                                           withString:@(codeSize).stringValue];

        NSUInteger screenSize = [defaults integerForKey:screenSizeKey];
        screenSize = MIN(50,(MAX(screenSize,10))); // range 10-50
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"SCREEN_SIZE"
                                                           withString:@(screenSize).stringValue];

        NSUInteger updateFreq = [defaults integerForKey:progressFreqKey];
        updateFreq = MIN(500,(MAX(updateFreq,25))); // range 25-500
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"WAVE_FREQ"
                                                           withString:@(updateFreq).stringValue];
        
        // the desired window height/width, if any set, otherwise use the defaults
        NSUInteger windowWidth = [defaults integerForKey:windowWidthKey];
        if(!windowWidth) {
            windowWidth = [bundle.infoDictionary[@"QLPreviewWidth"] unsignedIntValue];
        }
        
        NSUInteger windowHeight = [defaults integerForKey:windowHeightKey];
        if(!windowHeight) {
            windowHeight = [bundle.infoDictionary[@"QLPreviewHeight"] unsignedIntValue];
        }

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
        NSString *buttonStyle = @"inline-block";
        if(!strlen(screen_output)) {
            buttonStyle = @"none";
        }

        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"SCREEN_BUTTON_DISPLAY"
                                                           withString:buttonStyle];
        
        // hide Audio button if not a valid file
        buttonStyle = @"inline-block";
        if(!soundData || valid != program.status) {
            buttonStyle = @"none";
        }
        
        htmlString = [htmlString stringByReplacingOccurrencesOfString:@"AUDIO_BUTTON_DISPLAY"
                                                           withString:buttonStyle];


        // substitute in the sound data as base64 encoded
        if(soundData) {
            NSString *base64SoundDataString = [soundData base64EncodedStringWithOptions:0];
            
            htmlString = [htmlString stringByReplacingOccurrencesOfString:@"BASE64_DATA"
                                                               withString:base64SoundDataString];
        }
        
        // create data resources
        NSData *cssData = [NSData dataWithContentsOfURL:cssFile];
        NSData *fontData = [NSData dataWithContentsOfURL:fontFile];
        NSData *scriptData = [NSData dataWithContentsOfURL:scriptFile];
        NSData *htmlData = [htmlString dataUsingEncoding:NSUTF8StringEncoding];

        // Put metadata and attachment in a dictionary
        NSDictionary *properties = @{ // properties for the HTML data
            (__bridge NSString *)kQLPreviewPropertyTextEncodingNameKey : @"UTF-8",
            (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/html",
            
            // properties for attaching the CSS stylesheet, font and script files
            (__bridge NSString *)kQLPreviewPropertyAttachmentsKey : @{
                @"source.css" : @{
                    (__bridge NSString *)kQLPreviewPropertyTextEncodingNameKey : @"UTF-8",
                    (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/css",
                    (__bridge NSString *)kQLPreviewPropertyAttachmentDataKey: cssData,
                },
                @"zx81_vdu.ttf" : @{
                    (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"font/ttf",
                    (__bridge NSString *)kQLPreviewPropertyAttachmentDataKey: fontData,
                },
                @"source.js" : @{
                   (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/javascript",
                   (__bridge NSString *)kQLPreviewPropertyAttachmentDataKey: scriptData,
               },
            },
            // preview window size
            (__bridge NSString *)kQLPreviewPropertyHeightKey : @(windowHeight),
            (__bridge NSString *)kQLPreviewPropertyWidthKey : @(windowWidth)
        };
        
        // Pass preview data and metadata/attachment dictionary to QuickLook
        QLPreviewRequestSetDataRepresentation(preview,
                                              (__bridge CFDataRef)htmlData,
                                              kUTTypeHTML,
                                              (__bridge CFDictionaryRef)properties);
    }
    
    return kQLReturnNoError;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
