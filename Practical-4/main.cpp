#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

// Split input string by comma and trim spaces
vector<string> split_input(const string& input_str) {
    vector<string> tokens;
    stringstream ss(input_str);
    string token;
    while (getline(ss, token, ',')) {
        // Trim leading/trailing spaces
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos)
            tokens.push_back(token.substr(start, end - start + 1));
    }
    return tokens;
}

int main() {
    string retrieved_input, relevant_input;

    // Input retrieved documents (Answer set A)
    cout << "Enter retrieved documents (comma-separated, in rank order): ";
    getline(cin, retrieved_input);
    vector<string> answer_set = split_input(retrieved_input);

    // Input relevant documents (Set R)
    cout << "Enter relevant documents (comma-separated): ";
    getline(cin, relevant_input);
    vector<string> relevant_vec = split_input(relevant_input);
    set<string> relevant_set(relevant_vec.begin(), relevant_vec.end());

    int total_relevant = relevant_set.size(); // |R|
    int retrieved_relevant_count = 0;         // |Ra|

    // Print table header
    cout << "\n" << left << setw(60) << "Documents Retrieved So Far"
         << setw(20) << "Relevant∩Retrieved (|Ra|)"
         << setw(20) << "Retrieved Count (|A|)"
         << setw(15) << "Precision (%)"
         << setw(15) << "Recall (%)" << endl;
    cout << string(130, '-') << endl;

    // Iterate over retrieved docs
    for (size_t i = 0; i < answer_set.size(); ++i) {
        if (relevant_set.count(answer_set[i]))
            ++retrieved_relevant_count;

        double precision = (double)retrieved_relevant_count / (i + 1) * 100;
        double recall = total_relevant > 0 ? (double)retrieved_relevant_count / total_relevant * 100 : 0;

        // Build retrieved docs so far string
        string docs_so_far;
        for (size_t j = 0; j <= i; ++j) {
            docs_so_far += answer_set[j];
            if (j < i) docs_so_far += ", ";
        }

        // Print row
        cout << left << setw(60) << docs_so_far
             << setw(20) << retrieved_relevant_count
             << setw(20) << (i + 1)
             << setw(15) << fixed << setprecision(2) << precision
             << setw(15) << fixed << setprecision(2) << recall << endl;
    }

    return 0;
}