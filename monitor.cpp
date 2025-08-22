#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using std::cout;
using std::flush;
using std::string;
using std::vector;
using std::this_thread::sleep_for;
using std::chrono::seconds;

struct Vital {
    int min;
    int max;
    enum CheckType { RANGE, MINIMUM } checkType;
    string alertMessage;
};

 vector<Vital> vitals = {
        {95, 102, Vital::RANGE, "Temperature is critical!"},
        {60, 100, Vital::RANGE, "Pulse Rate is out of range!"},
        {90, 100, Vital::MINIMUM, "Oxygen Saturation out of range!"}
};

bool checkVital(int val, const Vital& v) {
    switch (v.checkType) {
        case Vital::RANGE:
            return (val < v.min || val > v.max);
        case Vital::MINIMUM:
            return (val < v.min);
        default:
            return false;
    }
}

void printWarningAnimation() {
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

void printAlerts(bool check, const Vital& v) {
        if (check) {
            cout << v.alertMessage << "\n";
            printWarningAnimation();
        }
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    vector<int> values = { static_cast<int>(temperature), static_cast<int>(pulseRate), static_cast<int>(spo2) };
    int retVal = 0;
    bool check;
    for (size_t i = 0; i < vitals.size(); ++i) {
        check = checkVital(values[i], vitals[i]);
        printAlerts(check, vitals[i]);
        retVal| = check;
    }
    return retVal;
}
