#include <iostream>
using namespace std;

class PositionKeeper {
private:
     double _cashbalance = 0;
     double _instrumentbalance = 0;
     string _symbol;
     string _ccy;
public:
     void addcash(double amt) {
         _cashbalance += amt;
     }
     void addinstrument(double amt) {
         _instrumentbalance += amt;
     }
     PositionKeeper(string ccy, double initcashbalance, string symbol, double initinstrumentbalance) {
        _ccy = ccy;
	_cashbalance = initcashbalance;
	_symbol = symbol;
	_instrumentbalance = initinstrumentbalance;
     }
     double cashbalance() { return _cashbalance; }
     double instrumentbalance() { return _instrumentbalance; }
     string ccy() { return _ccy; }
     string symbol() { return _symbol; }
     ~PositionKeeper() {
         cerr << "\nPositionKeeper Destructor: " << this << endl;
     }
};
