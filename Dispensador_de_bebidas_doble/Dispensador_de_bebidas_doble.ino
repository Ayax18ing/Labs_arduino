const int trig1 = 9;
const int echo1 = 10;
const int trig2 = 11;
const int echo2 = 12;
const int relePin1 = 7;
const int relePin2 = 8;

long duration1, duration2;
int distance1, distance2;

// Distancia de activación (en cm)
const int distanciaActivacion = 10;

void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(relePin1, OUTPUT);
  pinMode(relePin2, OUTPUT);
}

void loop() {
  // Sensor 1
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  delay(200);

  // Sensor 2
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  delay(200);

  if (distance1 <= distanciaActivacion) {
    digitalWrite(relePin1, HIGH);
  } else {
    digitalWrite(relePin1, LOW);
  }

  if (distance2 <= distanciaActivacion) {
    digitalWrite(relePin2, HIGH);
  } else {
    digitalWrite(relePin2, LOW);
  }
  delay(100);
}