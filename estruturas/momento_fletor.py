import numpy as np
import matplotlib.pyplot as plt

# Dados fornecidos
comprimento_viga = 240  # mm
posicoes_rodas = [65, 175]  # mm
massa_estrutura = 150  # g
massa_parafusos = 6 * 12  # g
massa_porcas = 12 * 6  # g
massa_ovo = 60  # g (aproximado)
massa_bateria = 100  # g
carga_distribuida = 400  # g

# Convertendo massas para N (considerando g=9.81 m/s^2)
carga_total_ponto = (massa_estrutura + massa_parafusos + massa_porcas + massa_ovo + massa_bateria) / 1000 * 9.81  # N
carga_distribuida = carga_distribuida / 1000 * 9.81  # N/m

# Reações nos apoios (R1 e R2)
# Somatória de forças e momentos para sistema isostático
R1 = (carga_distribuida * comprimento_viga / 2 + carga_total_ponto * (comprimento_viga - 175) / comprimento_viga)
R2 = (carga_distribuida * comprimento_viga - R1)

# Vetores para gráfico
x = np.linspace(0, comprimento_viga, 1000)
V = np.zeros_like(x)

# Cálculo da força cortante ao longo da viga
for i in range(len(x)):
    if x[i] < posicoes_rodas[0]:
        V[i] = R1
    elif x[i] < posicoes_rodas[1]:
        V[i] = R1 - carga_distribuida * (x[i] - posicoes_rodas[0])
    else:
        V[i] = R2 - carga_distribuida * (x[i] - posicoes_rodas[1]) - carga_total_ponto

# Cálculo do momento fletor ao longo da viga
M = np.zeros_like(x)
for i in range(1, len(x)):
    M[i] = M[i-1] + V[i] * (x[i] - x[i-1])

# Ajustando os valores do momento para garantir que feche em zero nos apoios
ajuste = M[-1] / comprimento_viga
M = M - ajuste * x

# Plotando gráfico de momento fletor e salvando a imagem com eixo positivo para baixo
plt.figure(figsize=(10, 6))
plt.plot(x, M, label="Momento Fletor", color='orange')
plt.xlabel("Posição ao longo da viga (mm)")
plt.ylabel("Momento fletor (N.mm)")
plt.title("Diagrama de Momento Fletor")
plt.axhline(0, color='black', linewidth=0.5)
plt.gca().invert_yaxis()  # Invertendo o eixo y para positivo para baixo
plt.grid(True)
plt.legend()
plt.savefig('/mnt/data/diagrama_momento_fletor_ideal_invertido.png')
plt.show()
