/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <SPI.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LoRa.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define mySerial Serial2
#define RXD2 16
#define TXD2 17
#define ss 5
#define rst 15
#define dio0 2
int counter = 0;

char figmode;
String ration_no;

String material="";

#include <Adafruit_Fingerprint.h>
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(17, 16);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

const byte n_rows = 4;
const byte n_cols = 4;
 
char keys[n_rows][n_cols] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
 
byte colPins[n_rows] = {13, 12, 14, 27};
byte rowPins[n_cols] = {26, 25, 33, 32};
 
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);
 

void setup(){
  Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);
  //Fingerprint sensor module setup
  Serial.begin(115200);
  // set the data rate for the sensor serial port
 
  finger.begin(57600);
 
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  //OLED display setup
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //displays main screen
  //displayMainScreen();
  display_keypad('-',true);

 lora_initialization();
}


void loop(){
 
  char myKey = myKeypad.getKey();
  if(figmode==0)
  {
    display_keypad('-',true);
    
    figmode=1;
    }
  if (myKey != NULL){
    Serial.print("Key pressed: ");
    Serial.println(myKey);
    display_keypad(myKey,false);
    ration_no += myKey;
  }
  if (myKey == 'D'){
    Serial.print("Key pressed: ");
    Serial.println(myKey);
    ration_no = "";
   figmode = 0;
  }

  if(ration_no == "1234")
  {
    ration_no = "";
 ledprint("put finger","on finger print");
  delay(50);
    Serial.println("entering in fingerprint mode");
    figmode =1;
    while(figmode == 1)
    {
  fingerprintID = getFingerprintIDez();
  delay(50);
  if(fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3){
    IDname = "SHASHANK"; // write your faily name
    displayUserGreeting(IDname);
    material= "wheat 5 kg";
    displayshow(material);
    LoRa.beginPacket();
    LoRa.print(material + " received by ");
    LoRa.print(IDname);
    LoRa.endPacket();
    figmode =0;    
          
       }  
    }
   display_keypad('-',1);
}

 if(ration_no == "4567")
  {
    ration_no = "";
 ledprint("put finger","on finger print");
  delay(50);
    Serial.println("entering in fingerprint mode");
    figmode =1;
    while(figmode == 1)
    {
  fingerprintID = getFingerprintIDez();
  delay(50);
  if(fingerprintID == 4 || fingerprintID == 5 || fingerprintID == 6){
    IDname = "SHRIPAD"; // write your faily name
    displayUserGreeting(IDname);
    material= "wheat 5 kg";
    displayshow(material);
    LoRa.beginPacket();
    LoRa.print(material + " received by ");
    LoRa.print(IDname);
    LoRa.endPacket();
    figmode =0;          
       }  
    }
   display_keypad('-',1);
}

if(ration_no == "7891")
  {
    ration_no = "";
 ledprint("put finger","on finger print");
  delay(50);
    Serial.println("entering in fingerprint mode");
    figmode =1;
    while(figmode == 1)
    {
  fingerprintID = getFingerprintIDez();
  delay(50);
  if(fingerprintID == 7 || fingerprintID == 8 || fingerprintID == 9){
    IDname = "ADESH"; // write your faily name
    displayUserGreeting(IDname);
    material= " wheat 5 kg";
    displayshow(material);
    LoRa.beginPacket();
    LoRa.print(material + " received by ");
    LoRa.print(IDname);
    LoRa.endPacket();
    figmode =0;          
       }  
    }
   display_keypad('-',1);
}


}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
 
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}

void ledprint(String upper,String lower){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(upper);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.println(lower);  
  display.display();
  delay(2000);
}

void displayUserGreeting(String Name){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Hello");
  display.setCursor(0,15);
  display.print(Name);
  display.display();
  delay(5000);
  fingerprintID = 0;
}

void displayshow(String Name){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("you got");
  display.setCursor(0,15);
  display.print(Name);
  display.display();
  delay(5000);
  fingerprintID = 0;
}

void display_keypad(char Name, bool display_clear){
 if (display_clear == true){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("ENTER RATION NO");
  display.setCursor(0,30);
  Serial.println("enter ration no");
 }
  display.print(Name);
  display.display();
  delay(500); 
}

void lora_initialization(){
   while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  }
