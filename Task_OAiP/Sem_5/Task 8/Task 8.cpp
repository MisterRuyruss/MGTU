#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>

using namespace std;

int main() {
 
    vector<int> speeds(50);

    for (int i = 0; i < 50; i++) speeds[i] = rand() % 181;

    cout << "Все скорости: " << endl;
    for_each(speeds.begin(), speeds.end(), [](int x) {
        cout << x << " ";
        });
    cout << endl;

    
    vector<int> ovr_spd;
    copy_if(speeds.begin(), speeds.end(), back_inserter(ovr_spd), [](int x) {return x > 120;});

    cout << "Превышения скорости: " << endl;
    for_each(ovr_spd.begin(), ovr_spd.end(), [](int x) {
        cout << x << " ";
        });
    cout << endl;

    
    double cr = accumulate(speeds.begin(), speeds.end(), 0.0) / speeds.size();
    cout << "Средняя скорость: " << cr << endl;

    
    bool always_moving = all_of(speeds.begin(), speeds.end(), [](int x) {
        return x > 0;
        });
    if (always_moving) {
        cout << "Автомобиль всегда двигался" << endl;
    }
    else {
        cout << "Автомобиль не всегда двигался" << endl;
    }

    
    vector<int> sr_spd = speeds;
    sort(sr_spd.begin(), sr_spd.end(), greater<int>());

    cout << "10 максимальных скоростей: " << endl;
    for_each(sr_spd.begin(), sr_spd.begin() + min(10, (int)sr_spd.size()), [](int x) {
        cout << x << " ";
        });
    cout << endl << endl;

    
}