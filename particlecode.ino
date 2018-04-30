#include <string>
#include <iostream>
using namespace std;

// this is where the analog port would be identified
int dryermotor;
int dryerbutton;
int washermotor;
int washerbutton;

bool dbuttonnow = FALSE;
bool dbuttonlast = TRUE;
bool wbuttonnow = FALSE;
bool wbuttonlast = TRUE;

// analog motor speed values 
int high = 4095;
int medium = 2047;
int low = 1023;

class washer {
    public:
        int temp; // 0 - Low, 1 - Med, 2 - Hi
        int cycle; // 10 - Short, 15 - Med, 20 - Hi
        int speed; // 1023 - Low, 2047 - Med, 4095 - Hi
        int remain; // Variable to read on java site for how much longer is in cycle
        washer(); // default constructor
        void RunWasher(string x, string y, string z); // input string: temperature, cycle length, cycle speed
        void StopWasher(string x); 
};
washer::washer() {
    temp = 0;
    cycle = 0;
    speed = 0;
    remain = 0;
}
void washer::RunWasher(string x, string y, string z) {
    if(x == "1") {
        temp = 1; } // Cold
    else if(x == "2") {
        temp = 2; } // Warm
    else if(x == "3") {
        temp = 3; } // Hot
        
    if(y == "1") {
        cycle = 10; 
    } // Short
    else if(y == "2") { 
        cycle = 15; 
    } // Medium
    else if(y == "3") {
        cycle = 20; 
    } // Long
            
    if(z == "1") {
        speed = low; 
    } // Low Speed
    else if(z == "2") { 
        speed = medium; 
    } // Medium Speed
    else if(z == "3") {
        speed = high; 
    } // High Speed

    unsigned long int currenttime = 1000*millis();
    remain = (cycle + currenttime) - currenttime; //time value when cycle is done minus current time

    //Code to turn on LED's for whatever color should go here
    
    analogWrite(washermotor, speed) //turns motor on
    if (remain <= 0) {
        analogWrite(washermotor, speed); //turns it off when cycle is done
    }
    
    return;
}
void washer::StopWasher(string x) {
    if(x == "1") {
        cycle = 0;
        remain = 0; 
    } 
    return;
}

void setup() {
    washer myWasher;
    dryer myDryer;

    pinMode(washerbutton, INPUT_PULLDOWN);
    pinMode(dryerbutton, INPUT_PULLDOWN);
    
    Serial.begin(9600);
    Particle.function("RunWasher", myWasher.RunWasher);
    Particle.function("StopWasher", myWasher.StopWasher);
    Particle.variable("WasherTimeLeft", myWasher.remain);
}

void loop() {
    dbuttonnow = digitalRead(dryerbutton);
    wbuttonnow = digitalRead(washerbutton);
    
    if(dbuttonnow == HIGH && dbuttonlast == LOW) { // Basically if the button is pressed, the dryer stop function should be called with an argument of 1
        myDryer.StopDryer("1");
        dbuttonlast = HIGH; 
    }
    else if (dbuttonnow == LOW) {
        dbuttonlast = LOW; 
    }
    
    if(wbuttonnow == HIGH && wbuttonlast == LOW) { // Same thing but with the washer
        myWasher.StopWasher("1");
        wbuttonlast = HIGH; 
    }
    else if (wbuttonnow == LOW) {
        wbuttonlast = LOW; 
    }
}
