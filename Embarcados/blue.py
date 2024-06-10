import serial
import json
import time
import os
import psycopg2
from datetime import datetime

bluetoothport = 'COM7'  # Substitua pela porta correta encontrada no Gerenciador de Dispositivos
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

#funcão para salvar os dados no banco de dados
def save_data_to_postgresql(data):
    global last_read_time, last_velocity
    
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

        # calcular velocidade e aceleração instantâneas
        if last_read_time is not None and last_velocity is not None:
            time_diff = (current_time - last_read_time).total_seconds()
            velocity_diff = data_json.get("velocidade") - last_velocity

            instant_velocity = data_json.get("velocidade")
            instant_acceleration = velocity_diff / time_diff
        else:
            instant_velocity = data_json.get("velocidade")
            instant_acceleration = 0

        last_read_time = current_time
        last_velocity = data_json.get("velocidade")

        insert_query = """
        INSERT INTO realiza (idCarrinho, idPercurso, dataRealizacao, velocidadeInstantanea, aceleracaoInstantanea, conclusaoPercurso, consumoEnergetico, versao, conclusao)
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
        """
        cursor.execute(insert_query, (
            1,  # idCarrinho
            1,  # idPercurso
            current_time,  # dataRealizacao
            instant_velocity,  # velocidadeInstantanea
            instant_acceleration,  # aceleracaoInstantanea
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

if __name__ == "_main_":
    print("Aguardando dados")

    try:
        while True:
            data = {
                "trajetoria": {"pontos": [{"x": 0, "y": 0}, {"x": 1, "y": 1}, {"x": 2, "y": 4}]},
                "tempo": 3,
                "velocidade": 56.9,
                "aceleracao": 98.9,
                "consumoEnergetico": 230.5
                }   # read_data_from_esp32()
            
            if data:
                print(f"Dado recebido: {data}")
                save_data_to_postgresql(data)
            time.sleep(1)
    except KeyboardInterrupt:
        print("Programa encerrado")
    """  finally:
        ser.close()
    """


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