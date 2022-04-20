#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "BlueBery";
const char* password = "1234567890";

//Your Domain name with URL path or IP address with path
String serverName = "https://5420-113-174-211-170.ap.ngrok.io/base/send";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
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

void loop()
{
  WiFiClient client;
  Serial.println("starting file upload");
  if (!client.connect("https://5420-113-174-211-170.ap.ngrok.io",80))
  { // check connection to host if untrue internet connection could be down
    Serial.println("couldn't connect to host");
  }

  HTTPClient http;
  const char* serverName = "https://5420-113-174-211-170.ap.ngrok.io/base/send";
  http.begin(client, serverName);
  Serial.println("connected");
  char *fname = "recording.wav";
  FILE *fp = fopen(fname, "rb"); // read in bytes
    
  //get file size
  fseek(fp, 0, SEEK_END); //send file pointer to end of file 
  int file_size = ftell(fp); //get end position of file
  fseek(fp, 0, SEEK_SET); //send pointer back to start
Serial.println("read file");
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
Serial.println("read file2");
  if (num_of_uploads_mod > 0)
  {
    int remainder = file_size - num_of_uploads * max_upload_size;
    uint8_t buff2[remainder] = {};
    fread(buff2, sizeof *buff2, sizeof buff2 / sizeof *buff2, fp);
    http.addHeader("File_name", "test file");
    http.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = http.POST(buff2, sizeof(buff2));
  }
Serial.println("done");
  http.end(); // Close connection
  delay(10 * 1000);
}



//void loop() {
//  //Send an HTTP POST request every 10 minutes
//  if ((millis() - lastTime) > timerDelay) {
//      
//    
////    //Check WiFi connection status
//    if(WiFi.status()== WL_CONNECTED){
//      WiFiClient client;
//      HTTPClient http;
//    if(client.connect(serverName.c_str())){
//       Serial.println("Connection successful!");  
//       String head = "--TEST\s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"recording.wav\"\r\nContent-Type: audio/wav\r\n\r\n";
//       String tail = "\r\n--TEST--\r\n";
//    }
//    client.println("POST /base/send HTTP/1.1");
//    client.println("Host: https://5420-113-174-211-170.ap.ngrok.io" );
////    client.println("Content-Length: " + String(totalLen));
//    client.println("Content-Type: multipart/form-data; boundary=TEST");
//    client.println();
//    client.print(head);
////      // Your Domain name with URL path or IP address with path
////      http.begin(serverName.c_str());
//
//      // Specify content-type header
////      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
////      // Data to send with HTTP POST
////      String httpRequestData = "abcd";           
////      // Send HTTP POST request
////      int httpResponseCode = http.POST(httpRequestData);
//      
////       If you need an HTTP request with a content type: application/json, use the following:
////      http.addHeader("Content-Type", "application/json");
////      int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
////
////      // If you need an HTTP request with a content type: text/plain
////      //http.addHeader("Content-Type", "text/plain");
////      //int httpResponseCode = http.POST("Hello, World!");
////     
////      Serial.print("HTTP Response code: ");
////      Serial.println(httpResponseCode);
////      String payload = http.getString();
////      Serial.println(payload);  
////      // Free resources
////      http.end();
//
//
//
//
//
//
////      HTTPClient http;
////
//////      String serverPath = serverName + "?temperature=24.37";
////      
////      // Your Domain name with URL path or IP address with path
////      http.begin(serverName.c_str());
////      
////      // Send HTTP GET request
////      int httpResponseCode = http.GET();
////      
////      if (httpResponseCode>0) {
////        Serial.print("HTTP Response code: ");
////        Serial.println(httpResponseCode);
////        String payload = http.getString();
////        Serial.println(payload);
////      }
////      else {
////        Serial.print("Error code: ");
////        Serial.println(httpResponseCode);
////      }
////      // Free resources
////      http.end();
//    }
//    else {
//      Serial.println("WiFi Disconnected");
//    }
//    lastTime = millis();
//  }
//}
