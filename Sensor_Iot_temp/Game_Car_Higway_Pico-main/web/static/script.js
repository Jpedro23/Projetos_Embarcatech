const physicalButton = document.getElementById('physicalButton');
document.addEventListener('DOMContentLoaded', function () {
    const temperatureElement = document.getElementById('temperature');
    const lastUpdateElement = document.getElementById('last-update');
    const statusMessageElement = document.getElementById('status-message');
    const temperatureContainer = document.getElementById('temperature-container');

    // Função para buscar a temperatura real do servidor
    async function atualizarTemperatura() {
        try {
            statusMessageElement.textContent = "Atualizando temperatura...";

            const response = await fetch('/ultima_temperatura');
            const data = await response.json();

            // Verifica se os dados estão no formato esperado (ex: [25.3])
            if (Array.isArray(data) && data.length > 0) {
                const temperatura = data[0];

                // Atualiza a interface
                temperatureElement.textContent = `${temperatura}`;

                // Adiciona efeito de animação
                temperatureContainer.classList.add('updating');
                setTimeout(() => {
                    temperatureContainer.classList.remove('updating');
                }, 1000);

                // Atualiza o horário da última atualização
                const now = new Date();
                const timeString = now.toLocaleTimeString('pt-BR');
                lastUpdateElement.textContent = timeString;

                statusMessageElement.textContent = "Sensor conectado";
            } else {
                statusMessageElement.textContent = "Dados inválidos recebidos do sensor.";
            }
        } catch (error) {
            statusMessageElement.textContent = "Erro ao conectar com o sensor. Tentando novamente...";
            console.error("Erro ao atualizar temperatura:", error);
        }
    }

    // Atualiza a temperatura imediatamente e depois a cada 3 segundos
    atualizarTemperatura();
    setInterval(atualizarTemperatura, 3000);

    // Adiciona interatividade ao clicar no cartão
    temperatureContainer.addEventListener('click', () => {
        atualizarTemperatura();
        statusMessageElement.textContent = "Atualizando manualmente...";
    });
});
