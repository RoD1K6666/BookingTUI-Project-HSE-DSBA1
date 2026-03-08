#include "booking/persistence/AtomicFileWriter.hpp"

namespace booking::persistence {

AtomicFileWriter::AtomicFileWriter(const std::filesystem::path& targetPath) : targetPath_(targetPath) , tempPath_(targetPath.string() + ".tmp")
{

    fileStream_.open(tempPath_, std::ios::out | std::ios::trunc);
    if (!fileStream_.is_open()) {
        throw StorageError("Cannot create temporary file: " + tempPath_.string());
    }
}

AtomicFileWriter::~AtomicFileWriter() noexcept {
    if (!committed_) {
        cleanup();
    }
}

void AtomicFileWriter::cleanup() noexcept {
    fileStream_.close();

    std::error_code ec;
    std::filesystem::remove(tempPath_, ec);
}

void AtomicFileWriter::commit() {
    fileStream_.close();

    std::error_code ec;
    std::filesystem::rename(tempPath_, targetPath_, ec);

    if (ec) {
        std::filesystem::remove(tempPath_, ec);
        throw StorageError("Cannot rename temporary file: " + ec.message());
    }

    committed_ = true;
}

// Конструктор перемещения
AtomicFileWriter::AtomicFileWriter(AtomicFileWriter&& other) noexcept : targetPath_(std::move(other.targetPath_))
    , tempPath_(std::move(other.tempPath_))
    , fileStream_(std::move(other.fileStream_))
    , committed_(other.committed_)
{
    other.committed_ = true;
}

AtomicFileWriter& AtomicFileWriter::operator=(AtomicFileWriter&& other) noexcept {
    if (this != &other) {
        if (!committed_) {
            cleanup();
        }

        targetPath_ = std::move(other.targetPath_);
        tempPath_ = std::move(other.tempPath_);
        fileStream_ = std::move(other.fileStream_);
        committed_ = other.committed_;

        other.committed_ = true;
    }
    return *this;
}

}