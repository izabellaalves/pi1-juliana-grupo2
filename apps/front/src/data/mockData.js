import { tokens } from "../theme";

// Calcular velocidade média incrementalmente
const instantSpeedData = [
  { x: 0, y: 0 },
  { x: 0.1, y: 10 },
  { x: 0.2, y: 20 },
  { x: 0.3, y: 15 },
  { x: 0.4, y: 25 },
  { x: 0.5, y: 30 },
  { x: 0.6, y: 35 },
  { x: 0.7, y: 40 },
  { x: 0.8, y: 45 },
  { x: 0.9, y: 50 },
  { x: 1.0, y: 55 },
  { x: 1.1, y: 60 },
  { x: 1.2, y: 65 },
];

let totalSpeed = 0;
const averageSpeedData = instantSpeedData.map((point, index) => {
  totalSpeed += point.y;
  return {
    x: point.x,
    y: totalSpeed / (index + 1),
  };
});

// Calcular aceleração instantânea
let lastSpeed = 0;
let lastTime = 0;
const instantAccelData = instantSpeedData.map((point, index) => {
  if (index === 0) {
    lastSpeed = point.y;
    lastTime = point.x;
    return { x: point.x, y: 0 };
  } else {
    const accel = (point.y - lastSpeed) / (point.x - lastTime);
    lastSpeed = point.y;
    lastTime = point.x;
    return { x: point.x, y: accel };
  }
});

export const mockLineData = [
  {
    id: "Velocidade Instantânea",
    color: tokens("dark").greenAccent[500],
    data: instantSpeedData,
  },
  {
    id: "Velocidade Média",
    color: tokens("dark").blueAccent[500],
    data: averageSpeedData,
  },
  {
    id: "Aceleração Instantânea",
    color: tokens("dark").redAccent[500],
    data: instantAccelData,
  },
];

export const mockLineSeep = [
  {
    id: "Velocidade Instantânea",
    color: tokens("dark").greenAccent[500],
    data: instantSpeedData,
  },
  {
    id: "Velocidade Média",
    color: tokens("dark").blueAccent[500],
    data: averageSpeedData,
  },
];

export const mockLineAcceleration = [
  {
    id: "Aceleração Instantânea",
    color: tokens("dark").redAccent[500],
    data: instantAccelData,
  },
];
