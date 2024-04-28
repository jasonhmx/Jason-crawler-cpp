#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <unordered_map>

#include <cpr/cpr.h>

class Crawler
{
    typedef std::string URL;

public:
    // TODO: properly initialise everything in constructor
    Crawler(URL url);
    // TODO: rule of zero on other 5
    void crawl();
    auto getUrls(const URL &url) -> std::vector<URL>;
    // TODO: move these to private after testing
    auto getHostName(const URL &url) -> std::string;
    auto getResult() -> std::vector<URL>;
    auto getErrorLog() -> std::unordered_map<long, std::vector<URL>> &;
    auto getActualUrl(const URL& url) -> URL;

private:
    const URL startUrl;
    const URL hostName;
    std::unordered_set<URL> visited;
    std::queue<URL> urlQueue;
    std::vector<std::thread> threadPool;
    std::unordered_map<long, std::vector<URL>> errorLog;
    std::mutex mtx;
    std::condition_variable cv;
    struct Worker;
    static const int NUMBER_OF_WORKERS = 4;
    int workingCount;
    // TODO: write tests and make sure things work
};