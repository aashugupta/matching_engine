#ifndef __ORDER_H__
#define __ORDER_H__

#include<string>
#include<iostream>

enum Side { BUY, SELL };

class Order
{
private:
    std::string    _symbol;
    std::string    _orderId; 
    Side           _side;
    long           _qty;
    long           _filledQty;
    long           _remainingQty;
    double         _price;

public:
    Order() {}
    Order(std::string symbol, std::string orderId, Side side, long qty, double price):
        _symbol(symbol),
        _orderId(orderId),
        _side(side),
        _qty(qty),
        _price(price),
        _filledQty(0),
        _remainingQty(qty)
        {}

    double getPrice()         { return _price;         }
    long   getQty()           { return _qty;           }
    long   getFilledQty()     { return _filledQty;     }
    long   getReminingQty()   { return _remainingQty;  }
    Side   getSide()          { return _side;          }
    std::string& getSymbol()  { return _symbol;        }
    std::string& getOrderId() { return _orderId;       }

    void fillQty(long qty)
    {
        _filledQty    += qty;
        _remainingQty -= qty;
    }

    void print()
    {
        std::cout << "Symbol : "     << _symbol 
                  << " OrderID : "   << _orderId 
                  << " Price : "     << _price 
                  << " Qty : "       << _qty 
                  << " Side : "      << _side 
                  << std::endl;
    }
};

#endif
