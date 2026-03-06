// Definições principais
#include <MatrizLed.h>
MatrizLed lc1;
MatrizLed lc2;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <EEPROM.h>
//EEPROM.put(addr, var);
//EEPROM.get(addr, var);

// Pino do buzzer
#define buzzerPin 9

// Botão para nova escolha
#define resetPin 10

// Pinos da Matrizes de LEDs
#define DIN_PIN1 7
#define CLK_PIN1 6
#define CS_PIN1 5
#define NUM_MATRIX1 2

#define DIN_PIN2 13
#define CLK_PIN2 12
#define CS_PIN2 11
#define NUM_MATRIX2 2

// Pinos do CI (74hc165n)
#define dataPin 2   // Q7
#define clockPin 3  // CP
#define latchPin 4  // PL

byte buttonState;  // Variável para leitura dos botões

// Dimensões totais das matrizes
const uint8_t MatrixLength = 32;  // x
const uint8_t MatrixHeight = 8;   // y

uint8_t splitGame = false;    // Variável que verifica se os jogos serão dividos
uint8_t soundEffects = true;  // Sons de efeito
uint8_t soundTrack = true;    // Música
uint8_t Xplayers = 1;
uint8_t brightness = 8;  // Brilho

const char scoreMessage[] PROGMEM = "Score pl. ";
uint16_t score[2] = { 0 };
bool scoreUpdated = false;

#define tetris_Best_ScoreEEPROMAddr 10
#define tetris_Your_ScoreEEPROMAddr 12
#define tetris_Dev_ScoreEEPROMAddr 14
#define snake_Best_ScoreEEPROMAddr 16
#define snake_Your_ScoreEEPROMAddr 18
#define snake_Dev_ScoreEEPROMAddr 20
#define pong_Best_ScoreEEPROMAddr 22
#define pong_Your_ScoreEEPROMAddr 24
#define pong_Dev_ScoreEEPROMAddr 26

uint16_t getScore(uint16_t addr);

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0

struct notes {
  uint16_t frequency;
  uint8_t duration;
};

