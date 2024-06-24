import serial
import random
import time
import json
import matplotlib.pyplot as plt

# Configuração da porta serial
# ser = serial.Serial('/dev/ttyUSB0', 115200)  # Substitua pela sua porta serial
# Simulação da porta serial
class FakeSerial:
    def __init__(self):
        self.in_waiting = True

    def readline(self):
        accX = random.uniform(-1, 1)  # Gera aceleração X aleatória entre -1 e 1
        accY = random.uniform(-1, 1)  # Gera aceleração Y aleatória entre -1 e 1
        data = json.dumps({'accX': accX, 'accY': accY})
        return data.encode('utf-8')

# Substitui a porta serial real por uma fake
ser = FakeSerial()

# Tempo entre as amostras
dt = 0.2

# Listas para armazenar os dados
accX_data = []
accY_data = []
posX_data = [0]
posY_data = [0]

# Inicialização das velocidades
velX = 0
velY = 0

def integrate(acc, vel, pos, dt):
    vel += acc * dt
    pos += vel * dt
    return vel, pos

# Coleta de dados por 10 segundos
start_time = time.time()
while time.time() - start_time < 10:
    if ser.in_waiting:
        line = ser.readline().decode('utf-8').strip()
        try:
            data = json.loads(line)
            accX = data['accX']
            accY = data['accY']
            
            accX_data.append(accX)
            accY_data.append(accY)
            
            # Integração para obter velocidade e posição
            velX, posX = integrate(accX, velX, posX_data[-1], dt)
            velY, posY = integrate(accY, velY, posY_data[-1], dt)
            
            posX_data.append(posX)
            posY_data.append(posY)
        except json.JSONDecodeError:
            print("Erro ao decodificar JSON")
        except KeyError:
            print("Chave não encontrada no JSON")

# Plotagem do percurso
plt.figure()
plt.plot(posX_data, posY_data, marker='o')
plt.title('Percurso do Carrinho')
plt.xlabel('Posição X (m)')
plt.ylabel('Posição Y (m)')
plt.grid()
plt.show()
