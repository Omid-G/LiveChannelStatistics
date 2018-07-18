#include <YoutubeApi.h>

/*******************************************************************
 *  An example of bot that echos back any messages received        *
 *                                                                 *
 *  written by Giacarlo Bacchio (Gianbacchio on Github)            *
 *  adapted by Brian Lough                                         *
 *******************************************************************/
// FINAL WITH TOUCH CAPACITIVE BUTTON

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Arduino.h>
#include <TM1637Display.h>

// Replace connection pins depending on what board you're using (Digital Pins)
#define CLK 13
#define DIO 12
#define TOUCHBUTTON 4 // Labeled D5 on board 



TM1637Display display(CLK, DIO);

//------- Replace the following! ------
char ssid[] = "YOUR_NETWORK_ID_HERE";       // your network SSID (name)
char password[] = "YOUR_NETWORK_PASSWORD_HERE";  // your network key
#define API_KEY "YOUR_API_KEY_HERE"  // your google apps API Token
#define CHANNEL_ID "YOUR_CHANNEL_ID_HERE" // makes up the url of channel




WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

int api_mtbs = 60000; //mean time between api requests
long api_lasttime;   //last time api request has been done

long subs = 0;

void setup() {

  Serial.begin(115200);
  pinMode(TOUCHBUTTON, INPUT);          // sets the digital pin 14 as input
  digitalWrite(TOUCHBUTTON, LOW);    // sets the input pin 14 to low

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {

 uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

  if (millis() > api_lasttime + api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }


      Serial.println(digitalRead(TOUCHBUTTON));
      display.showNumberDec(api.channelStats.subscriberCount, false);

      if (digitalRead(TOUCHBUTTON) == HIGH){
        display.setBrightness(3, true);  // Turn off
      }
      else {
        display.setBrightness(3, false);  // Turn off

      }
}