const notes Bloodtears[] PROGMEM{
  { REST, 4 }, { NOTE_G5, 4 }, { NOTE_A5, 4 }, { NOTE_AS5, 4 }, { NOTE_A5, 4 }, { NOTE_F5, 4 }, { NOTE_A5, 4 }, { NOTE_G5, 4 },
  { REST, 4 }, { NOTE_G5, 4 }, { NOTE_A5, 4 }, { NOTE_AS5, 4 }, { NOTE_C6, 4 }, { NOTE_AS5, 4 }, { NOTE_A5, 4 }, { NOTE_G5, 4 },
  { REST, 4 }, { NOTE_G5, 4 }, { NOTE_A5, 4 }, { NOTE_AS5, 4 }, { NOTE_A5, 4 }, { NOTE_F5, 4 }, { NOTE_A5, 4 }, { NOTE_G5, 4 },
  { NOTE_D6, 4 }, { REST, 8 }, { NOTE_C6, 8 }, { REST, 4 }, { NOTE_AS5, 4 }, { NOTE_A5, 4 }, { NOTE_AS5, 8 }, { NOTE_C6, 8 },
  { NOTE_F6, 8 }, { REST, 8 }, { REST, 4 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 },
  { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 },
  { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 },
  { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 },
  { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, 
  { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_C6, 16 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, 
  { REST, 16 }, { REST, 8 }, { REST, 4 }, { NOTE_C6, 16 }, { NOTE_AS5, 16 }, { NOTE_C6, -8 }, { NOTE_F6, -8 }, { NOTE_D6, -4 }, 
  { NOTE_C6, 8 }, { NOTE_AS5, 8 }, { NOTE_C6, 8 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { REST, 4 }, 
  { NOTE_C6, 8 }, { NOTE_D6, 8 }, { NOTE_DS6, -8 }, { NOTE_F6, -8 }, { NOTE_D6, -8 }, { REST, 16 }, { NOTE_DS6, 8 }, { REST, 8 }, 
  { NOTE_C6, 8 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { REST, 4 }, { NOTE_C6, 8 }, { NOTE_AS5, 8 }, 
  { NOTE_C6, -8 }, { NOTE_F6, -8 }, { NOTE_D6, -4 }, { NOTE_C6, 8 }, { NOTE_AS5, 8 }, { NOTE_C6, 8 }, { NOTE_F6, 16 }, 
  { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { NOTE_C6, 8 }, { NOTE_D6, 8 }, { NOTE_DS6, -8 }, { NOTE_F6, -8 }, 
  { NOTE_D5, 8 }, { NOTE_FS5, 8 }, { NOTE_F5, 8 }, { NOTE_A5, 8 }, { NOTE_A5, -8 }, { NOTE_G5, -4 }, { NOTE_A5, -8 }, 
  { NOTE_G5, -4 }, { NOTE_A5, -8 }, { NOTE_G5, -4 }, { NOTE_AS5, 8 }, { NOTE_A5, 8 }, { NOTE_G5, 8 }, { NOTE_F5, 8 }, 
  { NOTE_A5, -8 }, { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_A5, -8 }, { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_A5, -8 }, 
  { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_AS5, 4 }, { NOTE_C6, 4 }, { NOTE_A5, 4 }, { NOTE_AS5, 4 }, { NOTE_G5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 },
  { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, 
  { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, 
  { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, 
  { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_D6, 16 }, { NOTE_D5, 16 }, 
  { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_C6, 16 }, { NOTE_D5, 16 }, { NOTE_AS5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, 
  { NOTE_D5, 16 }, { NOTE_F5, 16 }, { NOTE_D5, 16 }, { NOTE_A5, 16 }, { NOTE_D5, 16 }, { NOTE_G5, 16 }, { NOTE_D5, 16 }, 
  { NOTE_C6, 16 }, { NOTE_C6, 16 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { REST, 4 }, { NOTE_C6, 16 }, 
  { NOTE_AS5, 16 }, { NOTE_C6, -8 }, { NOTE_F6, -8 }, { NOTE_D6, -4 }, { NOTE_C6, 8 }, { NOTE_AS5, 8 }, { NOTE_C6, 8 }, 
  { NOTE_F6, 16 }, { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { REST, 4 }, { NOTE_C6, 8 }, { NOTE_D6, 8 }, { NOTE_DS6, -8 }, 
  { NOTE_F6, -8 }, { NOTE_D6, -8 }, { REST, 16 }, { NOTE_DS6, 8 }, { REST, 8 }, { NOTE_C6, 8 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, 
  { REST, 16 }, { REST, 8 }, { REST, 4 }, { NOTE_C6, 8 }, { NOTE_AS5, 8 }, { NOTE_C6, -8 }, { NOTE_F6, -8 }, { NOTE_D6, -4 }, 
  { NOTE_C6, 8 }, { NOTE_AS5, 8 }, { NOTE_C6, 8 }, { NOTE_F6, 16 }, { NOTE_D6, 8 }, { REST, 16 }, { REST, 8 }, { NOTE_C6, 8 }, 
  { NOTE_D6, 8 }, { NOTE_DS6, -8 }, { NOTE_F6, -8 }, { NOTE_D5, 8 }, { NOTE_FS5, 8 }, { NOTE_F5, 8 }, { NOTE_A5, 8 }, 
  { NOTE_A5, -8 }, { NOTE_G5, -4 }, { NOTE_A5, -8 }, { NOTE_G5, -4 }, { NOTE_A5, -8 }, { NOTE_G5, -4 }, { NOTE_AS5, 8 }, 
  { NOTE_A5, 8 }, { NOTE_G5, 8 }, { NOTE_F5, 8 }, { NOTE_A5, -8 }, { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_A5, -8 }, 
  { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_A5, -8 }, { NOTE_G5, -8 }, { NOTE_D5, 8 }, { NOTE_AS5, 4 }, { NOTE_C6, 4 }, 
  { NOTE_A5, 4 }, { NOTE_AS5, 4 }
};

struct song {
  notes* note;
  uint16_t muchNotes;
  uint8_t tempo;
};

uint8_t currentSong = 0;

const song songs[] PROGMEM {
  {Bloodtears, (sizeof(Bloodtears) / sizeof(notes)), 144}
};


// Fim das definições

// Função para atualizar os estados dos botões
void readButtons() {
  // Atualiza os valores no CI
  digitalWrite(latchPin, LOW);
  delayMicroseconds(5);
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(5);  // Atrasos mínimos para garantir estabilidade

  buttonState = 0;  // Limpa o estado dos botões
  // Ordem MSBFIRST (Mais significante primeiro)
  for (int8_t i = 7; i >= 0; i--) {
    digitalWrite(clockPin, LOW);  // Prepara para o próximo pulso
    delayMicroseconds(2);

    buttonState |= digitalRead(dataPin) << i;  // Faz leitura do pino atual do CI e armazena na variável

    digitalWrite(clockPin, HIGH);  // Pulsa para a próxima entrada do CI
    delayMicroseconds(2);
  }
}

// Função para verificar se o tempo de delay passou
bool checkDelay(unsigned long& lastUpdate, unsigned long delay) {
  unsigned long now = millis();  // Obtém o tempo atual em milissegundos
  if (now - lastUpdate >= delay) {
    lastUpdate = now;  // Atualiza o tempo de última atualização
    return true;       // Retorna verdadeiro se o intervalo foi atingido
  }
  return false;  // Caso contrário, retorna falso
}

void showScores() {
  if (!scoreUpdated) {
    char buffer[11];
    strcpy_P(buffer, scoreMessage);
    lcd.clear();
    for (uint8_t i = 0; i < Xplayers; i++) {
      lcd.setCursor(0, i);
      lcd.print(buffer);
      lcd.print(i + 1);
      lcd.print(F(": "));
      lcd.print(score[i]);
    }
    scoreUpdated = true;
  }
}

// Classe base para cada jogo
class GameBase {
private:
  // Variáveis para debounce (atraso)
  unsigned long* buttonDebounce;

protected:
  // Tamanho da tela (útil para o split mode)
  const uint8_t Matrix_Length;
  const uint8_t Matrix_Height;
  const uint8_t gameAddr;  // Variável que indica qual matriz utilizar

  // Variáveis para inicialização da classe base
  byte** board;
  uint8_t players;
  bool gameOver = false;
  const uint8_t gameDelay;
  unsigned long gameDebounce = 0;

  void (GameBase::*VirtualLoop)();
  void (GameBase::*VirtualDestructor)() = nullptr;

public:
  GameBase(uint8_t Players, uint8_t Length, uint8_t Height, uint8_t playerAddr, uint8_t game_Delay)
    : players(Players), Matrix_Length(Length), Matrix_Height(Height), gameAddr(playerAddr), gameDelay(game_Delay) {
    board = new byte*[Matrix_Length / 8];  // Aloca a matriz das linhas
    for (uint8_t i = 0; i < Matrix_Length / 8; i++) {
      board[i] = new byte[Matrix_Height];  // Aloca a matriz das colunas em cada linha
    }

    buttonDebounce = new unsigned long[Players];

    for (uint8_t i = 0; i < Players; i++) {
      buttonDebounce[i] = 0;
    }

    clearBoard();  // Inicializa a matriz com 0 em todas posições
  }

  ~GameBase() {
    if (VirtualDestructor) {
      (this->*VirtualDestructor)();
    }
    for (uint8_t i = 0; i < Matrix_Length / 8; i++) {
      delete[] board[i];
    }

    delete[] board;
    delete[] buttonDebounce;
  }

  // Lógica loop para cada jogo
  void loop() {
    (this->*VirtualLoop)();
  };

protected:
  // Funções de base para classes derivadas
  void setLed(uint8_t x, uint8_t y, bool state) {
    // Adaptação para matrizes 8x8
    uint8_t MatrixIndex = x / 8;
    uint8_t row = x % 8;
    if (x >= 0 && x < Matrix_Length && y >= 0 && y < Matrix_Height) {
      if (!splitGame) {
        if (x < 16) {
          lc1.setLed(MatrixIndex, row, y, state);
        } else {
          lc2.setLed((MatrixIndex - 2), row, y, state);
        }
      } else {
        if (gameAddr == 1) {
          lc1.setLed(MatrixIndex, row, y, state);
        } else {
          lc2.setLed(MatrixIndex, row, y, state);
        }
      }
    }
  }


  void setBoard(uint8_t x, uint8_t y, bool state) {
    if (x >= 0 && x < Matrix_Length && y >= 0 && y < Matrix_Height) {
      bitWrite(board[x / 8][y], 7 - (x % 8), state);
    }
  }

  void clearBoard() {
    for (uint8_t i = 0; i < Matrix_Length; i++) {
      for (uint8_t j = 0; j < Matrix_Height; j++) {
        bitWrite(board[i / 8][j], 7 - (i % 8), 0);
        setLed(i, j, 0);
      }
    }
  }

  bool readBoard(uint8_t x, uint8_t y) {
    if (x >= 0 && x < Matrix_Length && y >= 0 && y < Matrix_Height) {
      return bitRead(board[x / 8][y], 7 - (x % 8));
    } else {
      return false;
    }
  }

  // Ponteiros para métodos de instância, simulando métodos virtuais economizando memória RAM
  void (GameBase::*player1Up)() = nullptr;
  void (GameBase::*player1Right)() = nullptr;
  void (GameBase::*player1Down)() = nullptr;
  void (GameBase::*player1Left)() = nullptr;
  void (GameBase::*player2Up)() = nullptr;
  void (GameBase::*player2Right)() = nullptr;
  void (GameBase::*player2Down)() = nullptr;
  void (GameBase::*player2Left)() = nullptr;

  // Método que chamará as funções de cada botão
  // Uso direto nas classes derivadas após declarar cada função virtual
  void handleButtons() {
    readButtons();
    if (buttonState) {
      if (!splitGame) {
        if (checkDelay(buttonDebounce[0], gameDelay)) {
          if (bitRead(buttonState, 0)) {
            if (player1Up) {
              (this->*player1Up)();
            }
          } else if (bitRead(buttonState, 1)) {
            if (player1Left) {
              (this->*player1Left)();
            }
          } else if (bitRead(buttonState, 2)) {
            if (player1Down) {
              (this->*player1Down)();
            }
          } else if (bitRead(buttonState, 3)) {
            if (player1Right) {
              (this->*player1Right)();
            }
          }
        }
        if (players == 2 && checkDelay(buttonDebounce[1], gameDelay)) {
          if (bitRead(buttonState, 4)) {
            if (player2Up) {
              (this->*player2Up)();
            }
          } else if (bitRead(buttonState, 5)) {
            if (player2Left) {
              (this->*player2Left)();
            }
          } else if (bitRead(buttonState, 6)) {
            if (player2Down) {
              (this->*player2Down)();
            }
          } else if (bitRead(buttonState, 7)) {
            if (player2Right) {
              (this->*player2Right)();
            }
          }
        }
      } else {
        if (checkDelay(buttonDebounce[0], gameDelay)) {
          uint8_t buttonIndex = (gameAddr == 1) ? 0 : 4;
          if (bitRead(buttonState, buttonIndex)) {
            if (player1Up) {
              (this->*player1Up)();
            }
          } else if (bitRead(buttonState, buttonIndex + 1)) {
            if (player1Left) {
              (this->*player1Left)();
            }
          } else if (bitRead(buttonState, buttonIndex + 2)) {
            if (player1Down) {
              (this->*player1Down)();
            }
          } else if (bitRead(buttonState, buttonIndex + 3)) {
            if (player1Right) {
              (this->*player1Right)();
            }
          }
        }
      }
    }
  }
};  // Fim de GameBase

// Classe do jogo Pong
class PongGame : public GameBase {
private:
  // Variáveis do jogo Pong
  uint8_t ballX, ballY;
  int8_t directionX, directionY;
  uint8_t paddle[2];
  uint8_t botErrorChance;
  uint8_t gameSpeed = gameDelay;

  // Funções para lógica do jogo Pong
  void setPaddle(uint8_t addr, bool state) {
    // Inicializa as raquetes conforme o estado
    for (int8_t i = -1; i <= 1; i++) {
      setLed((addr == 0) ? 0 : Matrix_Length - 1, (paddle[addr] + i), state);
    }
  }

  void Vplayer1Up() {
    if (paddle[0] < 6) {
      // Atualiza as raquetes
      setPaddle(0, 0);
      paddle[0]++;
      setPaddle(0, 1);
    }
  }
  void Vplayer2Up() {
    if (paddle[1] < 6) {
      setPaddle(1, 0);
      paddle[1]++;
      setPaddle(1, 1);
    }
  }
  void Vplayer1Down() {
    if (paddle[0] > 1) {
      setPaddle(0, 0);
      paddle[0]--;
      setPaddle(0, 1);
    }
  }
  void Vplayer2Down() {
    if (paddle[1] > 1) {
      setPaddle(1, 0);
      paddle[1]--;
      setPaddle(1, 1);
    }
  }

  void moveBall() {
    // Verifica colisões
    if ((ballY + directionY) >= Matrix_Height || (ballY + directionY) < 0) {
      directionY *= -1;
      // Som de colisão
      tone(buzzerPin, 300, 20);
      // Aumenta a velocidade de jogo
      if (gameSpeed > 50) {
        gameSpeed -= 2;
      }
    }
    if (((ballX + directionX) == 0 && ((ballY + directionY) == (paddle[0] - 1) || (ballY + directionY) == paddle[0] || (ballY + directionY) == (paddle[0] + 1))) || ((ballX + directionX) == (Matrix_Length - 1) && ((ballY + directionY) == (paddle[1] - 1) || (ballY + directionY) == paddle[1] || (ballY + directionY) == (paddle[1] + 1)))) {
      directionX *= -1;
      // Som de colisão
      tone(buzzerPin, 300, 20);
      // Aumenta a velocidade de jogo
      if (gameSpeed > 50) {
        gameSpeed -= 2;
      }
    }
    // Verifica se a bolinha passou dos limites da tela
    if ((ballX + directionX) >= Matrix_Length) {
      if (!splitGame && Xplayers == 2) {
        score[0]++;
      } else {
        score[gameAddr - 1]++;
      }
      gameOver = true;
    } else if ((ballX + directionX) < 0) {
      if (!splitGame && Xplayers == 2) {
        score[1]++;
      } else {
        score[gameAddr - 1] = (score[gameAddr - 1] > 0) ? score[gameAddr - 1] - 1 : 0;
      }
      gameOver = true;
    }

    // Atualiza a posição da bolinha
    setLed(ballX, ballY, 0);

    ballX += directionX;
    ballY += directionY;

    setLed(ballX, ballY, 1);
  }

  void botMove() {
    // Lógica do bot
    if (random(100) >= botErrorChance && ballX > (Matrix_Length / 2)) {
      if (paddle[1] < (ballY + directionY) && paddle[1] < 6) {
        setPaddle(1, 0);
        paddle[1] += 1;
        setPaddle(1, 1);
      }
      if (paddle[1] > (ballY + directionY) && paddle[1] > 1) {
        setPaddle(1, 0);
        paddle[1] -= 1;
        setPaddle(1, 1);
      }
    }
  }

  void reset() {
    // Animação de Game Over não implementada
    clearBoard();

    // Reinicia variáveis
    ballX = Matrix_Length / 2;
    ballY = random(Matrix_Height);
    do { directionX = random(-1, 2); } while (directionX == 0);
    do { directionY = random(-1, 2); } while (directionY == 0);
    gameSpeed = gameDelay;

    paddle[0] = Matrix_Height / 2;
    paddle[1] = Matrix_Height / 2;

    setPaddle(0, 1);
    setPaddle(1, 1);

    setLed(ballX, ballY, 1);
    scoreUpdated = false;
    gameOver = false;
  }

public:
  PongGame(uint8_t Players, uint8_t Length, uint8_t Height, uint8_t playerAddr, uint8_t gameDelay, uint8_t difficult = 1)
    : GameBase(Players, Length, Height, playerAddr, gameDelay) {
    player1Up = (void(GameBase::*)()) & PongGame::Vplayer1Up;
    player1Down = (void(GameBase::*)()) & PongGame::Vplayer1Down;
    player2Up = (void(GameBase::*)()) & PongGame::Vplayer2Up;
    player2Down = (void(GameBase::*)()) & PongGame::Vplayer2Down;

    VirtualLoop = (void(GameBase::*)()) & PongGame::Vloop;

    // Configura a chance de erro do bot conforme a dificuldade
    botErrorChance = 80 / difficult;

    gameOver = true;
  }

  void Vloop() {
    handleButtons();
    if (checkDelay(gameDebounce, gameSpeed)) {
      if (players == 1) {
        botMove();
      }
      moveBall();
      if (gameOver) {
        reset();
      }
    }
  }
};  // Fim de PongGame

// Classe do jogo da cobrinha
class SnakeGame : public GameBase {
private:
  // Estruturas de variáveis do jogo da cobrinha
  struct coords {
    uint8_t x;
    uint8_t y;
  };

  struct vars {
    uint8_t length, segment;
    int8_t directionX, directionY;
    bool SAMS;
    coords* body;
  };

  coords* food;
  bool wallCollision, gameOver;
  uint8_t foodQnt, initialLength;

  vars* Snake;
  unsigned long foodBlink = 0;

  // Funções do jogo da cobrinha
  void checkCollision(vars snake) {
    int nextX = snake.body[0].x + snake.directionX;
    int nextY = snake.body[0].y + snake.directionY;

    if (wallCollision) {
      if (nextY < 0 || nextY >= Matrix_Height || nextX < 0 || nextX >= Matrix_Length) {
        gameOver = true;
        return;
      }
    }

    if (readBoard(nextX, nextY)) {
      gameOver = true;
    }
  }

  void updateBody(vars& snake) {
    if (snake.segment < snake.length) {
      if (snake.SAMS) {  // Atualiza o último segmento da cobra quando ele estiver no seu tamanho máximo
        setLed(snake.body[snake.segment].x, snake.body[snake.segment].y, 0);
        setBoard(snake.body[snake.segment].x, snake.body[snake.segment].y, 0);
      }
      // Atualiza o "cache" de da posição de cada segmento
      snake.body[snake.segment].x = snake.body[0].x;
      snake.body[snake.segment].y = snake.body[0].y;

      snake.segment++;  // Incrimenta para a próxima atualização
      if (snake.segment >= snake.length) {
        snake.SAMS = true;
        snake.segment = 1;
      }
    } else {
      snake.segment = 1;
    }
  }

  void updateHead(vars& snake, uint8_t player) {
    // Atualiza as novas posições
    snake.body[0].x += snake.directionX;
    snake.body[0].y += snake.directionY;

    if (!wallCollision) {  // Atualiza caso não haja colisão com as bordas
      if (snake.body[0].y == 255) {
        snake.body[0].y = Matrix_Height - 1;
      } else if (snake.body[0].y >= Matrix_Height) {
        snake.body[0].y = 0;
      } else if (snake.body[0].x == 255) {
        snake.body[0].x = Matrix_Length - 1;
      } else if (snake.body[0].x >= Matrix_Length) {
        snake.body[0].x = 0;
      }
    }

    for (uint8_t i = 0; i < foodQnt; i++) {  // Verifica se a cobra passou pela comida
      if (snake.body[0].x == food[i].x && snake.body[0].y == food[i].y) {
        snake.length++;  // Incrimenta o tamanho da cobra
        score[(splitGame) ? (gameAddr - 1) : player]++;
        scoreUpdated = false;
        food[i].x = 255;  // Atualiza as posições da comida como inválidas
        food[i].y = 255;
        generateFood();  // Gera novas posições válidas
      }
    }
    // Atualiza as matrizes
    setLed(snake.body[0].x, snake.body[0].y, 1);
    setBoard(snake.body[0].x, snake.body[0].y, 1);
  }



  void generateFood() {
    for (uint8_t k = 0; k < foodQnt; k++) {        // Faz o processo para a quantidade de comida configurada
      if (food[k].x == 255 || food[k].y == 255) {  // Verifica se a posição é inválida
        bool validPosition = false;

        while (!validPosition) {
          food[k].x = random(Matrix_Length);
          food[k].y = random(Matrix_Height);

          validPosition = !readBoard(food[k].x, food[k].y);

          for (uint8_t l = 0; l < foodQnt; l++) {
            if (l != k && food[k].x == food[l].x && food[k].y == food[l].y) {
              validPosition = false;
              break;
            }
          }
        }
      }
    }
  }


  void displayFood() {
    unsigned long now = millis();
    if (now - foodBlink <= 3000) {  // O LED em que a comida está fica aceso por 3 segundos
      for (uint8_t k = 0; k < foodQnt; k++) {
        setLed(food[k].x, food[k].y, 1);
      }
    } else if (now - foodBlink >= 3100) {  // Recomeça o looping depois dos 3,1 segundos
      foodBlink = now;
    } else {
      for (uint8_t k = 0; k < foodQnt; k++) {  // Apaga o LED por 100ms
        setLed(food[k].x, food[k].y, 0);
      }
    }
  }

  void reset() {
    clearBoard();

    for (uint8_t i = 0; i < players; i++) {
      Snake[i].SAMS = false;
      Snake[i].length = initialLength;
      Snake[i].directionY = 0;
      Snake[i].segment = 1;
      if (score[(splitGame) ? gameAddr - 1 : i] > getScore(snake_Best_ScoreEEPROMAddr)) {
        EEPROM.put(snake_Best_ScoreEEPROMAddr, score[(splitGame) ? gameAddr - 1 : i]);
      }
      if (score[(splitGame) ? gameAddr - 1 : i] > getScore(snake_Your_ScoreEEPROMAddr)) {
        EEPROM.put(snake_Your_ScoreEEPROMAddr, score[(splitGame) ? gameAddr - 1 : i]);
      }
      score[(splitGame) ? gameAddr - 1 : i] = 0;
      do { Snake[i].directionX = random(-1, 2); } while (Snake[i].directionX == 0);
      Snake[i].body[0].x = (players == 2) ? ((i == 0) ? (Matrix_Length / 4) : ((3 * Matrix_Length) / 4)) : (Matrix_Length / 2);
      Snake[i].body[0].y = Matrix_Height / 2;
      setLed(Snake[i].body[0].x, Snake[i].body[0].y, 1);
      setBoard(Snake[i].body[0].x, Snake[i].body[0].y, 1);
    }

    for (uint8_t i = 0; i < foodQnt; i++) {
      food[i].x = 255;
      food[i].y = 255;
    }
    generateFood();
    scoreUpdated = false;
    gameOver = false;
  }

  void Vplayer1Up() {
    if (Snake[0].directionY == 0) {
      Snake[0].directionY = 1;
      Snake[0].directionX = 0;
    }
  }
  void Vplayer1Left() {
    if (Snake[0].directionX == 0) {
      Snake[0].directionX = -1;
      Snake[0].directionY = 0;
    }
  }
  void Vplayer1Down() {
    if (Snake[0].directionY == 0) {
      Snake[0].directionY = -1;
      Snake[0].directionX = 0;
    }
  }
  void Vplayer1Right() {
    if (Snake[0].directionX == 0) {
      Snake[0].directionY = 0;
      Snake[0].directionX = 1;
    }
  }
  void Vplayer2Up() {
    if (Snake[1].directionY == 0) {
      Snake[1].directionY = 1;
      Snake[1].directionX = 0;
    }
  }
  void Vplayer2Left() {
    if (Snake[1].directionX == 0) {
      Snake[1].directionX = -1;
      Snake[1].directionY = 0;
    }
  }
  void Vplayer2Down() {
    if (Snake[1].directionY == 0) {
      Snake[1].directionY = -1;
      Snake[1].directionX = 0;
    }
  }
  void Vplayer2Right() {
    if (Snake[1].directionX == 0) {
      Snake[1].directionY = 0;
      Snake[1].directionX = 1;
    }
  }

public:
  SnakeGame(uint8_t Players, uint8_t Length, uint8_t Height, uint8_t playerAddr, uint8_t gameDelay, bool WallCollision, uint8_t InitialLength, uint8_t FoodQnt)
    : GameBase(Players, Length, Height, playerAddr, gameDelay) {
    player1Up = (void(GameBase::*)()) & SnakeGame::Vplayer1Up;
    player1Left = (void(GameBase::*)()) & SnakeGame::Vplayer1Left;
    player1Down = (void(GameBase::*)()) & SnakeGame::Vplayer1Down;
    player1Right = (void(GameBase::*)()) & SnakeGame::Vplayer1Right;

    player2Up = (void(GameBase::*)()) & SnakeGame::Vplayer2Up;
    player2Left = (void(GameBase::*)()) & SnakeGame::Vplayer2Left;
    player2Down = (void(GameBase::*)()) & SnakeGame::Vplayer2Down;
    player2Right = (void(GameBase::*)()) & SnakeGame::Vplayer2Right;

    VirtualLoop = (void(GameBase::*)()) & SnakeGame::Vloop;
    VirtualDestructor = (void(GameBase::*)()) & SnakeGame::SnakeDestructor;

    Snake = new vars[Players];

    wallCollision = WallCollision;
    initialLength = InitialLength;
    foodQnt = (Players == 2) ? (FoodQnt * 2) : FoodQnt;

    for (uint8_t i = 0; i < Players; i++) {
      Snake[i].body = new coords[Matrix_Length * Matrix_Height];
    }
    food = new coords[foodQnt];
    gameOver = true;
  }

  SnakeDestructor() {
    for (uint8_t i = 0; i < players; i++) {
      delete[] Snake[i].body;
    }
    delete[] food;
    delete[] Snake;
  }

  void Vloop() {
    handleButtons();
    if (checkDelay(gameDebounce, gameDelay)) {
      for (uint8_t i = 0; i < players; i++) {
        updateBody(Snake[i]);
        checkCollision(Snake[i]);
        displayFood();
        updateHead(Snake[i], i);
        if (gameOver) {
          reset();
        }
      }
    }
  }
};

class TetrisGame : public GameBase {
private:
  // declara todas as formas de peças
  const byte Pieces[7][4] = {
    { 0b00000001,  // I
      0b00000001,
      0b00000001,
      0b00000001 },

    { 0b00000011,  // O
      0b00000011,
      0b00000000,
      0b00000000 },

    { 0b00000111,  // T
      0b00000010,
      0b00000000,
      0b00000000 },

    { 0b00000011,  // S
      0b00000110,
      0b00000000,
      0b00000000 },

    { 0b00000110,  // Z
      0b00000011,
      0b00000000,
      0b00000000 },

    { 0b00000010,  // L
      0b00000010,
      0b00000011,
      0b00000000 },

    { 0b00000010,  // J
      0b00000010,
      0b00000110,
      0b00000000 }
  };

  // variáveis de dimensão permutadas para evitar confusão
  const uint8_t tetris_Length;  // tetris_Length = Matrix_Height
  const uint8_t tetris_Height;  // tetris_Height = Matrix_Length

  /* Considerações:
  funções recebem x e y permutados.
  Ex.:
  setLed(y, x);
  setBoard(y, x);
  readBoard(y, x);
  */

  // variáveis da peça no jogo
  int8_t pieceX, pieceY;
  uint8_t currentPiece, nextPiece;
  uint8_t mode;
  byte piece[4] = {};       // {} inicializa com 0 em cada posição
  byte next_Piece[4] = {};  // define a próxima peça


  void drawPiece(bool state) {
    for (uint8_t x = 0; x < 4; x++) {
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(piece[y], x)) {               // Verifica a posição do bit ativo da peça
          setLed(y + pieceY, x + pieceX, state);  // Soma a posição do bit com a posição da peça
        }
      }
    }
  }

  void mergePiece() {
    for (uint8_t x = 0; x < 4; x++) {
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(piece[y], x)) {
          setBoard(y + pieceY, x + pieceX, 1);  // Mescla a peça atual ao tabuleiro
        }
      }
    }
  }

  bool checkCollision(byte Piece[4], uint8_t piece_y, uint8_t piece_x) {
    for (uint8_t x = 0; x < 4; x++) {
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(Piece[y], x)) {
          if (readBoard(y + piece_y, x + piece_x)) {  // Verifica se a posição já está ocupada
            return true;
          } else if (y + piece_y == tetris_Height || piece_x + x < 0 || piece_x + x >= tetris_Length) {  // Verifica se a posição ultrapassa as extremidades
            return true;
          }
        }
      }
    }
    return false;
  }

  void rotatePiece() {
    byte newPiece[4] = {};  // variável que irá armazenar a peça rotacionada

    for (uint8_t x = 0; x < 4; x++) {
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(piece[y], x)) {
          bitSet(newPiece[x], 3 - y);  // logica de rotação
        }
      }
    }

    // lógica para deixar os bytes sempre o mais a direita possível (para evitar alterações bruscas na posição da peça)
    for (uint8_t x = 0; x < 4; x++) {
      bool skip = false;  // Variável de controle que diz se a peça já está o mais a direita
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(newPiece[y], x) && x == 0) {  // Se detectar um bit 1 na posição do bit menos significante em qualquer um dos bytes
          skip = true;                            // Indica que a peça já está o mais a direita e quebra o laço de repetição
          break;
        } else if (bitRead(newPiece[y], x)) {  // Se detectar um bit 1 em qualquer posição diferente do bit menos significante
          for (uint8_t k = 0; k < 4; k++) {
            newPiece[k] = newPiece[k] >> x;  // Move todos os bytes à direita em relação da posição do bit encontrado
          }
          skip = true;  // Indica que o processo já foi concluído e quebra o laço
          break;
        }
      }
      if (skip) {
        break;
      }
    }

    if (!checkCollision(newPiece, pieceY, pieceX)) {  // Verifica se a peça irá colidir ao rotacionar
      for (uint8_t i = 0; i < 4; i++) {               // Se não, atualiza a peça atual
        piece[i] = newPiece[i];
      }
    }
  }

  void rotatePiece(byte Piece[4]) {
    byte newPiece[4] = {};  // variável que irá armazenar a peça rotacionada

    for (uint8_t x = 0; x < 4; x++) {
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(Piece[y], x)) {
          bitSet(newPiece[x], 3 - y);  // logica de rotação
        }
      }
    }

    for (uint8_t x = 0; x < 4; x++) {
      bool skip = false;
      for (uint8_t y = 0; y < 4; y++) {
        if (bitRead(newPiece[y], x) && x == 0) {
          skip = true;
          break;
        } else if (bitRead(newPiece[y], x)) {
          for (uint8_t k = 0; k < 4; k++) {
            newPiece[k] = newPiece[k] >> x;
          }
          skip = true;
          break;
        }
      }
      if (skip) {
        break;
      }
    }

    for (uint8_t i = 0; i < 4; i++) {
      Piece[i] = newPiece[i];  // Armazena a peça rotacionada
    }
  }

  void checkRow() {
    for (uint8_t y = 0; y < tetris_Height; y++) {
      bool fullRow = true;  // Variável de controle que indica quando temos uma linha totalmente preenchida
      for (uint8_t x = 0; x < tetris_Length; x++) {
        if (!readBoard(y, x)) {  // Se retornar 0, pula o looping para eficiência
          fullRow = false;
          break;
        }
      }

      if (fullRow) {  // Se a linha for cheia
        // Animação
        for (uint8_t i = 0; i < 2; i++) {
          for (uint8_t row = 0; row < tetris_Length; row++) {
            setLed(y, row, 0);  // Apaga a linha
          }
          // delay(100);
          for (uint8_t row = 0; row < tetris_Length; row++) {
            setLed(y, row, 1);  // Acende a linha
            // delay(100);
          }
        }
        for (uint8_t row = 0; row < tetris_Length; row++) {
          setBoard(y, row, 0);  // Atualiza a matriz
          setLed(y, row, 0);    // mantém a linha apagada
        }
        // Fim animação

        // Faz com que todos os pixeis ativos acima desçam 1 linha
        for (int8_t col = y; col >= 0; col--) {
          for (uint8_t row = 0; row < tetris_Length; row++) {
            setBoard(col, row, readBoard(col - 1, row));  // Passa os valores da linha de cima para a linha atual
            setLed(col, row, readBoard(col, row));        // Ativa o Led conforme o seu estado na matriz
          }
        }
        score[gameAddr - 1]++;
        scoreUpdated = false;
      }
    }
  }

  void generatePiece() {
    currentPiece = nextPiece;  // Atualiza a peça atual para a próxima peça
    nextPiece = random(7);     // Gera uma nova peça

    for (uint8_t i = 0; i < 4; i++) {
      piece[i] = next_Piece[i];  // Atualiza os bytes da peça atual para os bytes da próxima peça
    }

    for (uint8_t i = 0; i < 4; i++) {
      next_Piece[i] = Pieces[nextPiece][i];  // Cria uma nova próxima peça
    }

    for (int8_t i = random(4); i > 0; i--) {
      rotatePiece(next_Piece);  // Rotaciona aleatóriamente a nova próxima peça
    }
    // Atualiza as posições da peça
    pieceX = tetris_Length / 2;
    pieceY = -4;
  }

  void Vplayer1Up() {
    drawPiece(0);
    rotatePiece();
    drawPiece(1);
  }
  void Vplayer1Left() {
    if (!checkCollision(piece, pieceY, pieceX - 1)) {
      drawPiece(0);
      pieceX--;
      drawPiece(1);
    }
  }
  void Vplayer1Right() {
    if (!checkCollision(piece, pieceY, pieceX + 1)) {
      drawPiece(0);
      pieceX++;
      drawPiece(1);
    }
  }
  void Vplayer1Down() {
    if (!checkCollision(piece, pieceY + 1, pieceX)) {
      drawPiece(0);
      pieceY++;
      drawPiece(1);
    }
  }

  void reset() {
    nextPiece = random(7);
    for (uint8_t i = 0; i < 4; i++) {
      next_Piece[i] = Pieces[nextPiece][i];
    }
    for (int8_t i = random(4); i > 0; i--) {
      rotatePiece(next_Piece);
    }
    generatePiece();
    clearBoard();
    if (score[gameAddr - 1] > getScore(tetris_Best_ScoreEEPROMAddr)) {
      EEPROM.put(tetris_Best_ScoreEEPROMAddr, score[gameAddr - 1]);
    }
    if (score[gameAddr - 1] > getScore(tetris_Your_ScoreEEPROMAddr)) {
      EEPROM.put(tetris_Your_ScoreEEPROMAddr, score[gameAddr - 1]);
    }
    score[gameAddr - 1] = 0;
    scoreUpdated = false;
    gameOver = false;
  }

public:
  TetrisGame(uint8_t Player, uint8_t Length, uint8_t Height, uint8_t playerAddr, uint8_t gameDelay, uint8_t Mode)
    : GameBase(Player, Length, Height, playerAddr, gameDelay), tetris_Height(Length), tetris_Length(Height), mode(Mode) {
    player1Up = (void(GameBase::*)()) & TetrisGame::Vplayer1Up;
    player1Left = (void(GameBase::*)()) & TetrisGame::Vplayer1Left;
    player1Down = (void(GameBase::*)()) & TetrisGame::Vplayer1Down;
    player1Right = (void(GameBase::*)()) & TetrisGame::Vplayer1Right;

    VirtualLoop = (void(GameBase::*)()) & TetrisGame::Vloop;

    gameOver = true;
  }

  void Vloop() {
    handleButtons();
    if (checkDelay(gameDebounce, gameDelay)) {
      drawPiece(1);
      if (checkCollision(piece, pieceY + 1, pieceX)) {
        mergePiece();
        for (uint8_t i = 0; i < tetris_Length; i++) {
          if (readBoard(0, i)) {
            gameOver = true;
          }
        }
        checkRow();
        generatePiece();
      } else {
        drawPiece(0);
        pieceY++;
        drawPiece(1);
      }
      if (gameOver) {
        reset();
      }
    }
  }
};

