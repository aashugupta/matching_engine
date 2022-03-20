#include "Order.h"
#include "MatchingEngine.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <utility>
#include <iostream>
#include <stdexcept>


int main( int argc, char** argv )
{

    MatchingEngine me;

    me.parseFile("testOrders.txt");

    std::cout << "<on exit>\n";
    me.printAllBooks();

}
