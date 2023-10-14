#include <iostream>
#include <vector>
#include <ctime>

#ifndef ORDERINFO_INCL_H
#define ORDERINFO_INCL_H
#include "OrderInfo.h"
#endif

using namespace std;

class Utils {
public:
     static double avgprice(std::vector<std::pair<double,vector<OrderInfo>>>& orderqueue, double qty) {
        if (orderqueue.size() == 0) {
           return -1;
	} else {
           double qtycount = 0;
	   double sumprice = 0;
	   int ovsize = orderqueue.size();
	   for (int i = 0; i < ovsize && qtycount < qty; i++) {
		pair<double,vector<OrderInfo>> p = orderqueue[i];
		double price = p.first;
		vector<OrderInfo> iv = p.second;
		int ivsize = iv.size();
		for (int j = 0; j < ivsize && qtycount < qty; j++) {
                    OrderInfo oi = iv[j];
	            double outstandingqty = oi.outstandingqty();
                    double thisqty = ((qty-qtycount) > outstandingqty) ? outstandingqty : (qty-qtycount);
		    sumprice += (thisqty * price);
		    qtycount += thisqty; 
		}
	   }
           if (qtycount < qty) {
             return -1;
	   } else {
             return sumprice / qtycount;
	   }
	}
     } 

};