// Tag Game

class TagGame : public GameBase {
private:

  struct dot {
    uint8_t x = 0;
    uint8_t y = 0;
    bool isButtonPressed = false;
  };

  dot playerDot[2];

  void setLed8setBoard(uint8_t x, uint8_t y, bool state) {
    setLed(x, y, state);
    setBoard(x, y, state);
  }

  void Up(uint8_t player) {
    if (playerDot[player].isButtonPressed) {
      return;
    }

    if (playerDot[player].y < Matrix_Height - 1) {
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 0);
      playerDot[player].y++;
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 1);
    }

    playerDot[player].isButtonPressed = true;
  }

  void Down(uint8_t player) {
    if (playerDot[player].isButtonPressed) {
      return;
    }

    if (playerDot[player].y > 0) {
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 0);
      playerDot[player].y--;
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 1);
    }

    playerDot[player].isButtonPressed = true;
  }

  void Left(uint8_t player) {
    if (playerDot[player].isButtonPressed) {
      return;
    }

    if (playerDot[player].x > 0) {
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 0);
      playerDot[player].x--;
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 1);
    }

    playerDot[player].isButtonPressed = true;
  }

  void Right(uint8_t player) {
    if (playerDot[player].isButtonPressed) {
      return;
    }

    if (playerDot[player].x < Matrix_Length - 1) {
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 0);
      playerDot[player].x++;
      setLed8setBoard(playerDot[player].x, playerDot[player].y, 1);
    }

    playerDot[player].isButtonPressed = true;
  }

  void Vplayer1Up() {
    Up(0);
  }

  void Vplayer1Down() {
    Down(0);
  }

  void Vplayer1Left() {
    Left(0);
  }

  void Vplayer1Right() {
    Right(0);
  }

  void Vplayer2Up() {
    Up(1);
  }

  void Vplayer2Down() {
    Down(1);
  }

  void Vplayer2Left() {
    Left(1);
  }

  void Vplayer2Right() {
    Right(1);
  }

  void checkButtons() {
    readButtons();

    if (!bitRead(buttonState, 0) && !bitRead(buttonState, 1) && !bitRead(buttonState, 2) && !bitRead(buttonState, 3)) {
      playerDot[0].isButtonPressed = false;
    }

    if (players == 2) {
      if (!bitRead(buttonState, 4) && !bitRead(buttonState, 5) && !bitRead(buttonState, 6) && !bitRead(buttonState, 7)) {
        playerDot[1].isButtonPressed = false;
      }
    }
  }

  reset() {
    setLed8setBoard(playerDot[0].x, playerDot[0].y, 0);
    setLed8setBoard(playerDot[1].x, playerDot[1].y, 0);
    playerDot[0].x = 2;
    playerDot[0].y = 3;
    playerDot[1].x = 16;
    playerDot[1].y = 3;
    setLed8setBoard(playerDot[0].x, playerDot[0].y, 1);
    setLed8setBoard(playerDot[1].x, playerDot[1].y, 1);

    gameOver = false;
  }

  void Vloop() {
    if (gameOver) {
      reset();
    }
    checkButtons();
    handleButtons();
  }

