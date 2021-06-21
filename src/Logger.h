//
// Created by linuxpanda on 17/06/21.
//
#pragma once

#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <iostream>
#include <string>
#include <ctime>

class Logger {
private:
    std::string red = "\033[0;31m";
    std::string noColor = "\033[0m";
    std::string green = "\033[1;32m";
    std::string blue = "\033[0;34m";
    std::string orange = "\033[4;31m";
    std::string yellow = "\033[1;33m";
    time_t timetoday = time(nullptr);

    std::string r = "[";
    std::string l = "] : ";
    std::string timestamp = ctime(&timetoday);
    std::string logTimeFormat = r + timestamp.erase(timestamp.length() - 1) + l;


    std::string _info = noColor + green + "[INFO] " + logTimeFormat;
    std::string _debug = noColor + blue + "[DEBUG] " + logTimeFormat;
    std::string _warn = noColor + yellow + "[WARNING] " + logTimeFormat;
    std::string _error = noColor + red + "[ERROR] " + logTimeFormat;
    std::string _critical = noColor + orange + "[CRITICAL] " + logTimeFormat;

    template<typename T>
    void println(T msg, const std::string &type) {
        std::cout << type << msg << std::endl;
    }

    template<typename T>
    void print(T msg, const std::string &type) {
        std::cout << type << msg;
    }

    template<typename T>
    void printRawLn(T msg) {
        std::cout << msg << std::endl;
    }

public:

    template<typename T>
    void info(T msg, bool nextln = true, bool raw = false) {
        if (nextln) println(msg, _info);
        else if (raw) printRawLn(msg);
        else print(msg, _info);
    }

    template<typename T>
    void debug(T msg) {
        println(msg, _debug);
    }

    template<typename T>
    void warning(T msg) {
        println(msg, _warn);
    }

    template<typename T>
    void error(T msg) {
        println(msg, _error);
    }

    template<typename T>
    void critical(T msg) {
        println(msg, _critical);
    }

};

#endif //LOGGER_LOGGER_H