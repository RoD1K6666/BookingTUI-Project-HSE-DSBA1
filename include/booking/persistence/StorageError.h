//
// Created by Sophie Markina on 28.02.2026.
//

#ifndef BOOKINGTUI_STORAGEERROR_H
#define BOOKINGTUI_STORAGEERROR_H

#endif

#pragma once

#include <stdexcept>
#include <string>

namespace booking::persistence {

    class StorageError : public std::runtime_error {
    public:
        explicit StorageError(const std::string& message)
            : std::runtime_error(message) {}
    };

}

