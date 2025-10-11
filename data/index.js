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

function renderChart() {
  const chartCanvas = document.getElementById('chart');

  const chart = new Chart(chartCanvas, {
    type: 'line',
    plugins: [ChartDataLabels],
    data: {
      labels: keyframes.map(d => d.x),
      datasets: [
        {
          data: keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.colorTemperature })),
          tension: 0.4,
          pointHitRadius: 25,
        },
      ]
    },
    options: {
      responsive: true,
      layout: {
        padding: {
          top: 40,
          left: 20,
          right: 20,
          bottom: 10,
        }
      },
      scales: {
        x: {
          ticks: {
            callback: (index) => formatTime(keyframes[index].fractionOfSolarDay),
          },
        },
        y: {
          ticks: {
            display: false,
          },
          suggestedMin: 2700,
          suggestedMax: 6500,
        },
      },
      plugins: {
        legend: {
          display: false,
        },
        tooltip: {
          enabled: false,
        },
        annotation: {
          annotations: {
            currentFractionOfSolarDay: {
              type: 'line',
              borderColor: 'black',
              borderWidth: 2,
              xMin: currentSolarTime.currentFraction * 10,
              xMax: currentSolarTime.currentFraction * 10,
            }
          }
        },
        datalabels: {
          // display: (context) => context.dataIndex > 0 && context.dataIndex < context.dataset.data.length - 1,
          formatter: (value) => Math.round(value.y) + 'K',
          align: 'top',
          anchor: 'end',
        },
        dragData: {
          onDrag: (_event, datasetIndex, index, value) => {
            // Plainly setting a minimum doesn't floor the value during drag
            if (value.y < 2700) {
              chart.data.datasets[datasetIndex].data[index].y = 2700;
              chart.update('none');
              return false;
            }
          },
          onDragEnd: (_event, _datasetIndex, index, value) => {
            keyframes[index].colorTemperature = Math.round(value.y);
          }
        },
      },
    },
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

  renderChart();
}

document.addEventListener('DOMContentLoaded', main);
