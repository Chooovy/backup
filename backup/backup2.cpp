#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <bitset>


using namespace std;

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int freq) {
        this->data = data;
        this->freq = freq;
        left = nullptr;
        right = nullptr;
    }
};

struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

void buildHuffmanTree(priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes>& pq) {
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }
}

unordered_map<char, string> buildHuffmanCodes(HuffmanNode* root, string code = "") {
    unordered_map<char, string> huffmanCodes;
    if (root) {
        if (!root->left && !root->right) {
            huffmanCodes[root->data] = code;
        }
        unordered_map<char, string> leftCodes = buildHuffmanCodes(root->left, code + "0");
        huffmanCodes.insert(leftCodes.begin(), leftCodes.end());
        unordered_map<char, string> rightCodes = buildHuffmanCodes(root->right, code + "1");
        huffmanCodes.insert(rightCodes.begin(), rightCodes.end());
    }
    return huffmanCodes;
}

void compressFile(string inputFile, string outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    unordered_map<char, int> freqMap;
    char ch;

    while (inFile.get(ch)) {
        freqMap[ch]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;
    for (auto entry : freqMap) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }

    buildHuffmanTree(pq);
    HuffmanNode* root = pq.top();

    unordered_map<char, string> huffmanCodes = buildHuffmanCodes(root);

    inFile.clear();
    inFile.seekg(0);

    string encodedData = "";
    while (inFile.get(ch)) {
        encodedData += huffmanCodes[ch];
    }

    int padding = 8 - (encodedData.length() % 8);
    for (int i = 0; i < padding; i++) {
        encodedData += '0';
    }

    outFile.write(reinterpret_cast<const char*>(&padding), sizeof(int));

    for (size_t i = 0; i < encodedData.length(); i += 8) {
        string byte = encodedData.substr(i, 8);
        char byteValue = static_cast<char>(bitset<8>(byte).to_ulong());
        outFile.write(&byteValue, sizeof(char));
    }

    inFile.close();
    outFile.close();

    cout << "File compressed successfully!" << endl;
}

// void decompressFile(string inputFile, string outputFile) {
//     ifstream inFile(inputFile, ios::binary);
//     ofstream outFile(outputFile, ios::binary);

//     int padding;
//     inFile.read(reinterpret_cast<char*>(&padding), sizeof(int));

//     char ch;
//     string encodedData = "";

//     while (inFile.read(&ch, sizeof(char))) {
//         encodedData += bitset<8>(ch).to_string();
//     }

//     encodedData = encodedData.substr(0, encodedData.length() - padding);

//     HuffmanNode* root = new HuffmanNode('\0', 0);
//     HuffmanNode* currentNode = root;

//     for (char bit : encodedData) {
//         if (bit == '0') {
//             currentNode = currentNode->left;
//         } else {
//             currentNode = currentNode->right;
//         }

//         if (!currentNode->left && !currentNode->right) {//¿ÕÖ¸Õë´íÎó
//             outFile.put(currentNode->data);
//             currentNode = root;
//         }
//     }

//     inFile.close();
//     outFile.close();

//     cout << "File decompressed successfully!" << endl;
// }

void decompressFile(string inputFile, string outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error: Could not open files for decompression." << endl;
        return;
    }

    int padding;
    if (!inFile.read(reinterpret_cast<char*>(&padding), sizeof(int))) {
        cerr << "Error: Could not read padding from the compressed file." << endl;
        inFile.close();
        outFile.close();
        return;
    }

    char ch;
    string encodedData = "";

    while (inFile.read(&ch, sizeof(char))) {
        encodedData += bitset<8>(ch).to_string();
    }

    encodedData = encodedData.substr(0, encodedData.length() - padding);

    HuffmanNode* root = new HuffmanNode('\0', 0);
    HuffmanNode* currentNode = root;

    for (char bit : encodedData) {
        if (bit == '0') {
            if (currentNode->left) {
                currentNode = currentNode->left;
            } else {
                cerr << "Error: Invalid Huffman code encountered during decompression." << endl;
                inFile.close();
                outFile.close();
                return;
            }
        } else {
            if (currentNode->right) {
                currentNode = currentNode->right;
            } else {
                cerr << "Error: Invalid Huffman code encountered during decompression." << endl;
                inFile.close();
                outFile.close();
                return;
            }
        }

        if (!currentNode->left && !currentNode->right) {
            outFile.put(currentNode->data);
            currentNode = root;
        }
    }

    inFile.close();
    outFile.close();

    cout << "File decompressed successfully!" << endl;
}


int main() {
    string inputFile = "E:/desktop/1/from/from.txt";
    string compressedFile = "E:/desktop/1/from/compressed.bin";
    string decompressedFile = "E:/desktop/1/to/from.txt";

    compressFile(inputFile, compressedFile);
    decompressFile(compressedFile, decompressedFile);

    return 0;
}
