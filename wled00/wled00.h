#ifndef WLED00_H
#define WLED00_H
/*
 * Main sketch, global variable declarations
 */
/*
 * @title WLED project sketch
 * @version 0.8.4
 * @author Christian Schwinne
 */

#include "Arduino.h"
#include "dclarations.h"
#include "config.h"

//ESP8266-01 (blue) got too little storage space to work with all features of WLED. To use it, you must use ESP8266 Arduino Core v2.4.2 and the setting 512K(No SPIFFS).

//ESP8266-01 (black) has 1MB flash and can thus fit the whole program. Use 1M(64K SPIFFS).
//Uncomment some of the following lines to disable features to compile for ESP8266-01 (max flash size 434kB):

//You are required to disable over-the-air updates:
//#define WLED_DISABLE_OTA

//You need to choose 1-2 of these features to disable:
#define WLED_DISABLE_ALEXA
#define WLED_DISABLE_BLYNK
//#define WLED_DISABLE_CRONIXIE
#define WLED_DISABLE_HUESYNC
#define WLED_DISABLE_INFRARED //there is no pin left for this on ESP8266-01
//#define WLED_DISABLE_MOBILE_UI

//#define WLED_DISABLE_FILESYSTEM    //SPIFFS is not used by any WLED feature yet
#define WLED_ENABLE_FS_SERVING //Enable sending html file from SPIFFS before serving progmem version
#define WLED_ENABLE_FS_EDITOR  //enable /edit page for editing SPIFFS content. Will also be disabled with OTA lock

//to toggle usb serial debug (un)comment the following line
#define WLED_DEBUG

//library inclusions
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#ifndef WLED_DISABLE_OTA
#include <ArduinoOTA.h>
#endif
#include <SPIFFSEditor.h>
#include "src/dependencies/time/Time.h"
#include "src/dependencies/time/TimeLib.h"
#include "src/dependencies/timezone/Timezone.h"
#ifndef WLED_DISABLE_ALEXA
#define ESPALEXA_ASYNC
#define ESPALEXA_NO_SUBPAGE
#define ESPALEXA_MAXDEVICES 1
#include "src/dependencies/espalexa/Espalexa.h"
#endif
#ifndef WLED_DISABLE_BLYNK
#include "src/dependencies/blynk/BlynkSimpleEsp.h"
#endif
#include "src/dependencies/e131/E131.h"
#include "src/dependencies/async-mqtt-client/AsyncMqttClient.h"
#include "src/dependencies/json/AsyncJson-v6.h"
#include "src/dependencies/json/ArduinoJson-v6.h"
#include "html_classic.h"
#include "html_mobile.h"
#include "html_settings.h"
#include "html_other.h"
#include "WS2812FX.h"
#include "ir_codes.h"

#if IR_PIN < 0
#ifndef WLED_DISABLE_INFRARED
#define WLED_DISABLE_INFRARED
#endif
#endif

#define EEPSIZE 2560

#ifdef ARDUINO_ARCH_ESP32
/*#ifndef WLED_DISABLE_INFRARED
  #include <IRremote.h>
 #endif*/
//there are issues with ESP32 infrared, so it is disabled for now
#else
#ifndef WLED_DISABLE_INFRARED
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#endif
#endif

