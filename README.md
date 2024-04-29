# Jason-crawler-cpp

A web crawler written in C++ (turns out to be a terrible langauge choice for this but alas).

# Directions of Use

This program has been written and run on Windows, built using msvc.  You can use cmake to configure and build the executable `crawler_app.exe` yourself, or simply unzip `release.zip` into its separate folder, and run the pre-built executable from command line.

The program will ask you for a starting URL to start crawling, and print each url that satisfies the requirements along the way. In the end it will also print out a list of failed HTTP requests with their error codes.

# Source Code

The main source code is in 3 files: `Crawler.hpp`, `Crawler.cpp` and `main.cpp`. The program is run by constructing a `Crawler` object and running its `crawl()` member function, which uses a multi-threaded implementation of breadth-first search. The HTTP requests are achieved using the `cpr` library, and HTML is parsed using simple string searches and regular-expression matching.

By default, the crawler only parses URLs that are declared after `href=`. If all verbose url information is required, simply remove the compiler definition (`HREF_ONLY`) in the `CMakeLists.txt` file located in the `CrawlerApp` folder. This way the corresponding implementation of `getUrls()` will be compiled.

Unfortunately I ran out of time to implement tests for this class, GoogleTest is giving me trouble linking to the library and exe. The framework is in place, set up using cmake inside the `CrawlerTest` folder.