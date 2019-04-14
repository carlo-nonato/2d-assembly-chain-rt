#include "randint.hpp"

static std::random_device rd;
static std::default_random_engine engine(rd());

int randint(int min, int max) {
    return std::uniform_int_distribution<int>{min, max}(engine);
}