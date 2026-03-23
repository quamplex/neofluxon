#include "PathLoader.h"
#include <filesystem>
#include <chrono>
#include <stop_token>

using namespace std::chrono_literals;

PathLoader::PathLoader() = default;

PathLoader::~PathLoader() {
    // Stop scan and clear callbacks
    {
        std::lock_guard lock(m_mutex);
        m_itemCountChangedCallback = nullptr;
        m_fileFoundCallback = nullptr;
    }

    if (m_scanThread.joinable()) {
        m_stopFlag = true;
        m_scanThread.join();
    }
}

void PathLoader::setDirectory(const std::string& path, bool recursive) {
    std::lock_guard lock(m_mutex);

    // Stop previous scan
    if (m_scanThread.joinable()) {
        m_stopFlag = true;
        m_scanThread.join();
        m_stopFlag = false;
    }

    m_directory = path;
    m_recursive = recursive;
    m_files.clear();

    startScan();
}

void PathLoader::setItemCountChangedCallback(ItemCountChangedCallback cb) {
    std::lock_guard lock(m_mutex);
    m_itemCountChangedCallback = std::move(cb);
}

void PathLoader::setFileFoundCallback(FileFoundCallback cb) {
    std::lock_guard lock(m_mutex);
    m_fileFoundCallback = std::move(cb);
}

size_t PathLoader::numberOfFiles() const {
    std::lock_guard lock(m_mutex);
    return m_files.size();
}

std::string PathLoader::getFile(size_t index) const {
    std::lock_guard lock(m_mutex);
    if (index >= m_files.size()) return {};
    return m_files[index];
}

void PathLoader::startScan() {
    std::string directoryCopy;
    bool recursiveCopy;

    {
        std::lock_guard lock(m_mutex);
        directoryCopy = m_directory;
        recursiveCopy = m_recursive;
    }

    m_scanThread = std::jthread([this, directoryCopy, recursiveCopy](std::stop_token stopToken) {
        scanDirectory(directoryCopy, recursiveCopy, stopToken);
    });
}

void PathLoader::scanDirectory(const std::string& path, bool recursive, std::stop_token stopToken)
{
    namespace fs = std::filesystem;

    size_t notifyCounter = 0;
    auto lastNotifyTime = std::chrono::steady_clock::now();

    auto shouldNotify = [&]() {
        auto now = std::chrono::steady_clock::now();
        return notifyCounter >= 100 || (now - lastNotifyTime > 100ms);
    };

    std::function<void(const fs::path&)> scanRecursive;
    scanRecursive = [&](const fs::path& p) {
        for (auto& entry : fs::directory_iterator(p)) {
            if (stopToken.stop_requested() || m_stopFlag)
                return;

            if (entry.is_directory() && recursive)
                scanRecursive(entry.path());

            if (entry.is_regular_file()) {
                size_t index;
                {
                    std::lock_guard lock(m_mutex);
                    m_files.push_back(entry.path().string());
                    index = m_files.size() - 1;

                    if (m_fileFoundCallback)
                        m_fileFoundCallback(index);
                }

                notifyCounter++;
                if (shouldNotify()) {
                    notifyCounter = 0;
                    lastNotifyTime = std::chrono::steady_clock::now();
                    std::lock_guard lock(m_mutex);
                    if (m_itemCountChangedCallback)
                        m_itemCountChangedCallback();
                }
            }
        }
    };

    if (!path.empty())
        scanRecursive(path);

    // Final notification after scan completes
    std::lock_guard lock(m_mutex);
    if (m_itemCountChangedCallback)
        m_itemCountChangedCallback();
}
