import serial
import json
import time
import os
import sqlite3
from datetime import datetime
from flask import Flask, request, jsonify
from threading import Thread
from flask_cors import CORS
import numpy as np

# Configurações de conexão serial

""" bluetoothport = 'COM5'
baudrate = 115200

try:
    ser = serial.Serial(bluetoothport, baudrate, timeout=1)
    print(f"Conectado à porta {bluetoothport} com baudrate {baudrate}")
except Exception as e:
    print(f"Erro ao conectar na porta serial: {e}")
    exit() """

app = Flask(__name__)
CORS(app)

# Variáves globais

g_to_m_per_s2 = 9.8
velocidade_x_global = 0.0
velocidade_y_global = 0.0

delta_t = 1.0

def convert_g_to_m_per_s2(acceleration_g):
    return round(acceleration_g * g_to_m_per_s2, 2)

def resetar_velocidades_globais():
    global velocidade_x_global, velocidade_y_global
    velocidade_x_global = 0.0
    velocidade_y_global = 0.0

def calcular_velocidade_resultante(aceleracao_x, aceleracao_y):
    
    aceleracao_resultante = calcular_aceleracao_resultante(aceleracao_x, aceleracao_y)
    
    if aceleracao_resultante == 0.0: return 0.0
    
    global velocidade_x_global, velocidade_y_global
    
    aceleracao_x_m_s2 = convert_g_to_m_per_s2(aceleracao_x)
    aceleracao_y_m_s2 = convert_g_to_m_per_s2(aceleracao_y)

    # Atualiza as velocidades integrando as acelerações
    velocidade_x_global += aceleracao_x_m_s2 * delta_t
    velocidade_y_global += aceleracao_y_m_s2 * delta_t

    # Calcula a velocidade resultante
    velocidade_resultante = np.sqrt(velocidade_x_global**2 + velocidade_y_global**2)

    return velocidade_resultante

def calcular_aceleracao_resultante(aceleracao_x, aceleracao_y):
    # Converte as acelerações de g para m/s^2 com arredondamento
    aceleracao_x_m_s2 = convert_g_to_m_per_s2(aceleracao_x)
    aceleracao_y_m_s2 = convert_g_to_m_per_s2(aceleracao_y)
    
    # Calcula a aceleração resultante
    aceleracao_resultante = np.sqrt(aceleracao_x_m_s2**2 + aceleracao_y_m_s2**2)
    
    if (aceleracao_resultante < 0.5): return 0.0
    
    return aceleracao_resultante

# Configuração do banco de dados SQLite

DATABASE = 'projeto_integrador.db'

def init_db():
    conn = sqlite3.connect(DATABASE)
    cursor = conn.cursor()

    sql_commands = [
        '''
        CREATE TABLE IF NOT EXISTS ESTATISTICAS_CARRINHO (
          idEstatistica INTEGER PRIMARY KEY AUTOINCREMENT,
          quantidadePercursosConcluidos INT,
          mediaVelocidade DECIMAL(5,2),
          mediaAceleracao DECIMAL(5,2),
          mediaConsumoEnergetico DECIMAL(10,2),
          mediaTamanhoPercursos INT,
          mediaTempoPercursos TEXT,
          quantidadePercursos INT
        )
        ''',
        '''
        CREATE TABLE IF NOT EXISTS REALIZA (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          idCarrinho INT,
          idPercurso INT,
          dataRealizacao TEXT,
          velocidadeInstantanea DECIMAL(5,2),
          aceleracaoInstantanea DECIMAL(5,2),
          trajetoria TEXT,
          consumoEnergetico INT,
          versao VARCHAR(3),
          tempo INT
        )
        '''
    ]

    
    for command in sql_commands:
        cursor.execute(command)

    conn.commit()
    conn.close()

init_db()

""" def read_data_from_esp32():
    try:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Dado lido: {data}")
            return data
        else:
            print("Nenhum dado disponível na porta serial.")
    except Exception as e:
        print(f"Erro ao ler dados: {e}")
    return None """

def save_data_to_sqlite(data):
    try:
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()

        data_json = json.loads(data)

        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        cursor.execute('''
        INSERT INTO REALIZA (idCarrinho, idPercurso, dataRealizacao, velocidadeInstantanea, aceleracaoInstantanea, trajetoria, consumoEnergetico, versao, tempo)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (
            1,  # idCarrinho
            1,  # idPercurso
            current_time,  # dataRealizacao
            calcular_velocidade_resultante(round(data_json.get("aceleracaoX"), 2), round(data_json.get("aceleracaoY"), 2)),  # velocidadeInstantanea
            calcular_aceleracao_resultante(round(data_json.get("aceleracaoX"), 2), round(data_json.get("aceleracaoY"), 2)), # aceleracaoInstantanea
            json.dumps(data_json.get("trajetoria")),  # trajetoria
            data_json.get("consumoEnergetico"),  # consumoEnergetico
            "1.0",  # versao 
            data_json.get("tempo")  # tempo
        ))

        conn.commit()
        cursor.close()
        conn.close()

    except Exception as e:
        print(f"Erro: {e}")
        
def clear_db():
    try:
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()

        cursor.execute("DELETE FROM ESTATISTICAS_CARRINHO")
        cursor.execute("DELETE FROM REALIZA")

        conn.commit()
        cursor.close()
        conn.close()
    except Exception as e:
        print(f"Erro ao limpar o banco de dados: {e}")
        
def reset_db():
    try:
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()

        cursor.execute("DROP TABLE IF EXISTS ESTATISTICAS_CARRINHO")
        cursor.execute("DROP TABLE IF EXISTS REALIZA")

        conn.commit()

        init_db()
        
        cursor.close()
        conn.close()
    except Exception as e:
        print(f"Erro ao resetar o banco de dados: {e}")
        
# ROTAS

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.json
    save_data_to_sqlite(json.dumps(data))
    return jsonify({"status": "success"}), 201

@app.route('/data', methods=['GET'])
def get_data():
    try:
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()

        cursor.execute('SELECT * FROM REALIZA ORDER BY id DESC LIMIT 1')
        row = cursor.fetchone()

        if row:
            data = {
                "id": row[0],
                "idCarrinho": row[1],
                "idPercurso": row[2],
                "dataRealizacao": row[3],
                "velocidadeInstantanea": row[4],
                "aceleracaoInstantanea": row[5],
                "trajetoria": json.loads(row[6]),
                "consumoEnergetico": row[7],
                "versao": row[8],
                "tempo": row[9]
            }
        else:
            data = {}

        cursor.close()
        conn.close()

        return jsonify(data)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    print("Aguardando dados...")
    
    def start_flask():
        app.run(host='0.0.0.0', port=5000)


    flask_thread = Thread(target=start_flask)
    flask_thread.start()
    
    reset_db()
    resetar_velocidades_globais()

    try:
        while True:
            # data = read_data_from_esp32()
            data = '{"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 10, "aceleracaoX": 0.01, "aceleracaoY": -0.01, "aceleracaoZ": 0.84}'
            if data:
                print(f"Dado recebido: {data}")
                save_data_to_sqlite(data)
            time.sleep(1) # se estiver usando a ESP32, alterar para 0.02
    except KeyboardInterrupt:
        print("Programa encerrado")
    """ finally: 
        ser.close() """
