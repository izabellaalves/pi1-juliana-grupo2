const express = require('express')
const cors = require('cors')
const {Pool} = require('pg')


const PORT = 3333

const pool = new Pool({
    user: 'alunos',
    host: 'localhost',
    database: 'ProjetoIntegrador1',
    password: 'alunospi1',
    port: 5432,
  });

const app = express()

app.use(express.json())

app.get('/media', async (req, res) => {
  try {
    const media = await pool.query('SELECT * FROM ESTATISTICAS_CARRINHO');
    return res.status(200).send.json(media.rows);
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Internal Server Error' });
  }
});

app.get('/dados', async (req, res) => {
  try {
    const dados = await pool.query('SELECT * FROM REALIZA');
    return res.status(200).send.json(dados.rows);
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Internal Server Error' });
  }
});

app.listen(PORT, () => console.log(`Server running on PORT ${PORT}`))