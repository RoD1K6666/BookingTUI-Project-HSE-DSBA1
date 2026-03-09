#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "StorageError.h"

namespace booking::persistence {

    class AtomicFileWriter {
    public:
        explicit AtomicFileWriter(const std::filesystem::path& targetPath);
        AtomicFileWriter(const AtomicFileWriter&) = delete;
        AtomicFileWriter& operator=(const AtomicFileWriter&) = delete;
        AtomicFileWriter(AtomicFileWriter&& other) noexcept;
        AtomicFileWriter& operator=(AtomicFileWriter&& other) noexcept;
        ~AtomicFileWriter();
        std::ofstream& stream() { return fileStream_; }
        void commit();

    private:
        std::filesystem::path targetPath_;
        std::filesystem::path tempPath_;
        std::ofstream fileStream_;
        bool committed_ = false;
        void cleanup() noexcept;
    };

}