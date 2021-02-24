#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D2, D3); //RX | TX

#define FIREBASE_HOST "project-test-140b2.firebaseio.com"
#define FIREBASE_AUTH "wlqqKgK1RbNWwSXhh76ZHCgggzIQI3DIPt2BrELI"

#define WIFI_SSID       "Ae"
#define WIFI_PASSWORD   "11111111"

#define D0 16
#define check D0
int i;
String falsealarmcheck;

void setup() { 
  
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(check, OUTPUT);
  NodeSerial.begin(57600);
  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Do something
}
void loop() { 
    static String msg="";
    /*
    while (Serial.available() > 0) {
      char inByte = Serial.read();
      NodeSerial.write(inByte);
      Serial.write(inByte);
    } 
      */
  /**
   * เช็คว่าถ้ามีค่าเข้ามาจาก arduino ให้วนรับค่า 
   * และแสดงออกทาง Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
   * และออกจาก loop
   */
    
    while (NodeSerial.available() > 0) {
      char inByte = NodeSerial.read();
      
  
      //เก็บสะสมค่าไว้ใน String ชื่อ msg
      msg+= inByte;
  
      //จนกว่าค่าที่อ่านได้จะเป็นการขึ้นบรรทัดใหม่
      if(inByte =='\n') {
  
        //ดึงค่าแรก (index 0) ออกจาก String msg เก็บไว้บน value_1
        String value_1 = getValue(msg, ',', 0);
        
        //ดึงค่า (index 1) ออกจาก String msg เก็บไว้บน value_2
        String value_2 = getValue(msg, ',', 1);

        //ดึงค่า (index 2) ออกจาก String msg เก็บไว้บน value_3
        String value_3 = getValue(msg, ',', 2);
        
        //ดึงค่า (index 3) ออกจาก String msg เก็บไว้บน value_4
        String value_4 = getValue(msg, ',', 3);

        //ดึงค่า (index 4) ออกจาก String msg เก็บไว้บน value_5
        String value_5 = getValue(msg, ',', 4);
        
        //ดึงค่า (index 5) ออกจาก String msg เก็บไว้บน value_6
        String value_6 = getValue(msg, ',', 5);

        //ดึงค่า (index 6) ออกจาก String msg เก็บไว้บน value_7
        String value_7 = getValue(msg, ',', 6);
        
        
        
        Serial.print("S1LPG :");
        Serial.print( value_1.toInt() ); //แปลงค่าจาก String เป็นจำนวนเต็มด้วย toInt()
        Serial.print(" S1CO :");
        Serial.print( value_2.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        Serial.print(" S1SMOKE :");
        Serial.println( value_3.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        Serial.print("S2LPG :");
        Serial.print( value_4.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        Serial.print(" S2CO :");
        Serial.print( value_5.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        Serial.print(" S2SMOKE :");
        Serial.println( value_6.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        Serial.print(" SystemSttatus :");
        Serial.println( value_7.toInt() ); //แปลงค่าจาก string เป็นจำนวนเต็มด้วย toInt()
        
        msg = "";

        Firebase.setString("Sensor1/LPG", value_1);
        Firebase.setString("Sensor1/CO", value_2);
        Firebase.setString("Sensor1/SMOKE", value_3);

        Firebase.setString("Sensor2/LPG", value_4);
        Firebase.setString("Sensor2/CO", value_5);
        Firebase.setString("Sensor2/SMOKE", value_6);

        //set systemstatus 1 = detected, 0 = normal
        Firebase.setString("Data", value_7);
        
        falsealarmcheck = Firebase.getString("FalseAlarmStatus");
        Serial.println(falsealarmcheck);
        if (falsealarmcheck.equals("1")) {
          digitalWrite(check,HIGH);
        }
        else{
          digitalWrite(check,LOW);
        }

        
        /*
        Firebase.setInt("Sensor3/CO", i);
        Firebase.setInt("Sensor3/LPG", i);
        Firebase.setInt("Sensor3/SMOKE", i);
        */
        if (Firebase.failed()) {
            Serial.print("set /number failed:");
            Serial.println(Firebase.error());  
            return;
        }
      }
    }
}

/**
 * ใช้ดึงค่า String ออกจาก String โดยใช้การตัดคำจาก separator ซึ่ง
 * อาจจะเป็น , ; : หรืออะไรก็ได้ที่ใช้แยกคำ
 */
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
