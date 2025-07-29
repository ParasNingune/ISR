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

string suffixes[MAX_SUFFIXES] = {"ing", "ed", "ly", "ness", "ful", "ment", "tion", "sion", "er", "est", "able", "ible", "al", "ic", "ous", "ive", "y", "es", "s"};


// Change to lower case
string toLower(string s) 
{
    for (int i = 0; i < s.length(); i++) 
    {
        if (s[i] >= 'A' && s[i] <= 'Z') 
        {
            s[i] = s[i] + 32;
        }
    }
    return s;
}


// Remove punctuations
string clean(string s) 
{
    string result = "";
    for (int i = 0; i < s.length(); i++) 
    {
        if (isalpha(s[i])) 
        {
            result += s[i];
        }
    }

    return result;
}


// Load stopwords
void loadStopwords(const string& filename) 
{
    ifstream file(filename);
    string word;
    while (file >> word) 
    {
        stopwords[stopwordCount++] = toLower(clean(word));
    }

    file.close();
}

// Check if stopword
bool isStopword(const string& word) 
{
    for (int i = 0; i < stopwordCount; i++) 
    {
        if (stopwords[i] == word) 
        {
            return true;
        }
    }
    return false;
}


// Remove suffix
string stripSuffix(string word) 
{
    for (int i = 0; i < MAX_SUFFIXES; i++) 
    {
        string suff = suffixes[i];
        int wlen = word.length();
        int slen = suff.length();
        bool match = true;
        if (wlen > slen + 2) 
        {
            for (int j = 0; j < slen; j++) 
            {
                if (word[wlen - slen + j] != suff[j]) 
                {
                    match = false;
                    break;
                }
            }
            if (match) 
            {
                string root = "";
                for (int k = 0; k < wlen - slen; k++) 
                {
                    root += word[k];
                }
                return root;
            }
        }
    }
    return word;
}


// Add or update word frequency
void addWord(const string& word) 
{
    for (int i = 0; i < wordCount; i++) 
    {
        if (words[i] == word) 
        {
            freq[i]++;
            return;
        }
    }
    words[wordCount] = word;
    freq[wordCount] = 1;
    wordCount++;
}

int main() 
{
    loadStopwords("stopwords.txt");

    ifstream input("input.txt");
    if (!input) 
    {
        cout << "Could not open input file.\n";
        return 1;
    }

    ofstream output("output.txt");
    if (!output) 
    {
        cout << "Could not create output file.\n";
        return 1;
    }

    string word;
    while (input >> word) 
    {
        word = toLower(clean(word));
        if (word == "" || isStopword(word)) continue;
        word = stripSuffix(word);
        addWord(word);
    }
    input.close();

    for (int i = 0; i < wordCount; i++) 
    {
        output << words[i] << " : " << freq[i] << "\n";
    }

    output.close();
    cout << "Output saved to output.txt\n";

    return 0;
}