#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

// Helper function to split comma-separated input into list of tokens
vector<string> split_input(const string& input_str) {
    vector<string> tokens;
    stringstream ss(input_str);
    string token;
    while (getline(ss, token, ',')) {
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos)
            tokens.push_back(token.substr(start, end - start + 1));
    }
    return tokens;
}

// Calculate precision and recall
tuple<int, double, double> calculate_precision_recall(const vector<string>& A, const vector<string>& R) {
    set<string> setA(A.begin(), A.end());
    set<string> setR(R.begin(), R.end());

    int Ra = 0;
    for (const auto& doc : setA) {
        if (setR.count(doc)) Ra++;
    }

    double precision = setA.size() > 0 ? (double)Ra / setA.size() : 0;
    double recall = setR.size() > 0 ? (double)Ra / setR.size() : 0;

    return {Ra, precision, recall};
}

// Calculate F-measure
double calculate_f_measure(double precision, double recall) {
    return (precision + recall) > 0 ? (2 * precision * recall) / (precision + recall) : 0;
}

// Calculate E-measure
double calculate_e_measure(double precision, double recall, double beta) {
    if (precision == 0 && recall == 0) return 0;
    double numerator = (1 + beta * beta) * precision * recall;
    double denominator = (beta * beta * precision) + recall;
    return denominator > 0 ? numerator / denominator : 0;
}

// Generate table and collect precision/recall pairs
vector<pair<double, double>> generate_table(const vector<string>& retrieved_docs, const vector<string>& relevant_docs) {
    cout << left << setw(60) << "Documents"
         << setw(5) << "|Ra|"
         << setw(5) << "|A|"
         << setw(10) << "Precision"
         << setw(10) << "Recall" << endl;
    cout << string(95, '-') << endl;

    vector<pair<double, double>> results;

    for (size_t i = 1; i <= retrieved_docs.size(); ++i) {
        vector<string> current_set(retrieved_docs.begin(), retrieved_docs.begin() + i);
        int Ra_size;
        double precision, recall;
        tie(Ra_size, precision, recall) = calculate_precision_recall(current_set, relevant_docs);

        results.push_back({precision, recall});

        string docs_str;
        for (size_t j = 0; j < i; ++j) {
            docs_str += retrieved_docs[j];
            if (j < i - 1) docs_str += ", ";
        }

        cout << left << setw(60) << docs_str
             << setw(5) << Ra_size
             << setw(5) << i
             << setw(10) << fixed << setprecision(2) << precision * 100
             << setw(10) << fixed << setprecision(2) << recall * 100 << endl;
    }

    return results;
}

int main() {
    string retrieved_input, relevant_input;

    // Input retrieved documents
    cout << "Enter the retrieved documents (comma-separated): ";
    getline(cin, retrieved_input);
    vector<string> retrieved_docs = split_input(retrieved_input);

    // Input relevant documents
    cout << "Enter the relevant documents (comma-separated): ";
    getline(cin, relevant_input);
    vector<string> relevant_docs = split_input(relevant_input);

    // Generate table
    vector<pair<double, double>> results = generate_table(retrieved_docs, relevant_docs);

    // Choose index j
    int j;
    cout << "\nEnter value of j (0 - " << results.size() - 1 << ") to find F(j) and E(j): ";
    cin >> j;

    double precision = results[j].first;
    double recall = results[j].second;

    double f1 = calculate_f_measure(precision, recall);

    double e_greater = calculate_e_measure(precision, recall, 2);
    double e_equal = calculate_e_measure(precision, recall, 1);
    double e_less = calculate_e_measure(precision, recall, 0.5);

    cout << "\n---------------------------------\n";
    cout << "| Harmonic mean (F1) is: | " << fixed << setprecision(2) << f1 << " |\n";
    cout << "---------------------------------\n\n";

    cout << "        ----------------------------\n";
    cout << "        |          E-Value          |\n";
    cout << "        ----------------------------\n";
    cout << "        |   b>1   |   b=1   |  b<1  |\n";
    cout << "        ----------------------------\n";
    cout << "        |  " << fixed << setprecision(2) << e_greater
         << "  |  " << e_equal
         << "  |  " << e_less << " |\n";
    cout << "        ----------------------------\n";

    return 0;
}