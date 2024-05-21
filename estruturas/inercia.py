import pandas as pd

def obter_float(mensagem):
    while True:
        try:
            valor = float(input(mensagem))
            return valor
        except ValueError:
            print("Por favor, insira um número válido.")

def obter_int(mensagem):
    while True:
        try:
            valor = int(input(mensagem))
            return valor
        except ValueError:
            print("Por favor, insira um número inteiro válido.")

def obter_apoios():
    num_apoios = obter_int("Digite o número de apoios para a estrutura: ")
    apoios = []
    for _ in range(num_apoios):
        posicao_apoio = obter_float("Digite a posição do apoio (mm): ")
        tipo_apoio = input("Digite o tipo de apoio (simples, engaste): ").strip().lower()
        apoios.append({'posicao': posicao_apoio, 'tipo': tipo_apoio})
    return apoios

def obter_cargas():
    num_cargas = obter_int("Digite o número de cargas aplicadas para a estrutura: ")
    cargas = []
    for _ in range(num_cargas):
        intensidade_carga = obter_float("Digite a intensidade da carga (N): ")
        posicao_carga = obter_float("Digite a posição da carga (mm): ")
        tipo_carga = input("Digite o tipo de carga (pontual, distribuída): ").strip().lower()
        cargas.append({'intensidade': intensidade_carga, 'posicao': posicao_carga, 'tipo': tipo_carga})
    return cargas

def criar_dados_estruturais(nome_arquivo='inercia.xlsx'):
    estruturas = []
    while True:
        comprimento = obter_float("Digite o comprimento da estrutura (mm): ")

        apoios = obter_apoios()
        cargas = obter_cargas()

        estruturas.append({
            'comprimento': comprimento,
            'apoios': str(apoios),
            'cargas': str(cargas)
        })

        cont = input("Deseja adicionar outra estrutura? (s/n): ").strip().lower()
        if cont != 's':
            break

    df = pd.DataFrame(estruturas)
    df.to_excel(nome_arquivo, index=False)
    print(f'Dados salvos em {nome_arquivo}')

if __name__ == "__main__":
    criar_dados_estruturais()