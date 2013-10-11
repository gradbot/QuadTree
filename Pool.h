template <class T>
struct Pool {
	int capacity;
	T* pool;
	int count;
public:
	Pool(int capacity) : count(0), capacity(capacity), pool(new T[capacity]) {}
	~Pool() { 
		delete pool;
	}

	T* spawn(int i) {
		if (count + i < capacity) {
			T* result = &(pool[count]);
			memset(result, 0, sizeof(T) * i);
			count += i;
			return result;
		} else {
			return nullptr;
		}
	}

	void clear() {
		count = 0;
	}

	int size() {
		return count;
	}

	char* get() {
		return (char*)pool;
	}

	void push_back(const T& value) {
		T* temp = spawn(1);
		*temp = value;
	}
};