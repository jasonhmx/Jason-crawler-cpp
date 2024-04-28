#include "Crawler.hpp"
#include <iostream>

int main()
{
    std::string url("https://www.adamondra.com/");
    std::cout << "Program start" << "\n";
    Crawler crawler(url);
    std::cout << "Host name: " << crawler.getHostName(url) << "\n";
    auto urls = crawler.getUrls(url);
    for (auto &url : urls)
    {
        std::cout << url << "\n";
    }

    std::cout << "Now the crawling part" << "\n";
    crawler.crawl();

    for (auto &url : crawler.getResult())
    {
        std::cout << url << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";

    std::cout << "List of url request errors:" << "\n";
    for (auto &[errorCode, urls] : crawler.getErrorLog())
    {
        for (auto &url : urls)
        {
            std::cout << "Error " << errorCode << ": " << url << "\n";
        }
    }
    return 0;
}