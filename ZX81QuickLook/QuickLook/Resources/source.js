/* Function for 'selecting' and switching tabs. */
function openTab(anEvent,tabName) {
   var i;
   
   // hide all the tabs
   var x = document.getElementsByClassName("tab");
   for (i = 0; i < x.length; i++) {
       x[i].style.display = "none";
   }
   
   // show the one selected
   document.getElementById(tabName).style.display = "flex";
   
   // reset the 'active' style on the bar buttons
   buttons = document.getElementsByClassName("barButton");
   for (i = 0; i < buttons.length; i++) {
       buttons[i].className = buttons[i].className.replace(" active", "");
   }
   
   // set the 'active' style on the selected tab button
   anEvent.currentTarget.className += " active";
}

/* Adjusts the display width for the code listing, and stores the
setting (which doesn't work in a quicklook webview). */
function codeWidth() {
   var widthSelector = document.getElementById("widthSelector");
   var newWidth = (widthSelector.options[widthSelector.selectedIndex].value);
   document.getElementById("basicListing").style.maxWidth = newWidth;
   window.localStorage.setItem("listingWidth",newWidth);
   document.cookie = newWidth;
}

/* These adjust the font size for the code and screen listing, and store
the setting (which doesn't work in a quicklook webview). */
function fontSize() {
   var sliderValue = this.value;
   document.getElementById("basicListing").style.fontSize = sliderValue+"px";
   window.localStorage.setItem("fontSize",sliderValue);
}

function screenSize() {
   var sliderValue = this.value;
   document.getElementById("screenListing").style.fontSize = sliderValue+"px";
   window.localStorage.setItem("screenSize",sliderValue);
}

// set the audio source on the <audio> element
//document.getElementById("audio-element").src = soundURL;

/* Sets up the audio wave */

const NUMBER_OF_BUCKETS = 350; // number of "bars" the waveform should have
const SPACE_BETWEEN_BARS = 0.0; // from 0 (no gaps between bars) to 1 (only gaps - bars won't be visible)
let audioCtx = new window.webkitAudioContext();
fetch(soundURL).then(response => {
    response.arrayBuffer().then(audioData => {
        audioCtx.decodeAudioData(audioData, buffer => {
            decodedAudioData = buffer.getChannelData(0);
            let bucketDataSize = Math.floor(decodedAudioData.length / NUMBER_OF_BUCKETS);
            let buckets = [];
            for (var i = 0; i < NUMBER_OF_BUCKETS; i++) {
                let startingPoint = i * bucketDataSize;
                let endingPoint = startingPoint + bucketDataSize;
                let avg = 0
                for (var j = startingPoint; j <= endingPoint; j++) {
                        avg += decodedAudioData[j];
                }
                let size = Math.max(Math.abs(avg/(endingPoint-startingPoint)),0.00001);
                buckets.push(size * 150);
            }
            document.getElementById('waveform-mask').innerHTML = buckets.map((bucket, i) => {
                let bucketSVGWidth = 100.0 / buckets.length;
                let bucketSVGHeight = bucket * 100.0; // fill 3/4 of the height
                return `<rect
                    x=${bucketSVGWidth * i + SPACE_BETWEEN_BARS / 2.0}
                    y=${ (100 - bucketSVGHeight) / 2.0}
                    width=${bucketSVGWidth - SPACE_BETWEEN_BARS}
                    height=${bucketSVGHeight} />`;
            }).join('');
            let audioElement = document.getElementById('audio-element');
            audioElement.src = soundURL;
            let waveformProgress = document.getElementById('waveform-progress');
            // every 25 milliseconds, update the waveform-progress SVG with a new width - the percentage of time elapsed on the audio file
            setInterval(() => {
                waveformProgress.setAttribute('width', (audioElement.currentTime / audioElement.duration * 100) || 0);
            }, 25);
        }, e => {
            // callback for any errors with decoding audio data
            console.log('Error with decoding audio data' + e.err);
        },);
    }).catch(err => {
        // catch any errors fetching the buffer
        console.log(err);
    });
}).catch(err => {
    // catch any errors with fetching the audio
    console.log(err);
});


/* Add event listeners for the sliders. */
var codeSlider = document.getElementById('codeSlider');
codeSlider.addEventListener('input', fontSize);

var screenSlider = document.getElementById('screenSlider');
screenSlider.addEventListener('input', screenSize);

/* These restore the saved settings; sadly doesn't work in a
 quicklook webview. */
var storage = window.localStorage;
var inputEvent = new CustomEvent("input");

var codeFontSize = storage.getItem("fontSize");
if(codeFontSize) {
    codeSlider.value = codeFontSize;
    codeSlider.dispatchEvent(inputEvent);
}

var screenFontSize = storage.getItem("screenSize");
if(screenSize) {
    screenSlider.value = screenFontSize;
    screenSlider.dispatchEvent(inputEvent);
}

var listingWidth = storage.getItem("listingWidth");
if(listingWidth) {
    var widthSelector = document.getElementById('widthSelector');
    widthSelector.value = listingWidth;
    changeEvent = new CustomEvent("change");
    widthSelector.dispatchEvent(changeEvent);
}
