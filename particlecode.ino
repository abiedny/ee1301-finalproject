#include <string>
#include <iostream>
#include <sstream>
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
        unsigned long int cyclestarttime;
        string cyclename;
        bool active;
        washer(); // default constructor
        void RunWasher(string bigkahuna); // input string: temperature, cycle length, cycle speed
        void StopWasher(string x); 
};
washer::washer() {
    temp = 0;
    cycle = 0;
    speed = 0;
    remain = 0;
    cyclename = "No current cycle";
    active = false;
    cyclestarttime = 0;
}
void washer::RunWasher(string bigkahuna) { //input must be one string in the form of "name speed time temp"
    //streams out input string to individual settings
    string inname;
    int inspeed, incycletime, intemp;
    sstream(bigkahuna) >> inname >> inspeed >> incycletime >> intemp;
    cyclename = inname;

    //Interprets degree F values to one of three settings
    if(intemp < 85) {
        temp = 1; 
    } // Cold
    else if((intemp >= 85) && (intemp < 110)) {
        temp = 2; 
    } // Warm
    else if(intemp >= 110) {
        temp = 3;
    } // Hot
    
    //time of cycle is just input time in seconds
    cycle = incycletime;
            
    //Inputs of 1, 2, or 3 correspond to 3 speed settings
    if(inspeed == 1) {
        speed = low; 
    } // Low Speed
    else if(inspeed == 2) { 
        speed = medium; 
    } // Medium Speed
    else if(inspeed == 3) {
        speed = high; 
    } // High Speed

    //NOW all variables are assigned, to actually run it

    cyclestarttime = 1000*millis();

    if (temp == 1) {
        //write blue to the led
    }
    else if (temp == 2) {
        //write yellow to the led
    }
    else if (temp == 3) {
        //write red to the led
    }
    
    analogWrite(washermotor, speed) //turns motor on
    active = true;

    return;
}
void washer::StopWasher(string x) {
    if(x == "1") {
        cycle = 0;
        remain = 0; 
    } 
    return;
}

class dryer {
    public:
        int cycle; // 10 - Short, 15 - Med, 20 - Hi
        int speed; // 1023 - Low, 2047 - Med, 4095 - Hi
        int remain; // Variable to read on java site for how much longer is in cycle
        unsigned long int cyclestarttime;
        string cyclename;
        bool active;
        dryer(); // default constructor
        void RunDryer(string bigkahuna); // input string: temperature, cycle length, cycle speed
        void StopDryer(string x); 
};
dryer::dryer() {
    cycle = 0;
    speed = 0;
    remain = 0;
    cyclename = "No current cycle";
    active = false;
    cyclestarttime = 0;
}
void dryer::RunDryer(string bigkahuna) { //input must be one string in the form of "name speed time"
    //streams out input string to individual settings
    string inname;
    int inspeed, incycletime, intemp;
    sstream(bigkahuna) >> inname >> inspeed >> incycletime;
    cyclename = inname;

    //time of cycle is just input time in seconds
    cycle = incycletime;
            
    //Inputs of 1, 2, or 3 correspond to 3 speed settings
    if(inspeed == 1) {
        speed = low; 
    } // Low Speed
    else if(inspeed == 2) { 
        speed = medium; 
    } // Medium Speed
    else if(inspeed == 3) {
        speed = high; 
    } // High Speed

    //NOW all variables are assigned, to actually run it

    cyclestarttime = 1000*millis();

    analogWrite(dryermotor, speed) //turns motor on
    active = true;

    return;
}
void dryer::StopDryer(string x) {
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
    Particle.variable("WasherCycle", myWasher.cyclename);
    Particle.function("RunDryer", myDryer.RunDryer);
    Particle.function("StopDryer", myDryer.StopDryer);
    Particle.variable("DryerTimeLeft", myDryer.remain);
    Particle.variable("DryerCycle", myDryer.cyclename);
}

void loop() {
    unsigned long int currenttime = 1000*millis();
    dbuttonnow = digitalRead(dryerbutton);
    wbuttonnow = digitalRead(washerbutton);

    //if washer is on, watches for when to turn it off without blocking. Has to be in loop to do so. Aslo updates remain
    if (myWasher.active == true) {
        myWasher.remain = (myWasher.cyclestarttime + myWasher.cycle - currenttime);
        if (myWasher.remain <= 0) {
            analogWrite(washermotor, 0); //turns it off when cycle is done
            myWasher.cyclename = "Cycle Complete!"; //And resets this variable when cycle is done
            //turn led off //and turn the led off
            myWasher.active = false;
            Particle.publish("WasherDone") //and publish event for buzzer and text
            //buzzer for 2 seconds
        }
    }
    //same thing but with the dryer
    if (myDryer.active == true) {
        myDryer.remain = (myDryer.cyclestarttime + myDryer.cycle - currenttime);
        if (myDryer.remain <= 0) {
            analogWrite(dryermotor, 0); //turns it off when cycle is done
            myDryer.cyclename = "Cycle Complete!"; //And resets this variable when cycle is done
            myDryer.active = false;
            Particle.publish("DryerDone");
            //buzzer for 2 seconds
        }
    }
    
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
