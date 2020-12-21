#include "util.h"

#if defined(__unix__) || defined(__APPLE__)
#include <poll.h>
#elif defined(_WIN32)
#include <conio.h>
#include <windows.h>
#endif

namespace SailGame { namespace Common {

int Util::Wrap(int numToWrap, int range)
{
    int ret = numToWrap % range;
    if (ret < 0) {
        ret += range;
    }
    return ret;
}

char Util::GetCharWithTimeout(int milliseconds, bool autoFlush)
{
#if defined(__unix__) || defined(__APPLE__)
    std::unique_ptr<Terminal> terminal;
    if (autoFlush) {
        terminal = std::make_unique<Terminal>();
        terminal->SetModeAutoFlush();
    }

    struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
    int ret = poll(&pfd, 1, milliseconds);

    if (ret == 0) {
        throw std::runtime_error("timeout");
    }
    else if (ret == 1) {
        char c = getchar();
        return c;
    }
#elif defined(_WIN32)
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    auto ret = WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), milliseconds);

    if (ret == WAIT_TIMEOUT) {
        throw std::runtime_error("timeout");
    }
    else if (ret == WAIT_OBJECT_0) {
        char c = _getch();
        return c;
    }
#endif
    return 0;
}

char Util::GetCharImmediately()
{
#if defined(__unix__) || defined(__APPLE__)
    auto terminal = std::make_unique<Terminal>();
    terminal->SetModeAutoFlush();
    return getchar();
#elif defined(_WIN32)
    return _getch();
#endif
}

#if defined(__unix__) || defined(__APPLE__)
Terminal::Terminal()
{
    /// XXX: what if throwing an exception
    // save the old attr
    tcgetattr(STDIN_FILENO, &mOldAttr);
}

void Terminal::GetNewAttr()
{
    tcgetattr(STDIN_FILENO, &mNewAttr);
}

void Terminal::SetModeAutoFlush()
{
    GetNewAttr();
    mNewAttr.c_lflag &= ~ICANON;
    mNewAttr.c_lflag &= ~ECHO;
    ApplyNewAttr();
}

void Terminal::SetModeNoEcho()
{
    GetNewAttr();
    mNewAttr.c_lflag &= ~ECHO;
    ApplyNewAttr();
}

void Terminal::ApplyNewAttr()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &mNewAttr);
}

void Terminal::Recover()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &mOldAttr);
}

Terminal::~Terminal()
{
    Recover();
}
#endif

void Terminal::ClearStdInBuffer()
{
#if defined(__unix__) || defined(__APPLE__)
    tcflush(STDIN_FILENO, TCIFLUSH);
#elif defined(_WIN32)
    while (true) {
        auto ret = _kbhit();
        if (ret != 0) {
            _getch();
        }
        else {
            break;
        }
    }
#endif
}

}}