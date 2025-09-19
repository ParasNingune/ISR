#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_DOCS 10
#define MAX_TOKENS 100

float threshold = 0.85f;

// Convert int array to float array
void convertToFloat(int arr[], float floatArr[], int size) {
    for (int i = 0; i < size; i++) {
        floatArr[i] = static_cast<float>(arr[i]);
    }
}

// Cosine similarity
float cosSimilarity(float vec1[], float vec2[], int size) {
    float dot = 0.0, mag1 = 0.0, mag2 = 0.0;
    for (int i = 0; i < size; i++) {
        dot += vec1[i] * vec2[i];
        mag1 += vec1[i] * vec1[i];
        mag2 += vec2[i] * vec2[i];
    }

    if (mag1 == 0 || mag2 == 0) return 0.0;
    return dot / (sqrt(mag1) * sqrt(mag2));
}

// Calculate centroid
void clusterCentroid(int cluster[], int docMatrix[][MAX_TOKENS], int numTokens, float representative[]) {
    for (int i = 0; i < numTokens; i++) representative[i] = 0;

    int clusterSize = cluster[0];
    for (int i = 1; i <= clusterSize; i++) {
        int docIndex = cluster[i];
        for (int j = 0; j < numTokens; j++) {
            representative[j] += docMatrix[docIndex][j];
        }
    }

    for (int i = 0; i < numTokens; i++) {
        representative[i] /= clusterSize;
    }
}

// Clustering logic
void disjointCluster(int numDocs, int numTokens, int docMatrix[][MAX_TOKENS]) {
    int clusters[MAX_DOCS][MAX_DOCS + 1];
    float representatives[MAX_DOCS][MAX_TOKENS];

    clusters[0][0] = 1;
    clusters[0][1] = 0;
    for (int i = 0; i < numTokens; i++)
        representatives[0][i] = docMatrix[0][i];

    int numClusters = 1;
    float docVec[MAX_TOKENS];

    for (int i = 1; i < numDocs; i++) {
        convertToFloat(docMatrix[i], docVec, numTokens);
        float maxSim = -1.0;
        int bestCluster = -1;

        for (int j = 0; j < numClusters; j++) {
            float sim = cosSimilarity(docVec, representatives[j], numTokens);
            if (sim > threshold && sim > maxSim) {
                maxSim = sim;
                bestCluster = j;
            }
        }

        if (bestCluster == -1) {
            clusters[numClusters][0] = 1;
            clusters[numClusters][1] = i;
            for (int t = 0; t < numTokens; t++)
                representatives[numClusters][t] = docMatrix[i][t];
            numClusters++;
        } else {
            int index = ++clusters[bestCluster][0];
            clusters[bestCluster][index] = i;
            clusterCentroid(clusters[bestCluster], docMatrix, numTokens, representatives[bestCluster]);
        }
    }

    cout << "\nClusters Formed:\n";
    for (int i = 0; i < numClusters; i++) {
        cout << "Cluster " << i + 1 << ": ";
        for (int j = 1; j <= clusters[i][0]; j++) {
            cout << "Doc" << clusters[i][j] + 1 << " ";
        }
        cout << endl;
    }
}

int main() {
    int numDocs;
    cout << "Enter number of documents: ";
    cin >> numDocs;

    if (numDocs > MAX_DOCS) {
        cout << "Max supported documents: " << MAX_DOCS << endl;
        return 1;
    }

    vector<string> filenames(numDocs);
    cout << "Enter the filenames (one per line):\n";
    cin.ignore(); // clear newline
    for (int i = 0; i < numDocs; i++) {
        getline(cin, filenames[i]);
    }

    map<string, int> tokenToIndex;
    int currentTokenIndex = 0;
    int docMatrix[MAX_DOCS][MAX_TOKENS] = {0};

    for (int d = 0; d < numDocs; d++) {
        ifstream file(filenames[d]);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filenames[d] << endl;
            return 1;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            int freq;

            getline(ss, token, ':');
            ss >> freq;

            // Trim whitespace
            token.erase(remove(token.begin(), token.end(), ' '), token.end());

            if (tokenToIndex.find(token) == tokenToIndex.end()) {
                if (currentTokenIndex >= MAX_TOKENS) {
                    cerr << "Exceeded max tokens supported.\n";
                    return 1;
                }
                tokenToIndex[token] = currentTokenIndex++;
            }

            int index = tokenToIndex[token];
            docMatrix[d][index] = freq;
        }
        file.close();
    }

    int numTokens = currentTokenIndex;

    disjointCluster(numDocs, numTokens, docMatrix);

    return 0;
}
