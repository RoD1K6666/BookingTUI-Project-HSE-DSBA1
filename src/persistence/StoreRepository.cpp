#include "booking/persistence/StoreRepository.hpp"
#include "booking/persistence/StorageError.h"

#include <fstream>

namespace booking::persistence {

    Store StoreRepository::load(const std::string& path) const {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw StorageError("Не удалось открыть файл для чтения: " + path);
        }

        Store store;

        // TODO: реализовать чтение данных

        return store;
    }

    void StoreRepository::save(const std::string& path, const Store& store) const {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw StorageError("Не удалось открыть файл для записи: " + path);
        }

        // TODO: реализовать запись данных
    }

}