//#define BLYNK_TEMPLATE_ID "TMPL3h2ylcGf7"
//#define BLYNK_TEMPLATE_NAME "ecplus"
//#define BLYNK_AUTH_TOKEN "p_M-AMryYOnykKRjpP8F05S8FfRru71a"
#define BLYNK_AUTH_TOKEN "_pWPSRIRRYr9NTSZrIg6JIvFRZyRtc_z"
#define BLYNK_TEMPLATE_ID "TMPL3re6CFWsv"
#define BLYNK_TEMPLATE_NAME "soil"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SOIL_MOISTURE_PIN A0   
#define THRESHOLD_MOISTURE 25   
#define PUMP_PIN D1 
#define PUMP_SWITCH V6 

char auth[] = "_pWPSRIRRYr9NTSZrIg6JIvFRZyRtc_z";  
char ssid[] = "Vicky";   
char pass[] = "123456789";    

BlynkTimer timer;

bool isPumpOn = false; 

void sendSensorData()
{
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);  
  int soilMoisturePercentage = map(soilMoisture, 400, 1023, 100, 0);  

  Blynk.virtualWrite(V5, soilMoisturePercentage);


  Serial.print("Soil Moisture Percentage: ");
  Serial.println(soilMoisturePercentage);

  
  if (isPumpOn || soilMoisturePercentage > THRESHOLD_MOISTURE)
  {
    digitalWrite(PUMP_PIN, HIGH);  
    if (!isPumpOn) {
      Blynk.logEvent("moisture_alert", "Soil moisture is below the threshold! Pump ON");
     // Serial.println("Soil moisture is below the threshold. Pump turned ON.");
      Serial.println("Soil moisture is sufficient. Pump turned OFF.");
    }
  }
  else
  {
    if (!isPumpOn) {
      digitalWrite(PUMP_PIN, LOW);  
      //Serial.println("Soil moisture is sufficient. Pump turned OFF.");
      Serial.println("Soil moisture is below the threshold. Pump turned ON.");
    }
  }
}

BLYNK_WRITE(PUMP_SWITCH)
{
  isPumpOn = param.asInt();  
  if (isPumpOn) {
    digitalWrite(PUMP_PIN, HIGH);  
    Serial.println("Pump manually turned OFF");
  } else {
    digitalWrite(PUMP_PIN, LOW);  
    Serial.println("Pump manually turned ON");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);  

  Blynk.begin(auth, ssid, pass);  

  timer.setInterval(1000L, sendSensorData);  

  
  Blynk.virtualWrite(PUMP_SWITCH, isPumpOn);
  Blynk.syncVirtual(PUMP_SWITCH);
}

void loop()
{
  Blynk.run(); 
  timer.run();  
}
