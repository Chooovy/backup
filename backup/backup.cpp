#include <iostream>
#include <fstream>
#include <filesystem>

#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>

// namespace fs = std::filesystem;

// void back_up(){
//     try
//     {
//         fs::path sourcePath = "E:\desktop\1\from";
//         fs::path back_upPath = "E:\desktop\1\to";
//         fs::copy_options copyOpts = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
//         fs::copy(sourcePath, back_upPath, copyOpts); // �����ļ�
//         std::cout << "back up success!" << std::endl;
//     }
//     catch(const fs::filesystem_error& E)
//     {
//         std::cerr << E.what() << std::endl;
//     }
// }

// void restore(){
//     try
//     {
//         fs::path back_upPath = "E:\desktop\1\from";
//         fs::path sourcePath = "E:\desktop\1\to";
//         fs::copy_options copyOpts = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
//         fs::copy(back_upPath, sourcePath, copyOpts); // �����ļ�
//         std::cout << "restore success!" << std::endl;
//     }
//     catch(const fs::filesystem_error& E)
//     {
//         std::cerr << E.what() << std::endl;
//     }    
// }
struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;
    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
    Node(char data) : data(data), frequency(0), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};
//class����ʽ

//����Ƶ�ʱ���һ��Ƶ����������Ƶ�ʸߣ��ײ�Ƶ�ʵ�
Node* buildHuffmanTree_a(const std::unordered_map<char, int>& frequencyMap) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> pq;
    for (const auto& entry : frequencyMap) {
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();//���͵������
        Node* right = pq.top(); pq.pop();
        //ѡ������Ƶ����͵Ľڵ㣬��Ϊ�½ڵ�������ӽڵ�
        Node* parent = new Node('\0', left->frequency + right->frequency);
        //ÿ����ѡ���Ƶ�ʵ������ڵ�
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }
    return pq.top();
}

//���ݱ�������ַ���huffman����
//huffmanCodes <�ַ���huffman��>��pair
Node* buildHuffmanTree_b(const std::unordered_map<char, std::string>& huffmanCodes) {
    Node* root = new Node('\0');
    for (const auto& entry : huffmanCodes) {
        Node* current = root;
        const std::string& code = entry.second;//��ǰhuffman��
        for (char bit : code) {//����code��ÿ��bitλ
            if (bit == '0') {
                if (current->left == nullptr) {
                    current->left = new Node('\0');
                }
                current = current->left;
            } else {
                if (current->right == nullptr) {
                    current->right = new Node('\0');
                }
                current = current->right;
            }
        }
        current->data = entry.first;
    }
    return root;
}

// Node* buildHuffmanTree_b(const std::unordered_map<char, std::string>& huffmanCodes) {
//     std::priority_queue<Node*, std::vector<Node*>, CompareNodes> pq;

//     // ����Ҷ�ӽڵ㣬�������Ǽ������ȶ��У���Ƶ������
//     for (const auto& entry : huffmanCodes) {
//         pq.push(new Node(entry.first, entry.second.length())); // Ƶ���ñ��볤�ȱ�ʾ
//     }

//     while (pq.size() > 1) {
//         Node* left = pq.top(); pq.pop();
//         Node* right = pq.top(); pq.pop();

//         Node* parent = new Node('\0', left->frequency + right->frequency);
//         parent->left = left;
//         parent->right = right;

//         pq.push(parent);
//     }

//     return pq.top(); // ����Huffman���ĸ��ڵ�
// }

//root��huffman���ĸ��������ڵ㿪ʼ
void buildHuffmanCodes(Node* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (root !=nullptr && root->left == nullptr && root->right == nullptr) {//��Ҷ�ӽڵ�
        huffmanCodes[root->data] = code;
        return;
    }

    if (root !=nullptr && root->left != nullptr) {
        buildHuffmanCodes(root->left, code + '0', huffmanCodes);
    }

    if (root !=nullptr && root->right != nullptr) {
        buildHuffmanCodes(root->right, code + '1', huffmanCodes);
    }
}

// void compressFile(const std::string& inputFile, const std::string& compressedFile) {
//     std::ifstream inFile(inputFile, std::ios::in);
//     if (!inFile) {
//         std::cerr << "can not open file:" << inputFile << std::endl;
//         return;
//     }

//     std::unordered_map<char, int> frequencyMap;
//     char ch;
//     while (inFile.get(ch)) {
//         frequencyMap[ch]++;
//     }

//     Node* root = buildHuffmanTree_a(frequencyMap);
//     std::unordered_map<char, std::string> huffmanCodes;
//     buildHuffmanCodes(root, "", huffmanCodes);

//     std::ofstream outFile(compressedFile, std::ios::out | std::ios::binary);
//     for (const auto& entry : huffmanCodes) {
//         outFile << entry.first << " " << entry.second << " ";
//     }
//     outFile << "\n";//�����ѹ���ļ��ı����

//     inFile.clear();
//     inFile.seekg(0, std::ios::beg);
//     std::string encodedData;
//     while (inFile.get(ch)) {
//         encodedData += huffmanCodes[ch];
//     }//��infile�е�ÿһ���ַ�ת��huffmancode��Ȼ����뵽һ����

//     //���²���ѹ������
//     std::bitset<8> bits;
//     for (char bit : encodedData) {
//         bits <<= 1;
//         if (bit == '1') {
//             bits.set(0);
//         }
//         if (bits.size() == 8) {
//             char byte = static_cast<char>(bits.to_ulong());
//             outFile.put(byte);
//             bits.reset();
//         }
//     }

