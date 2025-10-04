let currentData = []; // { x: fractionOfSolarDay, y: value }
let currentSolarTime = { sunriseHours: 0, sunsetHours: 0, currentFraction: 0 };

const timeFormater = new Intl.DateTimeFormat(undefined, { timeStyle: 'short' });

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
      labels: currentData.map(d => d.x),
      datasets: [
        {
          data: currentData,
          tension: 0.4,
          pointHitRadius: 25,
        },
      ]
    },
    options: {
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
            callback: (index) => formatTime(currentData[index].x),
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
          }
        },
      },
    },
  });
}

async function main() {
  Chart.register(window['chartjs-plugin-annotation'])

  const [keyframes, solarTime] = await Promise.all([
    fetch('http://192.168.1.142/keyframes'),
    fetch('http://192.168.1.142/current-time'),
  ].map(p => p.then(r => r.json())))

  currentData = keyframes.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.colorTemperature }));
  currentSolarTime = solarTime;

  renderChart();
}

document.addEventListener('DOMContentLoaded', main);
