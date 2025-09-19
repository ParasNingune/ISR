#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>

using namespace std;

using InvertedIndex = unordered_map<string, unordered_set<int>>;

string stemWord(const string& word) {
    string w = word;
    transform(w.begin(), w.end(), w.begin(), ::tolower);
    if (w.size() > 3 && w.substr(w.size() - 3) == "ing") return w.substr(0, w.size() - 3);
    if (w.size() > 2 && w.substr(w.size() - 2) == "ed")  return w.substr(0, w.size() - 2);
    if (w.size() > 2 && w.substr(w.size() - 2) == "es")  return w.substr(0, w.size() - 2);
    if (w.size() > 2 && w.substr(w.size() - 2) == "ly")  return w.substr(0, w.size() - 2);
    return w;
}

vector<string> tokenize(const string& text, const unordered_set<string>& stopwords) {
    vector<string> res;
    string word;
    stringstream ss(text);
    while (ss >> word) {
        word.erase(remove_if(word.begin(), word.end(),
            [](char c){ return ispunct(static_cast<unsigned char>(c)); }),
            word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word.empty()) continue;
        if (stopwords.find(word) != stopwords.end()) continue;
        res.push_back(stemWord(word));
    }
    return res;
}

InvertedIndex buildIndex(const vector<string>& docs, const unordered_set<string>& stopwords) {
    InvertedIndex index;
    for (int docID = 0; docID < (int)docs.size(); ++docID) {
        vector<string> words = tokenize(docs[docID], stopwords);
        for (const string& w : words) index[w].insert(docID);
    }
    return index;
}

void printIndex(const InvertedIndex& index) {
    cout << "\nInverted Index:\n";
    for (auto it = index.begin(); it != index.end(); ++it) {
        cout << it->first << " -> { ";
        for (int docID : it->second) cout << "Doc" << docID << " ";
        cout << "}\n";
    }
}

// Evaluate a simple Boolean query (left-to-right, supports unary NOT)
set<int> booleanQuery(const InvertedIndex& index, const string& query, int numDocs) {
    vector<string> toks;
    string token;
    stringstream ss(query);
    while (ss >> token) toks.push_back(token);

    set<int> allDocs;
    for (int i = 0; i < numDocs; ++i) allDocs.insert(i);

    set<int> result;
    bool firstProcessed = false;
    string op = "or";

    for (size_t i = 0; i < toks.size(); ++i) {
        string tok = toks[i];
        string low = tok;
        transform(low.begin(), low.end(), low.begin(), ::tolower);

        if (low == "and" || low == "or") { op = low; continue; }

        if (low == "not") {
            if (i + 1 >= toks.size()) break; // nothing after NOT
            string next = toks[++i];
            string term = stemWord(next);
            unordered_set<int> postings_un;
            auto it = index.find(term);
            if (it != index.end()) postings_un = it->second;
            set<int> postings(postings_un.begin(), postings_un.end());

            set<int> notset;
            set_difference(allDocs.begin(), allDocs.end(), postings.begin(), postings.end(),
                           inserter(notset, notset.begin()));

            if (!firstProcessed) result = notset;
            else {
                set<int> temp;
                if (op == "and") set_intersection(result.begin(), result.end(), notset.begin(), notset.end(), inserter(temp, temp.begin()));
                else set_union(result.begin(), result.end(), notset.begin(), notset.end(), inserter(temp, temp.begin()));
                result = temp;
            }
            firstProcessed = true;
            continue;
        }

        // normal term
        string term = stemWord(tok);
        unordered_set<int> postings_un;
        auto it = index.find(term);
        if (it != index.end()) postings_un = it->second;
        set<int> postings(postings_un.begin(), postings_un.end());

        if (!firstProcessed) result = postings;
        else {
            set<int> temp;
            if (op == "and") set_intersection(result.begin(), result.end(), postings.begin(), postings.end(), inserter(temp, temp.begin()));
            else set_union(result.begin(), result.end(), postings.begin(), postings.end(), inserter(temp, temp.begin()));
            result = temp;
        }
        firstProcessed = true;
    }

    return result;
}

int main() {
    // sample documents (replace with file reads if you want)
    vector<string> documents = {
        "Information retrieval is the process of searching.",
        "Machine learning improves retrieval system.",
        "Natural language processing helps in document clustering."
    };

    unordered_set<string> stopwords = {"is", "the", "of", "in", "on", "and", "a", "an"};

    InvertedIndex index = buildIndex(documents, stopwords);

    printIndex(index);

    cout << "\nEnter query: ";
    string query;
    getline(cin, query);

    set<int> res = booleanQuery(index, query, (int)documents.size());

    cout << "\nQuery Result:\n";
    if (res.empty()) cout << "No documents found.\n";
    else {
        for (int d : res) cout << "Doc" << d << ": " << documents[d] << "\n";
    }

    return 0;
}