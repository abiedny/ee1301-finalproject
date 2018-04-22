#include <string>
#include <sstream>
using namespace std;

#define DEFAULTWASHERTEMP 1
#define DEFAULTWASHERSPEED 1
#define DEFAULTDRYERTEMP 1
#define WASHERSTOPBUTTON D0
#define DRYERSTOPBUTTON D1

class dryer {
  public:
    int temp = DEFAULTDRYERTEMP;
    string name;
    string owner;

    int CreateDryerProfile(string a) {
      sstream(a) >> temp >> name;
      return 1;
    }
    int RunDryer(string profile) { //This function and runwasher will accept a string in the form of "temp name"
      return 1;
    }
    int StopDryer(string a) {
      return 1;
    }
} ;
class washer {
  public:
    int temp = DEFAULTWASHERTEMP, speed = DEFAULTWASHERSPEED;
    string name;
    string owner;

    int CreateWasherProfile(string a) {
      sstream(a) >> temp >> speed >> name;
      return 1;
    }
    int RunWasher(string profile) { //argument is string in the form of "temp speed name"
      return 1;
    }
    int StopWasher(string a) {
      return 1;
    }
} ;

void setup() {
  washer myWasher;
  dryer myDryer;
  Particle.function("RunDryer", myDryer.RunDryer);
  Particle.function("StopDryer", myDryer.StopDryer);
  Particle.function("CreateDryerProfile", myDryer.CreateDryerProfile);
  Particle.function("RunWasher", myWasher.RunWasher);
  Particle.function("StopWasher", myWasher.StopWasher);
  Particle.function("CreateWasherProfile", myWasher.CreateWasherProfile);
  Particle.variable("owner", owner);
  Particle.variable("")
}

void loop() {
  //I think the loop only needs to be used for the physical buttons and stuff?
  if (digitalRead(WASHERSTOPBUTTON) == HIGH) {
    myWasher.StopWasher("Stop");
  }

  if (digitalRead(DRYERSTOPBUTTON) == HIGH) {
    myDryer.StopDryer("Stop");
  }
}