//     if (bits.size() > 0) {
//         char byte = static_cast<char>(bits.to_ulong());
//         outFile.put(byte);
//     }

//     inFile.close();
//     outFile.close();
// }

// �����ַ��� Huffman �����ӳ��
void generateHuffmanCodes(Node* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) {
        return;
    }

    if (root->data != '\0') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// ѹ���ļ��ĺ���
void compressFile(const std::string& inputFile, const std::string& outputFile) {
    // ��ȡԭʼ�ı�
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "�޷��������ļ���" << inputFile << std::endl;
        return;
    }

    std::string text;
    std::getline(inFile, text);
    inFile.close();

    // �����ַ�Ƶ�ʱ�
    std::unordered_map<char, int> charFrequency;
    for (char ch : text) {
        charFrequency[ch]++;
    }

    // ���� Huffman ��
    Node* root = buildHuffmanTree_a(charFrequency);

    // �����ַ��� Huffman �����ӳ��
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // ������ļ���д������
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "�޷���������ļ���" << outputFile << std::endl;
        return;
    }

    for (const auto& entry : huffmanCodes) {
        outFile << entry.first << ' ' << entry.second << ' ';
    }
    outFile << '\n';

    // ���벢д��ѹ������
    std::string compressedText;
    for (char ch : text) {
        compressedText += huffmanCodes[ch];
    }

    for (size_t i = 0; i < compressedText.size(); i += 8) {
        char byte = 0;
        for (int j = 0; j < 8; j++) {
            if (i + j < compressedText.size() && compressedText[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        outFile.put(byte);
    }

    outFile.close();
}

// void decompressFile(const std::string& compressedFile, const std::string& outputFile) {
//     std::ifstream inFile(compressedFile, std::ios::in | std::ios::binary);
//     if (!inFile) {
//         std::cerr << "can not open the compressed file:" << compressedFile << std::endl;
//         return;
//     }

//     std::unordered_map<char, std::string> huffmanCodes;
//     char ch;
//     while (inFile >> ch) {
//         if (ch == '\n') {
//             break;
//         }
//         std::string code;
//         inFile >> code;
//         huffmanCodes[ch] = code;
//     }

//     std::string encodedData;
//     while (inFile.get(ch)) {
//         encodedData += std::bitset<8>(ch).to_string();
//     }

//     Node* root = buildHuffmanTree_b(huffmanCodes);//������huffman��

//     std::ofstream outFile(outputFile, std::ios::out);
//     Node* current = root;
//     for (char bit : encodedData) {
//         if (bit == '0') {
//             current = current->left;
//         } else {
//             current = current->right;
//         }
//         if (current->left == nullptr && current->right == nullptr) {
//             outFile.put(current->data);
//             current = root;
//         }
//     }
//     outFile.close();

//     inFile.close();
// }

void decompressFile(const std::string& compressedFile, const std::string& outputFile) {
    std::ifstream inFile(compressedFile, std::ios::in | std::ios::binary);
    if (!inFile) {
        std::cerr << "can not open the compressed file:" << compressedFile << std::endl;
        return;
    }

    // ���ļ��ж�ȡ�����huffmanCodes��
    std::unordered_map<char, std::string> huffmanCodes;
    char ch;
    std::string code;
    while (inFile.get(ch)) {
        if (ch == ' ') {
            inFile.get(ch); // ��ȡ�ո����ַ�
            char character = ch;
            inFile.get(ch); // ��ȡ�������Ŀ֮��Ŀո�
            inFile >> code; // ��ȡ����
            huffmanCodes[character] = code;
        }
        else {
            break; // �ҵ������������
        }
    }

    // ���� Huffman ��
    Node* root = buildHuffmanTree_b(huffmanCodes);

    std::ofstream outFile(outputFile, std::ios::out);
    Node* current = root;
    char byte;
    while (inFile.get(byte)) {
        for (int i = 7; i >= 0; --i) {
            char bit = (byte >> i) & 1;
            if (bit == 0) {
                if (current->left != nullptr)   current = current->left;
            } else {// �����������������ǽ�ѹ������������ƥ��,����nullptr,Ӧ�������ﲻƥ�������
                if (current->right != nullptr)   current = current->right;
            }
            if (current != nullptr && current->left == nullptr && current->right == nullptr) {
                outFile.put(current->data);
                current = root;
            }
        }
    }
    outFile.close();
    inFile.close();
}

int main()
{
    int choice;
    std::cout << "1:compress    2:decompress :";
    std::cin >> choice;

    if (choice == 1) {
        std::string inputFile = "E:/desktop/1/from/from.txt";     // �����ļ���
        std::string compressedFile = "E:/desktop/1/from/compressed.huf"; // ѹ������ļ���
        try{
            compressFile(inputFile, compressedFile);
            std::cout << "success compress" << std::endl;
        }catch(const std::exception& e){
            std::cerr << "fatal:    " << e.what() << std::endl;
        }
    } else if (choice == 2) {
        std::string compressedFile = "E:/desktop/1/from/compressed.huf"; // ѹ������ļ���
        std::string outputFile = "E:/desktop/1/to/from.txt";
        try{
            decompressFile(compressedFile, outputFile);
            std::cout << "success decompress" << std::endl;
        }catch(const std::exception& e){
            std::cerr << "fatal:    " << e.what() << std::endl;//�쳣���������
        }
    }
    return 0;
}
//tree
//�ж��Ƿ񸲸�
//���ļ���include��.h�ļ�
//namespace