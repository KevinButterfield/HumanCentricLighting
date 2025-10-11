let keyframes = []; // { x: fractionOfSolarDay, y: value }
let currentSolarTime = { sunriseHours: 0, sunsetHours: 0, currentFraction: 0 };

const Routes = {
  keyframes: 'http://192.168.1.142/keyframes',
  currentSolarTime: 'http://192.168.1.142/current-time',
}

const timeFormater = new Intl.DateTimeFormat(undefined, { timeStyle: 'short' });

window.uploadKeyframes = async function () {
  const response = await fetch(Routes.keyframes, {
    method: 'POST',
    body: JSON.stringify(keyframes),
  });
}

function formatTime(fractionOfSolarDay) {
  const hoursIntoSolarDay = fractionOfSolarDay * (currentSolarTime.sunsetHours - currentSolarTime.sunriseHours);
  const time = currentSolarTime.sunriseHours + hoursIntoSolarDay;

  const hoursComponent = Math.floor(time);
  const minutesComponent = (time - hoursComponent) * 60;

  const output = new Date();
  output.setUTCHours(hoursComponent);
  output.setUTCMinutes(minutesComponent);
  return timeFormater.format(output);
}

function renderCharts() {
  window.renderChart(document.getElementById('color-temp-chart'), {
    data: keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.colorTemperature })),
    minValue: 2700,
    maxValue: 6500,
    formatDataLabel: (value) => Math.round(value.y) + 'K',
    onValueChanged: (index, value) => keyframes[index].colorTemperature = Math.round(value.y),
  });
  
  window.renderChart(document.getElementById('brightness-chart'), {
    data: keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.brightness })),
    minValue: 0,
    maxValue: 1,
    formatDataLabel: (value) => Math.round(value.y * 100) + '%',
    onValueChanged: (index, value) => keyframes[index].brightness = value.y,
  });
}

async function main() {
  Chart.register(window['chartjs-plugin-annotation'])

  const [a, b] = await Promise.all([
    fetch(Routes.keyframes),
    fetch(Routes.currentSolarTime),
  ].map(p => p.then(r => r.json())));
  keyframes = a;
  currentSolarTime = b;

  renderCharts();
}

document.addEventListener('DOMContentLoaded', main);
