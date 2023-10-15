#include <iostream>
#include <ctime>
using namespace std;


class Clock {
private:     
    unsigned int _base;
    bool ismanipulated;

public:
    Clock() { _base=(intmax_t)time(0); }
    Clock(unsigned int base) { _base = base;}
    time_t currenttime() { 
       if (ismanipulated) {
          time_t base = _base;
	  return base;
       } else {
          return time(0);
       }
    }
    void manipulate(bool x) {
       ismanipulated = x;
    }
    void adjust(int d) {
       _base +=d;
    } 
};
