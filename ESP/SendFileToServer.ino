#include <WiFi.h>
#include <HTTPClient.h>
  
#include "SPIFFS.h"
const char* ssid = "BlueBery";
const char* password = "1234567890";
String serverName = "9eb2-14-243-165-107.ap.ngrok.io/base/send";
unsigned long lastTime = 0;
const char* post_host = "9eb2-14-243-165-107.ap.ngrok.io";
const int post_port = 443; 
String url = "/base/send/";
WiFiClient client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  Serial.println("starting file upload");
  if (!client.connect(post_host,post_port))
  { // check connection to host if untrue internet connection could be down
    Serial.println("couldn't connect to host");
  }
  HTTPClient http;
  http.begin(client, serverName);
  if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }
  File fi = SPIFFS.open("/recording.wav",FILE_READ);
  FILE *fp = fopen("/recording.wav", "rb");
  int file_size = fi.size(); //get end position of file

  int max_upload_size = 10; // array size, larger = less uploads but too large can cause memory issues
  int num_of_uploads = file_size / max_upload_size; // figure out how many evenly sized upload chunks we need
  int num_of_uploads_mod = file_size % max_upload_size; //find out size of remaining upload chunk if needed
  int i;
    
  //upload file in even chunks    
  if (num_of_uploads > 0)
  {
    uint8_t buff1[max_upload_size] = {}; 
    for (i = 0; i < num_of_uploads; i++)
    {
      fread(buff1, sizeof *buff1, sizeof buff1 / sizeof *buff1, fp);
      http.addHeader("File_name", "test file"); //header to say what the file name is
      http.addHeader("Content-Type", "application/octet-stream");
      int httpResponseCode = http.POST(buff1, sizeof(buff1)); 
    }
  }
  if (num_of_uploads_mod > 0)
  {
    int remainder = file_size - num_of_uploads * max_upload_size;
    uint8_t buff2[remainder] = {};
    fread(buff2, sizeof *buff2, sizeof buff2 / sizeof *buff2, fp);
    http.addHeader("File_name", "test file");
    http.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = http.POST(buff2, sizeof(buff2));
  }
  http.end(); // Close connection
  delay(10 * 1000);
  Serial.println("testing");
}
