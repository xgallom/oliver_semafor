#include <atomic>

using namespace std;

class SharedNumber {
	atomic<bool> ready = false;
	int number = 0;

	void cpuRelax() const
	{
		for(int n = 0; n < 10; ++n)
			asm("pause\n");
	}

public:
	int readWhenReady()
	{
		while(!ready.load(memory_order_acquire))
			cpuRelax();


		int returnValue = number;

		ready.store(false, memory_order_release);

		return returnValue;
	}

	void writeWhenProcessed(int newNumber)
	{
		while(ready.load(memory_order_acquire))
			cpuRelax();

		number = newNumber;

		ready.store(true, memory_order_release);
	}
};

