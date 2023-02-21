#include<WiFi.h>
#include<Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "MQDL"
#define WIFI_PASSWORD "0515103954"
#define API_KEY "AIzaSyCTgDpZr-vrdREGFZm3n_Iiu2_3FHlHP0Q"
#define DATABASE_URL "https://ekekekek-fdf3b-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define LED1_PIN 12
#define LED1_PIN 14
#define tem 27

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

int sensorValue = 0;



float temp(){
      float sensorValue = analogRead(36);
      float v=sensorValue*3.3/4096;
//      float vout = (v * R) / (R + 1 / (2 * 3.141592 * C));
      float voltage=v*(15.00+68.00)/68.00;
      float V1=5.00*4.70/(8.20+4.70);
      float R=((330.00/150.00)*(1.00+150.00/330.00)*(330.00/(330.00+150.00))*1500.00*5.00)/(voltage+V1*330.00/150.00)-1500.00;
      float temperature=(R-1035.2000)/3.8758;
      return temperature;
}

 
//float avg(){ 
//  float sum = 0.00;
//  float l_sum = 0.00;
//  float l_avg = 0.00;
//  float avg = 0.00;
//  float t_avg = 0.00;
//  float p_avg = 0.00;
//  for (int i = 0; i < 100; i++) {
//    sum += temp();
//    delay(10);
//  }
//  l_avg = sum/100;
//  for(int k=0; k<10; k++){
//  if(temp()<(t_avg+1.00) && temp()>(t_avg-1.00)){
//     l_sum += temp();
//     delay(10);
//  } else{
//    l_sum += l_avg;
//    delay(10);
//  }
//  }
//  return round(l_sum)/10;
// }

 float avg(){ 
  float sum = 0.00;
  float avg = 0.00;
  for (int i = 0; i < 1000; i++) {
    sum += temp();
    delay(5);
  }
  return round(sum/100)/10;
 }



void setup(){
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "","")){
    Serial.println("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop(){
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if(Firebase.RTDB.setFloat(&fbdo,"Sensor/temperature",avg())){
      Serial.println(); Serial.println(avg());
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType()+ ")");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
//    if(Firebase.RTDB.setFloat(&fbdo,"Sensor/temperaturetest",avgtest())){
//      Serial.println(); Serial.print(avgtest());
//      Serial.print(" - successfully saved to: " + fbdo.dataPath());
//      Serial.println(" (" + fbdo.dataType()+ ")");
//    }else{
//      Serial.println("FAILED: " + fbdo.errorReason());
//    }
  }
}
