#include "MarketDataReader.h"

#include <chrono>
#include <cmath>
#include <simdjson.h>

#include "common/LimitOrderBook.h"

void loadMarketData(
    const std::string& filepath,
    const std::function<void(const MarketDataEvent&, std::map<std::string, LimitOrderBook> &)>& consumer
    ) {

    std::vector<MarketDataEvent> events;
    std::map<std::string, LimitOrderBook> orderbooks;

    size_t count = 0;
    auto t0 = std::chrono::steady_clock::now();

    simdjson::dom::parser parser;
    auto json = parser.load_many(filepath);
    for (simdjson::dom::element doc : json) {
        simdjson::dom::object obj = doc.get_object().value();
        consumer(MarketDataEvent::fromJson(obj), orderbooks);
        count++;
    }

    auto t1 = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(t1 - t0).count();

    std::cout << "Events:   " << count << "\n"
              << "Time:     " << std::fixed << std::setprecision(3) << elapsed << " s\n"
              << "Rate:     " << std::llround(count / elapsed) << " events/s\n";
}
