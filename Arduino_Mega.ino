#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

SoftwareSerial softSerial(10,11); // si no detecta al módulo, se deben invertir las conexiones o escribir (11,10) o los pines que se vayan a usar
 
DFRobotDFPlayerMini myDFPlayer;

const int trigPin = 2;   // Pines para el control de distancia(Sensor ultrasónico)
const int echoPin = 3;

const int boton = 22;  // Sensor capacitivo (táctil)

unsigned long previousMillis = 0; // Para almacenar el tiempo anterior
const unsigned long interval = 180000; // 2 minutos y 32 segundos en milisegundos

bool audioEsquivarReproducido = false; // Para controlar si el audio de esquivar ya se reprodujo

void setup()
{
  softSerial.begin(9600);  // Comunicación con el modulo mp3

  Serial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(softSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  pinMode(trigPin, OUTPUT);    // Pines para el control de distancia(Sensor ultrasónico)
  pinMode(echoPin, INPUT);

  pinMode(4,INPUT);     // Puentes de control para los motores y el control de esquivar obstaculos 
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(31,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(35,OUTPUT);
  pinMode(37,OUTPUT);

  pinMode(boton,INPUT);   //Botón de play para la canción
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15

  myDFPlayer.loopFolder(1); //loop all mp3 files in folder SD:/01.

}

void loop()
{
// Se debe adecuar la activación de las señales para el control de los motores si es necesario 
  int Am1=digitalRead(4);  //4-31   Detección del sentido a mover
  int Am2=digitalRead(5);  //5-33
  int Bm1=digitalRead(6);  //6-35
  int Bm2=digitalRead(7);  //7-37

  unsigned long currentMillis = millis(); // Tiempo actual
  
  if (Ultrasonido() <= 20){       //Esquivar
    myDFPlayer.playFolder(2, 1);  //play specific mp3 in SD:/02/001.mp3; Folder Name(1~99); File Name(1~255)
    Serial.print("\Derecha ");
    digitalWrite(31, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(35, LOW);
    digitalWrite(37, HIGH);
    delay(700);
  }
  // Detectar si el audio terminó y reiniciar el bucle de audios
  else if (audioEsquivarReproducido && myDFPlayer.available()) { 
    myDFPlayer.loopFolder(1);  // Reiniciar el bucle de audios en la carpeta SD:/01/
    audioEsquivarReproducido = false;  // Resetear la bandera de reproducción
  }
  else if (Am1 == HIGH && Am2 == LOW && Bm1 == HIGH && Bm2 == LOW){       //ADELANTE
    Serial.print("\Adelante ");
    digitalWrite(31, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(35, HIGH);
    digitalWrite(37, LOW);
  }
  else if(Am1==LOW && Am2==HIGH && Bm1==LOW && Bm2==HIGH){       //ATRAS 
    Serial.print("\Atras ");
    digitalWrite(31, LOW);
    digitalWrite(33, HIGH);
    digitalWrite(35, LOW);
    digitalWrite(37, HIGH);
  }
  else if(Am1==LOW && Am2==HIGH && Bm1==HIGH && Bm2==LOW){      //IZQUIERDA
    Serial.print("\Izquierda ");
    digitalWrite(31, LOW);
    digitalWrite(33, HIGH);
    digitalWrite(35, HIGH);
    digitalWrite(37, LOW);
  }
  else if(Am1==HIGH && Am2==LOW && Bm1==LOW && Bm2==HIGH){      //DERECHA
    Serial.print("\Derecha ");
    digitalWrite(31, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(35, LOW);
    digitalWrite(37, HIGH);
  }

  else{
    digitalWrite(31, LOW);
    digitalWrite(33, LOW);
    digitalWrite(35, LOW);
    digitalWrite(37, LOW);
  } 


  //%%%%%%%%%%%%%% Canción %%%%%%%%%%%%
  if(digitalRead(boton)==HIGH){
    myDFPlayer.playMp3Folder(1); //play specific mp3 in SD:/MP3/0001.mp3; File Name(0~65535)
    previousMillis = currentMillis; // Reinicia el temporizador
    delay(500);
  }
  else if(currentMillis - previousMillis >= interval){
    myDFPlayer.loopFolder(1); //loop all mp3 files in folder SD:/01.
    previousMillis = currentMillis; // Reinicia el temporizador
  }

}
        //%%%%%%%%%%%%%%%%%%%% Ultrasonido %%%%%%%%%%%%%%%%%%%%%%%
float Ultrasonido(void){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; 
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(100);

    return distance;
}