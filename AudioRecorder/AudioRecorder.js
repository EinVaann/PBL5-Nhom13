const devicesStatus = document.querySelector('.devices-status');

// snack bar
const snackBar = () => {
  setTimeout(() => {
    devicesStatus.classList.add('hide')
  }, 8000) // hide after 8s
}

// Check access permissions
navigator.permissions.query({
  name: 'microphone'
}).then((result) => {
  if(result.state === "granted") {
    devicesStatus.innerHTML = "Devices Access Granted"
    snackBar()
    console.log(result.state)
  }

  if(result.state === "prompt") {
    devicesStatus.innerHTML = "Accept Access Devices Request"
    snackBar()
    console.log(result.state)
  }

  if(result.state === "denied") {
    devicesStatus.innerHTML = "Please Enable Microphone"
    snackBar()
    console.log(result.state)
  }
})

// Select buttons
const title = document.querySelector('.title')
const stopWatch = document.querySelector('.stopWatch')
const record = document.querySelector('.record')
const stop = document.querySelector('.stop')
const audioPlay = document.querySelector('.audioPlay')

// Type of media
let typeOfMedia = {
  audio: true,
  //video: true
}

// Create chunks array container
let chunks = []

// Media options
let options = {
  audioBitsPerSecond: 128000,
  videoBitsPerSeconde: 2500000,
  mimeType: 'audio/webm'
}

// Download counter
let counter = 0;

// link styles
let linkStyles = "display: block; padding: 10px; color: red; text-decoration: none;"

// Create audio element to playback
function createAudioElement (blobURL) {

  const divEL = document.createElement('div');
  divEL.className = 'div-audio';

  const downloadEL = document.createElement('a');
  downloadEL.style = linkStyles;
  downloadEL.innerHTML = `Download-${counter = counter + 1}`;
  downloadEL.download = `Audio-${counter}.webm`;
  downloadEL.href =  blobURL;

  const audioEL = document.createElement('audio');
  audioEL.className = 'audio';
  audioEL.controls = true;

  const sourceEL = document.createElement('source');
  sourceEL.src = blobURL;
  sourceEL.type = 'audio/webm';

  audioEL.appendChild(sourceEL);

  divEL.appendChild(audioEL);
  divEL.appendChild(downloadEL);

  // Append all in the DOM
  document.body.appendChild(divEL);
}

//
// _______Rec FUNTION
//____________________________________________
// RecStream init
let recStream

const recFuntion = async () => {
  try {

    // Access computer devices
    const mediaDevices = await navigaton.mediaDevices.getUserMedia(typeOfMedia);

    if(mediaDevices.active === true) {

      // Create a new media recorder object
      recStream = new MediaRecorder(mediaDevices, options)

      recStream.ondataavailable = (e) => {
        // Push inside the array
        chunks.push(e.data)

        // If state is inactive, stop recording
        if(recStream.state === 'inactive') {

          // Create a new Blob
          let blob = new Blob(chunks,{
            type: 'audio/webm'
          })

          // Create a playback
          createAudioElement(URL.createObjectURL(blob))
        }
      }

      // Start record now
      recStream.start()
    }
  } catch (error) {
    if(error) throw error
  }
}

// Record on click
record.onClick = e => {
  
  // Disable rec button during registration
  record.disable = true;
  // Change back color
  record.style.backgroundColor = 'orange';
  //Animate rec button
  record.classList.add('scale');

  // Enable stop button
  stop.disable = false;
  //Change stop styles
  stop.style.backgroundColor = '#292964';
  stop.style.color = '#ffffff';

  // Change title back color
  title.style.color = '#2196f3'

  // start recording
  recFuntion();

  // Start stopwatch....
  clearInterval(swInterval);
  swInterval = setInterval(stopWatchFunction, 1000);
}

// Stop On Click
stop.onClick = e => {

  // Enable rec button
  record.disable = false;
  // Restore red color on rec button
  record.style.backgroundColor = 'red';
  // Disable rec animation
  record.classList.remove('scale');

  // Enable stop button
  stop.disable = true;
  //Change stop styles
  stop.style.backgroundColor = '#292929';
  stop.style.color = 'rgb(103, 103, 103)';

  // Change title color
  title.style.color = '#313142';

  // Stop and reset stopWatch
  clearInterval(swInterval);
  sec = 0;
  min = 0;

  // stop rec
  recStream.stop();
}

// StopWatch
let swInterval;
let displayStopwatch
let sec = 0;
let min = 0;
const stopWatchFunction = () => {
  sec++;
  if(sec <= 9) {
    sec = '0' + sec;
  }

  if(sec === 60) {
    sec = 0;
    min++
    if(sec <= 9) {
      min = '0' + min;
    }
  }

  if(min === 60) {
    min = 0;
  }

  displayStopwatch = "min : " + min + ' : ' + "second : " + sec; 
  stopWatch.innerHTML = displayStopwatch;
}