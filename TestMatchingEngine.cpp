#include "Order.h"

int main( int argc, char** argv )
{
    Order o1("2800.HK", 3.44, 10000, Side::BUY, "oid11");

    o1.print();
}
