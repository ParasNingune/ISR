#include<iostream>
#include<fstream>
#include<string>

#define MAX_WORDS 1000
#define MAX_STOPWORDS 200
#define MAX_SUFFIXES 20

using namespace std;

string stopwords[MAX_STOPWORDS];
int stopwordCount = 0;

string words[MAX_WORDS];
int freq[MAX_WORDS];
int wordCount = 0;

string suffixes[MAX_SUFFIXES] = {
    "ing", "ed", "ly", "ness", "ful", "ment", "tion", "sion", "est", "able", "ible", "al", "ic", "ous", "ive", "y", "es", "s"
};

// Convert to lowercase
string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
    }
    return s;
}

// Remove punctuation and non-alphabetic characters
string clean(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

// Load stopwords from file
void loadStopwords(const string& filename) {
    ifstream file(filename);
    string word;
    while (file >> word) {
        stopwords[stopwordCount++] = toLower(clean(word));
    }
    file.close();
}

// Check if a word is a stopword
bool isStopword(const string& word) {
    for (int i = 0; i < stopwordCount; i++) {
        if (stopwords[i] == word) {
            return true;
        }
    }
    return false;
}

string stripSuffix(string word) {
    int len = word.length();

    if (len > 4 && word.substr(len - 5) == "iness") {
        return word.substr(0, len - 5) + "y";
    }

    if (len > 5 && word.substr(len - 3) == "ing") {
        return word.substr(0, len - 3);
    }

    if (len > 3 && word.substr(len - 2) == "es") {
        return word.substr(0, len - 2);
    }

    if (len > 3 && word.substr(len - 2) == "ed") {
        return word.substr(0, len - 2);
    }

    if (len > 5 && word.substr(len - 3) == "ful") {
        return word.substr(0, len - 3);
    }

    if (len > 6 && word.substr(len - 4) == "ness") {
        return word.substr(0, len - 4);
    }

    if (len > 4 && word.substr(len - 2) == "ly") {
        return word.substr(0, len - 2);
    }

    // Remove basic suffixes if above didn’t match
    for (int i = 0; i < MAX_SUFFIXES; i++) {
        string suff = suffixes[i];
        int slen = suff.length();
        if (len > slen + 2 && word.substr(len - slen) == suff) {
            return word.substr(0, len - slen);
        }
    }

    return word;
}

// Add or update word frequency
void addWord(const string& word) {
    for (int i = 0; i < wordCount; i++) {
        if (words[i] == word) {
            freq[i]++;
            return;
        }
    }
    words[wordCount] = word;
    freq[wordCount] = 1;
    wordCount++;
}

int main() {
    loadStopwords("stopwords.txt");

    ifstream input("input.txt");
    if (!input) {
        cout << "Could not open input file.\n";
        return 1;
    }

    ofstream output("output.txt");
    if (!output) {
        cout << "Could not create output file.\n";
        return 1;
    }

    string word;
    while (input >> word) {
        word = toLower(clean(word));
        if (word == "" || isStopword(word)) continue;
        word = stripSuffix(word);
        addWord(word);
    }
    input.close();

    for (int i = 0; i < wordCount; i++) {
        output << words[i] << " : " << freq[i] << "\n";
    }

    output.close();
    cout << "Output saved to output.txt\n";

    return 0;
}
