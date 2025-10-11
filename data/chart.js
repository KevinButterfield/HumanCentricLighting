window.renderChart = function (canvas, params) {
  const chart = new Chart(canvas, {
    type: 'line',
    plugins: [ChartDataLabels],
    data: {
      labels: params.buildData().map(({ x }) => x),
      datasets: [
        {
          data: params.buildData(),
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
          suggestedMin: params.minValue,
          suggestedMax: params.maxValue,
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
          },
        },
        datalabels: {
          formatter: params.formatDataLabel,
          align: 'top',
          anchor: 'end',
        },
        dragData: {
          onDrag: (_event, datasetIndex, index, value) => {
            // Plainly setting a minimum doesn't floor the value during drag
            if (value.y < params.minValue) {
              chart.data.datasets[datasetIndex].data[index].y = params.minValue;
              chart.update('none');
              return false;
            }
          },
          onDragEnd: (_event, _datasetIndex, index, value) => params.onValueChanged(index, value),
        },
      },
    },
  });

  return () => {
    chart.data.datasets[0].data = params.buildData();
    chart.update();
  };
}