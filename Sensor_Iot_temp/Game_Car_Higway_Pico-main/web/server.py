# Importa as bibliotecas Flask e SocketIO
from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, emit

# Cria a instância do Flask
app = Flask(__name__)

# Configura o SocketIO para permitir conexões de qualquer origem
socketio = SocketIO(app, cors_allowed_origins="*")

# Lista de mensagens iniciais para a interface
mensagens = [
    "Sistema iniciado",
    "Aguardando comandos...",
    "Pico W conectado com sucesso"
]

# Armazena a última mensagem de temperatura (evita sobrescrever lista)
mensagem_atual = None

# Rota principal que serve a página HTML
@app.route('/')
def index():
    return render_template('index.html')


# Rota para a temperatura
@app.route('/temperature', methods=['GET'])
def temperature():
    global mensagem_atual
    temperature = request.args.get('value')

    if temperature:
        try:
            temperature_value = float(temperature)
        except ValueError:
            return jsonify({"status": "erro", "mensagem": "Temperatura inválida"}), 400

        nova_mensagem = f"{temperature_value:.2f}°C"  

        # Apenas atualiza se for diferente da última
        if mensagem_atual != nova_mensagem:
            mensagem_atual = nova_mensagem
            print("Nova temperatura registrada:", mensagem_atual)

        return jsonify([f"Temperatura lida: {temperature_value:.2f}°C"]), 200


    return jsonify({"status": "erro", "mensagem": "Nenhuma temperatura fornecida"}), 400

# Nova rota para fornecer a última temperatura registrada
@app.route('/ultima_temperatura', methods=['GET'])
def ultima_temperatura():
    global mensagem_atual
    if mensagem_atual:
        print('Ultima Temperatura lida', mensagem_atual)
        return jsonify([mensagem_atual]), 200
    else:
        return jsonify(["Nenhuma temperatura registrada ainda"]), 200

# Ponto de entrada principal da aplicação
if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)
