// Keyframe colors approximating light from warm to cool
const kelvinStops = [
  { K: 2500, color: [255, 184, 107] }, // warm amber
  { K: 3500, color: [255, 216, 168] }, // warm white
  { K: 4000, color: [255, 245, 235] }, // neutral white start (less yellow)
  { K: 5000, color: [255, 245, 235] }, // neutral white end (less yellow)
  { K: 5500, color: [234, 246, 255] }, // cool white, slight blue
  { K: 6500, color: [166, 208, 255] }  // daylight blue
];

const color = (r, g, b) => `rgb(${r}, ${g}, ${b})`;

/**
 * Interpolates between keyframes to get RGB for any Kelvin
 */
function kelvinToRGBInterpolated(K) {
  // Clamp range
  if (K <= kelvinStops[0].K) return color(...kelvinStops[0].color);
  if (K >= kelvinStops[kelvinStops.length - 1].K)
    return color(...kelvinStops[kelvinStops.length - 1].color);

  // Find the two surrounding keyframes
  for (let i = 0; i < kelvinStops.length - 1; i++) {
    const lowerBound = kelvinStops[i];
    const upperBound = kelvinStops[i + 1];
    if (K >= lowerBound.K && K <= upperBound.K) {
      const transit = (K - lowerBound.K) / (upperBound.K - lowerBound.K);
      const r = Math.round(lowerBound.color[0] + (upperBound.color[0] - lowerBound.color[0]) * transit);
      const g = Math.round(lowerBound.color[1] + (upperBound.color[1] - lowerBound.color[1]) * transit);
      const b = Math.round(lowerBound.color[2] + (upperBound.color[2] - lowerBound.color[2]) * transit);
      return color(r, g, b);
    }
  }
}

window.buildGradient = function (canvas, points) {
  const ctx = canvas.getContext('2d');
  const gradient = ctx.createLinearGradient(0, 0, canvas.width, 0);

  points.forEach((point) => {
    const normalizedX = (point.x + 0.1) / 1.2;
    const color = kelvinToRGBInterpolated(point.y);
    gradient.addColorStop(normalizedX, color);
  });

  return gradient;
}