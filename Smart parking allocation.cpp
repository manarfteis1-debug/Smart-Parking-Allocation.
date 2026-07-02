#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int MAX_N = 50; // اكبر رقم ممكن من السيارات تقدر ادخله

class Car {
public:
    int id;
    int size; // حجم السيارة (1 = small, 2 = medium, 3 = large)

    Car() {
        id = 0;
        size = 0;
    }

    Car(int id, int size) {
        this->id = id;
        this->size = size;
    }
};

class Spot {
public:
    int id;
    int size; // حجم الموقف (1 = small, 2 = medium, 3 = large)

    Spot() {
        id = 0;
        size = 0;
    }

    Spot(int id, int size) {
        this->id = id;
        this->size = size;
    }
};