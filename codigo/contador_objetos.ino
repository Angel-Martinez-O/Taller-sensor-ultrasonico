const int trigPin = 9;
const int echoPin = 10;

long duracion;
float distancia;

// Variables para la lógica del negocio
int totalObjetos = 0;
bool objetoPresente = false; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Total de objetos: 0");
}

void loop() {
  // 1. Generar pulso de activación
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 2. Medir duración y calcular distancia
  duracion = pulseIn(echoPin, HIGH);
  distancia = duracion * 0.0343 / 2;

  // 3. Lógica de estados para el contador
  if (distancia < 10 && distancia > 0) { // El objeto está a menos de 10 cm
    if (!objetoPresente) { // Transición: ESPERANDO OBJETO -> OBJETO PRESENTE
      objetoPresente = true;
      totalObjetos++;
      
      Serial.println("Objeto detectado");
      Serial.print("Total de objetos: ");
      Serial.println(totalObjetos);
      
      // Validar si el lote de 5 se ha completado usando el operador módulo
      if (totalObjetos % 5 == 0) {
        Serial.println("LOTE COMPLETADO");
      }
    }
  } else {
    // Transición: OBJETO PRESENTE -> ESPERANDO OBJETO
    objetoPresente = false;
  }

  delay(50); // Pequeña pausa para estabilidad del simulador
}