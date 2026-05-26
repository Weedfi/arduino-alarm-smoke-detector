const int trigPin = 6;
const int echoPin = 5;

#define GreenLED 11
#define YellowLED 10
#define RedLED 9
#define buzzer 3
#define mq2Pin A0

int smokeThreshold = 90; // próg alarmu, dobierz eksperymentalnie

void setup()
{
  Serial.begin(9600);

  pinMode(GreenLED, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW); // buzzer cichy na start

  Serial.println("Nagrzewanie MQ-2... 20s");
  delay(20000); // MQ-2 musi się nagrzać
}

void loop()
{
  long duration, cm;

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 29 / 2;

  int smokeLevel = analogRead(mq2Pin);

  // LEDy od dystansu
  digitalWrite(GreenLED, (cm <= 50 && cm > 0) ? HIGH : LOW);
  digitalWrite(YellowLED, (cm <= 30 && cm > 0) ? HIGH : LOW);
  digitalWrite(RedLED, (cm <= 15 && cm > 0) ? HIGH : LOW);

  // Buzzer reaguje na bliskość LUB dym
  if ((cm <= 15 && cm > 0) || smokeLevel > smokeThreshold) {
    digitalWrite(buzzer, HIGH);   // piszczy
    digitalWrite(RedLED, HIGH);  // czerwona też przy dymie
  }
  else {
    digitalWrite(buzzer, LOW);  // cisza
  }

  Serial.print(cm);
  Serial.print(" cm | dym: ");
  Serial.println(smokeLevel);

  delay(300);
}