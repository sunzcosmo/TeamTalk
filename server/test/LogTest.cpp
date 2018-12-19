#include <iostream>
#include <string>

#include "../src/base/TimLogging.h"

<template T>
void LogTest(T input)
{
    LOG_TRACE<<input;

    LOG_DEBUG<<input;

    LOG_INFO<<input;

    LOG_WARNING<<input;

    LOG_ERROR<<input;

    LOG_FATAL<<input;
}

int main(const int argc, const char * const argv[])
{
    std::string teststr("abcdefghijklmnopqrstuvwxyz 1234567890");

    tim::InitFileLogger("test.log");

    tim::Logger::ResetLogLevel(tim::Logger::SeverityLevel::TRACE);

    LogTest(teststr);

    std::cout<<"Log test finished, check result in test.log.\n";

    return 0;s
}