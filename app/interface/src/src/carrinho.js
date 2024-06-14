import './App.css';
import React from 'react';
import carrinho from './assets/carrinho.png';
import git from './assets/git.png';
import unb from './assets/unb.png';

const CarrinhoDeLinha = () => {
  return (
    <div className="container">
      <header className="header">
        <div className="title">CARRINHO DE LINHA</div>
        <div className="info">
          <div className="project-details">
            <span> PI1 </span>
            <span> | </span>
            <span> T4 </span>
            <span> | </span>
            <span> G2 </span>
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
      <div className="content">
        <img src={carrinho} alt="Carrinho de Linha" className="ImgCarrinho" />
      </div>
    </div>
  );
};

export default CarrinhoDeLinha;