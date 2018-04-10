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
    }
    int RunDryer(string a) { //we can only return ints but they can be pointers....
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
    }
    int RunWasher(string profile) {
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
  Particle.function("RunWasher", myWasher.RunWasher);
  Particle.function("StopWasher", myWasher.StopWasher);
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
