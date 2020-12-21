#pragma once

#if defined(__unix__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#endif

#include <memory>

namespace SailGame { namespace Common {

class Util {
public:
    static int Wrap(int numToWrap, int range);

    static char GetCharWithTimeout(int milliseconds, bool autoFlush);

    static char GetCharImmediately();
};

class Terminal {
public:
#if defined(__unix__) || defined(__APPLE__)
    Terminal();

    ~Terminal();

    void SetModeAutoFlush();

    void SetModeNoEcho();

    void Recover();
#endif

    static void ClearStdInBuffer();

#if defined(__unix__) || defined(__APPLE__)
private:
    void GetNewAttr();

    void ApplyNewAttr();

private:
    struct termios mNewAttr;
    struct termios mOldAttr;
#endif
};

}}