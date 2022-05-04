#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <fstream>
#include <cstring>
#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false
#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif
const char* ssid = "BlueBery";
const char* password = "1234567890";

//Your Domain name with URL path or IP address with path
String serverName = "https://9eb2-14-243-165-107.ap.ngrok.io/base/send";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
const char* post_host = "5c37-2405-4802-605b-2740-cc4f-d93-acdb-243d.ap.ngrok.io";
const int post_port = 443; 
String url = "/base/send/";
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
  if (!client.connect(post_host,post_port))
  { // check connection to host if untrue internet connection could be down
    Serial.println("couldn't connect to host");
  }

  HTTPClient http;
  const char* serverName = "https://5c37-2405-4802-605b-2740-cc4f-d93-acdb-243d.ap.ngrok.io/base/send";
  http.begin(client, serverName);
  Serial.println("connected");
   if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }
//  char *fname = "recording.wav";
  File fp = SPIFFS.open("/recording.wav","r");
  size_t file_size = fp.size();
  String fileName = fp.name();
  if(fp){
    String boundary = "TestAudio";
    String contentType = "audio/wav";
    String portString = String(post_port);
    String hostString = String(post_host);

    String postHeader = "POST " + url + " HTTP/1.1\r\n";
    postHeader += "Host: " + hostString + ":" + portString + "\r\n";
    postHeader += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
    postHeader += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
    postHeader += "Accept-Encoding: gzip,deflate\r\n";
    postHeader += "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
    postHeader += "User-Agent: Arduino/Solar-Server\r\n";
    postHeader += "Keep-Alive: 300\r\n";
    postHeader += "Connection: keep-alive\r\n";
    postHeader += "Accept-Language: en-us\r\n";
    
    String keyHeader = "--" + boundary + "\r\n";
    keyHeader += "Content-Disposition: form-data; name=\"key\"\r\n\r\n";
    keyHeader += "${filename}\r\n";
    
    String requestHead = "--" + boundary + "\r\n";
    requestHead += "Content-Disposition: form-data; name=\"file\"; filename=\"" + fileName + "\"\r\n";
    requestHead += "Content-Type: " + contentType + "\r\n\r\n";

    String tail = "\r\n--" + boundary + "--\r\n\r\n";

    int contentLength = keyHeader.length() + requestHead.length() + fp.size() + tail.length();
    postHeader += "Content-Length: " + String(contentLength, DEC) + "\n\n";

    // send post header
    char charBuf0[postHeader.length() + 1];
    postHeader.toCharArray(charBuf0, postHeader.length() + 1);
    client.write(charBuf0);
    Serial.print(charBuf0);

    // send key header
    char charBufKey[keyHeader.length() + 1];
    keyHeader.toCharArray(charBufKey, keyHeader.length() + 1);
    client.write(charBufKey);
    Serial.print(charBufKey);

    // send request buffer
    char charBuf1[requestHead.length() + 1];
    requestHead.toCharArray(charBuf1, requestHead.length() + 1);
    client.write(charBuf1);
    Serial.print(charBuf1);

    // create buffer
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

    // send tail
    char charBuf3[tail.length() + 1];
    tail.toCharArray(charBuf3, tail.length() + 1);
    client.write(charBuf3);
    Serial.print(charBuf3);

    // Read all the lines of the reply from server and print them to Serial
    Serial.println("request sent");
    String responseHeaders = "";

    while (client.connected()) {
      Serial.println("while client connected");
      String line = client.readStringUntil('\n');
      Serial.println(line);
      responseHeaders += line;
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    Serial.println("DONE");
//    String line = client.readStringUntil('\n');

    Serial.println("reply was:");
    Serial.println("==========");
//    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");

    // close the file:
//    fp.close();
    Serial.println(responseHeaders);

  }else{Serial.println("NOTFOUND");}

Serial.println("done");
  http.end(); // Close connection
  delay(10 * 1000);
}
