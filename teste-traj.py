import matplotlib.pyplot as plt
import json

# Função para plotar o percurso do carrinho com dados mockados
def plot_percurso_carrinho_mocked(data_list, dt=0.2):
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

# Carrega os dados do arquivo JSON
with open('dados.json', 'r') as file:
    data_list = json.load(file)

# Chama a função com os dados do JSON
plot_percurso_carrinho_mocked(data_list)
