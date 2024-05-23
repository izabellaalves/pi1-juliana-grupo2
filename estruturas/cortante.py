import numpy as np
import matplotlib.pyplot as plt

# Dados fornecidos
comprimento_viga = 240  # mm
posicoes_rodas = [65, 175]  # mm
carga_pontos = [(65, (150 + 6*12 + 12*6 + 100) / 1000), (175, (150 + 6*12 + 12*6 + 100) / 1000)]  # kg para cada roda, convertidos para N (considerando g=9.81 m/s^2)
carga_distribuida = 400 / 1000 * 9.81  # kg convertidos para N

# Convertendo kg para N (considerando g=9.81 m/s^2)
forca_pontos = [(pos, carga * 9.81) for pos, carga in carga_pontos]
carga_distribuida = carga_distribuida * 9.81

# Calculando reações nos apoios
R1 = (carga_distribuida * comprimento_viga / 2 + sum([F[1] for F in forca_pontos]) * 175 / comprimento_viga) / 2
R2 = (carga_distribuida * comprimento_viga - R1)

# Vetores para gráfico
x = np.linspace(0, comprimento_viga, 1000)
V = np.zeros_like(x)

# Adicionando forças pontuais
for pos, F in forca_pontos:
    V[x >= pos] -= F

# Adicionando carga distribuída
V -= carga_distribuida * (x >= 0) * (x <= comprimento_viga) * (x - 0.5 * comprimento_viga)

# Adicionando reações nos apoios
V[x >= posicoes_rodas[0]] += R1
V[x >= posicoes_rodas[1]] += R2

# Plotando gráfico de cortante e salvando a imagem
plt.figure(figsize=(10, 6))
plt.plot(x, V, label="Cortante")
plt.xlabel("Posição ao longo da viga (mm)")
plt.ylabel("Força cortante (N)")
plt.title("Diagrama de Força Cortante")
plt.axhline(0, color='black', linewidth=0.5)
plt.grid(True)
plt.legend()
plt.savefig('diagrama_forca_cortante.png')
plt.show()
