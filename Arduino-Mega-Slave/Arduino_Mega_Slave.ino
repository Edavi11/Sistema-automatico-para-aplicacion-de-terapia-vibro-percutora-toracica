#include <Wire.h>

byte rojos[]={22,24,26,28,30,32,34,36,38,40};

int c = 0;

byte PWM[]={4,5,6,7,8,9,10,11,12,13};
float AVGW = 0;

float PT = 0;

#include "HX711.h"
#define DOUT  3
#define CLK  2
HX711 scale(DOUT, CLK);
float calibration_factor = 20780.0;

int pulso = 0;

String g = "";
int e = 0;
float a = 0;
int m = 0;
int t = 0;

int chrono = 0;


void start_readpeso(){
  //Serial.println(volt);
      AVGW = AVGW + scale.get_units();
      c++;
      if(c == 5){
        c=0;
        
        PT = AVGW/5;
        Serial.println();
        Serial.print("peso promedio: " );
        Serial.print(PT);
        Serial.println();
        AVGW = 0;
        }
        Serial.println();
        Serial.print("peso: " );
        Serial.print(scale.get_units(),3 );
        Serial.println();
        
    }
    
void motores(byte rojo,byte pwm, byte volt){
    digitalWrite(rojo,HIGH);
    analogWrite(pwm,volt);
  }


void encender_motores(){
  //start_readpeso();
  byte volt = 0;
  float PM = 0;


  if(g == 'M'){
      PM = 49.5;
    }
    else{
        PM = 28.5;
      }
        
  volt = ((PT*255)/PM + (e*255)/60 + (a*255)/3)/3;
   
   if(volt < 127){
       volt =127;
    }else if(volt>255){
        volt=255;
    }
    Serial.println(volt);
    modos(volt);
  }

void modos(byte volt){
    switch(m){
        case 1:
        for(int j = 0; j < 10; j++ ){
          motores(rojos[j],PWM[j],volt);}
        break;
        case 2:
          motores(rojos[0],PWM[0],volt);
          motores(rojos[1],PWM[1],volt);
          motores(rojos[2],PWM[2],volt);
        break;
        case 3:
          motores(rojos[3],PWM[3],volt);
          motores(rojos[4],PWM[4],volt);
          motores(rojos[5],PWM[5],volt);
          motores(rojos[6],PWM[6],volt);
        break;
        case 4:
          motores(rojos[7],PWM[7],volt);
          motores(rojos[8],PWM[8],volt);
          motores(rojos[9],PWM[9],volt);
        break;
      }
  }

void apagar_motores(){
  for(byte j = 0; j < 10; j++){
   motores(rojos[j],PWM[j],0);
    }
  }

void receiveEvent(int howMany) {
  String genero = "";
  String edad = "";
  String altura = "";
  String modo = "";
  String tiempo = "";
  int s = 0;
  
  while (0 < Wire.available()) {
    char c = Wire.read(); // receive byte as a character
    
    if(c ==' '){
        s++;
      }else{
          switch(s){
            case 0:
              genero= genero + c;
              apagar_motores();
            break;
            case 1:
              edad = edad +c;
            break;
            case 2:
              altura = altura + c;
            break;
            case 3:
              modo = modo + c;
            break;
            case 4:
              tiempo = tiempo + c;
            break;
            }
        }
    
    //Serial.print(c);         // print the character
    
  }
   Serial.println();
  /*
  Serial.println(genero);         // to newline
  Serial.println(edad); 
  Serial.println(altura); */

  g = genero;
  e = edad.toInt();
  a = altura.toInt();
  m = modo.toInt();
  t =tiempo.toInt()*100;
  
 /* Serial.println(t);
  Serial.println(m);         // to newline
  Serial.println(a); 
  Serial.println(g);
  Serial.println(e);*/

if(g!='s'){
  encender_motores();}
}


void requestEvent() {
  Wire.write("Hello NodeMCU");//send string on request
}

void corazon(){
      pulso = analogRead(A0);
      //Serial.println("pulso: "+ pulso);

       Serial.println();
        Serial.print("pulso: " );
        Serial.print(pulso);
        Serial.println();
      
      if(pulso >= 600){
          apagar_motores();
        }
  }


void setup() {
Wire.begin(8);                // join i2c bus with address #8
Wire.onReceive(receiveEvent); // register receive event
Wire.onRequest(requestEvent); // register request event
Serial.begin(9600);
scale.set_scale(14680);  //Calibration Factor obtained from first sketch
}

void loop() {
  start_readpeso();
  corazon();
  delay(1000);

  if(t != 0){
     chrono = chrono+1;
     
    }
//Serial.println(chrono);
   if(t == chrono and chrono !=0){
      apagar_motores();
      chrono = 0;
      t = 0;
      c=0;
    }
}
