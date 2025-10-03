const exampleData = [
  { x: 0.0, y: 2700 },
  { x: 0.167, y: 4000 },
  { x: 0.333, y: 6500 },
  { x: 0.5, y: 4500 },
  { x: 0.667, y: 3500 },
  { x: 0.833, y: 3000 },
  { x: 1.0, y: 2700 },
];

const sunrise = 8;
const sunset = 20;

function renderTime(index) {
  const fractionOfSolarDay = exampleData[index].x
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
      labels: exampleData.map(d => d.x),
      datasets: [{
        data: exampleData,
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

function main() {
  renderChart();
}

document.addEventListener('DOMContentLoaded', main);
