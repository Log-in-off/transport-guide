#include "json.h"

using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(move(result));
}

Node LoadNumber(std::istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return Node(std::stoi(parsed_num));
            } catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return Node(std::stod(parsed_num));
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

Node LoadNull(istream& input) {
    const string nameNull = "null";
    for (size_t i = 0; i < nameNull.size(); i++)
    {
        if (nameNull.at(i) == input.get())
            continue;
        else
        {
            throw ParsingError("Null parsing error");
        }
    }
    return {};
}

Node LoadBool(istream& input) {
    const string nameFalse = "false";
    const string nameTrue = "true";
    char c = input.get();
    bool value = (c == 't');
    string const * name = value ? &nameTrue : &nameFalse;
    for (size_t i = 1; i < name->size(); i++)
    {
        if (name->at(i) == input.get())
            continue;
        else
        {
            throw ParsingError("Bool parsing error");
        }
    }
    return Node(value);

}

Node LoadString(std::istream& input) {
    using namespace std::literals;

    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }
    return Node(s);
}

Node LoadDict(istream& input) {
    Dict result;

    for (char c; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }

    return Node(move(result));
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else if (c == 'f' || c == 't') {
        input.putback(c);
        return LoadBool(input);
    } else if (c == 'n') {
        input.putback(c);
        return LoadNull (input);
    } else {
        input.putback(c);
        return LoadNumber(input);
    }
}

}  // namespace

Node::Node() {
    //node_ = std::nullptr_t;
}

Node::Node(Value value)
    : node_(move(value)) {}

bool Node::AsBool() const {
    if (!IsBool())
        throw std::logic_error("It isn't bool");
    return std::get<bool>(node_);
}

int Node::AsInt() const {
    if (!IsInt())
        throw std::logic_error("It isn't int");
    return std::get<int>(node_);
}

double Node::AsDouble() const {
    if (IsDouble())
    {
        if (IsPureDouble())
            return std::get<double>(node_);
        if (IsInt())
            return static_cast<double>(std::get<int>(node_));
    }
    throw std::logic_error("It isn't double");
}

const std::string & Node::AsString() const {
    if (!IsString())
        throw std::logic_error("It isn't string");
    return std::get<std::string>(node_);
}

const Array & Node::AsArray() const {
    if (!IsArray())
        throw std::logic_error("It isn't Array");
    return std::get<Array>(node_);
}

const Dict & Node::AsMap() const{
    if (!IsMap())
        throw std::logic_error("It isn't Dict");
    return std::get<Dict>(node_);
}

bool Node::IsBool() const{
    return std::holds_alternative<bool>(node_);
}

bool Node::IsInt() const {
    return std::holds_alternative<int>(node_);
}

bool Node::IsPureDouble() const {
    return std::holds_alternative<double>(node_) ;
}

bool Node::IsDouble() const {
    return IsInt() || IsPureDouble();
}

bool Node::IsString() const {
    return std::holds_alternative<std::string>(node_) ;
}

bool Node::IsNull() const {
    return std::holds_alternative<std::nullptr_t>(node_) ;
}

bool Node::IsArray() const {
    return std::holds_alternative<Array>(node_);
}

bool Node::IsMap() const {
    return std::holds_alternative<Dict>(node_) ;
}


Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

void Print(const Document& doc, std::ostream& output) {
    auto root = doc.GetRoot();
    PrintNode(root, output);
}

bool operator==(const Node &lh, const Node &rh)
{
    return lh.GetValue() == rh.GetValue();
}

bool operator!=(const Node &lh, const Node &rh)
{
    return !(lh == rh);
}

void PrintNode(const Node& node, std::ostream& out) {
    std::visit( [&out](const auto& value){ PrintValue(value, out); },
    node.GetValue());
}

ostream &operator<<(std::ostream &out, const Array &array)
{
    return out;
}

ostream &operator<<(std::ostream &out, const Dict &dict)
{
    return out;
}

// Перегрузка функции PrintValue для вывода значений null
void PrintValue(std::nullptr_t, std::ostream& out) {
    out << "null"sv;
}

void PrintValue(bool value, std::ostream& out) {
    out << std::boolalpha << value << std::noboolalpha;
}

void PrintValue(string value, std::ostream& out) {
    out << "\"";
    for (auto i = value.begin(); i < value.end(); i++)
    {
        if (*i == '\"')
        {
            out << '\\' << '\"';
        }
        else if (*i == '\n')
        {
            out << '\\' << 'n';
        }
        else if (*i == '\r')
        {
            out << '\\' << 'r';
        }
        else if (*i == '\\')
        {
            out << '\\' << '\\';
        }
        else
            out << *i;
    }
    out << "\"";
}

}  // namespace json