void setAllLeds();
void wledInit();
void handleSerial();
void handleNotifications();
void handleTransistions();
void userLoop();
void handleAlexa();
void handleTransitions();
void handleIO();
void handleIR();
void handleNetworkTime();
void handleOverlays();
void handleNightlight();
void handleHue();
void handleBlynk();
void setCountdown();
void setCronixie();
bool handleSet(AsyncWebServerRequest *request, const String &req);
void getThemeColors(char o[][9]);
void getTimeString(char *out);
void loadMacro(byte index, char *m);
void colorHStoRGB(uint16_t hue, byte sat, byte *rgb);
void colorUpdated(int callMode);
void initBlynk(const char *auth);
void reconnectHue();
void saveMacro(byte index, String mc, bool sing);
void clearEEPROM();
void saveSettingsToEEPROM();
void alexaInit();
char *XML_response(AsyncWebServerRequest *request, bool includeTheme, char *dest);
void colorFromDecOrHexString(byte *rgb, char *in);
void applyMacro(byte index);
void toggleOnOff();
void savePreset(byte index);
bool applyPreset(byte index, bool loadBri, bool loadCol, bool loadFX);
void arlsLock(uint32_t timeoutMs);
void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w);
void loadSettingsFromEEPROM(bool first);
void beginStrip();
void userBeginPreConnection();
void initCon();
void prepareIds();
void initServer();
void initE131();
void userBegin();
void handleButton();
void colorRGBtoRGBW(byte *rgb);
void notify(byte callMode, bool followUp);
void updateBlynk();
void publishMqtt();
void updateBlynk();
void initAP();
void _setRandomColor(bool _sec, bool fromButton);
void updateLocalTime();
void checkTimers();
void _overlayAnalogClock();
void _overlayNixieClock();
void _overlayCronixie();
bool checkCountdown();
void _overlayAnalogCountdown();
void _overlayNixieCountdown();
void sendHADiscoveryMQTT();
void serveSettings(AsyncWebServerRequest *request);
bool handleFileRead(AsyncWebServerRequest *request, String path);
void serveIndex(AsyncWebServerRequest *request);
void serveSettings(AsyncWebServerRequest *request);
void handleSettingsSet(AsyncWebServerRequest *request, byte subPage);
void serveJson(AsyncWebServerRequest *request);
bool deserializeState(JsonObject root);
void getCSSColors();
String msgProcessor(const String &var);
void serveIndexOrWelcome(AsyncWebServerRequest *request);
bool handleFileRead(AsyncWebServerRequest *, String path);
bool checkClientIsMobile(String useragent);
void getSettingsJS(byte subPage, char *dest);
bool initMqtt();
//version code in format yymmddb (b = daily build)
#define VERSION 1906201
extern char versionString[];

//AP and OTA default passwords (for maximum change them!)
extern char apPass[65];
extern char otaPass[33];

//Hardware CONFIG (only changeble HERE, not at runtime)
//LED strip pin, button pin and IR pin changeable in NpbWrapper.h!

extern byte auxDefaultState;   //0: input 1: high 2: low
extern byte auxTriggeredState; //0: input 1: high 2: low
extern char ntpServerName[];   //NTP server to use

