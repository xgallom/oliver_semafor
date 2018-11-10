#include <iostream>
#include <thread>

#ifdef ATOMIC
#include "shared_number_atomic.h"
#else
#include "shared_number_cv.h"
#endif

using namespace std;

int main()
{
	SharedNumber sharedNumber;

	thread writingThread([&sharedNumber]() {
		cout << "Writing Thread: Started\n";

		int number;

		while((number = sharedNumber.readWhenReady()) != -1)
			cout << "Writing Thread: Number entered: " << number << endl;
	});

	thread readingThread([&sharedNumber]() {
		cout << "Reading Thread: Started\n";

		for(;;) {
			int number;

			cout << "Reading Thread: Enter an integer, and -1 to quit:\n";
			cin >> number;

			sharedNumber.writeWhenProcessed(number);

			if(number == -1)
				break;
		}
	});

	cout << "Main: Waiting for writing thread to finish\n";
	writingThread.join();

	cout << "Main: Waiting for reading thread to finish\n";
	readingThread.join();

	return 0;
}

