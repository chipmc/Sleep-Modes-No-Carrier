/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/chipmc/Documents/Maker/Particle/Utilities/Sleep-Modes-No-Carrier/src/Sleep-Modes-No-Carrier.ino"
/*
* Project Sleep Modes - Project for Testing Sleep 2.0
* Description: Tests the 3rd Generation Device without Carrier Sleep 2.0 Modes
* Author: Charles McClelland
* Date: Started 11-17-2019 
* 
* Implements the following Tests:
* Test 0 - STOP mode sleep with network keep alive
* Test 1 - STOP mode sleep without network keep alive
* Test 2 - Hibernate mode sleep with user button wake
* Test 3 - Enable Pin Sleep with user button wake
* 
* v0.10 - Initial Release
* v0.11 - Simplified and added watchdog monitoring
* v1.00 - All Sleep modes working 
* v1.01 - Moved to longer numbers for longer sleep periods
* v1.02 - Reduced the numbers of tests to just the ones we will actually use - put in a test for dutation less than 15 minutes.
* v1.03 - Added Particle function for duration - took out verbiage about pass and fail
* v2.00 - Changed over to Sleep 2.0 which is part of the deviceOS@1.5.0 release
* v2.50 - Changed approach to support testing with the Coulomb Counter 
*/

void setup();
void loop();
void loadSleepConfig(int testCase);
bool meterParticlePublish(void);
int setDuration(String command);
int setTestNumber(String command);
#line 23 "/Users/chipmc/Documents/Maker/Particle/Utilities/Sleep-Modes-No-Carrier/src/Sleep-Modes-No-Carrier.ino"
char currentPointRelease[5] ="2.50";

// Included Libraries
#include "3rdGenDevicePinoutdoc.h"                              // Documents pinout

// Prototypes and System Mode calls
FuelGauge batteryMonitor;                                       // Prototype for the fuel gauge (included in Particle core library)
SystemSleepConfiguration config;                                // Config for sleep

#define testNumberAddr  0x00                                    // Where we store the current test number (1 byte)
#define testStartTimeAddr 0x05                                  // Where we record the test start time (since some tests cause a reboot) (4 bytes)
#define testDurationSecondsAddr 0x0A                            // How long is the test inseconds (4 bytes)

// Pin Constants for Boron
const int blueLED  = D7;                                         // This LED is on the Electron itself
const int userSwitch = D4;                                       // User switch with a pull-up resistor

// Program Variables                      
bool clearedToTest = false;
int testNumber;
int testDurationSeconds;

// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(userSwitch,INPUT);                                      // Button for user input
  pinMode(blueLED, OUTPUT);                                       // declare the Blue LED Pin as an output
  digitalWrite(blueLED,HIGH);                                     // Awake so turn on the LED

  Particle.variable("Release",currentPointRelease);

  Particle.function("Duration-Sec",setDuration);
  Particle.function("Select-Test",setTestNumber);

  testNumber = EEPROM.read(testNumberAddr);                       // Load values from EEPROM and bounds check (1st run will have random values)
  if (testNumber < 0 || testNumber > 3) testNumber = 0;
  
  waitUntil(meterParticlePublish);
  if (testNumber >= 2) Particle.publish("Status", "Continuing after reset",PRIVATE);
  else Particle.publish("Status", "Beginning Test Run",PRIVATE);
}

void loop() {
  if (clearedToTest) {
    clearedToTest = false;
    loadSleepConfig(testNumber);
    digitalWrite(blueLED,LOW);
    System.sleep(config);
    digitalWrite(blueLED,HIGH);
    waitUntil(meterParticlePublish);
    Particle.publish("Status","Test Complete - Select Next Test",PRIVATE);
  }
}

void loadSleepConfig(int testCase) {
    char preSleepMessage[64];
    switch (testCase)
    {
      case 0:
        snprintf(preSleepMessage, sizeof(preSleepMessage),"Test 0: STOP mode - Network Keep Alive for %i seconds",testDurationSeconds);
        waitUntil(meterParticlePublish);
        Particle.publish("Status",preSleepMessage,PRIVATE);
        delay(5000);
        config.mode(SystemSleepMode::STOP)
          .duration(testDurationSeconds * 1000)
          .network(NETWORK_INTERFACE_CELLULAR);
        break;
      case 1:
        snprintf(preSleepMessage, sizeof(preSleepMessage),"Test 1: STOP mode sleep for %i seconds",testDurationSeconds);
        waitUntil(meterParticlePublish);
        Particle.publish("Status",preSleepMessage,PRIVATE);
        delay(5000);
        config.mode(SystemSleepMode::STOP)
          .duration(testDurationSeconds * 1000);
        break;
      case 2:
        snprintf(preSleepMessage, sizeof(preSleepMessage),"Test 2: HIBERNATE mode sleep for %i seconds",testDurationSeconds);
        waitUntil(meterParticlePublish);
        Particle.publish("Status",preSleepMessage,PRIVATE);
        delay(5000);
        config.mode(SystemSleepMode::HIBERNATE)
          .gpio(userSwitch,FALLING)
          .duration(testDurationSeconds * 1000);
        break;
      case 3:
        snprintf(preSleepMessage, sizeof(preSleepMessage),"Test 3: ENABLE mode - Pull Enable Pin LOW");
        waitUntil(meterParticlePublish);
        Particle.publish("Status",preSleepMessage,PRIVATE);
        delay(5000);
        config.mode(SystemSleepMode::STOP)
          .gpio(userSwitch,FALLING);
        break;
    }
    if (testNumber == 0) {

    }

}

bool meterParticlePublish(void) {                                                  // Enforces Particle's limit on 1 publish a second
  static unsigned long lastPublish=0;                                              // Initialize and store value here
  if(millis() - lastPublish >= 1000) {                                             // Particle rate limits at 1 publish per second
    lastPublish = millis();
    return 1;
  }
  else return 0;
}

int setDuration(String command)
{
  char * pEND;
  char data[256];
  int tempTime = strtol(command,&pEND,10);                       // Looks for the first integer and interprets it
  if ((tempTime < 0) || (tempTime > 3600)) return 0;             // Make sure it falls in a valid range or send a "fail" result
  testDurationSeconds = (unsigned long)tempTime;
  EEPROM.put(testDurationSecondsAddr,testDurationSeconds);    // Write to EEPROM
  snprintf(data, sizeof(data), "Test duration set to %i seconds", testDurationSeconds);
  waitUntil(meterParticlePublish);
  Particle.publish("Status",data,PRIVATE);
  delay(1000);
  return 1;
}

int setTestNumber(String command)
{
  char * pEND;
  char data[256];
  int tempTest = strtol(command,&pEND,10);                       // Looks for the first integer and interprets it
  if ((tempTest < 0) || (tempTest > 3)) return 0;             // Make sure it falls in a valid range or send a "fail" result
  testNumber = tempTest;
  EEPROM.put(testNumberAddr,testNumber);    // Write to EEPROM
  snprintf(data, sizeof(data), "Test number set to %i", testNumber);
  waitUntil(meterParticlePublish);
  Particle.publish("Status",data,PRIVATE);
  delay(1000);
  clearedToTest = true;
  return 1;
}