extern char clientSSID[33];
extern char clientPass[65];
extern char cmDNS[33];          //mDNS address (placeholder, will be replaced by wledXXXXXXXXXXXX.local)
extern char apSSID[33];         //AP off by default (unless setup)
extern byte apChannel;          //2.4GHz WiFi AP channel (1-13)
extern byte apHide;             //hidden AP SSID
extern byte apWaitTimeSecs;     //time to wait for connection before opening AP
extern bool recoveryAPDisabled; //never open AP (not recommended)
extern IPAddress staticIP;      //static IP of ESP
extern IPAddress staticGateway; //gateway (router) IP
extern IPAddress staticSubnet;  //most common subnet in home networks
extern uint16_t ledCount;       //overcurrent prevented by ABL
extern bool useRGBW;            //SK6812 strips can contain an extra White channel
extern bool autoRGBtoRGBW;      //if RGBW enabled, calculate White channel from RGB
extern bool turnOnAtBoot;       //turn on LEDs at power-up
extern byte bootPreset;         //save preset to load after power-up
extern byte colS[];             //default RGB(W) color
extern byte colSecS[];          //default RGB(W) secondary color
extern byte briS;               //default brightness
extern byte effectDefault;
extern byte effectSpeedDefault;
extern byte effectIntensityDefault; //intensity is supported on some effects as an additional parameter (e.g. for blink you can change the duty cycle)
extern byte effectPaletteDefault;   //palette is supported on the FastLED effects, otherwise it has no effect
extern byte nightlightTargetBri;    //brightness after nightlight is over
extern byte nightlightDelayMins;
extern bool nightlightFade;        //if enabled, light will gradually dim towards the target bri. Otherwise, it will instantly set after delay over
extern bool fadeTransition;        //enable crossfading color transition
extern bool enableSecTransition;   //also enable transition for secondary color
extern uint16_t transitionDelay;   //default crossfade duration in ms//bool strip.reverseMode  = false;            //flip entire LED strip (reverses all effect directions) --> edit in WS2812FX.h
extern bool skipFirstLed;          //ignore first LED in strip (useful if you need the LED as signal repeater)
extern byte briMultiplier;         //% of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)
extern char serverDescription[33]; //Name of module
extern byte currentTheme;          //UI theme index for settings and classic UI
extern byte uiConfiguration;       //0: automatic (depends on user-agent) 1: classic UI 2: mobile UI
extern bool useHSB;                //classic UI: use HSB sliders instead of RGB by default
extern char cssFont[33];           //font to use in classic UI
extern bool useHSBDefault;
extern bool buttonEnabled;
extern bool irEnabled;                     //Infrared receiver
extern uint16_t udpPort;                   //WLED notifier default port
extern uint16_t udpRgbPort;                //Hyperion port
extern bool receiveNotificationBrightness; //apply brightness from incoming notifications
extern bool receiveNotificationColor;      //apply color
extern bool receiveNotificationEffects;    //apply effects setup
extern bool notifyDirect;                  //send notification if change via UI or HTTP API
extern bool notifyButton;                  //send if updated by button or infrared remote
extern bool notifyAlexa;                   //send notification if updated via Alexa
extern bool notifyMacro;                   //send notification for macro
extern bool notifyHue;                     //send notification if Hue light changes
extern bool notifyTwice;                   //notifications use UDP: enable if devices don't sync reliably
extern bool alexaEnabled;                  //enable device discovery by Amazon Echo
extern char alexaInvocationName[33];       //speech control name of device. Choose something voice-to-text can understand
extern char blynkApiKey[36];               //Auth token for Blynk server. If empty, no connection will be made
extern uint16_t realtimeTimeoutMs;         //ms timeout of realtime mode before returning to normal mode
extern int arlsOffset;                     //realtime LED offset
extern bool receiveDirect;                 //receive UDP realtime
extern bool arlsDisableGammaCorrection;    //activate if gamma correction is handled by the source
extern bool arlsForceMaxBri;               //enable to force max brightness if source has very dark colors that would be black
extern bool e131Enabled;                   //settings for E1.31 (sACN) protocol
extern uint16_t e131Universe;
extern bool e131Multicast;
extern char mqttDeviceTopic[33];   //main MQTT topic (individual per device, default is wled/mac)
extern char mqttGroupTopic[33];    //second MQTT topic (for example to group devices)
extern char mqttServer[33];        //both domains and IPs should work (no SSL)
extern bool huePollingEnabled;     //poll hue bridge for light state
extern uint16_t huePollIntervalMs; //low values (< 1sec) may cause lag but offer quicker response
extern char hueApiKey[47];         //key token will be obtained from bridge
extern byte huePollLightId;        //ID of hue lamp to sync to. Find the ID in the hue app ("about" section)
extern IPAddress hueIP;            //IP address of the bridge
extern bool hueApplyOnOff;
extern bool hueApplyBri;
extern bool hueApplyColor;
extern bool ntpEnabled;                    //get internet time. Only required if you use clock overlays or time-activated macros
extern bool useAMPM;                       //12h/24h clock format
extern byte currentTimezone;               //Timezone ID. Refer to timezones array in wled10_ntp.ino
extern int utcOffsetSecs;                  //Seconds to offset from UTC before timzone calculation
extern byte overlayDefault;                //0: no overlay 1: analog clock 2: single-digit clocl 3: cronixie
extern byte overlayMin, overlayMax;        //boundaries of overlay mode
extern byte analogClock12pixel;            //The pixel in your strip where "midnight" would be
extern bool analogClockSecondsTrail;       //Display seconds as trail of LEDs instead of a single pixel
extern bool analogClock5MinuteMarks;       //Light pixels at every 5-minute position
extern char cronixieDisplay[7];            //Cronixie Display mask. See wled13_cronixie.ino
extern bool cronixieBacklight;             //Allow digits to be back-illuminated
extern bool countdownMode;                 //Clock will count down towards date
extern byte countdownYear, countdownMonth; //Countdown target date, year is last two digits
extern byte countdownDay, countdownHour;
extern byte countdownMin, countdownSec;
extern byte macroBoot; //macro loaded after startup
extern byte macroNl;   //after nightlight delay over
extern byte macroCountdown;
extern byte macroAlexaOn, macroAlexaOff;
extern byte macroButton, macroLongPress, macroDoublePress;
extern bool otaLock;     //prevents OTA firmware updates without password. ALWAYS enable if system exposed to any public networks
extern bool wifiLock;    //prevents access to WiFi settings when OTA lock is enabled
extern bool aOtaEnabled; //ArduinoOTA allows easy updates directly from the IDE. Careful, it does not auto-disable when OTA lock is on
extern uint16_t userVar0, userVar1;
extern byte col[];    //target RGB(W) color
extern byte colOld[]; //color before transition
extern byte colT[];   //current color
extern byte colIT[];  //color that was last sent to LEDs
extern byte colSec[];
extern byte colSecT[];
extern byte colSecOld[];
extern byte colSecIT[];
extern byte lastRandomIndex; //used to save last random color so the new one is not the same
extern bool transitionActive;
extern uint16_t transitionDelayDefault;
extern uint16_t transitionDelayTemp;
extern unsigned long transitionStartTime;
extern float tperLast; //crossfade transition progress, 0.0f - 1.0f
extern bool nightlightActive;
extern bool nightlightActiveOld;
extern uint32_t nightlightDelayMs;
extern uint8_t nightlightDelayMinsDefault;
extern unsigned long nightlightStartTime;
extern byte briNlT; //current nightlight brightness
extern unsigned long lastOnTime;
extern bool offMode;
extern byte bri;
extern byte briOld;
extern byte briT;
extern byte briIT;
extern byte briLast;
extern bool buttonPressedBefore;
extern unsigned long buttonPressedTime;
extern unsigned long buttonWaitTime;
extern bool notifyDirectDefault;
extern bool receiveNotifications;
extern unsigned long notificationSentTime;
extern byte notificationSentCallMode;
extern bool notificationTwoRequired;
extern byte effectCurrent;
extern byte effectSpeed;
extern byte effectIntensity;
extern byte effectPalette;
extern bool onlyAP; //only Access Point active, no connection to home network
extern bool udpConnected, udpRgbConnected;
extern char cssCol[6][9];
extern bool showWelcomePage;
extern char hueError[25];
extern float hueXLast, hueYLast;
extern uint16_t hueHueLast, hueCtLast;
extern byte hueSatLast, hueBriLast;
extern unsigned long hueLastRequestSent;
extern bool hueAuthRequired;
extern bool hueReceived;
extern bool hueStoreAllowed, hueNewKey;
extern byte overlayCurrent;
extern byte overlaySpeed;
extern unsigned long overlayRefreshMs;
extern unsigned long overlayRefreshedTime;
extern int overlayArr[6];
extern uint16_t overlayDur[6];
extern uint16_t overlayPauseDur[6];
extern int nixieClockI;
extern bool nixiePause;
extern byte dP[];
extern bool cronixieInit;
extern unsigned long countdownTime;
extern bool countdownOverTriggered;
extern byte lastTimerMinute;
extern byte timerHours[];
extern byte timerMinutes[];
extern byte timerMacro[];
extern byte timerWeekday[]; //weekdays to activate on
extern bool blynkEnabled;
extern bool presetCyclingEnabled;
extern byte presetCycleMin, presetCycleMax;
extern uint16_t presetCycleTime;
extern unsigned long presetCycledTime;
extern byte presetCycCurr;
extern bool presetApplyBri, presetApplyCol, presetApplyFx;
extern bool saveCurrPresetCycConf;
extern bool realtimeActive;
extern IPAddress realtimeIP;
extern unsigned long realtimeTimeout;
extern long nextMQTTReconnectAttempt;
extern long lastInterfaceUpdate;
extern byte interfaceUpdateCallMode;
#if AUXPIN >= 0
extern byte auxTime;
extern unsigned long auxStartTime;
extern bool auxActive, auxActiveBefore;
#endif
extern String escapedMac;
#ifndef WLED_DISABLE_ALEXA
extern Espalexa espalexa;
extern EspalexaDevice *espalexaDevice;
#endif
extern DNSServer dnsServer;
extern bool dnsActive;
extern bool ntpConnected;
extern time_t local;
extern unsigned long ntpLastSyncTime;
extern unsigned long ntpPacketSentTime;
extern IPAddress ntpServerIP;
extern unsigned int ntpLocalPort;
#define NTP_PACKET_SIZE 48
#define OMAX 2048
extern char *obuf;
extern uint16_t olen;

extern String messageHead, messageSub;
extern byte optionType;
extern bool doReboot; //flag to initiate reboot from async handlers
extern AsyncWebServer server;
extern AsyncClient *hueClient;
extern AsyncMqttClient *mqtt;
extern WiFiUDP notifierUdp, rgbUdp;
extern WiFiUDP ntpUdp;
extern E131 *e131;
extern WS2812FX strip;
#ifdef WLED_DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x) Serial.printf(x)
extern unsigned long debugTime;
extern int lastWifiState;
extern unsigned long wifiStateChangedTime;
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x)
#endif
#ifndef WLED_DISABLE_FILESYSTEM
#include <FS.h>
#ifdef ARDUINO_ARCH_ESP32
#include "SPIFFS.h"
#endif
#include "SPIFFSEditor.h"
#endif
void serveMessage(AsyncWebServerRequest *, uint16_t, String, String, byte);
void reset();

//append new c string to temp buffer efficiently
bool oappend(char *txt);

//append new number to temp buffer efficiently
bool oappendi(int i);

//boot starts here
void setup();

//main program loop
void loop();
#endif