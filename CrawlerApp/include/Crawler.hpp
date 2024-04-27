#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

#include <cpr/cpr.h>
#include <lexbor/html/parser.h>

class Crawler
{
    typedef std::string URL;

public:
    // TODO: properly initialise everything in constructor
    Crawler(URL url);
    // TODO: rule of zero on other 5
    void crawl();
    auto getUrls(const URL &url) -> std::vector<URL>;

private:
    const URL startUrl;
    const URL hostName;
    std::unordered_set<URL> visited;
    std::queue<URL> urlQueue;
    std::vector<std::thread> threadPool;
    std::mutex mtx;
    std::condition_variable cv;
    struct Worker;
    static const int NUMBER_OF_WORKERS = 4;
    int workingCount;
    // TODO: html parser and http request maker, fascilate getUrls()
    // TODO: write tests and make sure things work

    auto getHostName(const URL &url) -> std::string;
};