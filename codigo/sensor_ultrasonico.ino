const int trigPin = 9;
const int echoPin = 10;

long duracion;
float distancia;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Generar pulso de activación
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Medir duración del eco
  duracion = pulseIn(echoPin, HIGH);

  // Calcular distancia
  distancia = duracion * 0.0343 / 2;

  // Mostrar resultado
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(500);
}