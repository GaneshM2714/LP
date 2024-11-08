#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class LamportClock {
private:
    int clock;

public:
    LamportClock() : clock(0) {}

    // Get the current time
    int getTime() const {
        return clock;
    }

    // Event occurs in the local process
    void tick() {
        clock += 1;
    }

    // Receive a message with a timestamp from another process
    void receiveEvent(int receivedTime) {
        clock = max(clock, receivedTime) + 1;
    }

    // Send a message with a timestamp
    int sendEvent() {
        tick();
        return getTime();
    }
};

int main() {
    LamportClock process1, process2;

    cout << "Initial Clock Times:\n";
    cout << "Process 1: " << process1.getTime() << "\n";
    cout << "Process 2: " << process2.getTime() << "\n";

    // Simulating events
    process1.tick();
    cout << "Process 1 executes an event, time: " << process1.getTime() << "\n";

    int messageTime = process1.sendEvent();
    cout << "Process 1 sends a message with timestamp " << messageTime << "\n";

    process2.receiveEvent(messageTime);
    cout << "Process 2 receives the message and updates time: " << process2.getTime() << "\n";

    process2.tick();
    messageTime = process2.getTime();
    cout << "Process 2 executes another event, time: " << messageTime << "\n";
    process1.receiveEvent(messageTime);
    cout<<process1.getTime();

    return 0;
}
