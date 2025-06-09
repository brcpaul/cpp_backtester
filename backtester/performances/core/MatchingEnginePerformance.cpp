#include <iostream>
#include <chrono>
#include <random>
#include <ostream>
#include "../../includes/core/MatchingEngine.h"
#include "../../includes/core/Order.h"

// Null stream pour désactiver la sortie du Logger
class NullBuffer : public std::streambuf {
public:
    int overflow(int c) override { return c; }
};

static NullBuffer nullBuffer;
static std::ostream nullStream(&nullBuffer);

// Génère un ordre aléatoire
Order generateRandomOrder(long long id, const std::string& instrument) {
    static std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    static std::uniform_real_distribution<double> priceDist(100.0, 200.0);
    static std::uniform_int_distribution<int> qtyDist(1, 100);
    static std::uniform_int_distribution<int> sideDist(0, 1);
    static std::uniform_int_distribution<int> typeDist(0, 1);

    OrderSide side = (sideDist(engine) == 0) ? OrderSide::BUY : OrderSide::SELL;
    OrderType type = (typeDist(engine) == 0) ? OrderType::LIMIT : OrderType::MARKET;
    double price = (type == OrderType::LIMIT) ? priceDist(engine) : 0.0;
    int qty = qtyDist(engine);
    long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return Order(id, instrument, side, type, price, qty, timestamp, 0, 0.0, 0.0);
}

void performanceTest(size_t orderCount) {
    MatchingEngine engine;

    auto start = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < static_cast<long long>(orderCount); ++i) {
        Order order = generateRandomOrder(i + 1, "AAPL");
        engine.submitOrder(order);
    }

    auto end = std::chrono::high_resolution_clock::now();

    double durationMs = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Performance test avec " << orderCount << " ordres soumis :\n";
    std::cout << "Temps total = " << durationMs << " ms\n";
    std::cout << "Temps moyen par ordre = " << durationMs / orderCount << " ms\n";
}

void runMatchingEnginePerf() {
    std::cout << "\n=== Test de performance MatchingEngine ===\n\n";

    size_t tests[] = {1000, 10000, 50000};
    for (auto count : tests) {
        MatchingEngine engine;

        // Désactiver les logs dans la console pour ce moteur
        engine.getLogger().setOutputStream(&nullStream);

        auto start = std::chrono::high_resolution_clock::now();

        for (long long i = 0; i < static_cast<long long>(count); ++i) {
            Order order = generateRandomOrder(i + 1, "AAPL");
            engine.submitOrder(order);
        }

        auto end = std::chrono::high_resolution_clock::now();

        double durationMs = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "Performance test avec " << count << " ordres soumis :\n";
        std::cout << "Temps total = " << durationMs << " ms\n";
        std::cout << "Temps moyen par ordre = " << durationMs / count << " ms\n\n";
    }

    std::cout << "=== Fin des tests MatchingEngine ===\n";
}
