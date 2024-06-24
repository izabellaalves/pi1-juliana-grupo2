import serial
import json
import time
import os
import psycopg2
from datetime import datetime

bluetoothport = '/dev/rfcomm0'  # Substitua pela porta correta encontrada no Gerenciador de Dispositivos
baudrate = 115200

try:
    ser = serial.Serial(bluetoothport, baudrate, timeout=1)
    print(f"Conectado à porta {bluetoothport} com baudrate {baudrate}")
except Exception as e:
    print(f"Erro ao conectar na porta serial: {e}")
    exit()

def read_data_from_esp32():
    try:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Dado lido: {data}")  # Mensagem de depuração
            return data
        else:
            print("Nenhum dado disponível na porta serial.")  # Mensagem de depuração
    except Exception as e:
        print(f"Erro ao ler dados: {e}")
    return None

def save_data_to_postgresql(data):
    
    try:
        connection = psycopg2.connect(
        user="alunos",
        password="alunospi1",
        host="localhost",
        port="5432",
        database="ProjetoIntegrador1"
        )
        
        cursor = connection.cursor()

        data_json = json.loads(data)

        current_time = datetime.now()


        insert_query = """
        INSERT INTO realiza (idCarrinho, idPercurso, dataRealizacao, velocidadeInstantanea, aceleracaoInstantanea, conclusaoPercurso, consumoEnergetico, versao, conclusao)
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
        """
        cursor.execute(insert_query, (
            1,  # idCarrinho
            1,  # idPercurso
            current_time,  # dataRealizacao
            data_json.get('velocidade'),  # velocidadeInstantanea
            data_json.get('aceleracao'),  # aceleracaoInstantanea
            json.dumps(data_json.get("trajetoria")),  # conclusaoPercurso
            data_json.get("consumoEnergetico"),  # consumoEnergetico
            1,  # versao 
            True  # conclusao 
        ))

        connection.commit()

        cursor.close()
        connection.close()

    except Exception as e:
        print(f"Erro: {e}")

if __name__ == "__main__":
    print("Aguardando dados...")

    try:
        while True:
            data = read_data_from_esp32()
            if data:
                print(f"Dado recebido: {data}")
                save_data_to_postgresql(data)
            time.sleep(1)
    except KeyboardInterrupt:
        print("Programa encerrado")
    finally:
        ser.close()