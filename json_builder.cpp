#include "json_builder.h"

namespace json
{

Builder::Builder()
{
    lastCommand_ = Commands::Creat;
}

KeyContext Builder::Key(std::string value)
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");
    if (lastCommand_ == Commands::Key)
        throw std::logic_error("trying to call key aftre key");
    if(!nodes_stack_.back()->IsMap())
        throw std::logic_error("trying to call key for not map");

    lastKey_ = std::move(value);
    lastCommand_ = Commands::Key;
    return KeyContext(*this);
}

BaseContext Builder::Value(Node::Value value)
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");

    if (lastCommand_ == Commands::Creat)
    {
        root_.GetValue() = value;
        lastCommand_ = Commands::Build;
        return *this;
    }
    else if (nodes_stack_.back()->IsMap())
    {
        if (lastCommand_ != Commands::Key)
             throw std::logic_error("trying to call value without key for map");

        Node t;
        t.GetValue() = value;
        std::get<Dict>(nodes_stack_.back()->GetValue()).emplace(std::move(lastKey_), std::move(t));
    }
    else if (nodes_stack_.back()->IsArray())
    {
        Node t;
        t.GetValue() = value;
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(std::move(t));
    }
    else
    {
        throw std::logic_error("uncorrect trying to call value");
    }

    lastCommand_ = Commands::Value;
    return *this;
}

StartDictContext Builder::StartDict()
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");

    json::Dict newNode;
    if (lastCommand_ == Commands::Creat)
    {
        root_.GetValue() = newNode;
        nodes_stack_.push_back(&root_);
    }
    else if (lastCommand_ == Commands::Key)
    {
        auto t = std::get<Dict>(nodes_stack_.back()->GetValue()).emplace(std::move(lastKey_), std::move(newNode));
        nodes_stack_.push_back(&t.first->second);
    }
    else if (nodes_stack_.back()->IsArray())
    {
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(newNode);
        nodes_stack_.push_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
    }
    else
    {
        throw std::logic_error("uncorrect trying to call start map");
    }

    lastCommand_ = Commands::StartDict;
    return StartDictContext(*this);
}

StartArrayContext Builder::StartArray()
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");

    json::Array newNode;
    if (lastCommand_ == Commands::Creat)
    {
        root_.GetValue() = newNode;
        nodes_stack_.push_back(&root_);
    }
    else if (lastCommand_ == Commands::Key)
    {
        auto t = std::get<Dict>(nodes_stack_.back()->GetValue()).emplace(std::move(lastKey_), std::move(newNode));
        nodes_stack_.push_back(&t.first->second);
    }
    else if (nodes_stack_.back()->IsArray())
    {
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(newNode);
        nodes_stack_.push_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
    }
    else
    {
        throw std::logic_error("uncorrect trying to call start array");
    }

    lastCommand_ = Commands::StartArray;
    return *this;
}

EndDictContext Builder::EndDict()
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");

    if (!nodes_stack_.back()->IsMap())
        throw std::logic_error("try to end map for not map");

    nodes_stack_.pop_back();
    if (nodes_stack_.empty())
        lastCommand_ = Commands::Build;
    else
        lastCommand_ = Commands::EndDict;
    return EndDictContext(*this);
}

EndArrayContext Builder::EndArray()
{
    if (lastCommand_ == Commands::Build)
        throw std::logic_error("trying to make after build");

    if (!nodes_stack_.back()->IsArray())
        throw std::logic_error("try to end array for not array");

    nodes_stack_.pop_back();
    if (nodes_stack_.empty())
        lastCommand_ = Commands::Build;
    else
        lastCommand_ = Commands::EndArray;
    return EndArrayContext(*this);
}

Node Builder::Build()
{
    if (lastCommand_ == Commands::Creat)
        throw std::logic_error("call build before make");

    if (!nodes_stack_.empty())
        throw std::logic_error("call build before finish last node");

    lastCommand_ = Commands::Build;
    return root_;
}

KeyContext BaseContext::Key(std::string str)
{
    return builder.Key(str);
}

BaseContext BaseContext::Value(Node::Value value)
{
    return builder.Value(value);
}

StartDictContext BaseContext::StartDict()
{
    return builder.StartDict();
}

StartArrayContext BaseContext::StartArray()
{
    return builder.StartArray();
}

EndDictContext BaseContext::EndDict()
{
    return builder.EndDict();
}

EndArrayContext BaseContext::EndArray()
{
    return builder.EndArray();
}

Node BaseContext::Build()
{
    return builder.Build();
}

StartDictContext KeyContext::Value(Node::Value value)
{
    return StartDictContext(builder.Value(value).builder);
}

StartArrayContext StartArrayContext::Value(Node::Value value)
{
    return StartArrayContext(builder.Value(value).builder);
}

}
