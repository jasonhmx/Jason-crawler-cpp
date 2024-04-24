#include "Crawler.hpp"

struct Crawler::Worker
{
    Crawler &crawler;

    void operator()()
    {
        while (true)
        {
            std::string from_url;

            { // lock scope
                std::unique_lock lock{crawler.mtx};
                crawler.cv.wait(lock, [&]()
                                { return !crawler.urlQueue.empty(); });
                from_url = crawler.urlQueue.front();
                crawler.urlQueue.pop();
                if (from_url.empty())
                {
                    // if we retrieved empty url signaled by termination (see later)
                    break;
                }
                crawler.workingCount++;
            }

            // getUrls() is the time-consuming bit involving http requests and html parsing
            // hence can be parallelised
            for (const auto &url : crawler.getUrls(from_url))
            {
                if (crawler.getHostName(url) != crawler.hostName)
                {
                    continue;
                }
                std::unique_lock lock{crawler.mtx};
                if (!crawler.visited.contains(url))
                {
                    crawler.visited.emplace(url);
                    crawler.urlQueue.emplace(url);
                    crawler.cv.notify_all();
                }
            }

            { // lock scope
                std::unique_lock lock{crawler.mtx};
                crawler.workingCount--;
                if (crawler.urlQueue.empty() && crawler.workingCount == 0)
                {
                    crawler.cv.notify_all();
                }
            }
        }
    }
};

Crawler::Crawler(URL url) : startUrl(std::move(url)),
                            urlQueue({startUrl}),
                            visited({startUrl}),
                            hostName(getHostName(startUrl))
{
}

void Crawler::crawl()
{
    for (int i = 0; i < NUMBER_OF_WORKERS; i++)
    {
        threadPool.emplace_back(Worker{*this});
    }
    { // lock scope for termination check
        std::unique_lock lock{mtx};
        cv.wait(lock, [&]()
                { return urlQueue.empty() && workingCount == 0; });
        for (const auto &t : threadPool)
        {
            // signal termination for each thread
            urlQueue.emplace();
        }
        cv.notify_all();
    }
    for (auto &t : threadPool)
    {
        t.join();
    }
    std::cout << "Crawling finished!" << "\n";
}

auto Crawler::getHostName(const URL &url) -> std::string
{
    static const std::string scheme = "http://";
    return url.substr(0, url.find('/', scheme.length()));
}
