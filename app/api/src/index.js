const express = require('express')

//const {Pool} = require('pg')

require('dotenv').config()

const PORT = 3333

/*const pool = new Pool({
    connectionString: process.env.POSTGRES_URL
})*/

const app = express()

app.use(express.json())

app.get('/', (req, res) => {console.log('teste')})

app.listen(PORT, () => console.log(`Server running on PORT ${PORT}`))