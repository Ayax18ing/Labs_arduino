#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

void setup() {

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();

}

void loop() {
  mpu6050.update();
    
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());

  // Adecuar las señales si es necesario
   if (mpu6050.getAccY() <= -0.40){       //ADELANTE
    Serial.print("\Adelante ");
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    
  }
  else if(mpu6050.getAccY() >= 0.40){       //ATRAS 
    Serial.print("\Atras ");
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);

  }
  else if(mpu6050.getAccX() >= 0.40){      //IZQUIERDA
    Serial.print("\Izquierda ");
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    
  }
  else if(mpu6050.getAccX() <= -0.40){      //DERECHA
    Serial.print("\Derecha ");
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    
  }

  else{
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  } 
 delay(50);
}
