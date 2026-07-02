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

    void closeLog() {// لاقفال الملف 
        logFile.close();
    }


    // طريقه الحل الاولى بالتراجع دون استخدام اي طرق تحسين للحل
    // السيارات و الاماكن توضع مبدئيا بالترتيب المدخلين بيه ويتم التراجع فقط في حالة لم يناسب المكان السيارة الحالية
    bool simpleBacktrack(int carIndex, int assign[], bool spotUsed[], Result &res) {
        if (carIndex == carCount) {
            return true; // هذا يقول لو انه تم ايجاد اماكن لكل السيارات عندها ارجع ترو
        }

        Car current = cars[carIndex];// فقط تخزين داخل متغير ليسهل استعمالة

for (int i = 0; i < spotCount; i++) {// تجربة المواقف كلها لايجاد اموقف المناسب للسيارة الحالية

    res.steps++;
    // الطباعة داخل الملف الخارجي 
    logFile << "Step " << res.steps
            << ": Car " << current.id
            << " (size " << current.size
            << ") checking Spot "
            << spots[i].id
            << " (size " << spots[i].size << ")" << endl;

    if (spotUsed[i]) {// اذا كان الموقع مأخوذ
        logFile << "   Rejected -> Spot already occupied." << endl;
        continue;
    }

    if (spots[i].size < current.size) {// اذا كان حجم الموقع اصغر من حجم السيارة
        logFile << "   Rejected -> Spot too small." << endl;
        continue;
    }

    logFile << "   Accepted -> Assign Car " // اذا كان الموقع مناسب
            << current.id
            << " to Spot "
            << spots[i].id << endl;

    assign[carIndex] = i;// يخزن الموقف في معلومات السيارة 
    spotUsed[i] = true;// يعطي انذار ان المكان تم اخذه

    if (simpleBacktrack(carIndex + 1, assign, spotUsed, res)) // الذهاب للسيارة التالية
        return true;

    logFile << "   Backtracking -> Remove Car " //  اذا احد السيارات لم تجد حل تراجع 
            << current.id
            << " from Spot "
            << spots[i].id << endl;

    assign[carIndex] = -1; // بعد التراجع يحذف الموقع من السيارة السابقه
    spotUsed[i] = false;// يتيح الموقف يلي كان مأخوذ للتجربة عليه
}
        return false; // لما ما يكونش فيه اي مكان مناسب للسياره 
    }

    // هذه اتعبي البيانات الطالعه من الحل البسيط في كلاس الحل للاحتفاظ بالمعلومات المتحصل عليها من الحل واخراجها لاحقا
    Result solveSimpleBacktracking() {
        Result res;
        int assign[MAX_N];
        bool spotUsed[MAX_N];
        for (int i = 0; i < carCount; i++) assign[i] = -1;
        for (int i = 0; i < spotCount; i++) spotUsed[i] = false;

        logFile << "===== Start of Simple Backtracking Solution =====" << endl;

        clock_t start = clock(); // بداية حساب الوقت
        res.success = simpleBacktrack(0, assign, spotUsed, res);
        clock_t end = clock();

        res.timeMs = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        for (int i = 0; i < carCount; i++) res.carToSpot[i] = assign[i];

        logFile << "===== End of Simple Backtracking Solution =====" << endl << endl;
        return res;
    }


    // الحل Backtracking + MRV + LCV + Constraint Propagation
   
    void buildDomains(bool spotUsed[], bool carAssigned[], int domains[][MAX_N], int domainCount[]) {
        for (int c = 0; c < carCount; c++) {
            domainCount[c] = 0;
            if (carAssigned[c]) continue;
            for (int s = 0; s < spotCount; s++) {
                if (!spotUsed[s] && spots[s].size >= cars[c].size) {
                    domains[c][domainCount[c]] = s;
                    domainCount[c]++;
                }
            }
        }
    }


    // اختيار السيارة مع اصغر مكان في المجال (MRV)
    int selectMRV(int domainCount[], bool carAssigned[]) {
        int best = -1;
        int bestSize = 1000000;
        for (int c = 0; c < carCount; c++) {
            if (!carAssigned[c]) {
                if (domainCount[c] < bestSize) {
                    bestSize = domainCount[c];
                    best = c;
                }
            }
        }
        return best;
    }

    // اختيار الموقف المناسب من مجموعة المواقف بعد ترتيب المواقف على حسب الحجم اولا بحيث يخلي الموقف الاكبر يلي تخلي فرص لسيارات اكتر ثم يرتب المواقف المتساوية على حسب الرقم اي الموقف 101 قبل 
   void orderLCV(int carIndex,
              int domain[],
              int domainSize,
              int domains[][MAX_N],
              int domainCount[],
              bool carAssigned[],
              int orderedSpots[],
              int &orderedCount)
{
    orderedCount = domainSize;

    // انسخ الدومين
    for (int i = 0; i < domainSize; i++)
    {
        orderedSpots[i] = domain[i];
    }

    // رتب حسب حجم الموقف ثم رقمه
    for (int i = 0; i < orderedCount - 1; i++)
    {
        for (int j = 0; j < orderedCount - 1 - i; j++)
        {
            bool swapNeeded = false;

            if (spots[orderedSpots[j]].size >
                spots[orderedSpots[j + 1]].size)
            {
                swapNeeded = true;
            }
            else if (spots[orderedSpots[j]].size ==
                     spots[orderedSpots[j + 1]].size &&
                     spots[orderedSpots[j]].id >
                     spots[orderedSpots[j + 1]].id)
            {
                swapNeeded = true;
            }

            if (swapNeeded)
            {
                int tmp = orderedSpots[j];
                orderedSpots[j] = orderedSpots[j + 1];
                orderedSpots[j + 1] = tmp;
            }
        }
    }
}