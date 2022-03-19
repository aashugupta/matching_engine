#ifndef __PQBook_H__
#define __PQBook_H__


#include <queue>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include "Order.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 1E-6

class PQBook
{
private:

    typedef double Price;
    typedef long Quantity;

    struct Less {
        bool operator() (const Price& lhs, const Price& rhs) const { return fabs(lhs-rhs) > EPSILON && lhs < rhs; }
    };
    
    struct Greater {
        bool operator() (const Price& lhs, const Price& rhs) const { return fabs(lhs-rhs) > EPSILON && lhs > rhs; }
    };

    typedef std::map< Price, std::list<std::string> , Greater > BuyOrders; 
    typedef std::map< Price, std::list<std::string> , Less    > SellOrders; 

    std::unordered_map<std::string, Order> _orders;

    BuyOrders  _buys;
    SellOrders _sells; 

    std::vector<std::string> tempListRemoveFilledIds;

    template< class T >
    void add( T& orders, Order& order)
    {
        //Check if this should generate a fill  
        if(order.getSide() == Side::BUY && order.getPrice() >= bestSellPrice() )
        {
            tempListRemoveFilledIds.clear();
            long qtyToFill = order.getQty();

            for(auto &sellPricesOrders : _sells)
            {
                if(sellPricesOrders.first <= order.getPrice())
                {
                    for(auto & orderId : sellPricesOrders.second)
                    {
                        Order& currOrd = _orders[orderId];

                        if(currOrd.getReminingQty() <= qtyToFill)
                        {
                            long qtyFilled = currOrd.getReminingQty();
                            currOrd.fillQty(currOrd.getReminingQty());

                            order.fillQty(currOrd.getReminingQty());

                            std::cout << "Fill on incoming order id : " << order.getOrderId() 
                                      << " WITH oid : "                 << currOrd.getOrderId()
                                      << " Price : "                    << currOrd.getPrice()
                                      << " QtyFilled : "                << qtyFilled << "\n";

                            if(! currOrd.getReminingQty())
                                tempListRemoveFilledIds.push_back(currOrd.getOrderId());

                            if(! order.getReminingQty())
                                return;

                        }
                    }
                }
            }

            for( auto & oid : tempListRemoveFilledIds)
                del(_orders[oid]);
        }
        else if( order.getPrice() <= bestBuyPrice() )
        {

        }

        // the order is not fillable add it to the book
        orders[order.getPrice()].push_back(order.getOrderId());
            
        _orders[order.getOrderId()] = order;
    }

    template< class T >
    void del( T& orders, Order& order)
    {
        typename T::iterator it = orders.find( order.getPrice() );
        if( it != orders.end () )
        {
            std::cout << "removing order : " << order.getOrderId() << "\n";
            std::list<std::string> & orderIdsList = orders[order.getPrice()];
            orderIdsList.remove(order.getOrderId());
            if(orderIdsList.empty())
            {
                std::cout << "Last order at price : "<<  order.getPrice() 
                          << " removed, deleting this price from map\n";
                orders.erase(it);
            }
        }
        else
        {
            std::cout << "Order doesn't exist in this queue .. "; 
            order.print();
        }

    }

public:
    void add(Order &o)
    {
        std::cout << "Adding Order " << o.getOrderId() << std::endl;
        if( o.getSide() == Side::BUY )
        {
            add( _buys, o);
        }
        else
        {
            add( _sells, o);
        }

        //printBook();   
    }

    void del(Order &o)
    {
        std::cout << "Deleting Order " << o.getOrderId() << std::endl;
        if( o.getSide() == Side::BUY )
        {
            del( _buys, o);
        }
        else
        {
            del( _sells, o);
        }
    }

    Price bestBuyPrice() const
    {
        Price ret = -1;
        if( _buys.size() )
            ret = _buys.begin()->first;

        return ret;
    }

    Price bestSellPrice() const
    {
        Price ret = -1;
        if( _sells.size() )
            ret = _sells.begin()->first;

        return ret;
    }

    void printBook()
    {
        std::cout << "Printing Book Buys : \n";
        for(auto & obj : _buys)
        {
            std::cout << "Price : " << obj.first << std::endl;
            for(auto & o : obj.second)
                _orders[o].print();
        }

        std::cout << "Printing Book Sells : \n";
        for(auto & obj : _sells)
        {
            std::cout << "Price : " << obj.first << std::endl;
            for(auto & o : obj.second)
                _orders[o].print();
        }
    }


};


#endif
