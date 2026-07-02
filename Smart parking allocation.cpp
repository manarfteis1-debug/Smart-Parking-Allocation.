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

 // هذا لكي يتأكد بعد كل عملية توزيع ان باقي السيارات عندهم مكان واحد على الاقل في المجال ابتعهم
    bool forwardCheckOK(int domainCount[], bool carAssigned[]) {
        for (int c = 0; c < carCount; c++) {
            if (!carAssigned[c] && domainCount[c] == 0) {
                return false; // لكي يعلم مبكرا ان فيه سيارة من غير مكان 
            }
        }
        return true;
    }

    //  هذه هي لاخذ الحساب وطباعته في الملف الخارجي 
    bool smartBacktrack(int assign[], bool spotUsed[], bool carAssigned[], Result &res, int remaining) {
        if (remaining == 0) 
        {
            logFile << "\nAll cars assigned successfully." << endl;
            return true;
        }

        int domains[MAX_N][MAX_N];
        int domainCount[MAX_N];
        buildDomains(spotUsed, carAssigned, domains, domainCount);

        logFile << "\n==============================" << endl;
        logFile << "Current Domains:" << endl;

        for (int c = 0; c < carCount; c++) {

            if (carAssigned[c])
                continue;

            logFile << "Car " << cars[c].id << " : { ";

            for (int j = 0; j < domainCount[c]; j++) {
                logFile << spots[domains[c][j]].id << " ";
            }

            logFile << "}  (" << domainCount[c] << " choices)" << endl;
        }

        logFile << "==============================" << endl;

        // اختيار افضل سيارة اي يلي عندها اقل عدد مواقف
        int carIndex = selectMRV(domainCount, carAssigned);
        logFile << "[MRV] Selected Car "
        << cars[carIndex].id
        << " because it has the smallest domain ("
        << domainCount[carIndex]
        << " possible spots)." << endl;

        if (carIndex == -1) return true;

        if (domainCount[carIndex] == 0) {
            // لو مافيش حل لاحد السيارات 
            logFile << "[Smart-CSP] Failure: no available spots for Car " << cars[carIndex].id << endl;
            return false;
        }

        // ترتيب المواقف
        int orderedSpots[MAX_N];
        int orderedCount = 0;
        orderLCV(carIndex, domains[carIndex], domainCount[carIndex], domains, domainCount,
                 carAssigned, orderedSpots, orderedCount);
        
        logFile << "[LCV] Spot order for Car "
        << cars[carIndex].id
        << " : ";

        for(int i=0;i<orderedCount;i++)
        {
            logFile << spots[orderedSpots[i]].id << " ";
        }

        logFile << endl;


        for (int k = 0; k < orderedCount; k++) {
            int s = orderedSpots[k];
            res.steps++;
            logFile << "\nStep " << res.steps << endl;

        logFile << "Trying Car "
                << cars[carIndex].id
                << " -> Spot "
                << spots[s].id
                << endl;
            // توزيع مؤقت
            assign[carIndex] = s;
            spotUsed[s] = true;
            carAssigned[carIndex] = true;

            // تحديث المجالات بعد كل اختيار
            int newDomains[MAX_N][MAX_N];
            int newDomainCount[MAX_N];
            buildDomains(spotUsed, carAssigned, newDomains, newDomainCount);
            logFile << "Domains after assignment:" << endl;

            for(int c=0;c<carCount;c++)
            {
                if(carAssigned[c])
                continue;

                logFile << "Car " << cars[c].id << " : ";

                for(int j=0;j<newDomainCount[c];j++)
                    logFile << spots[newDomains[c][j]].id << " ";

                    logFile << endl;
            }

            if (forwardCheckOK(newDomainCount, carAssigned)) {
                if (smartBacktrack(assign, spotUsed, carAssigned, res, remaining - 1)) {
                    return true;
                }
            } else {
                logFile << "[Forward Checking]" << endl;

                for(int c=0;c<carCount;c++)
                {
                    if(!carAssigned[c] && newDomainCount[c]==0)
                    {
                        logFile << "Car "
                        << cars[c].id
                        << " has NO remaining spot." << endl;
                    }
                }

                logFile << "Reject assignment of Car "
                        << cars[carIndex].id
                        << " -> Spot "
                        << spots[s].id
                        << endl;
                    }

            //التراجع في حال عدم ايجاد موقع مناسب 
            logFile << "[Backtracking]" << endl;

            logFile << "Undo assignment :" << endl;

            logFile << "Car "
                    << cars[carIndex].id
                    << " removed from Spot "
                    << spots[s].id
                    << endl;
        }

        return false;
    }

     Result solveSmartCSP() {
        Result res;
        int assign[MAX_N];
        bool spotUsed[MAX_N];
        bool carAssigned[MAX_N];

        for (int i = 0; i < carCount; i++) {
            assign[i] = -1;
            carAssigned[i] = false;
        }
        for (int i = 0; i < spotCount; i++) spotUsed[i] = false;

        logFile << "===== Start of Backtracking + MRV + LCV + Constraint Propagation Solution =====" << endl;

        clock_t start = clock(); // البدأ في حساب الوقت
        res.success = smartBacktrack(assign, spotUsed, carAssigned, res, carCount);
        clock_t end = clock();

        res.timeMs = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        for (int i = 0; i < carCount; i++) res.carToSpot[i] = assign[i];

        logFile << "===== End of Backtracking + MRV + LCV + Constraint Propagation Solution =====" << endl << endl;
        return res;
    }

    // لطباعه النتيجة على الشاشه
    void printResult(const string &title, Result &res) {
        cout << "---------------------------------------------" << endl;
        cout << title << endl;
        cout << "---------------------------------------------" << endl;

        if (!res.success) {
            cout << "No solution found for all cars." << endl;
        } else {
            for (int i = 0; i < carCount; i++) {
                if (res.carToSpot[i] != -1) {
                    int spotIndex = res.carToSpot[i];
                    cout << "Car " << cars[i].id << " (size " << cars[i].size
                         << ")  ->  Spot " << spots[spotIndex].id
                         << " (size " << spots[spotIndex].size << ")" << endl;
                } else {
                    cout << "Car " << cars[i].id << " did not find a suitable spot" << endl;
                }
            }
        }

        cout << "Number of search steps (attempts): " << res.steps << endl;
        printf("Time taken: %.4f ms\n", res.timeMs);
        cout << "---------------------------------------------" << endl << endl;
    }
};

