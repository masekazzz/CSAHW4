#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <omp.h>

using namespace std;


class Boy {
public:
    int number;

    string message;

    bool isLucky = false;

    Boy(int n) {
        number = n + 1;
        message = "Привет, я №" + to_string(number) + ". Пойдешь со мной на свидание?";
    }
};

bool isAgreed;


static void makeDecision(Boy &boy) {
#pragma omp critical
    {
        if (!isAgreed) {
            cout << boy.message << endl;
            cout << "Введите 1, чтобы согласиться или 2, чтобы отказаться" << endl;
            cout << "> ";
            int N;
            cin >> N;
            while (N > 2 || N < 1) {
                cout << "Неверный ввод, введите 1, чтобы согласиться или 2, чтобы отказаться" << endl;
                cin.clear();
                cin.ignore(32767, '\n');
                cin >> N;
            }
            if (N == 1)
                isAgreed = true;
            boy.isLucky = N == 1;
        } else
            boy.isLucky = false;
    }
}

static void sendAnswer(const Boy &boy) {
#pragma omp critical
    {
        if (boy.isLucky)
            cout << "Я выбираю тебя №" + to_string(boy.number) + "!" << endl;
        else
            cout << "Прости, №" + to_string(boy.number) + ", дело не в тебе, дело во мне..." << endl;
    }
}


int main() {
    vector<Boy> boys;
    int N;

    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "Введите N > 0" << endl;
    cout << "> ";
    cin >> N;
    while (N < 1) {
        cout << "Неверный ввод, введите целое число больше нуля " << endl;
        cin.clear();
        cin.ignore(32767, '\n');
        cin >> N;
    }
    for (int i = 0; i < N; ++i) {
        boys.emplace_back(Boy(i));
    }
    omp_set_dynamic(0);
#pragma omp parallel for num_threads(N)
    for (int i = 0; i < N; ++i) {
        makeDecision(ref(boys[i]));
    }
#pragma omp parallel for num_threads(N)
    for (int i = 0; i < N; ++i) {
        sendAnswer(boys[i]);
    }
    return 0;
}