#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cctype>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void slowPrint(const string& text, int delay_ms = 25) {
    for (char c : text) {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

// Понижение регистра для UTF-8 (ASCII + кириллица)
string toLowerUtf8(const string& s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        if (c < 0x80) {
            out += char(tolower(c));
            ++i;
        } else if (c == 0xD0 && i + 1 < s.size()) {
            unsigned char n = s[i + 1];
            if (n >= 0x90 && n <= 0x9F) {          // А..П
                out += char(0xD0); out += char(0xB0 + (n - 0x90));
            } else if (n >= 0xA0 && n <= 0xAF) {   // Р..Я
                out += char(0xD1); out += char(0x80 + (n - 0xA0));
            } else if (n == 0x81) {                // Ё
                out += char(0xD1); out += char(0x91);
            } else {
                out += s[i]; out += s[i + 1];
            }
            i += 2;
        } else {
            out += s[i++];
        }
    }
    return out;
}

string trim(const string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

class MysteriousNPC {
    struct Riddle {
        string question;
        string answer;
        string hint;
    };

    string name;
    vector<Riddle> riddles;

public:
    MysteriousNPC() : name("Неизвестный") {
        riddles = {
            { "Я не имею рта, но я говорю с тобой. Я не имею тела, но я существую. Что я?",
              "эхо",   "Попробуй крикнуть в пустой комнате..." },
            { "Чем больше из меня берут, тем больше я становлюсь. Что я?",
              "яма",   "Если копать глубже, что останется?" },
            { "Я всегда иду, но никогда не прихожу. Что я?",
              "время", "Оно есть у всех, но его нельзя потрогать." }
        };
    }

    const string& getName() const { return name; }
    size_t riddleCount() const { return riddles.size(); }

    void greet() {
        slowPrint("[СИСТЕМА]: Обнаружено неизвестное соединение...");
        slowPrint("[" + name + "]: Ты здесь. Хорошо. Я не могу сказать тебе всё напрямую.");
        slowPrint("[" + name + "]: Но я могу проверить, достоин ли ты знать больше. Ответь на мои загадки.");
    }

    bool askRiddle(size_t index) {
        if (index >= riddles.size()) return true;
        const Riddle& r = riddles[index];

        slowPrint("\n[" + name + "]: " + r.question);
        cout << "> ";
        string input;
        if (!getline(cin, input)) return false;

        string normalized = toLowerUtf8(trim(input));
        if (normalized == r.answer) {
            slowPrint("[" + name + "]: ...Верно. Ты видишь чуть больше, чем остальные.");
            return true;
        }
        slowPrint("[" + name + "]: ...Нет. Слабо. Попробуй ещё раз.");
        slowPrint("[" + name + "]: Подсказка: " + r.hint);
        return false;
    }

    void farewell() {
        slowPrint("\n[" + name + "]: Ты прошёл. Но это только начало. Я найду тебя снова.");
        slowPrint("[СИСТЕМА]: Соединение разорвано.");
    }
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");

    MysteriousNPC npc;
    npc.greet();

    const int maxAttempts = 2;

    for (size_t i = 0; i < npc.riddleCount(); ++i) {
        bool solved = false;
        for (int attempt = 0; attempt < maxAttempts && !solved; ++attempt)
            solved = npc.askRiddle(i);

        if (!solved) {
            slowPrint("\n[СИСТЕМА]: Неизвестный разочарован твоей неспособностью.");
            slowPrint("[" + npc.getName() + "]: ...Слабо. Возвращайся, когда будешь готов.");
            return 0;
        }
    }

    npc.farewell();
    return 0;
}
