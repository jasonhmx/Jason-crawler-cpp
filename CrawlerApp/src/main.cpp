#include "Crawler.hpp"
#include <iostream>

int main()
{
    std::cout << "Program start" << "\n";
    Crawler crawler("http://monzo.com/");
    auto urls = crawler.getUrls("http://monzo.com/");
    for (auto &url : urls)
    {
        std::cout << url << "\n";
    }
    return 0;
}