
const sunrise = 8;
const sunset = 20;

let currentData = []; // { x: fractionOfSolarDay, y: value }

function renderTime(index) {
  const fractionOfSolarDay = currentData[index].x
  const time = sunrise + (sunset - sunrise) * fractionOfSolarDay;
  console.log({ time })

  const percent = Math.round(fractionOfSolarDay * 100);
  return percent + '%';
}

function renderChart() {
  const chartCanvas = document.getElementById('chart');

  const chart = new Chart(chartCanvas, {
    type: 'line',
    plugins: [ChartDataLabels],
    data: {
      labels: currentData.map(d => d.x),
      datasets: [{
        data: currentData,
        tension: 0.4,
        pointHitRadius: 25,
      }]
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
            callback: renderTime,
          }
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
  try {
    const response = await fetch('http://192.168.1.142/keyframes');
    const data = await response.json();
    currentData = data.map(kf => ({ x: kf.fractionOfSolarDay, y: kf.colorTemperature }));

    renderChart();
  } catch (error) {
    console.error('Error fetching keyframes:', error);
  }
}

document.addEventListener('DOMContentLoaded', main);
