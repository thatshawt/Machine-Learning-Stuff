#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <iterator>
#include <map>
#include <ostream>
#include <string>

#ifdef max
#undef max
#endif // DEBUG

#ifdef min
#undef min
#endif // DEBUG

//https://gist.github.com/cbsmith/5538174
template <typename RandomGenerator = std::default_random_engine>
struct random_selector
{
	//On most platforms, you probably want to use std::random_device("/dev/urandom")()
	random_selector(RandomGenerator g = RandomGenerator(std::random_device()()))
		: gen(g) {}

	template <typename Iter>
	Iter select(Iter start, Iter end) {
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(gen));
		return start;
	}

	//convenience function
	template <typename Iter>
	Iter operator()(Iter start, Iter end) {
		return select(start, end);
	}

	//convenience function that works on anything with a sensible begin() and end(), and returns with a ref to the value type
	template <typename Container>
	auto operator()(const Container& c) -> decltype(*begin(c))& {
		return *select(begin(c), end(c));
	}

private:
	RandomGenerator gen;
};

template <class T>
static void fill_n(std::vector<T> v, T n) {
	for (int i = 0; i < v.capacity(); i++) {
		v.push_back(n);
	}
}

template<class type>
std::string array_to_string(type* begin, int size) {
	std::string str = "{ ";
	for (int i = 0; i < size; i++) {
		str.append(std::to_string(*(begin+i)) + ", ");
	}
	str.append(" }");
	return str;
}

template<class type>
static void print_array(type* begin, int size) {
	for (int i = 0; i < size;i++) {
		printf("%d, ", *((begin + i)) );
	}
	printf("\n");
}
template <class type>
static void print_array(type data, int length) {
	print_array<type>(data, length, ', ');
}

template <class type>
static void print_array(type data, int length, char separator) {
	printf("{ ");
	for (int i = 0; i < length; i++) {
		printf("%s%c", to_string(data[i]), separator);
	}
	printf(" }\n");
}

//https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c/16388594
static constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

//https://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
static std::string toLower(std::string val) {
	std::string data(val);
	std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return data;
}

//probably also from stackoverflow if we are going to be honest here
static inline bool isInteger(const std::string & s){
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

//i actually made something
static std::vector<std::string>splitString(const char* str, int size, char delimiter) {
	std::vector<std::string> strings;

	bool isDelimiter = false;
	std::string currentString;
	for (int i = 0; i < size; i++) {
		const char* c = &str[i];

		if (*c == delimiter && !isDelimiter) {
			if(!currentString.empty())strings.push_back(std::string(currentString));
			currentString.clear();
			isDelimiter = true;
		} else if(*c != delimiter) {
			currentString.push_back(*c);
			isDelimiter = false;
		}
	}

	if (!isDelimiter) {
		if (!currentString.empty())strings.push_back(std::string(currentString));
		currentString.clear();
		isDelimiter = true;
	}

	return strings;
}

static std::vector<std::string> splitString(std::string* str, char delimiter) {
	return splitString(str->c_str(), str->size(), delimiter);
}

//static std::default_random_engine generator;
static std::minstd_rand generator(time(NULL));

static const std::uniform_int_distribution<int> intDist(std::numeric_limits<int>::min(),
	std::numeric_limits<int>::max());
static int irand() {
	return intDist(generator);
}

//static double D_MAX = std::numeric_limits<double>::max() - 1;
static const std::uniform_real_distribution<double> dDist(0,1);
static double drand() {
	return dDist(generator);
}

//static float MAX = std::numeric_limits<float>::max();
static const std::uniform_real_distribution<float> fDist(0, 1);
static float frand() {
	//return fmap(dist(generator), (float)0, MAX, (float)0, max);
	return fDist(generator);
}

static float fmap(float value, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

static double dmap(double value, double start1, double stop1, double start2, double stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

static float frand(float max) {
	float num = frand();
	//printf("%f\n", num);
	//return fmod(num, max);
	return frand() * max;
}

static double drand(double max) {
	//return fmod(drand(), max);
	return drand() * max;
}

static int irand(int max) {
	return (int)drand((double)max);
}

//from https://www.shannonentropy.netmark.pl/
static double shannonEntropy(std::string str) {
	std::map<char, int> frequencyTable;
	
	for (int i = 0; i < str.size();i++) {
		char c = str.at(i);
		if (frequencyTable.count(c))//its in there
			frequencyTable.insert({c, frequencyTable.at(c) + 1});
		else
			frequencyTable.insert({ c, 1 });
	}

	float sum = 0;
	for (auto iter = frequencyTable.begin(); iter != frequencyTable.end(); iter++) {
		float num = (float)iter->second / (float)str.size();
		sum += num*log2(num);
	}

	return -sum;
}

//https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
namespace Color {
	enum Code {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};
	
	static std::string toString(Code code) {
		return "\033[" + std::to_string(code) + "m";
	}
}

static std::string combine(std::vector<std::string> strings) {
	std::string str;
	for (int i = 0; i < strings.size(); i++) {
		str.append(strings[i]);
	}
	return str;
}

static double shannonEntropy(std::vector<std::string> strings) {
	return shannonEntropy(combine(strings));
}

static std::string removeSpace(std::string str) {
	std::string result;
	for (int i = 0; i < str.size(); i++) {
		if (!isspace(str[i]))result += str[i];
	}
	return result;
}

static int toNum(std::string str) {
	std::string result;
	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if (!isdigit(c))return 0;;
	}
	return 1;
}

static void bin(unsigned n) {
	//thx stackoverflow
	/* step 1 */
	if (n > 1)
		bin(n / 2);

	/* step 2 */
	printf("%d", n % 2);
}

static void printBitArray(unsigned char* bytes, int length, int offset) {
	for (int i = 0; i < length; i++) {
		bin(bytes[i]); printf(" ");
	}
	printf("\n");
}

//not sure how trash this is but lol ok
class Timer {
private:
	std::chrono::steady_clock::time_point time;
public:
	void start(void) {
		this->time = std::chrono::high_resolution_clock::now();
	}

	long long getNano(void) {
		auto finish = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(finish - time).count();
	}

	long long getMilli(void) {
		auto finish = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(finish - time).count();
	}
};

//http://www.cplusplus.com/reference/algorithm/max/
template <class T> const T& max(const T& a, const T& b) {
	return (a < b) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}