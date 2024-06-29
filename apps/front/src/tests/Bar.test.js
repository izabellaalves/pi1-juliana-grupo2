import React from 'react';
import { render, screen } from '@testing-library/react';
import '@testing-library/jest-dom/extend-expect';
import Bar from '../scenes/Bar';

jest.mock('../components/Header', () => () => <div>Header Component</div>);
jest.mock('../components/BarChart', () => () => <div>BarChart Component</div>);

test('renders Bar component with Header and BarChart', () => {
  render(<Bar />);

  // Verifica se o Header é renderizado com os textos corretos
  const headerElement = screen.getByText(/Header Component/i);
  expect(headerElement).toBeInTheDocument();

  // Verifica se o BarChart é renderizado
  const barChartElement = screen.getByText(/BarChart Component/i);
  expect(barChartElement).toBeInTheDocument();
});
