// TASK 1: Make class into a template that can store any two data types
class Pair {
public:
	Pair(int first, int second): first(first), second(second) {}
	int get_first() { return first; }
	int get_second() { return second; }

private:
	int first;
	int second;
};
