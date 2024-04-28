#include "Crawler.hpp"
#include <exception>
#include <regex>

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

Crawler::Crawler(URL url) : startUrl(getActualUrl(url)),
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

auto Crawler::getUrls(const URL &url) -> std::vector<URL>
{
    std::vector<std::string> urls;

    // Make an HTTP GET request to the starting URL
    cpr::Response response = cpr::Get(cpr::Url{ url });

    if (response.status_code != 200) {
        std::unique_lock lock{ mtx };
        errorLog[response.status_code].push_back(url);
        return urls;
    }

    // Parse the HTML content to find URLs
    std::string htmlContent = response.text;
    size_t pos = 0;
    while ((pos = htmlContent.find("href=\"", pos)) != std::string::npos) {
        pos += 6; // Move past "href=\""
        size_t endPos = htmlContent.find("\"", pos);
        if (endPos != std::string::npos) {
            std::string potentialUrl = htmlContent.substr(pos, endPos - pos);
            // Check if the URL starts with "http://" or "https://"
            if (potentialUrl.substr(0, 7) == "http://" || potentialUrl.substr(0, 8) == "https://") {
                urls.push_back(potentialUrl);
            }
        }
    }

    return urls;
}

auto Crawler::getHostName(const URL &url) -> std::string
{
    // Regular expression to extract the scheme and host
    std::regex urlRegex("(https?://[^/]+/)");

    std::smatch match;
    if (std::regex_search(url, match, urlRegex))
    {
        return match[1].str();
    }
    else
    {
        // Return empty string if no match found
        return "";
    }
}

auto Crawler::getResult() -> std::vector<URL>
{
    return {visited.begin(), visited.end()};
}

auto Crawler::getErrorLog() -> std::unordered_map<long, std::vector<URL>> &
{
    return errorLog;
}

auto Crawler::getActualUrl(const URL &url) -> URL
{
    auto response = cpr::Get(cpr::Url{url});
    if (response.status_code != 200)
    {
        throw std::runtime_error("HTTP request failed with status code: " + std::to_string(response.status_code));
    }
    return response.url.str();
}
