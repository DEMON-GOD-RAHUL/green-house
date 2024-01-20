#define AOUT_PIN A0 // Arduino pin that connects to AOUT pin of moisture sensor
#define motor 8
void setup() {
  pinMode(3,INPUT);
  pinMode(12,OUTPUT);
  Serial.begin(9600);
}

void loop() {
if (digitalRead(3)==0) {
  Serial.println("Fire detecteted");
}

  int value = analogRead(AOUT_PIN); // read the analog value from sensor

  Serial.print("Moisture: ");
  Serial.println(value);
  if(value>850){
digitalWrite(12,HIGH);
  }
  if(value<850){
digitalWrite(12,LOW);
  }
}