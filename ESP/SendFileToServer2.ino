#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>

#include <UDHttp.h>

#include <SPIFFS.h>

#include "UDHttp.h"

const char* ssid     = "BlueBery";
const char* password = "1234567890";

File root;
int responsef(uint8_t *buffer, int len){
  Serial.printf("%s\n", buffer);
  return 0;
}
//read data callback
int rdataf(uint8_t *buffer, int len){
  //read file to upload
  if (root.available()) {
    return root.read(buffer, len);
  }
  return 0;
}
//write data callback
int wdataf(uint8_t *buffer, int len){
  //write downloaded data to file
  return root.write(buffer, len);
}

void progressf(int percent){
  Serial.printf("%d\n", percent);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  bool success = SPIFFS.begin();

  if (!success) {
    Serial.println("Error mounting the file system");
    return;
  }
  {
    UDHttp udh;
    //open file on sdcard to read
    File fb = SPIFFS.open("/recording.wav","r");
    if (!fb) {
       Serial.println("can not open file!");
       return;
    }
    Serial.println(fb.name());
    Serial.println(fb.size());
    udh.upload("https://9eb2-14-243-165-107.ap.ngrok.io:443/base/send", "/recording.wav", fb.size(), rdataf, progressf, responsef);
    Serial.printf("done uploading\n");
    fb.close();
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
