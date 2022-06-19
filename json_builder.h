#pragma once

#include "json.h"
#include <iostream>
#include <string>

namespace json
{

class BaseContext;
class StartDictContext;
class StartArrayContext;
class KeyContext;
class ValueContext;
class ArrayContext;
class EndArrayContext;
class EndDictContext;



class Builder
{
public:
    Builder();
    KeyContext Key(std::string value);
    BaseContext Value(Node::Value value);
    StartDictContext StartDict();
    StartArrayContext StartArray();
    EndDictContext EndDict();
    EndArrayContext EndArray();
    Node Build();
    std::vector<Node*> nodes_stack_;
private:
    enum class Commands
    {
        Creat,
        Key,
        Value,
        StartDict,
        EndDict,
        StartArray,
        EndArray,
        Build
    };
    std::string lastKey_;
    Commands lastCommand_;
    Node root_;
};


class BaseContext
{
public:
    BaseContext(Builder& b):builder(b){}
    KeyContext Key(std::string str);
    BaseContext Value(Node::Value value);
    StartDictContext StartDict();
    StartArrayContext StartArray();
    EndDictContext EndDict();
    EndArrayContext EndArray();
    Node Build();
    Builder & builder;
};


class KeyContext : public BaseContext
{
public:
    KeyContext(Builder& b):BaseContext(b) {}
    KeyContext Key(std::string) = delete;
    EndDictContext EndDict() = delete;
    EndArrayContext EndArray() = delete;
    Node Build() = delete;
    StartDictContext Value(Node::Value value);

};

class StartDictContext : public BaseContext
{
public:
    StartDictContext(Builder& b):BaseContext(b) {}
    ValueContext Value(Node::Value value) = delete;
    StartDictContext StartDict() = delete;
    StartArrayContext StartArray() = delete;
    EndArrayContext EndArray() = delete;
    Node Build() = delete;
};

class StartArrayContext : public BaseContext
{
public:
    StartArrayContext(Builder& b):BaseContext(b) {}
    StartArrayContext Value(Node::Value value);
    KeyContext Key(std::string) = delete;
    EndDictContext EndDict() = delete;
    Node Build() = delete;
};

class EndDictContext : public BaseContext
{
public:
    EndDictContext(Builder& b):BaseContext(b) {}
};

class EndArrayContext : public BaseContext
{
public:
    EndArrayContext(Builder& b):BaseContext(b) {}
};

}
