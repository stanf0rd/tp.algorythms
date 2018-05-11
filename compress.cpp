#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <cmath>

#define BYTE_SIZE 8

using std::string;
using std::vector;
using std::array;
using std::unordered_map;
using std::stack;

class InBitStream {
public:
    InBitStream(string filename);
    bool readBit();
    u_char readByte();
    u_char readLastByte();
    u_long getRemainingBits();
    bool endOfStream() const;
    void rewind();
    ~InBitStream();
private:
    vector<u_char> *buffer;
    u_long bitPos;
};

InBitStream::InBitStream(string filename) : buffer(new vector<u_char>), bitPos(0) {
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof()) {
        char byte = file.get();
        buffer->push_back((u_char)byte);
    }
    buffer->pop_back();  // remove EOF
    file.close();
}

InBitStream::~InBitStream() {
    delete buffer;
}

bool InBitStream::readBit() {
    bool bit = (buffer->at(bitPos/BYTE_SIZE) >> (BYTE_SIZE - bitPos%BYTE_SIZE - 1)) & 1;
    bitPos++;
    return bit;
}

u_char InBitStream::readByte() {
    u_char byte = 0;
    byte |= buffer->at(bitPos/BYTE_SIZE) << bitPos%BYTE_SIZE;
    bitPos += BYTE_SIZE;
    if (bitPos%BYTE_SIZE != 0)
        byte |= buffer->at(bitPos/BYTE_SIZE) >> (BYTE_SIZE - bitPos%BYTE_SIZE);
    return byte;
}

u_char InBitStream::readLastByte() {
    u_char byte = 0;
    byte = buffer->back();
    return byte;
}

u_long InBitStream::getRemainingBits() {
    return buffer->size() * BYTE_SIZE - bitPos;
}

bool InBitStream::endOfStream() const {
    return bitPos == buffer->size() * BYTE_SIZE;
}

void InBitStream::rewind() {
    bitPos = 0;
}

class OutBitStream {
public:
    OutBitStream(string filename);
    void writeBit(bool);
    void writeByte(u_char);
    void endCompress();
    void close();
private:
    u_char buffer;
    int bitsCount;
    std::ofstream output;
    void pushBuffer();
};

OutBitStream::OutBitStream(string filename) :
    buffer(0), bitsCount(0),
    output(filename, std::ios::binary) {
}

void OutBitStream::pushBuffer() {
    output.put(buffer);
    buffer = 0;
}

void OutBitStream::writeBit(bool bit) {
    if (bit == 1) buffer |= 1 << (BYTE_SIZE - bitsCount - 1);
    bitsCount++;
    if (bitsCount == 8) {
        pushBuffer();
        bitsCount = 0;
    }
}

void OutBitStream::writeByte(u_char byte) {
    buffer |= byte >> bitsCount;
    pushBuffer();
    if (bitsCount%BYTE_SIZE != 0)
        buffer |= byte << (BYTE_SIZE - bitsCount);
}

void OutBitStream::endCompress() {
    pushBuffer();
    buffer = BYTE_SIZE - bitsCount;
    pushBuffer();
}

void OutBitStream::close() {
    output.close();
}

struct node {
    int frequency;
    u_char byte;
    node *left;
    node *right;
    node(int _freq, u_char _byte)
    : frequency(_freq), byte(_byte), left(nullptr), right(nullptr) {};
    node(int _freq, node *l, node *r)
    : frequency(_freq), left(l), right(r) {};
    node(u_char _byte) : byte(_byte), left(nullptr), right(nullptr) {};
    node(node *l, node *r) : left(l), right(r) {};
};

bool nodeCompare(const node *first, const node *second) {
    return (first->frequency > second->frequency);
}

node* makeTree(array<int, 256> frequencies) {
    vector<node*> nodes;
    for (uint i = 0; i < frequencies.size(); i++) {
        if (frequencies[i] != 0) {
            node *newNode = new node(frequencies[i], i);
            nodes.push_back(newNode);
        }
    }
    std::make_heap(nodes.begin(), nodes.end(), nodeCompare);

    while (nodes.size() != 1) {
        std::pop_heap(nodes.begin(), nodes.end(), nodeCompare);
        node *node1 = nodes.back(); nodes.pop_back();
        std::pop_heap(nodes.begin(), nodes.end(), nodeCompare);
        node *node2 = nodes.back(); nodes.pop_back();
        node *newNode = new node(
            node1->frequency + node2->frequency,
            node1, node2
        );
        nodes.push_back(newNode);
        std::push_heap(nodes.begin(), nodes.end(), nodeCompare);
    }
    node *treeRoot = nodes.back();
    return treeRoot;
}

