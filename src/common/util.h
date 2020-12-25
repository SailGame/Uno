#pragma once

#if defined(__unix__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#endif

#include <memory>
#include <vector>
#include <type_traits>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/repeated_field.h>

namespace SailGame { namespace Common {

using google::protobuf::Any;
using google::protobuf::RepeatedField;

class Util {
public:
    static int Wrap(int numToWrap, int range);

    static char GetCharWithTimeout(int milliseconds, bool autoFlush);

    static char GetCharImmediately();

    template<typename T>
    static std::vector<T> ConvertGrpcRepeatedFieldToVector(const RepeatedField<T> &field) {
        /// XXX: it seems also ok to move instead of copy
        static_assert(std::is_copy_constructible_v<T>);
        std::vector<T> v;
        for (const auto &element : field) {
            v.push_back(element);
        }
        return v;
    }

    template<typename T>
    static T UnpackGrpcAnyTo(const Any &any) {
        assert(any.Is<T>());
        T ret;
        any.UnpackTo(&ret);
        return ret;
    }
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