public:
  TagGame(uint8_t Player, uint8_t Length, uint8_t Height, uint8_t playerAddr, uint8_t gameDelay, uint8_t Mode)
    : GameBase(Player, Length, Height, playerAddr, gameDelay) {
    player1Up = (void(GameBase::*)()) & TagGame::Vplayer1Up;
    player1Left = (void(GameBase::*)()) & TagGame::Vplayer1Left;
    player1Down = (void(GameBase::*)()) & TagGame::Vplayer1Down;
    player1Right = (void(GameBase::*)()) & TagGame::Vplayer1Right;
    player2Up = (void(GameBase::*)()) & TagGame::Vplayer2Up;
    player2Left = (void(GameBase::*)()) & TagGame::Vplayer2Left;
    player2Down = (void(GameBase::*)()) & TagGame::Vplayer2Down;
    player2Right = (void(GameBase::*)()) & TagGame::Vplayer2Right;

    VirtualLoop = (void(GameBase::*)()) & TagGame::Vloop;

    gameOver = true;
  }
};

void runGame();

uint16_t getScore(uint16_t addr) {
  uint16_t score;
  EEPROM.get(addr, score);
  return score;
}

enum Menus : uint8_t {
  Main,
  Config,
  GameList,
  TetrisGameOptions,
  SnakeGameOptions,
  PongGameOptions,
  BestScores,
  YourBestScores,
  OfAllTimeBestScores,
  DevBestScores,
  isGameRunning
};