int main() {

    int carCount = 0;
    cout << "Enter number of cars: ";
    cin >> carCount;

    Car cars[MAX_N];
    cout << "Enter size of each car (1 = small, 2 = medium, 3 = large):" << endl;
    for (int i = 0; i < carCount; i++) {
        int s;
        cout << "Car " << (i + 1) << " size: ";
        cin >> s;
        cars[i] = Car(i + 1, s);
    }

    int spotCount = 0;
    cout << "Enter number of parking spots: ";
    cin >> spotCount;

    Spot spots[MAX_N];
    cout << "Enter size of each spot (1 = small, 2 = medium, 3 = large):" << endl;
    for (int i = 0; i < spotCount; i++) {
        int s;
        cout << "Spot " << (i + 1) << " size: ";
        cin >> s;
        spots[i] = Spot(100 + i + 1, s);
    }

    //اسم الملف الخارجي
    string logFileName = "search_trace.txt";

    // تنضيف الملف من النتائج السابقه وادخال النتائج الجديده في كل مره اندير فيها run
    ofstream clearFile(logFileName);
    clearFile << "Search Trace file for the Car Parking CSP project" << endl;
    clearFile << "=====================================================" << endl << endl;
    clearFile.close();

    // تشغيل الحل البسيط
    ParkingCSP csp1(cars, carCount, spots, spotCount);
    csp1.openLog(logFileName);
    Result result1 = csp1.solveSimpleBacktracking();
    csp1.closeLog();

    // تشغيل الحل المحسن
    ParkingCSP csp2(cars, carCount, spots, spotCount);
    csp2.openLog(logFileName);
    Result result2 = csp2.solveSmartCSP();
    csp2.closeLog();

    // طباعه الحل للمقارنة
    csp1.printResult("Result 1: Simple Backtracking", result1);
    csp2.printResult("Result 2: Backtracking + MRV + LCV + Constraint Propagation", result2);

    cout << "===================== Comparison =====================" << endl;
    cout << "Simple Backtracking steps : " << result1.steps << endl;
    cout << "Smart CSP steps           : " << result2.steps << endl;
    printf("Simple Backtracking time  : %.4f ms\n", result1.timeMs);
    printf("Smart CSP time            : %.4f ms\n", result2.timeMs);
    cout << "All search steps were saved in file: " << logFileName << endl;
    cout << "=====================================================" << endl;

    return 0;
}

