//#include "DHT.h"
#include "String.h"
#include "ArduinoJson.h"
#define BUTTON_PIN 25
#define led  33
#define den  18
#define quat 5
//#define DHTPIN 13 // chân kết nối cảm biến nhiệt độ
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);
// Micro INMP441
#include <driver/i2s.h>
#include <SPIFFS.h>
#define I2S_WS 15
#define I2S_SD 4
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (16 * 1024)
#define RECORD_TIME       (20) //Seconds
#define I2S_CHANNEL_NUM   (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

File file;
const char filename[] = "/recording.wav";
const int headerSize = 44;
// Date time
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//const char* wifiName = "Family";
//const char* password = "danthy2103";
String wifiname = "";
String pass = "";
const char* host = "esp32fs";

//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
//String formattedDate;
//String dayStamp;
//String timeStamp;
//Down File
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false
#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif
//MP3
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"
HardwareSerial mySerial(1);
int i = -1;
int currentState = 0;

DFRobotDFPlayerMini myDFPlayer;


//Send File to Server
#include <WiFiClientSecure.h>

const char*  server = "d24b-14-236-47-31.ap.ngrok.io";  // Server URL

const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";
WiFiClientSecure client;


void setup() {
  
  
  Serial.begin(115200);

  Serial1.begin(19200, SERIAL_8N1,27,26);
  while (true)
  {
    if (Serial1.available()) {
    wifiname=Serial1.readString();
    break;}
   
  }
  delay(100);
  Serial.print("WifiName:");
  Serial.println(wifiname);
   while (true)
  {
    if (Serial1.available()) {
      pass=Serial1.readString();
      break;}
  }
  delay(100);
  Serial.print("password:");
  Serial.println(pass);
  const char* wifiName = wifiname.c_str();
  const char* password = pass.c_str();
  WiFi.begin(wifiName, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial1.print(wifiname);
  Serial1.flush();
  delay(100);
  Serial1.end();
  

    mySerial.begin(9600, SERIAL_8N1, 16, 17);
   if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(myDFPlayer.readType(), HEX);
    Serial.println(F("Error starting, check:"));
    Serial.println(F("1.The module connection."));
    Serial.println(F("2.If the SD card was inserted correctly."));
    while (true);
  }
  

 

//  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//  timeClient.begin();
//  timeClient.setTimeOffset(25200);  
//  dht.begin();
  client.setCACert(test_root_ca);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(led, OUTPUT); 
  pinMode(den, OUTPUT);
  pinMode(quat, OUTPUT);
  digitalWrite(den,HIGH);
  digitalWrite(quat,HIGH);
  myDFPlayer.setTimeOut(500);

  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  Serial.print(F("Files found on SD card: "));
  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));

  myDFPlayer.EQ(0);    
  myDFPlayer.playFolder(1, 1);
  delay(5000);

}

void loop() {
  digitalWrite(led,LOW);
  currentState = digitalRead(BUTTON_PIN);
  if(currentState == LOW){
    SPIFFSInit();
    i2sInit();
    int i2s_read_len = I2S_READ_LEN;
    size_t bytes_read;
    char* i2s_read_buff = (char*) calloc(i2s_read_len, sizeof(char));
    uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));
    i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
    i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);   
   // Serial.println(" ***Recording Start*** ");
    int flash_wr_size = 0;
    while (currentState != HIGH && FLASH_RECORD_SIZE>flash_wr_size) {
       currentState = digitalRead(BUTTON_PIN);
        digitalWrite(led,HIGH);
        i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);
        file.write((const byte*) flash_write_buff, i2s_read_len);
        //ets_printf(".");
        flash_wr_size += i2s_read_len;
    }
    //ets_printf("Complete");
    file.close();  
    digitalWrite(led,LOW);
    free(i2s_read_buff);
    i2s_read_buff = NULL;
    free(flash_write_buff);
    flash_write_buff = NULL;
    listSPIFFS();
    SendFile(myDFPlayer);
