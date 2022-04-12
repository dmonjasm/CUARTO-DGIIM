#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

std::random_device device;
std::default_random_engine engine(device());
std::uniform_int_distribution distribution;
auto rng = std::bind(distribution, engine);
// std::ranlux24_base rng(device());

template<class Iterator> void init_a(Iterator begin, Iterator end)
{
	std::generate(begin, end, rng);
}

template<class Iterator> void init_b(Iterator begin, Iterator end)
{
	std::iota(begin, end, 0);
}

int main()
{
	const size_t N = 1 << 24;
	std::vector<int> a(N), b(N);
	
	init_a(a.begin(), a.end());
	init_b(b.begin(), b.end());
	
	return std::accumulate(a.begin(), a.end(), 0) + 
	       std::accumulate(b.begin(), b.end(), 0);
}
