//THIS IS COMPILING SUCCESSFULLY!!!
#include <sstream>
#include <neopixel.h>

#define dryermotor D0
#define dryerbutton D1
#define washermotor D2
#define washerbutton D3

#define PIXEL_PIN D4
#define PIXEL_COUNT 1
#define PIXEL_TYPE WS2811

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

bool dbuttonnow = false;
bool dbuttonlast = true;
bool wbuttonnow = false;
bool wbuttonlast = true;

int high = 4095;
int medium = 2047;
int low = 1023;


int washertemp = 0;
int washercycle= 0;
int washerspeed = 0;
int washerremain = 0;
unsigned long int washercyclestarttime = 0;
String washercyclename = "No Current Cycle.";
bool washeractive = false; 

int RunWasher(String bigkahuna) { //input must be one string in the form of "name speed time temp"
    //streams out input string to individual settings
    String inname;
    int inspeed, incycletime, intemp;
    //new method
    int a = bigkahuna.indexOf(" ");
    inname = bigkahuna.substring(0, a);
    int b = bigkahuna.indexOf(" ", a + 1);
    inspeed = bigkahuna.substring(a + 1, b).toInt();
    int c = bigkahuna.indexOf(" ", b + 1);
    incycletime = bigkahuna.substring(b + 1, c).toInt();
    intemp = bigkahuna.substring(c).toInt();
    
    washercyclename = inname;

    //Interprets degree F values to one of three settings
    if(intemp < 85) {
        washertemp = 1; 
    } // Cold
    else if((intemp >= 85) && (intemp < 110)) {
        washertemp = 2; 
    } // Warm
    else if(intemp >= 110) {
        washertemp = 3;
    } // Hot
    
    //time of cycle is just input time in seconds
    washercycle = incycletime;
            
    //Inputs of 1, 2, or 3 correspond to 3 speed settings
    if(inspeed == 1) {
        washerspeed = low; 
    } // Low Speed
    else if(inspeed == 2) { 
        washerspeed = medium; 
    } // Medium Speed
    else if(inspeed == 3) {
        washerspeed = high; 
    } // High Speed

    //NOW all variables are assigned, to actually run it

    washercyclestarttime = 1000*millis();

    int red = strip.Color(255, 0, 0);
    int yellow = strip.Color(255, 255, 0);
    int blue = strip.Color(0, 0, 255);
    
    if (washertemp == 1) {
        strip.setPixelColor(0, blue);
        strip.show();
    }
    else if (washertemp == 2) {
        strip.setPixelColor(0, yellow);
        strip.show();
    }
    else if (washertemp == 3) {
        strip.setPixelColor(0, red);
        strip.show();
    }
    
    analogWrite(washermotor, washerspeed); //turns motor on
    washeractive = true;

    return 1;
}
int StopWasher(String x) {
    if(x == "1") {
        washercycle = 0;
        washerremain = 0; 
    } 
    return 1;
}


int dryercycle = 0; // 10 - Short, 15 - Med, 20 - Hi
int dryerspeed = 0; // 1023 - Low, 2047 - Med, 4095 - Hi
int dryerremain = 0; // Variable to read on java site for how much longer is in cycle
unsigned long int dryercyclestarttime = 0;
String dryercyclename = "No Current Cycle";
bool dryeractive = false;

int RunDryer(String bigkahuna) { //input must be one string in the form of "name speed time"
    //streams out input string to individual settings
    String inname;
    int inspeed, incycletime;
    
    //new method
    int a = bigkahuna.indexOf(" ");
    inname = bigkahuna.substring(0, a);
    int b = bigkahuna.indexOf(" ", a + 1);
    inspeed = bigkahuna.substring(a + 1, b).toInt();
    incycletime = bigkahuna.substring(b + 1).toInt();
    
    dryercyclename = inname;

    //time of cycle is just input time in seconds
    dryercycle = incycletime;
            
    //Inputs of 1, 2, or 3 correspond to 3 speed settings
    if(inspeed == 1) {
        dryerspeed = low; 
    } // Low Speed
    else if(inspeed == 2) { 
        dryerspeed = medium; 
    } // Medium Speed
    else if(inspeed == 3) {
        dryerspeed = high; 
    } // High Speed

    //NOW all variables are assigned, to actually run it

    dryercyclestarttime = 1000*millis();

    analogWrite(dryermotor, dryerspeed); //turns motor on
    dryeractive = true;

    return 1;
}
int StopDryer(String x) {
    if(x == "1") {
        dryercycle = 0;
        dryerremain = 0; 
    } 
    return 1;
}

void setup() {
    pinMode(washerbutton, INPUT_PULLDOWN);
    pinMode(dryerbutton, INPUT_PULLDOWN);
    
    Serial.begin(9600);
    Particle.function("RunWasher", RunWasher);
    Particle.function("StopWasher", StopWasher);
    Particle.variable("WTimeLeft", washerremain);
    Particle.variable("WasherCycle", washercyclename);
    Particle.function("RunDryer", RunDryer);
    Particle.function("StopDryer", StopDryer);
    Particle.variable("DTimeLeft", dryerremain);
    Particle.variable("DryerCycle", dryercyclename);
}

void loop() {
    unsigned long int currenttime = 1000*millis();
    dbuttonnow = digitalRead(dryerbutton);
    wbuttonnow = digitalRead(washerbutton);

    //if washer is on, watches for when to turn it off without blocking. Has to be in loop to do so. Aslo updates remain
    if (washeractive == true) {
        washerremain = (washercyclestarttime + washercycle - currenttime);
        if (washerremain <= 0) {
            analogWrite(washermotor, 0); //turns it off when cycle is done
            washercyclename = "Cycle Complete!"; //And resets this variable when cycle is done
            //turn led off //and turn the led off
            washeractive = false;
            Particle.publish("WasherDone", "done", 60, PUBLIC); //and publish event for buzzer and text
            //buzzer for 2 seconds
        }
    }
    //same thing but with the dryer
    if (dryeractive == true) {
        dryerremain = (dryercyclestarttime + dryercycle - currenttime);
        if (dryerremain <= 0) {
            analogWrite(dryermotor, 0); //turns it off when cycle is done
            dryercyclename = "Cycle Complete!"; //And resets this variable when cycle is done
            dryeractive = false;
            Particle.publish("DryerDone", "done", 60, PUBLIC);
            //buzzer for 2 seconds
        }
    }
    
    if(dbuttonnow == HIGH && dbuttonlast == LOW) { // Basically if the button is pressed, the dryer stop function should be called with an argument of 1
        StopDryer("1");
        dbuttonlast = HIGH; 
    }
    else if (dbuttonnow == LOW) {
        dbuttonlast = LOW; 
    }
    
    if(wbuttonnow == HIGH && wbuttonlast == LOW) { // Same thing but with the washer
        StopWasher("1");
        wbuttonlast = HIGH; 
    }
    else if (wbuttonnow == LOW) {
        wbuttonlast = LOW; 
    }
}