//    myDFPlayer.next();
  }
}
//Send file to Server
void SendFile(DFRobotDFPlayerMini myDFPlayer){
    //Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    //Serial.println("Connected to server!");

     if(!SPIFFS.begin(true)){
       Serial.println("An Error has occurred while mounting SPIFFS");
       return;
      }
    File fp = SPIFFS.open("/recording.wav","r");
    if(!fp){
      Serial.println("Khong co file");
      return;
    }
      String boundary = "--------------------------928315622432575348929725";
      String postHeader = "POST /base/recognition HTTP/1.1\r\n";
      postHeader += "Host: d24b-14-236-47-31.ap.ngrok.io\r\n";
      postHeader += "Connection: keep-alive\r\n";
      postHeader += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
  
      String keyHeader = "--" + boundary + "\r\n";
      keyHeader += "Content-Disposition: form-data; name=\"name\"\r\n\r\n";
      keyHeader += "device01\r\n";

      String keyHeader1 = "--" + boundary + "\r\n";
      keyHeader1 += "Content-Disposition: form-data; name=\"device_id\"\r\n\r\n";
      keyHeader1 += "1\r\n";
      
      String requestHead = "--" + boundary + "\r\n";
      requestHead += "Content-Disposition: form-data; name=\"audio\"; filename=\"recording.wav\"\r\n";
      requestHead += "Content-type: audio/wav\r\n\r\n";
      
      String tail = "\r\n--" + boundary + "--\r\n\r\n";
      int contentLength = keyHeader.length() + keyHeader1.length() + requestHead.length() + fp.size() + tail.length();
      postHeader += "Content-Length: " + String(contentLength, DEC) + "\n\n";
    
      Serial.println(postHeader);
      Serial.println(keyHeader);
      Serial.println(keyHeader1);
      Serial.println(requestHead);
      Serial.println(tail);
      
      client.print(postHeader);
      client.print(keyHeader);
      client.print(keyHeader1);
      client.print(requestHead);

      const int bufSize = 2048;
      byte clientBuf[bufSize];
      int clientCount = 0;
    while (fp.available()) {
      clientBuf[clientCount] = fp.read();
      clientCount++;
      if (clientCount > (bufSize - 1)) {
        client.write((const uint8_t *)clientBuf, bufSize);
        clientCount = 0;
      }
    }
    if (clientCount > 0) {
      client.write((const uint8_t *)clientBuf, clientCount);
      Serial.println("Sent LAST buffer");
    }
      
      client.print(tail);

    Serial.println("Sent");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    char json[1000]="";
    while (client.available()) {
      char c = client.read();
      char ch[1];
      ch[0] = c;
      ch[1] = '\0';
      strcat(json,ch);
    }
    Serial.println(json);
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc,json);
    if(err){
      Serial.println("ERROR:"); 
    }
    else{
      int acid         =  doc["action"]["action_id"];
      const char* type =  doc["action"]["type"];     
      if ( acid == 2){
         int content = doc["content"]["device_id"];
         bool state = doc["content"]["state"];
         if(content==1)
         {
          if (state == true){
            digitalWrite(den,LOW);
            myDFPlayer.playFolder(13,1);
            delay(1100);
          }
          else{
            digitalWrite(den,HIGH);
            myDFPlayer.playFolder(13,2);
            delay(1100);
          }
         }
         else if(content==2)
         {
          if (state == true){
            digitalWrite(quat,LOW);
            myDFPlayer.playFolder(13,3);
            delay(1100);
          }
          else{
            digitalWrite(quat,HIGH);
            myDFPlayer.playFolder(13,4);
            delay(1100);
          }
         }
         else{
          if (state == true){
            myDFPlayer.playFolder(12,1);
          }
          else{
            myDFPlayer.stop();
            delay(100);
            myDFPlayer.playFolder(13,5);
            delay(1100);
          }      
         }
      }
      else {
        int coid = doc["content"]["id"];
        if (coid==1){
          int ho = doc["content"]["main"]["hour"];
          int mi = doc["content"]["main"]["min"];
          reGio(myDFPlayer,ho,mi);
        }
        else if(coid==2){
          int ye = doc["content"]["main"]["year"];
          int mo = doc["content"]["main"]["month"];
          int da = doc["content"]["main"]["day"]; 
          reDate(myDFPlayer,da,mo,ye);     
        }
        else if(coid==3){
          myDFPlayer.playFolder(4,1);
          delay(1200); 
          int temp = doc["content"]["main"]["temperature"];
          playNum(temp);
          delay(100);
          myDFPlayer.playFolder(4,2);
          delay(1200);
                
        }
        else if(coid==4){
          myDFPlayer.playFolder(5,1);
          delay(1200);
          int weid = doc["content"]["main"]["weather_id"]; 
          myDFPlayer.playFolder(5,weid);      
        }
        
      }
////      int coid = doc["content"]["id"];
//////      Serial.println(acid);
//////      Serial.println(coid);
//      Serial.println(coid);
    }
    client.stop();
  }
}
void reDate(DFRobotDFPlayerMini myDFPlayer,int da,int mo, int year){
    myDFPlayer.playFolder(2,1);
    delay(2300);
    playNum(da);
    myDFPlayer.playFolder(11,2);
    delay(1200);
    playNum(mo);
    delay(300);
    myDFPlayer.playFolder(11,1);
    delay(2500);
}
void reGio(DFRobotDFPlayerMini myDFPlayer,int ho,int mi){
      myDFPlayer.playFolder(3,1);
      delay(2200);
      playNum(ho);
      myDFPlayer.playFolder(3,2);
      delay(1000);
      playNum(mi);
      myDFPlayer.playFolder(3,3);
      delay(1000);
}
void playNum(int ho)
{
        if(ho==10){
        myDFPlayer.playFolder(10,10);
        delay(1200);
      }
      else if(ho<10){
        myDFPlayer.playFolder(10,ho);
        delay(1200);
      }
      else {
        int h1 = ho/10;
        myDFPlayer.playFolder(10,h1*10);
        delay(1200);
        int h2 = ho%10;
        if(h2==1){
           myDFPlayer.playFolder(10,99);
           delay(1200);
        }
        else{
          myDFPlayer.playFolder(10,h2);
          delay(1200);
          }             
      }
}
//void Date(NTPClient timeClient){
//  timeClient.forceUpdate();
//  formattedDate = timeClient.getFormattedDate();
//
//  int splitT = formattedDate.indexOf("T");
//  dayStamp = formattedDate.substring(0, splitT);
//  Serial.print("Ngày: ");
//  Serial.println(dayStamp);
//}
//void Time(NTPClient timeClient){
//  timeClient.forceUpdate();
//  formattedDate = timeClient.getFormattedDate();
//  int splitT = formattedDate.indexOf("T");
//  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
//  Serial.print("Giờ: ");
//  Serial.println(timeStamp);
//
//}

