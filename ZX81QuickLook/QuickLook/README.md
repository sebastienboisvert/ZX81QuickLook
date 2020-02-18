# ZX81QuickLook

This folder contains the main code for the Quicklook generator. The file `GeneratePreviewForURL.m` implements the main function to generate the preview. The general process is:

1. Creates the buffers for holding the listing, screen and audio
2. Checks the file for validity, then calls into the [functions](../ZX81%20Functions) to generate the data
3. Substitutes in the generated data and various preference values into the `source.html` file in `Resources`
4. Generates the final data representation with the updated source HTML and resource attachments back to the Quicklook system for display

## Xcode Schemes

There are three schemes for building the project:

1. `ZX81QuickLook`: This is the main scheme the generates the plugin. This scheme includes a 'Build Post-action' that was meant to automatically
copy the compiled plugin into `~/Library/QuickLook` then execute `qlmanage -r` for quick testing; however it seems under macOS 10.14
(and likely above) QuickLook plugins aren't supported in the 'home Library' QuickLook folder (🤷🏻‍♂️), so it is disabled. I instead copied it to
`/Library/QuickLook` manually, and had the `Refresh QL system` folder action set on that folder to automatically execute `qlmanage -r` when its
contents change.
2. `ZX81QuickLook Run`: This was meant to run the plugin in a Quicklook 'debug' environment, however that doesn't work with HTML-based preview
generators (crashes on some WebKit stuff), but is still useful for debugging up to the point a window would normally show.
3. `ZX81QL`: This scheme implements a CLI-only tool for processing the .P file and outputs the listing and screen data to the console. I used
this for testing during early development.

## Resources

The `Resources` folder contains what's needed to create the preview:

- `source.html`: the main HTML that serves as the interface in the preview; its placeholders get replaced with the generated data
- `source.css`: for the styling of the HTML
- `source.js`: the JavaScript functions used
- `zx81_vdu.ttf`: the ZX81 font used for listing and screen display in the HTML
