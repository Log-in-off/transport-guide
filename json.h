#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>

namespace json {

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

std::ostream & operator<< (std::ostream &out, const Array &array);
std::ostream & operator<< (std::ostream &out, const Dict &dict);

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class Node {
public:
    using Value = std::variant<std::nullptr_t, int, double, std::string, bool, Array, Dict>;
   /* Реализуйте Node, используя std::variant */

    Node();
    Node(std::nullptr_t value);
    Node(bool value);
    Node(int value);
    Node(double value);
    Node(const std::string &value);
    Node(const Dict &value);
    Node(const Array &value);
    const Value& GetValue() const;

    bool AsBool() const;
    int AsInt() const;
    double AsDouble() const;
    const std::string& AsString() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;

    bool IsBool() const;
    bool IsInt() const;
    bool IsPureDouble() const;
    bool IsDouble() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;

private:
    Value node_;
};

bool operator==(const Node &lh, const Node &rh);
bool operator!=(const Node &lh, const Node &rh);

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

bool operator==(const Document &lh, const Document &rh);
bool operator!=(const Document &lh, const Document &rh);

Document Load(std::istream& input);

void Print(const Document& doc, std::ostream& output);
void PrintNode(const Node& node, std::ostream& out);

// Перегрузка функции PrintValue для вывода значений null
void PrintValue(std::nullptr_t, std::ostream& out);
void PrintValue(bool value, std::ostream& out);
void PrintValue(std::string value, std::ostream& out);
void PrintValue(int value, std::ostream& out);
void PrintValue(double value, std::ostream& out);
void PrintValue(const Array &array, std::ostream& out);
void PrintValue(const Dict &dict, std::ostream& out);

}  // namespace json
