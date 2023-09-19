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
//         fs::copy(sourcePath, back_upPath, copyOpts); // 复制文件
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
//         fs::copy(back_upPath, sourcePath, copyOpts); // 复制文件
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
//class的形式

//根据频率表建立一棵频率树，顶部频率高，底部频率低
Node* buildHuffmanTree_a(const std::unordered_map<char, int>& frequencyMap) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> pq;
    for (const auto& entry : frequencyMap) {
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();//更低的在左边
        Node* right = pq.top(); pq.pop();
        //选出两个频率最低的节点，成为新节点的左右子节点
        Node* parent = new Node('\0', left->frequency + right->frequency);
        //每次挑选最低频率的两个节点
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }
    return pq.top();
}

//根据编码表构建字符和huffman的树
//huffmanCodes <字符，huffman码>的pair
Node* buildHuffmanTree_b(const std::unordered_map<char, std::string>& huffmanCodes) {
    Node* root = new Node('\0');
    for (const auto& entry : huffmanCodes) {
        Node* current = root;
        const std::string& code = entry.second;//当前huffman码
        for (char bit : code) {//遍历code的每个bit位
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

//     // 创建叶子节点，并将它们加入优先队列（按频率排序）
//     for (const auto& entry : huffmanCodes) {
//         pq.push(new Node(entry.first, entry.second.length())); // 频率用编码长度表示
//     }

//     while (pq.size() > 1) {
//         Node* left = pq.top(); pq.pop();
//         Node* right = pq.top(); pq.pop();

//         Node* parent = new Node('\0', left->frequency + right->frequency);
//         parent->left = left;
//         parent->right = right;

//         pq.push(parent);
//     }

//     return pq.top(); // 返回Huffman树的根节点
// }

//root是huffman树的根，从最大节点开始
void buildHuffmanCodes(Node* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (root !=nullptr && root->left == nullptr && root->right == nullptr) {//是叶子节点
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
//     outFile << "\n";//这段是压缩文件的编码表

//     inFile.clear();
//     inFile.seekg(0, std::ios::beg);
//     std::string encodedData;
//     while (inFile.get(ch)) {
//         encodedData += huffmanCodes[ch];
//     }//将infile中的每一个字符转成huffmancode，然后读入到一行中

//     //以下才是压缩编码
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

// 生成字符到 Huffman 编码的映射
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

// 压缩文件的函数
void compressFile(const std::string& inputFile, const std::string& outputFile) {
    // 读取原始文本
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "无法打开输入文件：" << inputFile << std::endl;
        return;
    }

    std::string text;
    std::getline(inFile, text);
    inFile.close();

    // 构建字符频率表
    std::unordered_map<char, int> charFrequency;
    for (char ch : text) {
        charFrequency[ch]++;
    }

    // 构建 Huffman 树
    Node* root = buildHuffmanTree_a(charFrequency);

    // 生成字符到 Huffman 编码的映射
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // 打开输出文件并写入编码表
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "无法创建输出文件：" << outputFile << std::endl;
        return;
    }

    for (const auto& entry : huffmanCodes) {
        outFile << entry.first << ' ' << entry.second << ' ';
    }
    outFile << '\n';

    // 编码并写入压缩数据
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

//     Node* root = buildHuffmanTree_b(huffmanCodes);//编码表的huffman树

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

    // 从文件中读取编码表（huffmanCodes）
    std::unordered_map<char, std::string> huffmanCodes;
    char ch;
    std::string code;
    while (inFile.get(ch)) {
        if (ch == ' ') {
            inFile.get(ch); // 读取空格后的字符
            char character = ch;
            inFile.get(ch); // 读取编码表条目之间的空格
            inFile >> code; // 读取编码
            huffmanCodes[character] = code;
        }
        else {
            break; // 找到编码表结束标记
        }
    }

    // 构建 Huffman 树
    Node* root = buildHuffmanTree_b(huffmanCodes);

    std::ofstream outFile(outputFile, std::ios::out);
    Node* current = root;
    char byte;
    while (inFile.get(byte)) {
        for (int i = 7; i >= 0; --i) {
            char bit = (byte >> i) & 1;
            if (bit == 0) {
                if (current->left != nullptr)   current = current->left;
            } else {// 处理错误情况，可能是解压缩数据与编码表不匹配,出现nullptr,应该是这里不匹配的问题
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
        std::string inputFile = "E:/desktop/1/from/from.txt";     // 输入文件名
        std::string compressedFile = "E:/desktop/1/from/compressed.huf"; // 压缩后的文件名
        try{
            compressFile(inputFile, compressedFile);
            std::cout << "success compress" << std::endl;
        }catch(const std::exception& e){
            std::cerr << "fatal:    " << e.what() << std::endl;
        }
    } else if (choice == 2) {
        std::string compressedFile = "E:/desktop/1/from/compressed.huf"; // 压缩后的文件名
        std::string outputFile = "E:/desktop/1/to/from.txt";
        try{
            decompressFile(compressedFile, outputFile);
            std::cout << "success decompress" << std::endl;
        }catch(const std::exception& e){
            std::cerr << "fatal:    " << e.what() << std::endl;//异常结束不输出
        }
    }
    return 0;
}
//tree
//判断是否覆盖
//多文件，include，.h文件
//namespace