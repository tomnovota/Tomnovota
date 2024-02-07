template<typename TFirst, typename TSecond>
class Pair {
public:
	Pair(TFirst first, TSecond second): first(first), second(second) {}
	TFirst get_first() { return first; }
	TSecond get_second() { return second; }

private:
	TFirst first;
	TSecond second;
};
