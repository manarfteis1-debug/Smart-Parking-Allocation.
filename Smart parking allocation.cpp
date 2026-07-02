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


// كلاس ايجاد الحلول
class ParkingCSP {
private: //يحتوي هذا الكلاس على 
    Car cars[MAX_N];//  جميع السيارات حيث الموقع في المصفوفه هي السيارة والقيمه الداخليه هي الحجم
    Spot spots[MAX_N];// جميع المواقف حيث الموقع في المصفوفه هو الموقف و القيمة هي حجم الموقف
    int carCount;// عدد السيارات 
    int spotCount;//عدد المواقف لكي لا نظطر للمرور على اكتر من العدد الموجود للمقارنه مع السيارات

    ofstream logFile; // لكتابة البيانات داخل ملف خارجي لكي يسهل على المبرمجات تتبعه ومعرفة ان كان الكود به خطأ

public:
    ParkingCSP(Car carArr[], int cCount, Spot spotArr[], int sCount) {
        carCount = cCount; //لادخال عدد السيارات من المستخدم الى الدالة
        spotCount = sCount;//لادخال عدد المواقف من المستخدم الى الدالة 
        for (int i = 0; i < carCount; i++) cars[i] = carArr[i]; // لادخال حجم كل سيارة في المصفوفة 
        for (int i = 0; i < spotCount; i++) spots[i] = spotArr[i];// لادخال حجم كل موقف الى المصفوفه 
    }

    void openLog(const string &filename) {
        logFile.open(filename, ios::app); // لفتح الملف الذي سنطبع فيه
    }
