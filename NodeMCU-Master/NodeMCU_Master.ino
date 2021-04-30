#include <Wire.h>

#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Huawei P20";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";
const char* PARAM_INPUT_5 = "input5";
const char* PARAM_INPUT_6 = "input6";

String inputMessage1 = "", inputMessage2 = "", inputMessage3 = "", inputMessage4 = "",inputMessage5 = "",inputMessage6 = "";
String inputParam1,inputParam2,inputParam3,inputParam4,inputParam5,inputParam6;

const char* a_html PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

</head>

<body>
            <form action="/get_1">
                <input class="btn btn-primary" type="submit" value="S" name="input6">
            </form>
</body>
</html>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"
        integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js"
        integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
        crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"
        integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
        crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"
        integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
        crossorigin="anonymous"></script>
</head>

<body>
    <div class="row justify-content-center">
        <div class="col">
            <form action="/get">
                 <br>
                  <select class="custom-select" name="input4">
                    <option value="1" selected>Todas las zonas</option>
                    <option value="2">Trapecios(zona superior)</option>
                    <option value="3">Dorsales(zona media)</option>  
                    <option value="4">Lumbares(zona inferior)</option>
                 </select>
                 <br>
                  <select class="custom-select" name="input5">
                    <option value="0" selected>Tiempo indefinido</option>
                    <option value="3">5 minutos</option>
                    <option value="6">10 minutos</option>  
                    <option value="9">15 minutos</option>
                 </select>
                 <br>
                 <select class="custom-select" name="input1">
                    <option value="M">Masculino</option> 
                    <option value="F" selected>Femenino</option>
                 </select>
                <br>
                <select class="custom-select" name="input3">
                    <option value="1">1.60 mts o menos</option>
                    <option value="2">1.60-1.80 mts</option>  
                    <option value="3">1.80 mts o mas</option>
                 </select>
                 <br>
                <input class="form-control" placeholder="EDAD" type="text" name="input2">
                <br>
                
                <input class="btn btn-primary" type="submit" value="SEND">
            </form>
        </div>
    </div>
</body>
</html>

)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

char* String_toChar(String papita) {
  int s  = papita.length()+1;
  Serial.println();
  char charbuf[s];
  papita.toCharArray(charbuf,s);
  return charbuf;
  }
void setup() {

  Serial.begin(9600);
  Wire.begin(14, 12);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputParam1 = PARAM_INPUT_1;
      
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      
      inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      inputParam3 = PARAM_INPUT_3;

      inputMessage4 = request->getParam(PARAM_INPUT_4)->value();
      inputParam4 = PARAM_INPUT_4;

      inputMessage5 = request->getParam(PARAM_INPUT_5)->value();
      inputParam5 = PARAM_INPUT_5;
    }
  
    request->send(200, "text/html", a_html);
  });
  
  server.on("/get_1", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_6)) {
      inputMessage6 = request->getParam(PARAM_INPUT_6)->value();
      inputParam6 = PARAM_INPUT_6;
    }
  request->send(200, "text/html", index_html);

 });
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
  server.onNotFound(notFound);
  server.begin();

}

void loop() {

  if(!inputMessage1.equals("") and !inputMessage2.equals("") and !inputMessage3.equals("") and !inputMessage4.equals("") and !inputMessage5.equals("")){
    
    Wire.beginTransmission(8);
    char* line  = String_toChar(inputMessage1+" "+inputMessage2+" "+inputMessage3+" "+inputMessage4+" "+inputMessage5);
  
    Wire.write(line,13);
   
    Serial.println(String_toChar(inputMessage1+" "+inputMessage2+" "+inputMessage3+" "+inputMessage4+" "+inputMessage5));
    Wire.endTransmission();    

    inputMessage1="";
    inputMessage2="";
    inputMessage3="";
    inputMessage4="";
    inputMessage5="";

    Wire.requestFrom(8, 13); 
    while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
    }
    Serial.println();  
  }
  if(!inputMessage6.equals("")){
    Wire.beginTransmission(8);
    Wire.write(String_toChar(inputMessage6));
    Wire.endTransmission();    
      Serial.println(inputMessage6);
      inputMessage6="";
      Wire.requestFrom(8, 13);
       
    while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
    }
  } 
  delay(1000);
}
