#pragma once

#include <iostream>

namespace SailGame { namespace Common {

class Event {
public:
    Event(int id) : mId(id) {}

    void Process();

private:
    int mId;
};
}}