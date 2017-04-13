#include <bits/stdc++.h>

using namespace std;



const int N = 1000;
int n, L, best, worth_line, sum = 0, iter = 0;
double  x[1000], B[1000], sx_tab[N][N], delta[N], v[N], base[N], z[N], solve = 0;
int  l[1000], ans[1000];
double target[N];


struct Formater {
    std::vector<std::pair<std::string, bool> > strings;
    std::size_t width;

    Formater() {
        cout.setf(ios::fixed | ios::showpoint);
        cout.precision(2);
    }

    ~Formater() {
        flush();
    }

    template <typename T>
    void print(const T &t, bool resize = true) {
        std::stringstream ss;
        ss.setf(ios::fixed | ios::showpoint);
        ss.precision(2);
        ss << t;
        strings.emplace_back(ss.str(), resize);
    }

    void flush() {
        width = 0;
        for (auto str : strings) {
            if (str.second) {
                width = std::max(width, str.first.length());
            }
        }
        width += 1;

        for (auto &str : strings) {
            if (str.second)
                std::cout << std::string(width - str.first.length(), ' ') + str.first << " ";
            else
            std::cout << str.first << " ";
        }

        strings.clear();
    }
} formater;

/*Создаём таблицу*/
void create_table() {

    for (int i = 0; i < n; i++) {
        base[i] = -200;
    }

    for (int i = 0; i < n; i++) {
        sx_tab[i][0] = B[i];
    }

    for (int i = 0; i < n; i++) {
        delta[i] = -100;
    }

    for (int i = 0; i < n; i++) {
        sx_tab[n][i + 1] = delta[i];
    }

    for (int i = 0; i < n; i++) {
        sx_tab[i][i + 1] = 1;
    }
}

/*Выводим таблицу на экран*/
void print_table() {

    static int iter = 1;

    formater.print("Table: \n", false);
    formater.print("Iteration = ", false);
    formater.print(iter, false);
    formater.print("\n");
    int k = 0;
    for (int i = 0; i < n + 1; i++) {
        formater.print(k + 1);
        for (int j = 0; j < n + 2; j++) {
            formater.print(sx_tab[i][j]);
        }
        formater.print("\n");
        k++;
    }

    formater.print("\n");
    formater.print("Vect: \n", false);
    for (int i = 0; i < n; i++) {
        formater.print((-1) * v[i]);
    }

    formater.print("\n\n");
    formater.print("Solve = \n", false);
    formater.print(solve);
    formater.print("\n\n");
    iter++;
}

/*Вычисляем z*/
void evaluate_z() {

    for (int i = 0; i < n; i++) {
        z[i] = 0;
        for (int j = 0; j < n; j++) {
            z[i] += sx_tab[i][j + 1] * x[j];
        }
    }

    for (int i = 0; i < n; i++) {
        sx_tab[i][n + 1] = z[i];
    }
}

/*Линейный раскрой*/
void linear_cutting() {
    for (int i = 0; i <= L; i++) {
        ans[i] = 0;
        target[i] = 0;
        for (int j = 0; j < n; j++) {
            if (l[j] <= i && (target[i] < target[i - l[j]] + (-1) * v[j])) {
                target[i] = target[i - l[j]] + (-1) * v[j];
                ans[i] = j + 1;
            }
        }
    }
}

/*Расшифровка ответа линейного раскроя*/
void show_opt_cutting() {
    iter++;
    for (int i = 0; i < n; i++) {
        x[i] = 0;
    }
    int y = L;
    while (ans[y] > 0) {
        x[ans[y] - 1]++;
        y -= l[ans[y] - 1];
    }
}

double check_solve() {

    solve = 0;
    for (int i = 0; i < n; i++) {
        solve += z[i] * (-1) * delta[i];
    }

    return solve;
}

void check_base() {

    for (int i = 0; i < n; i++) {
        if (base[i] != -200) {
            delta[i] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        sx_tab[n][i + 1] = delta[i];
    }
}

/*Вычисляем двойственные переменные*/
void evaluate_dual_variables() {

  for (int i = 0; i < n; i++) {
        v[i] = 0;
        for (int j = 0; j < n; j++) {
            v[i] += delta[j] * sx_tab[j][i + 1];
        }
    }
}

/*Находим индекс подходящего элемента в стоблце*/
void find_best() {
    int min = INT_MAX;
    best = (int)(min_element(z, z + n) - z);
    for (int i = 0; i < n; i++) {
        if (z[i] < min && z[i] != 0) {
            best = i;
        }
    }
}

/*Находим строку, которая будет ведущей*/
void find_worth_line() {

    worth_line = -1;
    double worth_value = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (sx_tab[i][n + 1] > 0 && worth_value > sx_tab[i][0] / sx_tab[i][n + 1]) {
            worth_value = sx_tab[i][0] / sx_tab[i][n + 1];
            worth_line = i;
        }
    }

    base[worth_line] = 1;

}

/*Вычисляем таблицу*/
bool evaluate_table() {

    check_base();
    evaluate_dual_variables();
    linear_cutting();
    show_opt_cutting();
    evaluate_z();
    find_best();
    find_worth_line();
    if (check_solve() - 1 <= 1e-6) {
        return true;
    }

    print_table();
    check_solve();

    double coefficient = sx_tab[worth_line][n + 1];
    for (int i = 0; i < n + 2; i++) {
        sx_tab[worth_line][i] /= coefficient;
    }

    for (int i = 0; i < n + 1; i++) {
        if (i == worth_line) continue;
        coefficient = sx_tab[i][n + 1];
        for (int j = 0; j < n + 2; j++) {
            sx_tab[i][j] -= coefficient * sx_tab[worth_line][j];
        }
    }
    if (worth_line == -1) {
        cout << "Функция неограничена сверху, решения не существует" << endl;
        return true;
    }

    if (check_solve() - 1 <= 0) {
           return true;
    }
    return false;
}

int main() {

    freopen("input", "r", stdin);
    
    freopen("output", "w", stdout);

    cin >> L >> n;
    for (int i = 0; i < n; i++) {
        cin >> l[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> B[i];
    }
    cout.precision(2);
    cout.width(10);
    create_table();

    while(!evaluate_table());
    double value = 0;
    for (int i = 0; i < n; i++) {
        value += sx_tab[i][0];
    }
    formater.print("Finish table", false);
    formater.print("\n\n");
    print_table();
    formater.print("Volume of cut: \n", false);
    formater.print(value);
    formater.print("\n");

    return 0;
}
