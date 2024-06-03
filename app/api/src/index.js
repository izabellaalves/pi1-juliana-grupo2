const express = require('express')

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

app.get('/dadosCarrinho', async (req, res) => {
    try {
      const result = await pool.query('SELECT * FROM ESTATISTICAS_CARRINHO');
      res.json(result.rows);
    } catch (err) {
      console.error(err);
      res.status(500).json({ error: 'Internal Server Error' });
    }
  });

app.listen(PORT, () => console.log(`Server running on PORT ${PORT}`))