enum actionType : uint8_t {
  changeMenu,
  delayManagement,
  selectGame,
  gameStart,
  variableManagement,
  boolVariableManagement,
  dinamicVariableManagement,
  dinamicBoolVariableManagement,
  showScore
};

enum Games : uint8_t {
  Tetris,
  Snake,
  Pong,
  unselected
};

Menus actualMenu = Main;
GameBase* Game[2] = { nullptr, nullptr };

const char messages[] PROGMEM = "Back\0Games\0Config\0Tetris\0Snake\0Pong\0Start\0SplitGame %s\0Players %d\0Brightness %d\0SoundEff %s\0SoundTrack %s\0Delay %d\0Mode %d\0Difficulty %d\0WallCollision %s\0Food %d\0StartLength %d\0BestScores\0OfAllTime\0OfDeveloper\0YourScores\0Tetris: %d\0Snake: %d\0Pong: %d";

struct baseVars {
  uint8_t gameDelay = 150;
};

struct TetrisVars : public baseVars {
  uint8_t mode = 1;
};

struct SnakeVars : public baseVars {
  uint8_t wallCollision = true;
  uint8_t foodQnt = 3;
  uint8_t initialLength = 3;
};

struct PongVars : public baseVars {
  uint8_t difficulty = 3;
};

