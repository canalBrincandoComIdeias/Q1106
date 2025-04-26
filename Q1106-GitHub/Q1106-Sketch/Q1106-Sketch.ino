#include "DFRobotDFPlayerMini.h"
//#include <SoftwareSerial.h>  //PARA ARDUINO

//PARA ARDUINO
//#define pinRX 10
//#define pinTX 11

//PARA ESP32
#define pinRX 18
#define pinTX 19

//SoftwareSerial serialMP3(pinRX, pinTX);  //RX, TX    //PARA ARDUINO
#define serialMP3 Serial1                          //PARA ESP32               

DFRobotDFPlayerMini moduloMP3;
void printRetorno(uint8_t type, int value);

void setup() {

  //serialMP3.begin(9600);                                        //PARA ARDUINO 
  serialMP3.begin(9600, SERIAL_8N1, pinRX, pinTX); //RX, TX   //PARA ESP32
  
  Serial.begin(9600);

  if (!moduloMP3.begin(serialMP3, true, true)) {
    Serial.println(F("Erro ao iniciar o modulo MP3:"));
    Serial.println(F("1.Verifique as conexoes!"));
    Serial.println(F("2.Insira o cartao SD!"));
    while (true) {
      delay(0);
    }
  }

  moduloMP3.volume(30);  //Configura o volume do audio. De 0 ate 30

  //----Configuração do Equalizador----
  moduloMP3.EQ(DFPLAYER_EQ_NORMAL);
  //  moduloMP3.EQ(DFPLAYER_EQ_POP);
  //  moduloMP3.EQ(DFPLAYER_EQ_ROCK);
  //  moduloMP3.EQ(DFPLAYER_EQ_JAZZ);
  //  moduloMP3.EQ(DFPLAYER_EQ_CLASSIC);
  //  moduloMP3.EQ(DFPLAYER_EQ_BASS);

  moduloMP3.playFolder(1, 99);
  delay(1500);
  Serial.println(F("Sketch Iniciado!"));
}

void loop() {

  if (Serial.available()) {
    String pacote = Serial.readString();

    int andar = pacote.toInt();
    falaAndar(andar);
  }

  if (serialMP3.available()) {
    printRetorno(moduloMP3.readType(), serialMP3.read());
  }
}

void falaAndar(int numeroAndar) {
  //int pausaFala[] = { 0, 720, 888, 864, 1056, 1104, 1224, 1224, 1032, 1008 };
  int pausaFala[] = { 674, 746, 986, 962, 1082, 1154, 1226, 1346, 1154, 1106 };

  if (numeroAndar <= 10) {
    moduloMP3.playFolder(1, numeroAndar);
  } else if (numeroAndar <= 99) {
    moduloMP3.playFolder(2, numeroAndar / 10);

    delay(pausaFala[numeroAndar / 10] - 80);

    if ((numeroAndar % 10) == 0) {
      moduloMP3.playFolder(2, 0);
    } else {
      moduloMP3.playFolder(1, numeroAndar % 10);
    }
  }
}

void printRetorno(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.print('.');
      //Serial.println(F("Tempo Esgotado!"));
      break;
    case WrongStack:
      Serial.println(F("Pilha Invalida!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Cartao Inserido!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Cartao Removido!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Cartao Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserido!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removido!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Numero:"));
      Serial.print(value);
      Serial.println(F(" Reproducao Concluida!"));
      break;
    case DFPlayerError:
      Serial.print(F("Erro no modulo MP3:"));
      switch (value) {
        case Busy:
          Serial.println(F("Cartao nao Encontrado"));
          break;
        case Sleeping:
          Serial.println(F("Dormindo"));
          break;
        case SerialWrongStack:
          Serial.println(F("Pilha Incorreta Recebida"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Soma de Verificacao nao Confere"));
          break;
        case FileIndexOut:
          Serial.println(F("Indice de Arquivo Fora dos Limites"));
          break;
        case FileMismatch:
          Serial.println(F("Arquivo nao encontrado"));
          break;
        case Advertise:
          Serial.println(F("Em Publicidade"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
