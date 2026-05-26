# 🔊 Arduino — System detekcji dystansu i wykrywania dymu

System bezpieczeństwa oparty na Arduino Uno, który monitoruje odległość od obiektów i wykrywa obecność dymu/gazu. Sygnalizuje zagrożenie za pomocą diod LED i buzzera.

---

## 📦 Komponenty

| Komponent | Funkcja |
|---|---|
| Arduino Uno R3 | Jednostka centralna |
| HC-SR04 | Czujnik ultradźwiękowy – pomiar dystansu |
| MQ-2 | Czujnik dymu i gazu (LPG, alkohol, CO) |
| Buzzer (moduł 3-pinowy) | Sygnalizacja akustyczna |
| LED Zielony (D11) | Obiekt w zasięgu 50 cm |
| LED Żółty (D10) | Obiekt w zasięgu 30 cm |
| LED Czerwony (D9) | Obiekt w zasięgu 15 cm lub wykryty dym |
| Rezystory 220Ω | Ograniczniki prądu dla LEDów |

---

## 🔌 Schemat połączeń

| Komponent | Pin komponentu | Pin Arduino |
|---|---|---|
| HC-SR04 | VCC | 5V (szyna) |
| HC-SR04 | GND | GND (szyna) |
| HC-SR04 | TRIG | D6 |
| HC-SR04 | ECHO | D5 |
| MQ-2 | VCC | 5V (szyna) |
| MQ-2 | GND | GND (szyna) |
| MQ-2 | A0 | A0 |
| Buzzer | – (minus) | GND |
| Buzzer | + (plus) | D3 |
| Buzzer | S (signal) | niepodłączone |
| LED Zielony | anoda | D11 (przez rezystor 220Ω) |
| LED Żółty | anoda | D10 (przez rezystor 220Ω) |
| LED Czerwony | anoda | D9 (przez rezystor 220Ω) |
| Wszystkie LEDy | katoda | GND (szyna) |

> **Uwaga:** 5V i GND rozdzielone przez szynę breadboardu — HC-SR04 i MQ-2 podłączone równolegle.

---

## ⚙️ Opis działania

- **Czuwanie:** HC-SR04 wysyła fale ultradźwiękowe co 400 ms. MQ-2 ciągle odczytuje poziom gazu/dymu.
- **Detekcja dystansu:** poniżej 50 cm → zielona LED, poniżej 30 cm → żółta LED, poniżej 15 cm → czerwona LED + buzzer.
- **Detekcja dymu:** jeśli odczyt MQ-2 przekroczy `smokeThreshold` → czerwona LED + buzzer.
- **Alarm:** buzzer piszczy i czerwona LED świeci dopóki zagrożenie jest wykrywane.

---

## 💻 Kod

```cpp
const int trigPin = 6;
const int echoPin = 5;

#define GreenLED 11
#define YellowLED 10
#define RedLED 9
#define buzzer 3
#define mq2Pin A0

int smokeThreshold = 100; // dobierz eksperymentalnie

void setup() {
  Serial.begin(9600);
  pinMode(GreenLED, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  Serial.println("Nagrzewanie MQ-2... 20s");
  delay(20000);
}

void loop() {
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

  digitalWrite(GreenLED, (cm <= 50 && cm > 0) ? HIGH : LOW);
  digitalWrite(YellowLED, (cm <= 30 && cm > 0) ? HIGH : LOW);
  digitalWrite(RedLED,    (cm <= 15 && cm > 0) ? HIGH : LOW);

  if ((cm <= 15 && cm > 0) || smokeLevel > smokeThreshold) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(RedLED, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  Serial.print(cm);
  Serial.print(" cm | dym: ");
  Serial.println(smokeLevel);

  delay(300);
}
```

---

## 📝 Uwagi

- **MQ-2 nagrzewanie:** czujnik potrzebuje ~20 sekund po włączeniu — przed tym czasem odczyty są nierzetelne.
- **smokeThreshold:** w czystym powietrzu baseline to ~280. Próg ustaw powyżej tej wartości (np. 400), żeby uniknąć fałszywych alarmów.
- **Buzzer:** moduł aktywuje się stanem HIGH. Jeśli zachowanie jest odwrotne, zamień `HIGH` i `LOW` przy buzzerze w kodzie.
- **Wspólna masa:** GND wszystkich komponentów musi być połączone ze sobą (przez szynę breadboardu).
