let keyframes = []; // { x: fractionOfSolarDay, y: value }
let currentSolarTime = { sunriseHours: 0, sunsetHours: 0, currentFraction: 0 };
let chartUpdateCallbacks = []

const routePrefix = window.location.toString().startsWith('file://') ? 'http://192.168.1.142' : '';
const Routes = {
  keyframes: `${routePrefix}/keyframes`,
  currentSolarTime: `${routePrefix}/current-time`,
}

const timeFormater = new Intl.DateTimeFormat(undefined, { timeStyle: 'short' });

async function keyframeRequest(params) {
  try {
    const response = await fetch(Routes.keyframes, params);
    keyframes = await response.json();

    chartUpdateCallbacks.forEach(update => update());
  } catch (err) {
    console.error(err);
  }
}

window.uploadKeyframes = () => keyframeRequest({
  method: 'POST',
  body: JSON.stringify(keyframes),
});

window.resetKeyframes = () => keyframeRequest({
  method: 'DELETE',
});

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
  chartUpdateCallbacks = [
    window.renderChart(document.getElementById('color-temp-chart'), {
      buildData: () => keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.colorTemperature })),
      minValue: 2700,
      maxValue: 6500,
      formatDataLabel: (value) => Math.round(value.y) + 'K',
      onValueChanged: (index, value) => keyframes[index].colorTemperature = Math.round(value.y),
      backgroundColor: (...args) => window.buildGradient(...args),
    }),

    window.renderChart(document.getElementById('brightness-chart'), {
      buildData: () => keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.brightness })),
      minValue: 0.25,
      maxValue: 1,
      formatDataLabel: (value) => Math.round(value.y * 100) + '%',
      onValueChanged: (index, value) => keyframes[index].brightness = value.y,
    }),
  ]
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
