const recordButton = document.querySelector('.record');
const stopButton = document.querySelector('.stop');
recordButton.addEventListener('click', (e) => {   
  e.preventDefault();
  var device = navigator.mediaDevices.getUserMedia({audio: true});
  var items = [];
  device.then( stream => {
    var recorder = new MediaRecorder(stream);
    recorder.ondataavailable = e => {
      items.push(e.data);
      if(recorder.state == 'inactive') {
        var blob = new Blob(items, {type: 'audio/webm'});
        var audio = document.getElementById('audio');
        var mainAudio = document.createElement('audio');
        mainAudio.setAttribute('controls', 'controls');
        audio.appendChild(mainAudio);
        mainAudio.innerHTML = `<source src="${URL.createObjectURL(blob)}" type="video/webm"/>`;
      }
    }
    recorder.start();

    stopButton.addEventListener('click', (e) => {
      e.preventDefault();
      recorder.stop();
    })
  })
})


