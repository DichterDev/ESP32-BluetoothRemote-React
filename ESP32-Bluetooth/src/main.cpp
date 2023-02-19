#include <Arduino.h>
#include <IRremote.hpp>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ArduinoJson.h>        // https://arduinojson.org/v6/doc/installation/
#include <SPIFFS.h>
#include "RemoteCodes.h"
#include "PinDefinitions.h" // Define macros for input and output pin etc.

// Web-server
String request;
DynamicJsonDocument doc(200);
AsyncWebServer server(80);

const char* ssid = "UPC4103056";
const char* password = "xBc3ytzwrtMz";

IPAddress local_IP(192, 168, 0, 199);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

unsigned long remoteCode = 0x00;
boolean pending = false;

const int senderPin = 13;

void irSendNECblk( uint32_t code, uint8_t irLedPin, uint8_t freqKhz = 38, bool inverted = false, bool invertedBitOrder = false ) {
	// sends an NEC IR code. No library required. Just copy this function into your sketch.
	// where:
	// code:  the code to send e.g. 0x00FDA857
	// irLedPin: the output pin e.g. 3
	// freqKhz: the carrier frequency in KHz
	// inverted: (default false). Default case : led wired low side so pin HIGH = on. otherwise pin LOW = on.
	// invertedBitOrder (default false). Default case: MSB as used in ver <= 2  of the Ken Shirriff IR library. Otherwise mirrored for V3+

	// sample call: irSendNECblk( 0x00FF1A9B, 3, 38 )  // pin 3, 38kHz carrier, pin HIGH puts led on, original (non mirrored) hex code format.
	// Note: blocks the loop() for approx 70 ms .
	// Author: 6v6gt 04.05.2021 https://forum.arduino.cc/t/extreme-lightweight-ir-sender-for-nec-protocol/858910

	uint16_t NecBurstUnit = ( freqKhz * 562L) / 1000L ; // IR carrier waves for 1 NEC mark or 1 NEC space( bit 0).
	uint8_t carrierPeriodUs = (int16_t)1000 / freqKhz ;
	uint8_t * codeSplit ;
	codeSplit = ( uint8_t* ) &code ;  // treat uint32_t as byte array ;

	auto xmit = [irLedPin,  inverted, carrierPeriodUs ]( bool isOn, uint16_t waves ) {
		// send carrier burst. on = IR mark, otherwise IR space (filler)
		uint32_t burstStartUs = micros() ;

		for ( uint16_t i = 0 ; i < waves ; i ++ ) {
			digitalWrite( irLedPin , isOn != inverted ? HIGH : LOW ) ;  // Xor. carrier mark or filler
			delayMicroseconds( carrierPeriodUs / 3 ) ;  // 33% duty cycle
			digitalWrite( irLedPin, inverted ? HIGH : LOW ) ;   // carrier space
			while (  micros() - ( burstStartUs + ( i * carrierPeriodUs ) ) < carrierPeriodUs )  ;  // idle until end of wave(i)
		} ;
	} ;

	pinMode( irLedPin, OUTPUT ) ;
	xmit( true , NecBurstUnit * 16 ) ;   // header mark 9000 us
	xmit( false , NecBurstUnit * 8 ) ;   // header space 4500 us
	for ( uint8_t i = 0 ; i < 32 ; i ++ ) {  // 32 bits
		xmit( true , NecBurstUnit ) ; // NEC mark
		uint8_t codeByte = !invertedBitOrder ? 3 - i / 8 /*MSB*/ : i/8 /*LSB*/ ;
		uint8_t codeBit = !invertedBitOrder ? 7 - i % 8 /*MSB*/ : i % 8 /*LSB*/ ;
		xmit( false, bitRead( *(codeSplit + codeByte) , codeBit ) == 1   ? NecBurstUnit * 3 : NecBurstUnit ) ;  // NEC space(0) 562us or NEC space(1) ~1675us
	}
	xmit( true , NecBurstUnit ) ;  // terminator
} // end of irSendNECblk()

void sendSignal(uint8_t id) {
  Serial.println(id);
  switch (id)
  {
    case 1:
      irSendNECblk(RemoteCodes.btnTogglePower, senderPin, 38);
      break;
    case 2:
      irSendNECblk(RemoteCodes.btnToggleMute, senderPin, 38);
      break;
    case 3:
      irSendNECblk(RemoteCodes.btnDecreaseVolume, senderPin, 38);
      delay(5);
      irSendNECblk(RemoteCodes.btnDecreaseVolume, senderPin, 38);
      break;
    case 4:
      irSendNECblk(RemoteCodes.btnIncreaseVolume, senderPin, 38);
      delay(5);
      irSendNECblk(RemoteCodes.btnIncreaseVolume, senderPin, 38);
      break;
    case 5:
      irSendNECblk(RemoteCodes.btnEnableLine1, senderPin, 38);
      break;
    case 6:
      irSendNECblk(RemoteCodes.btnEnableLine2, senderPin, 38);
      break;
    case 7:
      irSendNECblk(RemoteCodes.btnEnableOPT, senderPin, 38);
      break;
    case 8:
      irSendNECblk(RemoteCodes.btnEnableCOX, senderPin, 38);
      break;
    case 9:
      irSendNECblk(RemoteCodes.btnEnableBluetooth, senderPin, 38);
      break;
    case 10:
      irSendNECblk(RemoteCodes.btnTogglePlay, senderPin, 38);
      break;
    case 11:
      irSendNECblk(RemoteCodes.btnPrevious, senderPin, 38);
      break;
    case 12:
      irSendNECblk(RemoteCodes.btnNext, senderPin, 38);
      break;
    default:
      break;
  }
}

void onRequest(AsyncWebServerRequest *request) {
    // dummy callback function for handling params, etc.
}

void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
    // dummy callback function signature, not in used in our code
}


void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DeserializationError error = deserializeJson(doc, (char*)data);

  if (error) {
    request -> send(404, "text/plain", "Error");
    Serial.println("Error in POST");
    return;
  }

  const uint32_t command = doc["cmd"];
  sendSignal(command);
  request -> send(200, "text/plain", "Ok");
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
    
  // Initialize SPIFFS (SPI Flash File System)
  SPIFFS.begin(true);

  // serving static content for GET requests on '/' from SPIFFS directory '/'
  server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html")
        .setCacheControl("max-age=86400");

  // Which functions to use for processing the /api requests
  server.on("/api", HTTP_POST, onRequest, onFileUpload, onBody);

  // What to do when page does not exist
  server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404, "text/plain", "Page Not Found");
  });

  // Start the server
  server.begin();

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
}



void loop() {
  
}