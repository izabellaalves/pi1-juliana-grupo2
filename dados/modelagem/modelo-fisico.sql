CREATE TABLE IF NOT EXISTS ESTATISTICAS_CARRINHO (
  idEstatistica SERIAL PRIMARY KEY,
  quantidadepercursosConcluidos INT,
  mediaVelocidade DECIMAL(5,2),
  mediaAceleracao DECIMAL(5,2),
  mediaConsumoEnergetico DECIMAL(10,2),
  mediaTamanhoPercursos INT,
  mediaTempoPercursos TIME,
  quantidadePercursos INT
);

CREATE TABLE IF NOT EXISTS CARRINHO (
  idCarrinho SERIAL PRIMARY KEY,
  versao VARCHAR(3),
  UNIQUE(idCarrinho, versao),
  dataLancamento DATE,
  idEstatisticaCarrinho INT,
  CONSTRAINT CARRINHO_ESTATISTICA_FK FOREIGN KEY (idEstatisticaCarrinho) REFERENCES ESTATISTICAS_CARRINHO (idEstatistica)
);

CREATE TABLE IF NOT EXISTS PERCURSO (
  idPercurso SERIAL PRIMARY KEY,
  trajetoria VARCHAR(60)
);

CREATE TABLE IF NOT EXISTS REALIZA (
  idCarrinho INT,
  idPercurso INT,
  dataRealizacao DATE,
  velocidadeInstantanea DECIMAL(5,2),
  aceleracaoInstantanea DECIMAL(5,2),
  conclusaoPercurso TIME,
  consumoEnergetico INT,
  versao VARCHAR(3),
  conclusao BOOLEAN,
  CONSTRAINT REALIZA_PK PRIMARY KEY (idCarrinho, idPercurso, dataRealizacao),
  CONSTRAINT REALIZA_CARRINHO_FK FOREIGN KEY (idCarrinho, versao) REFERENCES CARRINHO (idCarrinho, versao),
  CONSTRAINT REALIZA_PERCURSO_FK FOREIGN KEY (idPercurso) REFERENCES PERCURSO (idPercurso)
);

