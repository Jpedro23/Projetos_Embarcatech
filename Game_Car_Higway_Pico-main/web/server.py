# Importa as bibliotecas Flask e SocketIO
from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit
from flask import jsonify  # Adiciona essa importação lá em cima também

# Cria a instância do Flask
app = Flask(__name__)

# Configura o SocketIO para permitir conexões de qualquer origem
socketio = SocketIO(app, cors_allowed_origins="*")

# listagem de  mensagens para exibir na pagina

mensagens = [
    "Sistema iniciado",
    "Aguardando comandos...",
    "Pico W conectado com sucesso"
    ]

# Rota principal que serve a página HTML
@app.route('/')
def index():
    return render_template('teste.html', mensagens=mensagens) # Renderiza o template web/templates/index.html

# Rota para as mensagens
@app.route('/get_messages', methods= ['GET', 'POST'])
def get_messages():
    print("A mensagem foi enviado")
    socketio.emit('command', {'action': 'messages'})  # Envia comando via WebSocket
    return jsonify(mensagens)
    

# Rota para a temperatura
@app.route('/temperature', methods=['GET'])
def temperature():
    temperature = request.args.get('value')

    if temperature:
        try:
            temperature_value = float(temperature)
        except ValueError:
            return jsonify({"status": "erro", "mensagem": "Temperatura inválida"}), 400

        nova_mensagem = f"Temperatura lida: {temperature_value:.2f}°C"

        # Verifica se já existe uma mensagem de temperatura e atualiza
        temperatura_ja_adicionada = False
        for i, msg in enumerate(mensagens):
            if "Temperatura lida" in msg:
                mensagens[i] = nova_mensagem
                temperatura_ja_adicionada = True
                break

        # Se não encontrou nenhuma, adiciona no final
        if not temperatura_ja_adicionada:
            mensagens.append(nova_mensagem)

        return jsonify({"status": "sucesso", "temperatura": f"{temperature_value:.2f}"}), 200

    return jsonify({"status": "erro", "mensagem": "Nenhuma temperatura fornecida"}), 400


# Rota para comando "left"
@app.route('/left', methods=['GET', 'POST'])
def left():
    print("Comando: Left")  # Imprime no console (útil para debug)
    socketio.emit('command', {'action': 'left'})  # Envia comando via WebSocket
    return 'Left command sent', 200  # Retorna uma resposta HTTP padrão

# Rota para comando "right"
@app.route('/right', methods=['GET', 'POST'])
def right():
    print("Comando: Right")
    socketio.emit('command', {'action': 'right'})
    return 'Right command sent', 200

# Ponto de entrada principal da aplicação
if __name__ == '__main__':
    # Inicia o servidor Flask com suporte a WebSockets
    socketio.run(app, host='0.0.0.0', port=5000)
