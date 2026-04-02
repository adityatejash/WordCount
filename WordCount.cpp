// WordCount.cpp — Enhanced C++ Word Count Backend

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <fstream>

using namespace std;

// ─── Platform socket headers ────────────────────────────────────────────────
#ifdef _WIN32
  #include <winsock2.h>
  #pragma comment(lib,"ws2_32.lib")
  typedef int socklen_t;
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <unistd.h>
  #define SOCKET int
  #define INVALID_SOCKET -1
  #define closesocket close
#endif

// ══════════════════════════════════════════════════════════════════════════════
//  ANALYSIS FUNCTIONS
// ══════════════════════════════════════════════════════════════════════════════

int Count_Words(const string& text)
{
    int count = 0;
    bool inWord = false;
    for (unsigned char ch : text) {
        if (isspace(ch)) inWord = false;
        else if (!inWord) { inWord = true; ++count; }
    }
    return count;
}

int Count_Characters(const string& text)
{
    return (int)text.size();
}

int Count_Sentences(const string& text)
{
    int count = 0;
    for (char ch : text)
        if (ch == '.' || ch == '!' || ch == '?') ++count;
    return count;
}

int Count_Spaces(const string& text)
{
    int count = 0;
    for (char ch : text)
        if (ch == ' ' || ch == '\t') ++count;
    return count;
}

int Count_Lines(const string& text)
{
    int count = 0;
    for (char ch : text)
        if (ch == '\n') ++count;
    return count;
}

int Count_Syllables_In_Word(const string& word)
{
    if (word.empty()) return 1;
    string w = word;
    for (char& c : w) c = tolower(c);

    if (w.size() > 1 && w.back() == 'e') w.pop_back();

    int syllables = 0;
    bool lastVowel = false;
    string vowels = "aeiouy";

    for (char c : w) {
        bool isVowel = (vowels.find(c) != string::npos);
        if (isVowel && !lastVowel) ++syllables;
        lastVowel = isVowel;
    }
    return max(syllables, 1);
}

int Count_Total_Syllables(const string& text)
{
    int total = 0;
    istringstream ss(text);
    string token;
    while (ss >> token) {
        string word;
        for (char c : token)
            if (isalpha(c)) word += c;
        if (!word.empty())
            total += Count_Syllables_In_Word(word);
    }
    return total;
}

struct ReadabilityResult {
    double score;
    string grade;
};

ReadabilityResult Flesch_Score(const string& text)
{
    int words = Count_Words(text);
    int sentences = Count_Sentences(text);
    int syllables = Count_Total_Syllables(text);

    if (words == 0 || sentences == 0)
        return {0.0, "N/A"};

    double score = 206.835
                 - 1.015 * ((double)words / sentences)
                 - 84.6  * ((double)syllables / words);

    score = max(0.0, min(100.0, score));

    string grade;
    if      (score >= 90) grade = "Very Easy";
    else if (score >= 80) grade = "Easy";
    else if (score >= 70) grade = "Fairly Easy";
    else if (score >= 60) grade = "Standard";
    else if (score >= 50) grade = "Fairly Difficult";
    else if (score >= 30) grade = "Difficult";
    else                  grade = "Very Confusing";

    return {score, grade};
}

string Normalize_Sentence(const string& s)
{
    string out;
    for (unsigned char c : s) {
        if (isalnum(c)) out += tolower(c);
        else if (isspace(c)) out += ' ';
    }

    string result;
    bool lastSpace = true;
    for (char c : out) {
        if (c == ' ') {
            if (!lastSpace) {
                result += ' ';
                lastSpace = true;
            }
        } else {
            result += c;
            lastSpace = false;
        }
    }
    if (!result.empty() && result.back() == ' ') result.pop_back();
    return result;
}

vector<string> Split_Sentences(const string& text)
{
    vector<string> sentences;
    string current;
    for (char c : text) {
        current += c;
        if (c == '.' || c == '!' || c == '?') {
            sentences.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) sentences.push_back(current);
    return sentences;
}

struct DuplicateResult {
    vector<string> duplicates;
    int unique_count;
    int duplicate_count;
};

DuplicateResult Find_Duplicates(const string& text)
{
    auto sentences = Split_Sentences(text);
    unordered_map<string, int> seen;
    unordered_set<string> reported;

    DuplicateResult result{};

    for (auto& sent : sentences) {
        string norm = Normalize_Sentence(sent);

        if (seen.count(norm)) {
            if (!reported.count(norm)) {
                result.duplicates.push_back(sent);
                reported.insert(norm);
                result.duplicate_count++;
            }
        } else {
            seen[norm] = 1;
            result.unique_count++;
        }
    }
    return result;
}

// ══════════════════════════════════════════════════════════════════════════════
//  MAIN (Simple test version)
// ══════════════════════════════════════════════════════════════════════════════

int main()
{
    string text = "Hello world. Hello world! This is a test.";

    cout << "Words: " << Count_Words(text) << endl;
    cout << "Characters: " << Count_Characters(text) << endl;
    cout << "Sentences: " << Count_Sentences(text) << endl;

    auto res = Find_Duplicates(text);
    cout << "Duplicate Sentences: " << res.duplicate_count << endl;

    auto readability = Flesch_Score(text);
    cout << "Readability Score: " << readability.score << endl;
    cout << "Grade: " << readability.grade << endl;

    return 0;
}