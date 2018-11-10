#include <mutex>
#include <condition_variable>

using namespace std;

class SharedNumber {
	mutex mx;
	condition_variable cv;

	bool ready = false;
	int number;

public:
	int readWhenReady()
	{
		unique_lock<mutex> lock(mx);
		cv.wait(lock, [this]() { return ready; });

		ready = false;
		int returnValue = number;

		lock.unlock();
		cv.notify_one();

		return returnValue;
	}

	void writeWhenProcessed(int newNumber)
	{
		unique_lock<mutex> lock(mx);
		cv.wait(lock, [this]() { return !ready; });

		ready = true;
		number = newNumber;

		lock.unlock();
		cv.notify_one();
	}
};


