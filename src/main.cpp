/*
1 - Alternar em 2 tasks, 1 por nucleo as funcionalidades de controle pwm de um led 
e na outra piscar um led com determinada frequencia
*/

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

SemaphoreHandle_t SMF;
TaskHandle_t handlePWM_t, handleBlink_t;

//Definição dos pinos
#define LedG 2 //pino do led Verde
#define LedR 5 //pino do led Amarelo
#define ButtonPin 13 //Pino do botão

//Rotina de interrupção
void IRAM_ATTR Pressed(){
  xSemaphoreGiveFromISR(SMF, NULL);
}

void handlePWM(void* z);
void handleBlink(void* z);


void setup() {
  //inicialização dos pinos
  pinMode(LedG, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);

  //inicia interrupção
  attachInterrupt(ButtonPin, Pressed, FALLING);

  //inicia semaforo binário
  SMF = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(handlePWM, "handlePWM", 4096, NULL, 1, &handlePWM_t, 0);
  xTaskCreatePinnedToCore(handleBlink, "handleBlink", 4096, NULL, 1, &handleBlink_t, 1);
}

void loop() {
  
}

void handlePWM(void* z){
  while(true){
    for(byte i=0; i<=255 ; i++){
      analogWrite(LedG, i);
      delay(50);
    }
    for(byte i=255; i>=0 ; i--){
      analogWrite(LedG, i);
      delay(50);
    }
  }
}

void handleBlink(void* z){
  digitalWrite(LedR, HIGH);
  delay(300);
  digitalWrite(LedR,LOW);
  delay(300);
}