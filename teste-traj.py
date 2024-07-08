import matplotlib.pyplot as plt
import json
import time

# Função atualizada para aceitar dados mockados
def plot_percurso_carrinho_mocked(data_list, dt=0.2, duration=10):
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

    # Processa os dados mockados
    for data in data_list:
        accX = data['aceleracaoX']
        accY = data['aceleracaoY']

        accX_data.append(accX)
        accY_data.append(accY)

        # Integração para obter velocidade e posição
        velX, posX = integrate(accX, velX, posX_data[-1], dt)
        velY, posY = integrate(accY, velY, posY_data[-1], dt)

        posX_data.append(posX)
        posY_data.append(posY)

    # Plotagem do percurso
    plt.figure()
    plt.plot(posX_data, posY_data, marker='o')
    plt.title('Percurso do Carrinho')
    plt.xlabel('Posição X (m)')
    plt.ylabel('Posição Y (m)')
    plt.grid()
    plt.show()

# Array de dados mockados
data_list = [
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 154, "aceleracaoX": 0.038818359, "aceleracaoY": 0.217041016},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 154, "aceleracaoX": 0.115478516, "aceleracaoY": 0.114013672},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 155, "aceleracaoX": 0.039550781, "aceleracaoY": -0.132568359},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 155, "aceleracaoX": -0.063232422, "aceleracaoY": -0.124267578},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 155, "aceleracaoX": -0.056396484, "aceleracaoY": 0.1328125},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 155, "aceleracaoX": -0.049804688, "aceleracaoY": 0.172607422},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 155, "aceleracaoX": -0.056396484, "aceleracaoY": 0.158691406},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 156, "aceleracaoX": 0.055175781, "aceleracaoY": -0.004150391},
    {"trajetoria": [[1, 2], [3, 4], [5, 6]], "consumoEnergetico": 100, "tempo": 156, "aceleracaoX": 0.180419922, "aceleracaoY": 0.041259766}
]

# Chamando a função com os dados mockados
plot_percurso_carrinho_mocked(data_list)