struct bestScores {
  uint16_t tetris_Best_Score = getScore(tetris_Best_ScoreEEPROMAddr);
  uint16_t tetris_Your_Score = getScore(tetris_Your_ScoreEEPROMAddr);
  uint16_t tetris_Dev_Score = getScore(tetris_Dev_ScoreEEPROMAddr);
  uint16_t snake_Best_Score = getScore(snake_Best_ScoreEEPROMAddr);
  uint16_t snake_Your_Score = getScore(snake_Your_ScoreEEPROMAddr);
  uint16_t snake_Dev_Score = getScore(snake_Dev_ScoreEEPROMAddr);
  uint16_t pong_Best_Score = getScore(pong_Best_ScoreEEPROMAddr);
  uint16_t pong_Your_Score = getScore(pong_Your_ScoreEEPROMAddr);
  uint16_t pong_Dev_Score = getScore(pong_Dev_ScoreEEPROMAddr);
};

struct menuItem {
  uint8_t messageOffSet;
  actionType action;
  Menus menu__varOffSet;
  uint8_t* variable;
  uint8_t varMin;
  uint8_t varMax;
};

const menuItem MainMenu[] PROGMEM = {
  { 5, changeMenu, GameList, nullptr, 0, 0 },     // Games
  { 11, changeMenu, Config, nullptr, 0, 0 },      // Config
  { 177, changeMenu, BestScores, nullptr, 0, 0 }  // Best Scores
};

