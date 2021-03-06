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
        bool orderFullyFilled = false;
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

                        long qtyFilled = 0;
                        if(currOrd.getReminingQty() <= order.getReminingQty()) // sell Qty < buy Qty
                        {
                            qtyFilled = currOrd.getReminingQty();
                        }
                        else  // buy Qty < sell Qty
                        {
                            qtyFilled = order.getReminingQty();
                        }

                        currOrd.fillQty(qtyFilled);
                        order.fillQty(qtyFilled);

                        std::cout << "Trade:  : " 
                                  << order.getSymbol() << " "
                                  << order.getOrderId()  << " "
                                  << qtyFilled << " " 
                                  << currOrd.getPrice()  
                                  << "\n";

                        std::cout << "Trade:  : " 
                                  << order.getSymbol() << " "
                                  << currOrd.getOrderId()  << " "
                                  << qtyFilled << " " 
                                  << currOrd.getPrice()  
                                  << "\n";


                        if(! currOrd.getReminingQty())
                        {   
                            tempListRemoveFilledIds.push_back(currOrd.getOrderId());
                        }

                        if(! order.getReminingQty())
                        {
                            orderFullyFilled = true;
                            break;
                        }

                    }

                    if(orderFullyFilled)
                        break;
                }
            }

            for( auto & oid : tempListRemoveFilledIds)
            {
                //std::cout << "removing filled order id : " << oid << "\n";
                del(_orders[oid]);
            }
                        
            if(! order.getReminingQty()) // return if order if fully filled, else continue to add it to book
                return;
        }
        else if( order.getSide() == Side::SELL && order.getPrice() <= bestBuyPrice() ) // Incoming Sell order
        {

            tempListRemoveFilledIds.clear();
            long qtyToFill = order.getQty();

            for(auto &buyPricesOrders : _buys)
            {
                if(buyPricesOrders.first >= order.getPrice())
                {
                    for(auto & orderId : buyPricesOrders.second)
                    {
                        Order& currOrd = _orders[orderId];

                        long qtyFilled = 0;
                        if(currOrd.getReminingQty() <= order.getReminingQty()) // buy Qty < sell Qty
                        {
                            qtyFilled = currOrd.getReminingQty();
                        }
                        else  // sell Qty < buy Qty
                        {
                            qtyFilled = order.getReminingQty();
                        }

                        currOrd.fillQty(qtyFilled);
                        order.fillQty(qtyFilled);

                        std::cout << "Trade:  : " 
                                  << order.getSymbol() << " "
                                  << order.getOrderId()  << " "
                                  << qtyFilled << " " 
                                  << currOrd.getPrice()  
                                  << "\n";

                        std::cout << "Trade:  : " 
                                  << order.getSymbol() << " "
                                  << currOrd.getOrderId()  << " "
                                  << qtyFilled << " " 
                                  << currOrd.getPrice()  
                                  << "\n";


                        if(! currOrd.getReminingQty())
                            tempListRemoveFilledIds.push_back(currOrd.getOrderId());

                        if(! order.getReminingQty())
                        {
                            orderFullyFilled = true;
                            break;
                        }

                    }
                }
                if(orderFullyFilled)
                    break;
            }

            for( auto & oid : tempListRemoveFilledIds)
                del(_orders[oid]);
            
            if(! order.getReminingQty()) // return if order if fully filled, else continue to add it to book
                return;

        }

        // the order is not fillable add it to the book
        orders[order.getPrice()].push_back(order.getOrderId());
            
        _orders[order.getOrderId()] = order;
    }

    template< class T >
    void del( T& orders, Order& order)
    {
        double price = _orders[order.getOrderId()].getPrice();

        //typename T::iterator it = orders.find( order.getPrice() );
        typename T::iterator it = orders.find( price );
        if( it != orders.end () )
        {
            //std::cout << "removing order : " << order.getOrderId() << "\n";
            std::list<std::string> & orderIdsList = orders[order.getPrice()];
            orderIdsList.remove(order.getOrderId());
            if(orderIdsList.empty())
            {
                //std::cout << "Last order at price : "<<  order.getPrice() 
                //          << " removed, deleting this price from map\n";
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

    bool validateOrder(OrderAction action, std::string& oid)
    {
        switch(action)
        {
            case OrderAction::NEW:
                if( _orders.find(oid) != _orders.end())
                {
                    std::cout << "Duplicate order id : " << oid << "\n";
                    return false;
                }
                break;
            case OrderAction::AMEND:
            case OrderAction::CANCEL:
                if( _orders.find(oid) == _orders.end())
                {
                    std::cout << "order id not find to cancel/amend OID : " << oid << "\n";
                    return false;
                }
                else if( _orders[oid].getReminingQty() == 0 )
                {
                    std::cout << "order remaining qty is 0  OID : " << oid << "\n";
                    return false;
                }
                break;
            default:
                    std::cout << "Unrecognised order action " << action << "\n";
        }

        return true;
    }

    void add(Order &o)
    {
        //std::cout << "Adding Order " << o.getOrderId() << std::endl;
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
        //std::cout << "Deleting Order " << o.getOrderId() << std::endl;
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

    void printBookFull()
    {
        std::cout << "BUYs : \n";
        for(auto & obj : _buys)
        {
            //std::cout << "Price : " << obj.first << std::endl;
            for(auto & o : obj.second)
                _orders[o].print();
        }

        std::cout << "SELLs : \n";
        for(auto & obj : _sells)
        {
            //std::cout << "Price : " << obj.first << std::endl;
            for(auto & o : obj.second)
                _orders[o].print();
        }
    }

    void printBook()
    {
        std::cout << "BUYs : \n";
        int level = 0;
        int maxLevel = 5;
        for(auto & obj : _buys)
        {
            for(auto & o : obj.second)
            {
                _orders[o].print();
            }
            level ++;
            if(level == maxLevel)
                break;
        }

        level = 0;
        std::cout << "SELLs : \n";
        for(auto & obj : _sells)
        {
            for(auto & o : obj.second)
            {
                _orders[o].print();
            }
            level ++;
            if(level == maxLevel)
                break;
        }
    }


};


#endif
