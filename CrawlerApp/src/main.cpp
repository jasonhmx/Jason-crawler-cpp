#include "Crawler.hpp"
#include <iostream>

int main()
{
    std::string url;
    std::cout << "Enter the URL to crawl: ";
    std::cin >> url;
    try
    {
        Crawler crawler(url);
        std::cout << "URL valid. Crawling in progress." << "\n";
        std::cout << "See list of crawled URLs below:" << "\n";
        crawler.crawl();
        std::cout << "Crawling finished!" << "\n";

        std::cout << "\n";

        auto errorLog = crawler.getErrorLog();
        if (!errorLog.empty())
        {
            std::cout << "List of url request errors:" << "\n";
            for (auto &[errorCode, urls] : errorLog)
            {
                for (auto &url : urls)
                {
                    std::cout << "Error " << errorCode << ": " << url << "\n";
                }
            }
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}