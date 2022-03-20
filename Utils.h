#ifndef __UTILS_H__
#define __UTILS_H__


#include<iostream>
#include<string>

enum Side {
    BUY,
    SELL,
    Side_UNDEFINED
};

enum OrderAction {
    NEW,
    CANCEL,
    AMEND,
    OA_UNDEFINED
};

static const std::string OrderActionStr[] = { "N",
                                              "A",
                                              "C"
                                            };

static const std::string SideStr[] = { "B",
                                       "S"
                                     };


#endif
