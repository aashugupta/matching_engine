#ifndef __MATCHING_ENGINE_H__
#define __MATCHING_ENGINE_H__

#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <utility>
#include <iostream>
#include <stdexcept>


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
        PQBook &book = _books[order.getSymbol()];

        if(book.validateOrder(OrderAction::NEW, order.getOrderId()) )
            _books[order.getSymbol()].add(order);
    }

    void onCancelOrder(Order& order)
    {
        PQBook &book = _books[order.getSymbol()];

        if(book.validateOrder(OrderAction::CANCEL, order.getOrderId()) )
            _books[order.getSymbol()].del(order);
    }

    void onAmendOrder(Order& order)
    {
        PQBook &book = _books[order.getSymbol()];

        if(book.validateOrder(OrderAction::AMEND, order.getOrderId()) )
        {
            _books[order.getSymbol()].del(order);
            _books[order.getSymbol()].add(order);
        }
    }

    void printBookForSymbol(const std::string& sym)
    {
        std::cout << "\nBook for : " << sym << "\n";
        _books[sym].printBook();
    }

    void printAllBooks()
    {
        for(auto &kv : _books)
            printBookForSymbol(kv.first);
    }

    void parseFile(std::string&& file)
    {
        std::ifstream orderActionsfile(file);
        std::string line;

        while(std::getline(orderActionsfile, line))
        {
            std::cout << line << "\n";

            std::vector<std::string> orderDetails = getSubStrings(line, ',');   

            if(orderDetails.size() == 6)
            {
                // valid format : <exchange_ticker>, <request_type>, <order_id>, <side>, <quantity>, <price>
                long quantity = 0; 
                double price = 0.0;
                std::string& exchange_ticker  = orderDetails[0];
                OrderAction oa                = getOrderAction( orderDetails[1]);
                std::string& order_id         = orderDetails[2];
                Side side                     = getSide(orderDetails[3]);
                if(oa == OrderAction::OA_UNDEFINED)
                {
                    std::cout << "ERR : Order action not ok, skipping line.. " << "\n";
                    continue;
                }
                if(side == Side::Side_UNDEFINED)
                {
                    std::cout << "ERR : Side not ok, skipping line.. " << "\n";
                    continue;
                }
                try
                {
                    quantity = stol(orderDetails[4]);
                    if(quantity < 0)
                    {
                        std::cout << "Skipping.. this line, quantity not ok : " << orderDetails[4] << "\n";;
                        continue;
                    }
                }
                catch(std::invalid_argument const& ex)
                {
                    std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
                    std::cout << "Skipping.. this line, quantity not ok : " << orderDetails[4] << "\n";;
                    continue;
                }
                try
                {
                    price = stod(orderDetails[5]);
                    if(price < 0)
                    {
                        std::cout << "Skipping.. this line, price not ok : " << orderDetails[5] << "\n";;
                        continue;
                    }
                }
                catch(std::invalid_argument const& ex)
                {
                    std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
                    std::cout << "Skipping.. this line, price not ok : " << orderDetails[5] << "\n";;
                    continue;
                }

                Order o(exchange_ticker, order_id, side, quantity, price);
                
                switch(oa)
                {
                    case OrderAction::NEW:
                        onNewOrder(o);
                        break;
                    case OrderAction::CANCEL:
                        onCancelOrder(o);
                        break;
                    case OrderAction::AMEND:
                        onAmendOrder(o);
                        break;
                    default:
                        std::cout << "ERR : Undefined order action \n"; 
                }

            }
            else
            {
                std::cout << "Skipping.. this line, Error in line : number of columns != 6\n";
            }

        }
    }

    OrderAction getOrderAction(const std::string & oaStr)
    {
        if( oaStr == "N")
            return OrderAction::NEW;
        else if( oaStr == "A" )
            return OrderAction::AMEND;
        else if( oaStr == "C" )
            return OrderAction::CANCEL;
        else
            return OrderAction::OA_UNDEFINED;
    }

    Side getSide(const std::string & sideStr )
    {
        if( sideStr == "B" )
            return Side::BUY;
        else if( sideStr == "S" )
            return Side::SELL;
        else
            return Side::Side_UNDEFINED;
    }

    std::vector<std::string> getSubStrings(const std::string & str, char sep)
    {
        std::vector<std::string> list;
        std::stringstream ss(str);
        while(ss.good())
        {
            std::string substr;
            getline( ss, substr, sep);
            list.push_back(substr);
        }

        return list; // list is converted to r-value and return value is move constructed.
    }

};


#endif
