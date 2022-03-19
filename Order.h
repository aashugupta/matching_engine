#ifndef __ORDER_H__
#define __ORDER_H__

#include<string>
#include<iostream>

enum Side { BUY, SELL };

class Order
{
private:
    std::string    _symbol;
    double         _price;
    long           _qty;
    Side           _side;
    std::string    _orderId; 

public:
    std::string& getSymbol()   { return _symbol; }
    double getPrice()          { return _price; }
    long getQty()              { return _qty; }
    Side getSide()             { return _side; }
    std::string& getOrderID()  { return _orderId; }

    Order(std::string symbol, double price, long qty, Side side, std::string orderId):
        _symbol(symbol),
        _price(price), 
        _qty(qty),
        _side(side),
        _orderId(orderId)
        {}

    void print()
    {
        std::cout << "Symbol : " << _symbol << " Price : " << _price << " Qty : " << _qty << " Side : " << _side 
                  <<   " OrderID : " << _orderId << std::endl;
    }
};

#endif
