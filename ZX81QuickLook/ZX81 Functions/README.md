# ZX81QuickLook Functions

The main processing functions are found here. Most of the functions take a `FILE` pointer of the .P file being processed, and a `string_buffer`
struct with a character buffer that will contain the processed data. In-memory buffers are used to avoid creating temporary files for the data, since Quicklook imposes some limits on access to external resources for HTML display.

The main functions are:

- `checkFile()`: this does some sanity checks on the file, and returns a `ZX81_Program` struct that contains various pieces of info used by other
functions
- `get_basic_listing()`: this produces the basic listing
- `get_screen()`: generates the screen output, then checks if there are actual printable characters to determine if the interface should show that tab
- `get_save_sound()`: generates WAV data for the save audio for playback. This uses the `p2raw.c` functions found in `ThirdParty/` for generating
the raw audio. For this function, in-memory `FILE` objects are used since the original code operated on files directly.

These functions are self-contained and should be easy to use independently (no dependencies to other project code - both the CLI `ZX81QL` scheme and the main Quicklook plugin schemes share these functions). All the code is commented to (hopefully) be easy to follow.