const menuItem ConfigMenu[] PROGMEM = {
  { 42, boolVariableManagement, Config, &splitGame, 0, 1 },     // SplitGame
  { 55, variableManagement, Config, &Xplayers, 1, 2 },          // Players
  { 66, variableManagement, Config, &brightness, 1, 15 },       // Brightness
  { 80, boolVariableManagement, Config, &soundEffects, 0, 1 },  // SoundEffects
  { 92, boolVariableManagement, Config, &soundTrack, 0, 1 },    // SoundTrack
  { 0, changeMenu, Main, nullptr, 0, 0 }                        // Back
};

const menuItem GameListMenu[] PROGMEM{
  { 18, selectGame, TetrisGameOptions, nullptr, 0, 3 },  // Tetris
  { 25, selectGame, SnakeGameOptions, nullptr, 0, 3 },   // Snake
  { 31, selectGame, PongGameOptions, nullptr, 0, 3 }     // Pong
};

const menuItem TetrisGameOptionsMenu[] PROGMEM{
  { 115, dinamicVariableManagement, offsetof(TetrisVars, mode), nullptr, 1, 2 },  // Mode
  { 106, delayManagement, offsetof(TetrisVars, gameDelay), nullptr, 50, 250 },    // Delay
  { 36, gameStart, GameList, nullptr, 0, 0 }                                      // Start
};

const menuItem SnakeGameOptionsMenu[] PROGMEM{
  { 137, dinamicBoolVariableManagement, offsetof(SnakeVars, wallCollision), nullptr, 0, 1 },  // WallCollision
  { 154, dinamicVariableManagement, offsetof(SnakeVars, foodQnt), nullptr, 1, 5 },            // Food
  { 162, dinamicVariableManagement, offsetof(SnakeVars, initialLength), nullptr, 1, 5 },      // StartLength
  { 106, delayManagement, offsetof(SnakeVars, gameDelay), nullptr, 50, 250 },                 // Delay
  { 36, gameStart, GameList, nullptr, 0, 0 }                                                  // Start
};

const menuItem PongGameOptionsMenu[] PROGMEM{
  { 123, dinamicVariableManagement, offsetof(PongVars, difficulty), nullptr, 1, 5 },  // Difficulty
  { 106, delayManagement, offsetof(PongVars, gameDelay), nullptr, 50, 250 },          // Delay
  { 36, gameStart, GameList, nullptr, 0, 0 }                                          // Start
};

const menuItem yourBestScoresMenu[] PROGMEM{
  { 221, showScore, offsetof(bestScores, tetris_Your_Score), nullptr, 0, 0 },  // Tetris:
  { 232, showScore, offsetof(bestScores, snake_Your_Score), nullptr, 0, 0 },   // Snake:
  { 242, showScore, offsetof(bestScores, pong_Your_Score), nullptr, 0, 0 },    // Pong:
  { 0, changeMenu, BestScores, nullptr, 0, 0 }
};

const menuItem ofAllTimeBestScoresMenu[] PROGMEM{
  { 221, showScore, offsetof(bestScores, tetris_Best_Score), nullptr, 0, 0 },  // Tetris:
  { 232, showScore, offsetof(bestScores, snake_Best_Score), nullptr, 0, 0 },   // Snake:
  { 242, showScore, offsetof(bestScores, pong_Best_Score), nullptr, 0, 0 },    // Pong:
  { 0, changeMenu, BestScores, nullptr, 0, 0 }
};

const menuItem devBestScoresMenu[] PROGMEM{
  { 221, showScore, offsetof(bestScores, tetris_Dev_Score), nullptr, 0, 0 },  // Tetris:
  { 232, showScore, offsetof(bestScores, snake_Dev_Score), nullptr, 0, 0 },   // Snake:
  { 242, showScore, offsetof(bestScores, pong_Dev_Score), nullptr, 0, 0 },    // Pong:
  { 0, changeMenu, BestScores, nullptr, 0, 0 }
};

const menuItem bestScoresMenu[] PROGMEM{
  { 210, changeMenu, YourBestScores, nullptr, 0, 0 },       // YourScores
  { 188, changeMenu, OfAllTimeBestScores, nullptr, 0, 0 },  // OfAllTime
  { 200, changeMenu, DevBestScores, nullptr, 0, 0 }         // OfDeveloper
};

struct Menu {
  menuItem* Items;
  uint8_t optionsCount;
};

const Menu menuTable[] PROGMEM{
  { MainMenu, 3 },
  { ConfigMenu, 6 },
  { GameListMenu, 3 },
  { TetrisGameOptionsMenu, 3 },
  { SnakeGameOptionsMenu, 5 },
  { PongGameOptionsMenu, 3 },
  { bestScoresMenu, 3 },
  { yourBestScoresMenu, 4 },
  { ofAllTimeBestScoresMenu, 4 },
  { devBestScoresMenu, 4 }
};

class manageMenu {
private:
  uint8_t currentOption = 0;
  uint8_t optionsCount;
  bool menuUpdated = false;
  char buffer[18] = { '\0' };
  menuItem currentItem;

  unsigned long buttonDebounce = 0;
  unsigned long textDelay = 0;
  int16_t textPosition = 32;
  uint8_t actualPlayer = 1;
  Games selectedGame = unselected;

  void* gameVars = nullptr;

  void (manageMenu::*right)() = nullptr;
  void (manageMenu::*left)() = nullptr;

  uint16_t* score = nullptr;

  void updateMenu() {
    actualMenu = currentItem.menu__varOffSet;
    if (currentItem.action == selectGame) {
      selectedGame = currentOption;
      if (selectedGame == Tetris) {
        gameVars = new TetrisVars;
      } else if (selectedGame == Snake) {
        gameVars = new SnakeVars;
      } else if (selectedGame == Pong) {
        gameVars = new PongVars;
      }
    } else if (actualMenu == BestScores && !gameVars) {
      gameVars = new bestScores;
    }
    if (actualMenu == GameList && splitGame) {
      lcd.clear();
      lcd.print(F("Config.player: "));
      lcd.print(actualPlayer);
    }
    currentOption = 0;
    loadItem();
  }

  void mainMenu() {
    for (uint8_t i = 0; i < 2; i++) {
      if (Game[i] != nullptr) {
        delete Game[i];
        Game[i] = nullptr;
      }
    }
    if (gameVars) {
      delete gameVars;
      gameVars = nullptr;
    }
    score = nullptr;
    actualMenu = Main;
    actualPlayer = 1;
    currentOption = 0;
    lcd.clear();
    loadItem();
  }