void encodeTreeToStream(node *root, OutBitStream &stream) {
    if (root->left) {
        encodeTreeToStream(root->left, stream);
        encodeTreeToStream(root->right, stream);
        stream.writeBit(0);
    } else {
        stream.writeBit(1);
        stream.writeByte(root->byte);
    }
}

void deleteTree(node *root) {
    if (root->left) deleteTree(root->left);
    if (root->right) deleteTree(root->right);
    delete root;
}

void makeCodeTable(node *root,
    unordered_map<u_char, vector<bool>> &codeTable,
    vector<bool> *code = new vector<bool>) {

    if (!root->left && !root->right) {
        codeTable.insert({root->byte, *code});
        return;
    }

    code->push_back(1);
    makeCodeTable(root->right, codeTable, code);
    code->pop_back();

    code->push_back(0);
    makeCodeTable(root->left, codeTable, code);
    code->pop_back();

    if (!code->size()) delete code;
}

double deviation(array<int, 256> frequencies) {
    double sum = 0;
    for (auto it : frequencies) sum += (double)it;
    double middle = sum/(double)frequencies.size();
    sum = 0;
    for (auto it : frequencies)
        sum += pow(middle - (double)it, 2);
    middle = sum/(double)frequencies.size();
    return sqrt(middle);
}

void encode(string inputFile, string outputFile) {
    array<int, 256> frequencies;
    frequencies.fill(0);

    InBitStream input(inputFile);
    OutBitStream output(outputFile);

    while (!input.endOfStream()) {
        u_char byte = input.readByte();
        frequencies[byte]++;
    } input.rewind();

    // 150 - магическая константа
    // опытным путём было выяснено, что файлы с меньшим
    // среднеквадратичным отклонением частот байтов сжимать неэффективно
    if (deviation(frequencies) < 150) {
        while (!input.endOfStream()) {
            u_char byte = input.readByte();
            output.writeByte(byte);
        } output.close();
        return;
    }

    node *treeRoot = makeTree(frequencies);

    unordered_map<u_char, vector<bool>> codeTable;
    makeCodeTable(treeRoot, codeTable);

    output.writeByte(12);
    output.writeByte(228);
    output.writeByte(codeTable.size() - 1);

    encodeTreeToStream(treeRoot, output);
    deleteTree(treeRoot); treeRoot = nullptr;

    while (!input.endOfStream()) {
        u_char byte = input.readByte();
        auto got = codeTable.find(byte);
        for (auto it : got->second) output.writeBit(it);
    }

    output.endCompress();
    output.close();
    encode(outputFile, outputFile);
}

void decode(string inputFile, string outputFile) {
    InBitStream input(inputFile);
    OutBitStream output(outputFile);

    if (input.readByte() != 12 || input.readByte() != 228) {
        input.rewind();
        while (!input.endOfStream()) {
            u_char byte = input.readByte();
            output.writeByte(byte);
        } output.close();
        return;
    }

    stack<node*> nodeStack;
    int letterCount = (int)input.readByte() + 1;

    while(letterCount != 0 || nodeStack.size() != 1) {
        bool bit = input.readBit();
        if (bit == 1) {
            u_char byte = input.readByte();
            node *newNode = new node(byte);
            nodeStack.push(newNode);
            letterCount--;
        } else {
            node *rNode = nodeStack.top(); nodeStack.pop();
            node *lNode = nodeStack.top(); nodeStack.pop();
            node *newNode = new node(lNode, rNode);
            nodeStack.push(newNode);
        }
    }
    node *treeRoot = nodeStack.top(); nodeStack.pop();

    u_long remainingBits = input.getRemainingBits() - BYTE_SIZE - input.readLastByte();
    for (u_long i = 0; i < remainingBits; ) {
        node *current = treeRoot;
        while (current->right) {
            current = input.readBit() == 1 ? current->right : current->left;
            i++;
        }
        output.writeByte(current->byte);
    }
    output.close();
    deleteTree(treeRoot);
    decode(outputFile, outputFile);
}

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "use\n./a.out compress from_file to_file" << endl;
        cout << "or" << endl;
        cout << "./a.out decompress from_file to_file" << endl;
        return 0;
    }
    string mode(argv[1]);
    string from_file(argv[2]);
    string to_file(argv[3]);
    if (mode == "compress") encode(from_file, to_file);
    else if (mode == "decompress") decode(from_file, to_file);
    else cout << "argument error" << endl;
    return 0;
}