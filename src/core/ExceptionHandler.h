#pragma once

#include <QString>
#include <exception>

class ExceptionHandler {
public:
    static void handle(const std::exception &e);
    static void show(const QString &message);
};
