# Arcade Modular - Sistema de Jogos

## Descrição
Projeto de arcade portátil programado em Arduino, com sistema modular que permite rodar múltiplos jogos de forma dinâmica.  
Suporta um ou dois jogadores simultaneamente e oferece interface física com botões, telas de LED e display LCD.

---

## Funcionalidades Implementadas

### Jogos
- **Snake** (Cobrinha)
- **Tetris**
- **Pong**

> **Observação:** Todos os jogos são renderizados em telas LED 8x8, utilizando matrizes controladas por MAX7219.

### Sistema Modular
- Cada jogo implementa uma classe base (`GameBase`) que permite troca dinâmica de jogos no menu.
- Permite rodar dois jogos ao mesmo tempo, um por jogador.

### Interface
- **Botões:** 8 botões lidos via `74HC165 shift register` e 1 botão para voltar ao menu lido pelos pinos do Arduino Uno.
- **Displays:** 4 matrizes LED 8x8 divididas em dois pares + LCD 16x2 para exibição de informações.
- **Buzzer:** 2 buzzer para efeitos sonoros e músicas.
- **Menu:** Menu de configuração para escolher jogos, verificar pontuações e configurar arcade.

### Multiplayer e Configurações
- Suporte para até dois jogadores simultâneos.
- Menu de seleção permite configurar jogos, tal como velocidade e configurações particulares de cada jogo.
- Sistema projetado para futura integração de multiplayer via Wi-Fi e sincronização de pontuações entre unidades.

---

## Tecnologias Utilizadas
- **Linguagem:** C/C++.
- **Plataforma:** Arduino Uno.
- **Componentes:** Shift register 74HC165, matrizes LED 8x8 com MAX7219, LCD 16x2 I2C, buzzers e botões físicos.
- **Arquitetura:** Sistema modular com classes base para jogos e ponteiros para troca dinâmica.

---

## Observações e Limitações
- Pontuações ainda não são sincronizadas entre unidades; suporte Wi-Fi futuro planejado.
> Limitações de memória e ausência de suporte nativo Wi-Fi força a substituição por placas que se adequem melhor à futuras expansões do projeto.
- Sons são limitados ao buzzer integrado, apenas usando durações e frequências.
- Interface física depende de montagem correta; sem o hardware completo, os jogos não podem ser testados integralmente.
- A estrutura modular permite adição de novos jogos, mas exige adaptação à memória e aos pinos disponíveis.

---

## Status Atual
- Jogos implementados: Snake, Tetris, Pong
- Funcionalidades: Menu, 1 ou 2 jogadores, leitura de botões, controle de LEDs, buzzer, pontuações locais
- Futuro: Multiplayer online, suporte Wi-Fi, mais jogos, melhorias no menu e interface
