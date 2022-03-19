#ifndef __MATCHING_ENGINE_H__
#define __MATCHING_ENGINE_H__

#include<string>
#include<iostream>
#include"Order.h"


class MatchingEngine
{
private:
    long incomingSequence = 0;

public:
    void addOrder(Order & o);

    void sendFill();
    void sendCancel();

    void cancelOrder(Order & o);
};

#endif
