# ZX81QuickLook

## The idea

The [Sinclair ZX81](https://en.wikipedia.org/wiki/ZX81) was my first real computer when I was a kid, so I’ve routinely hung around various Sinclair-based computer forums (such as [Sinclair ZX World](https://www.sinclairzxworld.com)), and often check out various programs some people still produce for the ZX81 using emulators. One day the thought occurred that I could create a Quicklook plugin to generate a preview of the listing for ZX81 files.

My original vision was simply generating the BASIC listing only, but later also thought that having the ability to output the original save audio would be a neat addition. Shortly after it also occurred to me that being able to show the saved screen (which is part of the program file) would be another neat trick. However trying to show all 3 things in a single view (as I had envisioned for listing + audio) would likely look crowded, so I thought some sort of tabbed interface might be best.

### Researching things…

Having created another Quicklook generator before, I knew that it was possible to use HTML as the ‘preview’ mechanism, so the first order of things was to make sure it was feasible to use some sort of tabbing interface within a Quicklook context - especially if using Javascript would work properly. A quick ’n dirty test revealed this worked and I was good to get started on coding things up.

I remembered quite a bit about the ZX81’s memory layout and BASIC program structure, but needed a refresher, especially about special control characters and how they’d be handled in REM statements (I remembered losing many ZX81 coding sessions because of an inadvertent POKE issued that would send the ZX81 into an inescapable refresh loop when it tried to list the program!)

After reading up on the [BASIC line structure](http://www.worldofspectrum.org/ZX81BasicProgramming/chap27.html) in memory, I did a few tests to figure out how a real ZX81 dealt with some of the control characters when they’re in strategically placed spots in the program, which I documented in [this post](https://sinclairzxworld.com/viewtopic.php?f=5&t=3342) - turns out the ZX81’s listing routines disregard line lengths as a tradeoff for listing speed, with sometimes odd results. Understanding this was key to make sure to avoid corrupting the listing.

### The search for code

My goal for this project was to write it with clear separation of the functions that process the program files from the presentation, so that those functions could easily be reused by someone else, or as part of another project if needed.

I had seen several programs published for creating listings of ZX81 programs - most produced ASCII-only output, and just about all of them were Windows executables. I figured I’d be able to grab the routines from some of the open source ones as a jumping off point, and found a few on GitHub: [zx81utils](https://github.com/leiradel/zx81utils) by @leiradel (the read up on how he created the two different listing generators is worth the read!), and [zx81putil](https://github.com/mcleod-ideafix/zx81putil). There were a couple of other ones out on the Internet too.

The problem with most of them however was that their main processing functions were all within the `main` function call, and so were not easily “extractable” to use separately, and nearly all of them had hard-coded assumptions when dealing with tokens (more on this later). At this point I figured it’d be easier (and, probably, more fun) to write up the routines myself.

After a few hours of coding, my first text-only output tests produced good results, and I was ready to incorporate its output into the main Quicklook generator that produces the HTML for the preview. This is where I took a refresher on CSS styling, as well as looking up how to use [flex boxes](https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Flexible_Box_Layout/Basic_Concepts_of_Flexbox) to get the scrolling layout I wanted to achieve.

### Getting the s p a cing ri g h t

The next phase was probably one of the hardest, but also a lot of fun. But first, some background:

To conserve memory, and also likely to save users' fingers while typing on its membrane keyboard, the ZX81’s BASIC includes what’s known as ‘tokens’; essentially, they’re the function names that are automatically spelled out for you while in a specific keyboard entry mode, so typing in a line like:

```
10 PRINT ”HI”
```

only requires typing in the two digits, the letter ‘P’, and then the “HI” with quotes. When hitting ‘P’ the computer will automatically expand the function token the key represents (in that specific mode) to “PRINT”. The computer will also automatically add the necessary spacing where needed, so the space key doesn’t need to be hit when entering this line. It will also know what entry mode to use next so the letter 'P' can be used to reference variables (such as `PRINT P`) or used in strings (`PRINT ”ZX81 PROGRAM”`)

Many of the other programs that produce ZX81 listings make the assumption that the tokens have the needed spacing built into the token itself, but if one looks up [how the tokens are defined in the ZX81 ROM](https://cdn.hackaday.io/files/289631239152992/ZX81_dual_2018-02-09.htm#TOKEN_TABLE), it’s clear that’s not the case. So the ZX81’s listing routines automatically figure out the spacing, but that spacing can get tricky; take this line that includes multiple tokens as an example:

```
10 IF A<>8 THEN PRINT A;” IS NOT 8”
```

Here “IF”, “<>”, “THEN” and “PRINT” are all tokens; notice the spacing (or lack thereof) around them. If the spacing was assumed to be built-in for tokens (as many other programs do), something like the following would get output instead (note the extra spacing):

```
10  IF A <> 8 THEN  PRINT A;” IS NOT 8”
```

Here the ZX81 is smart not to include extra spaces after line numbers, between consecutive tokens or when tokens shouldn’t have any spacing (such as “<>”), so I wanted to replicate that behaviour for accuracy. That led me to brush up on my [Z80 assembly](http://z80.info) instructions, reading up on the [relevant functions](https://cdn.hackaday.io/files/289631239152992/ZX81_dual_2018-02-09.htm#LIST) in the [ZX81 ROM disassembly](https://cdn.hackaday.io/files/289631239152992/ZX81_dual_2018-02-09.htm), as well as brushing up on [ZX81 assembly programming](http://www.users.waitrose.com/~thunor/mmcoyzx81/) to figure out how the ZX81 processed BASIC lines for screen output. I worked on disassembling and understanding the ROM code on and off for a little over a week, which led to [this project](https://github.com/sebastienboisvert/ZX81Annotations).

Ultimately, it wasn’t feasible or practical (or even necessary) to replicate exactly what the ZX81 does into my code, but it gave me the understanding needed to properly handle output of tokens and their spacing. (You have to hand it to the creator(s) of the ZX81’s code for using internal flags, arithmetic carry flags, and judicious positioning of the tokens within the [ZX81’s character set](http://www.worldofspectrum.org/ZX81BasicProgramming/appxa.html) for cleverly working out when to add or suppress spacing.)

### Got sound?

After getting the code to generate both the listing and screen output working, the next step was generating the audio output. This part I had left for last since I wasn’t sure if I could even get audio to play in a Quicklook context, and needed to find some sort of visual waveform generator to display in the audio tab. I decided to find the waveform generator first and just use a random .WAV file for testing.

At first I looked at [waveplayer.js](https://github.com/michaeldzjap/waveplayer.js), but since Quicklook’s WebKit doesn’t seem to render `canvas` elements, that was a no-go. Continuing my search I found [Winds Waveform Example](https://github.com/kenhoff/winds-waveform-example) that is based on SVG elements - it had less dependencies, was less processor intensive, lighter and easy to customize, and worked perfectly in a Quicklook window :thumbsup:.  


 After tweaking the original code a bit to make it resize dynamically and getting it to show like I wanted, the next step was the actual ZX81 save audio generation; again there were several programs out there that did this (with the same problems as mentioned before), but I knew nothing of how the ZX81 generates the audio pulses for the 0’s and 1’s that get saved to tape, so I couldn’t write the routines myself. Luckily, Bert van Oortmarssen [wrote some code to do just this](https://odemar.home.xs4all.nl/zx81/zx81.htm) back in the late 90’s, and it was simple and self-contained enough that it could easily be incorporated into my code. After [reading up](http://soundfile.sapp.org/doc/WaveFormat/) on how to wrap the generated raw audio into a proper WAV format, I was ready to put it into the generated preview.

But I hit a snag.

Initially, I thought that Quicklook previews using HTML as output couldn’t do audio, because trying to load the WAV file using a ‘cid:' resource (as Apple’s documentation suggest for Quicklook HTML resources) wouldn’t work, and I thought I’d have to forgo having the save audio as part of the preview :(

Luckily, I remembered a trick I used at work for embedding data into HTML ([solution here](https://stackoverflow.com/questions/20312300/quicklook-generator-audio/24112975#24112975)), which worked flawlessly (and was quicker at loading the audio data to boot).

And with that final piece of the puzzle in place, it was time to wrap the project up...

### Wrapping things up

With all the components in place, it was just a matter of cleaning up the code a bit, putting in the acknowledgement info, adding some preference options, write up all the READMEs, fix a few other bugs and shipping it out. Overall this was a fun project and I learned a few new things along the way.
