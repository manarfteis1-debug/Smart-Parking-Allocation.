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


// كلاس النتايج انخزن فيه المعلومات المهم عن كل طريقة حل
class Result {
public:
    int carToSpot[MAX_N]; // الرقم ابتع كل موقع مأخوذ من قبل السيارة (س)
    double timeMs;          // الوقت المستغرق لايجاد الحل باجزاء الثانية (مللي ثانية)
    int steps;               // عدد الخطوات يلي خذيناهم لكي وصلنا للحل النهائي (عدد المحاولات)
    bool success; // هل تم ايجاد حل ام لا

    Result() {
        // اعطاء قيم افتراضية لتجنب اخذ قيم عشوائية
        timeMs = 0;
        steps = 0;
        success = false;
        for (int i = 0; i < MAX_N; i++) carToSpot[i] = -1;//-1 للتعبير عن ان السيارة لم تاخذ موقف بعد اعطيناها قيمة سالبة لايمكن ان تكون موقف
    }
};