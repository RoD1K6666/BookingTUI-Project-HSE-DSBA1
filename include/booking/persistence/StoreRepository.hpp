#pragma once

#include <string>
#include "booking/persistence/Store.hpp"

namespace booking::persistence {

    class StoreRepository {
    public:
        Store load(const std::string& path) const;
        void save(const std::string& path, const Store& store) const;
    };

}