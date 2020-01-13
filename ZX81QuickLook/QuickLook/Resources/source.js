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

/* Adjusts the display width for the code listing. */
function codeWidth() {
   var widthSelector = document.getElementById("widthSelector");
   var newWidth = (widthSelector.options[widthSelector.selectedIndex].value);
   document.getElementById("basicListing").style.maxWidth = newWidth;
}

/* These adjust the font size for the code and screen listing. */
function fontSize() {
    var codeSlider = document.getElementById('codeSlider');
    var sliderValue = codeSlider.value;
    document.getElementById("basicListing").style.fontSize = sliderValue+"px";
}

function screenSize() {
    var screenSlider = document.getElementById('screenSlider');
    var sliderValue = screenSlider.value;
    document.getElementById("screenListing").style.fontSize = sliderValue+"px";
}

function overlayOn() {
  document.getElementById("infoOverlay").style.display = "flex";
}

function overlayOff() {
  document.getElementById("infoOverlay").style.display = "none";
}

/* Sets up the audio wave */

const SPACE_BETWEEN_BARS = -0.05; // from 0 (no gaps between bars) to 1 (only gaps - bars won't be visible)
let audioCtx = new window.webkitAudioContext();
fetch(soundURL).then(response => {
    response.arrayBuffer().then(audioData => {
        audioCtx.decodeAudioData(audioData, buffer => {
            decodedAudioData = buffer.getChannelData(0);
            let bucketDataSize = Math.floor(decodedAudioData.length / NUMBER_OF_BUCKETS);
            let buckets = [];
            var largest = 0; // track the largest value to use as ratio
            for (var i = 0; i < NUMBER_OF_BUCKETS; i++) {
                let startingPoint = i * bucketDataSize;
                let endingPoint = startingPoint + bucketDataSize;
                                            
                /* We'll caculate an average for the 'slice', which gives a pretty
                 good representation of the sound */
                let total = 0
                for (var j = startingPoint; j <= endingPoint; j++) {
                        total += decodedAudioData[j];
                }
            
                /* As there's never any silence in a save file, we want a minimum
                 value to show so there's no blank spots in the waveform. */
                var avg = Math.max(total/(endingPoint-startingPoint),0.00001);
                while(avg < 0.0001) {
                    // boost the average until we have something substantial
                    avg *= 10;
                }

                largest = Math.max(largest,avg);
                buckets.push(avg * 100);
            }

            /* We want the largest (tallest) bar to fill the view the most, and
             all other bars will be a relative ratio of it (here we want the
             tallest bar to fill 85% of the view height). */
            let ratio = (0.85/largest);
            
            let bucketSVGWidth = 100.0 / buckets.length;
            document.getElementById('waveform-mask').innerHTML = buckets.map((bucket, i) => {
                let bucketSVGHeight = bucket * ratio;
                return `<rect
                    x=${bucketSVGWidth * i + SPACE_BETWEEN_BARS / 2.0}
                    y=${ (100 - bucketSVGHeight) / 2.0}
                    width=${bucketSVGWidth - SPACE_BETWEEN_BARS}
                    height=${bucketSVGHeight} />`;
            }).join('');
                                            
            let audioElement = document.getElementById('audio-element');
            audioElement.src = soundURL;
            let waveformProgress = document.getElementById('waveform-progress');
            // every WAVE_UPDATE_FREQ milliseconds, update the waveform-progress SVG with a new width - the percentage of time elapsed on the audio file
            setInterval(() => {
                waveformProgress.setAttribute('width', (audioElement.currentTime / audioElement.duration * 100) || 0);
            }, WAVE_UPDATE_FREQ);
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

// force update for current settings on initial load
screenSize();
fontSize();
codeWidth();
