#include "./monitor.h"
#include <assert.h>
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
    float min;
    float max;
    bool (*CheckFunc)(float, Vital*);
    string alertMessage;
};

bool vitalRangeCheck(float val, Vital* v) {
    return (val < v->min || val > v->max);
}
bool vitalMinCheck(float val, Vital* v) {
    return (val< v->min);
}

vector<Vital> vitals = {
{95, 102, vitalRangeCheck, "Temperature is critical!"},
{60, 100, vitalRangeCheck, "Pulse Rate is out of range!"},
{90, 100, vitalMinCheck, "Oxygen Saturation out of range!"}
};

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
    vector<float> values = {temperature, pulseRate, spo2};
    int retVal = 1;
    bool check;
    for (size_t i = 0; i < vitals.size(); ++i) {
        check = vitals[i].CheckFunc(values[i], &vitals[i]);
        printAlerts(check, vitals[i]);
        retVal &= (!check);
    }
    return retVal;
}
