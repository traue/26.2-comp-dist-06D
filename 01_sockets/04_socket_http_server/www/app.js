/**
 * =============================================================================
 *  JAVASCRIPT DO SITE DE DEMONSTRAÇÃO
 * =============================================================================
 *
 *  Este arquivo JS é servido pelo nosso servidor HTTP em C.
 *  O navegador faz a requisição: GET /app.js
 *  O servidor responde com Content-Type: application/javascript
 *
 *  Funcionalidades:
 *    1. Verificação de status do servidor
 *    2. Relógio atualizado em tempo real
 *    3. Teste interativo de requisições HTTP
 *    4. Contador simples (demonstra que o JS está funcionando)
 *
 * =============================================================================
 */

/* ========================= VARIÁVEIS GLOBAIS ========================= */

/** Valor atual do contador de demonstração */
let contadorValor = 0;

/* ========================= INICIALIZAÇÃO ========================= */

/**
 * Quando a página terminar de carregar, executamos as funções iniciais.
 * O evento 'DOMContentLoaded' dispara quando o HTML foi completamente
 * parseado (antes de imagens e CSS terminarem de carregar).
 */
document.addEventListener('DOMContentLoaded', function () {
    console.log('🌐 Site carregado com sucesso pelo servidor HTTP em C!');

    /* Verificar se o servidor está respondendo */
    verificarStatusServidor();

    /* Iniciar o relógio que atualiza a cada segundo */
    atualizarRelogio();
    setInterval(atualizarRelogio, 1000);
});

/* ========================= STATUS DO SERVIDOR ========================= */

/**
 * verificarStatusServidor() - Faz uma requisição para verificar se o servidor
 * está online e respondendo corretamente.
 *
 * Usa a Fetch API para fazer uma requisição GET ao próprio servidor.
 * Se a resposta for bem-sucedida, exibe "Online" em verde.
 * Se falhar, exibe "Offline" em vermelho.
 */
function verificarStatusServidor() {
    let indicador = document.getElementById('status-indicator');

    /* 
     * fetch() é a API moderna para fazer requisições HTTP via JavaScript.
     * Aqui estamos pedindo a página raiz '/' do nosso servidor.
     */
    fetch('/')
        .then(function (resposta) {
            if (resposta.ok) {
                indicador.textContent = '● Online';
                indicador.className = 'status-value status-online';
            } else {
                indicador.textContent = '● Erro (' + resposta.status + ')';
                indicador.className = 'status-value status-offline';
            }
        })
        .catch(function (erro) {
            indicador.textContent = '● Offline';
            indicador.className = 'status-value status-offline';
            console.error('Erro ao verificar status:', erro);
        });
}

/* ========================= RELÓGIO ========================= */

/**
 * atualizarRelogio() - Atualiza o elemento de hora na página
 *
 * Cria um objeto Date com a hora atual e formata como HH:MM:SS.
 * É chamada a cada 1 segundo via setInterval().
 */
function atualizarRelogio() {
    let agora = new Date();

    let horas = String(agora.getHours()).padStart(2, '0');
    let minutos = String(agora.getMinutes()).padStart(2, '0');
    let segundos = String(agora.getSeconds()).padStart(2, '0');

    let horaFormatada = horas + ':' + minutos + ':' + segundos;

    let elemento = document.getElementById('hora-local');
    if (elemento) {
        elemento.textContent = horaFormatada;
    }
}

/* ========================= TESTE DE REQUISIÇÕES ========================= */

/**
 * testarRequisicao() - Faz uma requisição GET ao servidor e exibe os detalhes
 *
 * Esta função demonstra como o protocolo HTTP funciona:
 * 1. O navegador (cliente) envia um GET para o caminho especificado
 * 2. O servidor processa a requisição
 * 3. O servidor responde com cabeçalhos + corpo
 * 4. Exibimos os detalhes da resposta na página
 *
 * @param {string} caminho  O caminho a requisitar (ex: '/', '/style.css')
 */
function testarRequisicao(caminho) {
    let resultadoDiv = document.getElementById('resultado-teste');
    let inicio = performance.now(); /* Marca o tempo de início */

    /* Mostrar a área de resultado */
    resultadoDiv.style.display = 'block';

    /* Atualizar a URL exibida */
    document.getElementById('resultado-url').textContent = caminho;
    document.getElementById('resultado-status').textContent = 'Carregando...';
    document.getElementById('resultado-tipo').textContent = '...';
    document.getElementById('resultado-tamanho').textContent = '...';
    document.getElementById('resultado-tempo').textContent = '...';
    document.getElementById('resultado-corpo').textContent = '...';

    /* 
     * Fazer a requisição HTTP usando fetch()
     *
     * fetch() retorna uma Promise que resolve quando os cabeçalhos
     * da resposta são recebidos. O corpo é lido separadamente.
     */
    fetch(caminho)
        .then(function (resposta) {
            let fim = performance.now();
            let tempoMs = (fim - inicio).toFixed(2);

            /* Exibir informações dos cabeçalhos */
            let statusTexto = resposta.status + ' ' + resposta.statusText;
            document.getElementById('resultado-status').textContent = statusTexto;
            document.getElementById('resultado-tipo').textContent =
                resposta.headers.get('Content-Type') || 'não informado';
            document.getElementById('resultado-tamanho').textContent =
                (resposta.headers.get('Content-Length') || '?') + ' bytes';
            document.getElementById('resultado-tempo').textContent = tempoMs + ' ms';

            /* Colorir o status baseado no código */
            let statusEl = document.getElementById('resultado-status');
            if (resposta.status >= 200 && resposta.status < 300) {
                statusEl.style.color = '#4ade80'; /* Verde */
            } else if (resposta.status >= 400) {
                statusEl.style.color = '#f87171'; /* Vermelho */
            }

            /* Ler o corpo da resposta como texto */
            return resposta.text();
        })
        .then(function (corpo) {
            /* Exibir os primeiros 500 caracteres do corpo */
            let preview = corpo.substring(0, 500);
            if (corpo.length > 500) {
                preview += '\n\n... [truncado - ' + corpo.length + ' caracteres no total]';
            }
            document.getElementById('resultado-corpo').textContent = preview;
        })
        .catch(function (erro) {
            /* Em caso de erro na requisição */
            document.getElementById('resultado-status').textContent = 'ERRO';
            document.getElementById('resultado-status').style.color = '#f87171';
            document.getElementById('resultado-corpo').textContent =
                'Erro na requisição: ' + erro.message;
        });

    /* Scroll suave até o resultado */
    resultadoDiv.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
}

/* ========================= CONTADOR ========================= */

/**
 * incrementar() - Aumenta o valor do contador em 1
 * Demonstra que o JavaScript foi carregado e executado corretamente.
 */
function incrementar() {
    contadorValor++;
    atualizarContador();
}

/**
 * decrementar() - Diminui o valor do contador em 1
 */
function decrementar() {
    contadorValor--;
    atualizarContador();
}

/**
 * atualizarContador() - Atualiza o elemento visual do contador
 * e adiciona uma breve animação de escala.
 */
function atualizarContador() {
    let elemento = document.getElementById('contador');
    elemento.textContent = contadorValor;

    /* Animação rápida de "pop" */
    elemento.style.transform = 'scale(1.2)';
    setTimeout(function () {
        elemento.style.transform = 'scale(1)';
    }, 150);
}