  void startGame() {
    if (selectedGame == Tetris) {
      uint8_t Mode = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(TetrisVars, mode)));
      uint8_t gDelay = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(TetrisVars, gameDelay)));
      if (!splitGame && Xplayers == 2) {
        Game[0] = new TetrisGame(1, MatrixLength / 2, MatrixHeight, 1, gDelay, Mode);
        Game[1] = new TetrisGame(1, MatrixLength / 2, MatrixHeight, 2, gDelay, Mode);
      } else {
        Game[actualPlayer - 1] = new TetrisGame(1, ((splitGame) ? (MatrixLength / 2) : MatrixLength), MatrixHeight, actualPlayer, gDelay, Mode);
      }
    } else if (selectedGame == Snake) {
      uint8_t WallCollision = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(SnakeVars, wallCollision)));
      uint8_t FoodQnt = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(SnakeVars, foodQnt)));
      uint8_t InitialLength = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(SnakeVars, initialLength)));
      uint8_t gDelay = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(SnakeVars, gameDelay)));
      Game[actualPlayer - 1] = new SnakeGame(((splitGame) ? 1 : Xplayers), ((splitGame) ? (MatrixLength / 2) : MatrixLength), MatrixHeight, actualPlayer, gDelay, WallCollision, InitialLength, FoodQnt);
    } else if (selectedGame == Pong) {
      uint8_t gDelay = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(PongVars, gameDelay)));
      uint8_t Difficulty = *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + offsetof(PongVars, difficulty)));
      Game[actualPlayer - 1] = new PongGame(((splitGame) ? 1 : Xplayers), ((splitGame) ? (MatrixLength / 2) : MatrixLength), MatrixHeight, actualPlayer, gDelay, Difficulty);
    }

    delete gameVars;

    if (actualPlayer == 1 && splitGame) {
      actualPlayer++;
      updateMenu();
    } else {
      if (selectedGame == Tetris && Xplayers == 2 && !splitGame) {
        splitGame = true;
      }
      runGame();
    }
  }

  void loadItem() {
    menuItem* currentMenuItems = (menuItem*)pgm_read_ptr(&menuTable[actualMenu].Items);
    memcpy_P(&currentItem, &currentMenuItems[currentOption], sizeof(menuItem));
    optionsCount = pgm_read_byte(&menuTable[actualMenu].optionsCount);

    if (currentItem.action == changeMenu || currentItem.action == selectGame) {
      right = &manageMenu::updateMenu;
      left = &manageMenu::mainMenu;
    } else if (currentItem.action == gameStart) {
      right = &manageMenu::startGame;
      left = &manageMenu::mainMenu;
    } else {
      right = nullptr;
      left = nullptr;
    }

    if (currentItem.action == dinamicVariableManagement || currentItem.action == dinamicBoolVariableManagement || currentItem.action == delayManagement) {
      currentItem.variable = reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(gameVars) + currentItem.menu__varOffSet);
    } else if (currentItem.action == showScore) {
      score = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(gameVars) + currentItem.menu__varOffSet);
    }
    menuUpdated = false;
    textPosition = 32;
  }

  void clearBoard() {
    for (uint8_t i = 0; i < MatrixLength / 2; i++) {
      uint8_t addr = i / 8;
      uint8_t row = i % 8;
      for (uint8_t j = 0; j < MatrixHeight; j++) {
        lc1.setLed(addr, row, j, 0);
        lc2.setLed(addr, row, j, 0);
      }
    }
  }

  void updateMessage() {
    clearBoard();
    memset(buffer, '\0', sizeof(buffer));
    if (currentItem.variable || currentItem.action == showScore) {
      if (currentItem.action == boolVariableManagement || currentItem.action == dinamicBoolVariableManagement) {
        snprintf_P(buffer, sizeof(buffer), &messages[currentItem.messageOffSet],
                   ((*currentItem.variable) ? "On" : "Off"));
      } else {
        uint16_t var = (currentItem.action == showScore) ? *score : *currentItem.variable;
        snprintf_P(buffer, sizeof(buffer), &messages[currentItem.messageOffSet], var);
      }
    } else {
      strcpy_P(buffer, &messages[currentItem.messageOffSet]);
    }
    menuUpdated = true;
  }

public:
  manageMenu() {
    mainMenu();
  }

  void displayMessage() {
    if (!menuUpdated) { updateMessage(); }
    unsigned long now = millis();
    if (now - textDelay >= 20) {
      lc1.escribirFrase(buffer, textPosition);
      lc2.escribirFrase(buffer, textPosition - 16);
      textPosition--;
      if (textPosition < -((int16_t)strlen(buffer) * 8)) {
        textPosition = 32;
      }
      textDelay = now;
    }
  }

  void handleInteraction() {
    readButtons();
    if (buttonState && checkDelay(buttonDebounce, 500)) {
      uint8_t buttonIdx = (actualPlayer == 1) ? 0 : 4;
      if (bitRead(buttonState, buttonIdx)) {
        currentOption = (currentOption > 0) ? currentOption - 1 : optionsCount - 1;
        loadItem();
        return;
      } else if (bitRead(buttonState, buttonIdx + 2)) {
        currentOption = (currentOption + 1 < optionsCount) ? currentOption + 1 : 0;
        loadItem();
        return;
      } else if (bitRead(buttonState, buttonIdx + 1)) {
        if (left) {
          (this->*left)();
        } else if (currentItem.action == delayManagement) {
          if (*currentItem.variable > currentItem.varMin) { (*currentItem.variable) -= 50; }
        } else {
          if (*currentItem.variable > currentItem.varMin) {
            (*currentItem.variable)--;
          }
          if (Xplayers == 1) {
            splitGame = false;
          }
        }
      } else if (bitRead(buttonState, buttonIdx + 3)) {
        if (right) {
          (this->*right)();
        } else if (currentItem.action == delayManagement) {
          if (*currentItem.variable < currentItem.varMax) { (*currentItem.variable) += 50; }
        } else {
          if (*currentItem.variable < currentItem.varMax) {
            (*currentItem.variable)++;
          }
          if (Xplayers == 1) {
            splitGame = false;
          }
        }
      }
      menuUpdated = false;
      if (currentItem.variable == &brightness) {
        lc1.setIntensidad(brightness);
        lc2.setIntensidad(brightness);
      }
    }
  }
};

manageMenu* menuManager;

void runGame() {
  delete menuManager;
  actualMenu = isGameRunning;
}

void resetArcade() {
  lcd.setCursor(0, 0);
  menuManager = new manageMenu;
  score[0] = 0;
  score[1] = 0;
}

unsigned long lastNote = 0;
uint16_t currentNote = 0;
uint16_t noteDuration = 0;
uint16_t muchNotes = sizeof(Bloodtears) / sizeof(notes);
uint8_t loops = 0;

void playSong(){
  uint16_t wholeNote = (60000 * 4) / reinterpret_cast<uint8_t>(pgm_read_byte(&songs[currentSong].tempo));
  int8_t divider = reinterpret_cast<uint8_t>(pgm_read_byte(&reinterpret_cast<const notes*>(pgm_read_ptr(&reinterpret_cast<const song*>(&songs[currentSong])->note))[currentNote].duration));

  if(currentNote == 0){
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholeNote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholeNote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
  }

  if(checkDelay(lastNote, noteDuration)){
  tone(buzzerPin, reinterpret_cast<uint16_t>(pgm_read_word(&reinterpret_cast<const notes*>(pgm_read_ptr(&reinterpret_cast<const song*>(&songs[currentSong])->note))[currentNote].frequency)), noteDuration * 0.9);

  currentNote++;
  if(currentNote >= muchNotes){
  //  if(loops != 0){
    //} else {
      currentNote = 0;
    //  loops++;
   // }
  }

  if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholeNote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholeNote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

  }
}

void setup() {
  randomSeed(analogRead(0));                            // Garante que cada inicialização seja imprevisível
  lc1.begin(DIN_PIN1, CLK_PIN1, CS_PIN1, NUM_MATRIX1);  // Inicialização das matrizes
  lc2.begin(DIN_PIN2, CLK_PIN2, CS_PIN2, NUM_MATRIX2);
  lc1.setIntensidad(brightness);
  lc2.setIntensidad(brightness);
  // Configura as portas do CI
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
  // Configuração do buzzer
  pinMode(buzzerPin, OUTPUT);
  // Configuração do botão de nova escolha
  pinMode(resetPin, INPUT_PULLUP);
  // Inicialização do display LCD
  lcd.init();
  lcd.backlight();
  // Reinicia o "YourBestScore" como 0 na EEPROM
  EEPROM.put(tetris_Your_ScoreEEPROMAddr, 0);
  EEPROM.put(snake_Your_ScoreEEPROMAddr, 0);
  EEPROM.put(pong_Your_ScoreEEPROMAddr, 0);
  // Inicia o menu
  resetArcade();
}

void loop() {
  
  if (actualMenu != isGameRunning) {
    menuManager->handleInteraction();
    menuManager->displayMessage();
  } else {
    Game[0]->loop();
    if (Game[1] != nullptr) {
      Game[1]->loop();
    }
    if (!digitalRead(resetPin)) {
      resetArcade();
    }
    showScores();
  }

  if(soundTrack){
    playSong();
  }
  
 // Game[1]->loop();
}