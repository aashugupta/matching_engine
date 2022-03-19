#include "Order.h"
#include "PQBook.h"


class MatchingEngine
{

private:

typedef std::map< std::string, PQBook > BookMap;
BookMap _books;

public:
void onNewOrder(Order& order)
{
    _books[order.getSymbol()].add(order);
}

void onCancelOrder(Order& order)
{
    _books[order.getSymbol()].del(order);
}

void printBookForSymbol(const std::string& sym)
{
    _books[sym].printBook();
}

};

int main( int argc, char** argv )
{
    Order o1("2800.HK", "oid11", Side::BUY, 10000, 21.44);
    Order o2("2800.HK", "oid12", Side::BUY, 10000, 21.44);
    Order o3("2800.HK", "oid13", Side::BUY, 10000, 21.46);
    Order o4("2800.HK", "oid14", Side::SELL, 10000, 21.48);
    Order o5("2800.HK", "oid15", Side::BUY, 10000, 21.48);
    Order o6("2800.HK", "oid16", Side::SELL, 10000, 21.50);

    //std::cout << "Oid for o1 : "  << o1.getOrderID() << std::endl;


    MatchingEngine me;

    me.onNewOrder(o1);
    me.onNewOrder(o2);
    me.onNewOrder(o3);
    me.onNewOrder(o6);
    me.onNewOrder(o4);
    me.onNewOrder(o5);

    /*
    me.onCancelOrder(o2);
    me.onCancelOrder(o1);
    me.onCancelOrder(o3);
    */

    me.printBookForSymbol("2800.HK");

    //o1.print();
}