//void NhietDoDoAm(DHT dht){
// float h = dht.readHumidity();
// float t = dht.readTemperature();
// float f = dht.readTemperature(true);
// if (isnan(h) || isnan(t) || isnan(f)) {
// Serial.println("Failed to read from DHT sensor!");
// return;
// }
// float hif = dht.computeHeatIndex(f, h);
// float hic = dht.computeHeatIndex(t, h, false);
//
// Serial.print("Độ ẩm: ");
// Serial.print(h);
// Serial.print(" %\t");
// Serial.print("Nhiệt độ: ");
// Serial.print(t);
// Serial.print(" *C ");
// Serial.print(f);
// Serial.print(" *F\t");
// Serial.print("Chỉ số nhiệt: ");
// Serial.print(hic);
// Serial.print(" *C ");
// Serial.print(hif);
// Serial.println(" *F");
//}

// Phần mic INMP441
void SPIFFSInit(){
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS initialisation failed!");
    while(1) yield();
  }

  SPIFFS.remove(filename);
  file = SPIFFS.open(filename, FILE_WRITE);
  if(!file){
    Serial.println("File is not available!");
  }
  byte header[headerSize];
  wavHeader(header, FLASH_RECORD_SIZE);
  file.write(header, headerSize);
  listSPIFFS();
}
void i2sInit(){
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 64,
    .dma_buf_len = 1024,
    .use_apll = 1
  };
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };
  i2s_set_pin(I2S_PORT, &pin_config);
}

void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len)
{
    uint32_t j = 0;
    uint32_t dac_value = 0;
    for (int i = 0; i < len; i += 2) {
        dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
        d_buff[j++] = 0;
        d_buff[j++] = dac_value * 256 / 4096;
    }
}
void example_disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}

void wavHeader(byte* header, int wavSize){
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSize = wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;
  header[21] = 0x00;
  header[22] = 0x01;
  header[23] = 0x00;
  header[24] = 0x80;
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;
  header[29] = 0x7D;
  header[30] = 0x00;
  header[31] = 0x00;
  header[32] = 0x02;
  header[33] = 0x00;
  header[34] = 0x10;
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
}

void listSPIFFS(void) {
  Serial.println(F("\r\nListing SPIFFS files:"));
  static const char line[] PROGMEM =  "=================================================";
  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));
  fs::File root = SPIFFS.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }
  fs::File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }
    file = root.openNextFile();
  }
  Serial.println(FPSTR(line));
  Serial.println();
  delay(1000);
}
