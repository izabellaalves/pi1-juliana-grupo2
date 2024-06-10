import serial
import json
import time
import os

bluetoothport = 'COM7'  # Substitua pela porta correta encontrada no Gerenciador de Dispositivos
baudrate = 115200
save_directory = 'C:\PITest'  # Substitua pelo caminho desejado

# Cria o diretório se ele não existir
if not os.path.exists(save_directory):
    os.makedirs(save_directory)

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

def save_data_to_json(data, filename='data.json'):
    try:
        filepath = os.path.join(save_directory, filename)
        with open(filepath, 'a') as file:
            file.write(data + '\n')  # Escreve a string JSON diretamente no arquivo
    except Exception as e:
        print(f"Erro ao salvar dados: {e}")

if __name__ == "__main__":
    print("Aguardando dados...")

    try:
        while True:
            data = read_data_from_esp32()
            if data:
                print(f"Dado recebido: {data}")
                save_data_to_json(data)
            time.sleep(1)
    except KeyboardInterrupt:
        print("Programa encerrado")
    finally:
        ser.close()