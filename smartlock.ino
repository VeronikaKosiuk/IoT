#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "iPhone Вероніка";
const char* password = "wwwwwwww";
int lock1 = 1;
int lock2 = 1;
int cd1 = 0;
bool updateSite = false;
bool updateSite2 = true;
int cd2 = 0;
TaskHandle_t Task1;
TaskHandle_t Task2;

void Task1code( void * pvParameters ){
  

  for(;;){

    update();

  }

}

void Task2code( void * pvParameters ){
  
  
  for(;;){
int buttonState = digitalRead(21);
  int buttonState1 = digitalRead(19);
 int buttonState2 = digitalRead(18);
//  Serial.println(cd1);
    if( cd1 < 0){

      if(buttonState == 0){
        updateSite = true;
        updateSite2 = false;
        if(lock1 == 1){
          lock1 = 0;
        }else{
          lock1 = 1;
        }
        cd1 = 200;
      }
    }else{
      cd1--;
    }

    if( cd2 < 0){

      if(buttonState1 == 0){
        updateSite = true;
        updateSite2 = false;
        if(lock2 == 1){
          lock2 = 0;
        }else{
          lock2 = 1;
        }
        cd2 = 200;
      }
    }else{
      cd2--;
    }
 if( cd2 < 0 && cd1 < 0){

      if(buttonState2 == 0){
        updateSite = true;
        updateSite2 = false;
        lock1 = 0;
        lock2 =0;
        cd2 = 200;
        cd1 = 200;
      }
    }


    digitalWrite(13,lock1);

    digitalWrite(14,lock2);
    digitalWrite(27,!lock1);

    digitalWrite(26,!lock2);
    delay(1);
  }

}

void setup() {
  Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
  
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(21, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    -1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

void update(){
  if ((WiFi.status() == WL_CONNECTED)) {


    // создаем объект для работы с HTTP
    HTTPClient http;
    if(updateSite){
          int tmp1 = 0;
          int tmp2 = 0;
          if(lock1 == 1){
                tmp1 = 0;
              }else{
                tmp1=1;
              }
              if(lock2 == 1){
                tmp2 = 0;
              }else{
                tmp2=1;
              }
          http.begin("https://veronikakosiuk.azurewebsites.net/keys.php?act=put&k1="+String(tmp1)+"&k2="+String(tmp2));
            // делаем GET запрос
            Serial.println("https://veronikakosiuk.azurewebsites.net/keys.php?act=put&k1="+String(tmp1)+"&k2="+String(tmp2));
            int httpCode = http.PUT("Request");
            
            // проверяем успешность запроса
            if (httpCode > 0) {
              updateSite = false;
              updateSite2 = true;
            }
    }else{
      http.begin("https://veronikakosiuk.azurewebsites.net/keys.php?act=get");
            // делаем GET запрос
            int httpCode = http.GET();
            
            // проверяем успешность запроса
            if (httpCode > 0) {
              // выводим ответ сервера
              String req = http.getString();
            
            if(updateSite2){
              lock1 = req[0]-'0';
              lock2 = req[2]-'0';
              if(lock1 == 1){
                lock1 = 0;
              }else{
                lock1=1;
              }
              if(lock2 == 1){
                lock2 = 0;
              }else{
                lock2=1;
              }
            }
                
            }
    }
      http.end();
   
  }
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
 
}
