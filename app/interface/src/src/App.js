import React from 'react';
import './teste.css';
import git from './assets/git.png';
import unb from './assets/unb.png';
import carro from './assets/carro.png';

const CarrinhoDashboard = () => {
  return (
    <div className="container">
      <header className="header">
        <div className="title">CARRINHO DE LINHA</div>
        <div className="info">
          <div className="project-details">
            <span>PI1</span>
            <span>T4</span>
            <span>G2</span>
          </div>
          <div className="icons">
            <a href="https://github.com/izabellaalves/pi1-juliana-grupo2" target="_blank" rel="noopener noreferrer">
              <img src={git} alt="GitHub" className="icon-image" />
            </a>
            <a>
            <img src={unb} alt="Unb" className="unb-image" />
            </a>
          </div>
        </div>
      </header>
      <div className="dashboard">
        <div className="section velocity">
          <h3>VELOCIDADE INSTANTÂNEA</h3>
          <div className="velocity-info">
            <div className="velocity-gauge"></div>
            <div className="velocity-data">
              <div className="value">202,5 M/S</div>
              <div className="time-distance">
                <div>Tempo (t): 202,5 SEG</div>
                <div>Distância (s): 2025 M</div>
              </div>
            </div>
          </div>
        </div>
        <div className="section trajectory">
          <h3>TRAJETÓRIA PERCORRIDA</h3>
          <div className="trajectory-map"></div>
        </div>
        <div className="section time">
          <h3>TEMPO DE PERCURSO</h3>
          <div className="timer">00:00</div>
          <div className="energy-consumption">
            <div>Consumo energético</div>
            <div className="energy-value">202,5 KWh</div>
            <div className="energy-bar">
              <div className="energy-level k-w">K-W 450</div>
              <div className="energy-level nm">NM 791</div>
            </div>
          </div>
        </div>
        <div className="section acceleration">
          <h3>ACELERAÇÃO INSTANTÂNEA</h3>
          <div className="acceleration-chart"></div>
        </div>
      </div>
      <div className="car-image">
        <img src={carro} alt="Carrinho de Linha" className="ImgCarrinho" />
      </div>
    </div>
  );
};

export default CarrinhoDashboard;
