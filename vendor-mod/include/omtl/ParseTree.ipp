// BSD 3-Clause License

// Copyright (c) 2022, Alex Tarasov
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <omtl/ParseTree.hpp>


namespace omtl {
    inline size_t ParseTreeBuilder::findMatchingBracket(std::vector<Token>& tokens, size_t i) {
        for (; i < tokens.size(); i++) {
            if (tokens[i].getRaw() == "[") break;
        }
        i++;
        for (;;) {
            if (i >= tokens.size()) {
                return tokens.size();
            } else if (tokens[i].getRaw() == "[") {
                i = findMatchingBracket(tokens, i) + 1;
            } else if (tokens[i].getRaw() == "]") {
                return i;
            } else {
                i++;
            }
        }
    }

    inline bool ParseTreeBuilder::isTuple(std::vector<Token>& tokens, size_t i) {
        for (; i < tokens.size(); i++) {
            if (tokens[i].getRaw() == "[") break;
        }
        i++;
        for (;;) {
            if (i >= tokens.size()) {
                return false;
            } else if (tokens[i].getRaw() == "]") {
                return false;
            } else if (tokens[i].getRaw() == ":") {
                return true;
            } else if (tokens[i].getRaw() == ",") {
                return true;
            } else if (tokens[i].getRaw() == "[") {
                i = findMatchingBracket(tokens, i) + 1;
            } else {
                i++;
            }
        }
    }

    inline Element ParseTreeBuilder::parseStatement(std::vector<Token>& tokens, size_t& i) {
        Element result;
        result.statement = std::deque<std::pair<std::string, Element>>();
        if (tokens.size() > i) result.location = tokens[i].location;
        for (;;) {
            if (i >= tokens.size()) { throw std::runtime_error("statement did not end: " + result.location); } // error
            if (tokens[i].getRaw() == ",") break;                                                              // done
            if (tokens[i].getRaw() == "]") break;                                                              // done
            if (tokens[i].getRaw() == "[") {
                if (isTuple(tokens, i)) {
                    Element elem = parseTuple(tokens, i);
                    result.statement->push_back({std::to_string(result.statement->size()), elem});
                } else {
                    Element elem;
                    i++;
                    elem = parseStatement(tokens, i);
                    i++;
                    result.statement->push_back({std::to_string(result.statement->size()), elem});
                }
            } else {
                Element elem;
                elem.value = tokens[i];
                elem.location = tokens[i].location;
                result.statement->push_back({std::to_string(result.statement->size()), elem});
                i++;
            }
        }
        result.reindexStatment();
        return result;
    }

    /*
 parses a tuple with the parentheses example:

 ```
 [a: 1, b: 2, c: 3, d: [e: 4, f: 5 + 4],]
 ```
 */
    inline Element ParseTreeBuilder::parseTuple(std::vector<Token>& tokens, size_t& i) {
        Element result;
        result.tuple = std::deque<std::pair<std::string, Element>>();
        for (; i < tokens.size(); i++) {
            if (tokens[i].getRaw() == "[") {
                result.location = tokens[i].location;
                break;
            }
        }
        i++;
        for (;;) {
            if (i >= tokens.size()) { throw std::runtime_error("tuple did not end: " + result.location); } // error
            if (tokens[i].getRaw() == "]") {
                i++;
                break;
            }
            if (tokens[i].getRaw() == ":") {
                throw std::runtime_error("unexpected colon in tuple at: " + tokens[i].location);
            } // error
            if (tokens[i].getRaw() == ",") {
                i++;
                continue;
            } // skip over
            std::string name = "";
            Element statement;
            if (i + 1 < tokens.size() && tokens[i + 1].getRaw() == ":") {
                if (!tokens[i].isName()) // TODO: support strings, only names are supported for now
                    throw std::runtime_error("unexpected tag in tuple at: " + tokens[i].location);
                name = tokens[i].getRaw();
                i += 2;
                statement = parseStatement(tokens, i);
            } else {
                statement = parseStatement(tokens, i);
            }
            result.tuple->push_back(std::pair<std::string, Element>(name, statement));
        }

        result.reindexTuple();
        return result;
    }

    inline Element ParseTreeBuilder::buildParseTree(std::vector<Token> tokens) {
        tokens.insert(tokens.begin(), Token("[", "START"));
        tokens.push_back(Token("]", "END"));
        size_t startIndex = 0;
        return parseTuple(tokens, startIndex);
    }

    inline Element::Element() {}

    inline Element::Element(Token v) {
        value = v;
        location = v.location;
    }

    inline Element::Element(std::deque<std::pair<std::string, Element>> t) { tuple = t; }

    inline Element::Element(std::deque<Element> s) {
        statement = std::deque<std::pair<std::string, Element>>();
        for (size_t i = 0; i < s.size(); i++) { statement->push_back({std::to_string(i), s[i]}); }
    }

    inline Element::Element(const Element& e) {
        tuple = e.tuple;
        statement = e.statement;
        value = e.value;
        location = e.location;
    }

    inline std::string Element::getDiagnosticString() {
        using namespace std;
        using namespace estd::string_util;
        if (this->tuple != nullptr) {
            string result = "";
            size_t idx = -1;
            for (std::pair<string, Element> namedStatement : this->tuple) {
                idx++;
                if (namedStatement.first != "") {
                    result += namedStatement.first;
                    result += ": ";
                } else {
                    // result += to_string(idx);
                    // result += ": ";
                }

                result += namedStatement.second.getDiagnosticString();
                result += ",\n";
            }
            if (result == "") result = "[]";
            else
                result = "[\n" + indent(result, "   ") + "]";
            return result;
        } else if (this->statement != nullptr) {
            string result = "";
            bool notFirst = false;
            for (auto& [k, e] : this->statement) {
                if (notFirst) { result += " "; }
                notFirst = true;
                if (e.isStatement()) {
                    result += "[" + e.getDiagnosticString() + "]";
                } else {
                    result += e.getDiagnosticString();
                }
            }
            return result;
        } else if (this->value != nullptr) {
            return this->value->getRaw();
        }
        return "unknown type";
    }

    inline estd::clone_ptr<Element> Element::operator[](std::string name) {
        Element& e = *this;

        if (e.tuple != nullptr) {
            for (size_t i = 0; i < e.tuple->size(); i++) {
                if (e.tuple[i].first == name || name == std::to_string(i)) { return e.tuple[i].second; }
            }
        }
        if (e.statement != nullptr) {
            for (size_t i = 0; i < e.statement->size(); i++) {
                if (e.statement[i].first == name) { return e.statement[i].second; }
            }
        }
        return nullptr;
    }

    inline estd::clone_ptr<Element> Element::operator[](size_t id) {
        Element& e = *this;
        if (e.tuple != nullptr) {
            if (id >= e.tuple->size()) return nullptr;
            return e.tuple[id].second;
        } else if (e.statement != nullptr) {
            if (id >= e.statement->size()) return nullptr;
            return e.statement[id].second;
        }
        return nullptr;
    }

    inline std::string Element::getLabel(size_t id) {
        Element& e = *this;
        if (e.tuple != nullptr) {
            if (id >= e.tuple->size())
                throw std::runtime_error("element " + std::to_string(id) + " is out of range of tuple" + location);
            return e.tuple[id].first;
        } else {
            throw std::runtime_error("element is not a tuple" + location);
        }
    }

    inline size_t Element::size() {
        Element& e = *this;
        if (e.tuple != nullptr) {
            return e.tuple->size();
        } else if (e.statement != nullptr) {
            return e.statement->size();
        }
        return 0;
    }

    inline bool Element::onlyContains(std::set<std::string> names) {
        // TODO: make this work for statement (you can just use size currently)
        if (tuple == nullptr) return false;
        for (size_t i = 0; i < this->tuple->size(); i++) {
            if (names.find(this->tuple[i].first) == names.end()) { return false; }
        }
        return true;
    }

    inline bool Element::contains(std::string name) {
        if (tuple != nullptr) {
            for (size_t i = 0; i < this->tuple->size(); i++) {
                if (this->tuple[i].first == name || name == std::to_string(i)) { return true; }
            }
        } else if (this->statement != nullptr) {
            try {
                if (std::stoull(name, nullptr, 10) <= this->statement->size()) return true;
            } catch (...) {}
        }
        return false;
    }

    inline bool Element::contains(size_t id) {
        if (this->tuple != nullptr) {
            if (id >= this->tuple->size()) return false;
            return true;
        } else if (this->statement != nullptr) {
            if (id >= this->statement->size()) return false;
            return true;
        }
        return false;
    }

    inline void Element::reindexStatment() {
        for (size_t i = 0; i < statement->size(); i++) { statement[i].first = std::to_string(i); }
    }
    inline void Element::reindexTuple() {
        int idx = 0;
        for (size_t i = 0; i < tuple->size(); i++) {
            // if it is either a number or an empty string.
            // number labels cannot be added to tuples, they are instead just not labled.
            if (tuple[i].first.empty() || tuple[i].first.find_first_not_of("0123456789") == std::string::npos) {
                tuple[i].first = std::to_string(idx);
                idx++;
            }
        }
    }

    inline Element Element::popFront() {
        if (tuple != nullptr) {
            Element t = tuple->front().second;
            tuple->pop_front();
            reindexTuple();
            return t;
        }
        if (statement != nullptr) {
            Element t = statement->front().second;
            statement->pop_front();
            reindexStatment();
            return t;
        }
        throw std::runtime_error("No elements to pop");
    }
    inline void Element::pushFront(Element e) { pushFront("", e); }
    inline void Element::pushFront(std::string n, Element e) {
        if (tuple != nullptr) {
            tuple->push_front({n, e});
            reindexTuple();
        }
        if (statement != nullptr) {
            if (n != "") throw std::runtime_error("Error: cannot use this function with non empty string on statment.");
            statement->push_front({n, e});
            reindexStatment();
        }
    }
    inline Element Element::popBack() {
        if (tuple != nullptr) {
            Element t = tuple->back().second;
            tuple->pop_back();
            reindexTuple();
            return t;
        }
        if (statement != nullptr) {
            Element t = statement->back().second;
            statement->pop_back();
            reindexStatment();
            return t;
        }
        throw std::runtime_error("No elements to pop");
    }
    inline void Element::pushBack(Element e) { pushBack("", e); }
    inline void Element::pushBack(std::string n, Element e) {
        if (tuple != nullptr) {
            tuple->push_back({n, e});
            reindexTuple();
        }
        if (statement != nullptr) {
            if (n != "") throw std::runtime_error("Error: cannot use this function with non empty string on statment.");
            statement->push_back({n, e});
            reindexStatment();
        }
    }

    inline void Element::popBack(size_t n) {
        for (size_t i = 0; i < n; i++) popBack();
    }

    inline void Element::popFront(size_t n) {
        for (size_t i = 0; i < n; i++) popFront();
    }

    inline Element Element::slice(size_t left, size_t right) {
        Element copy = *this;
        if (tuple == nullptr && statement == nullptr) throw std::runtime_error("No elements to slice");
        if (right < size()) copy.popBack(size() - right);
        copy.popFront(left);
        return copy;
    }


    inline bool Element::isTuple() { return this->tuple != nullptr; }
    inline bool Element::isStatement() { return this->statement != nullptr; }
    inline bool Element::isToken() {
        Element& e = getSingleElement();
        return e.value != nullptr;
    }

    inline bool Element::isString() { return isToken() && getToken().isString(); }
    inline bool Element::isComment() { return isToken() && getToken().isComment(); }
    inline bool Element::isName() { return isToken() && getToken().isName(); }
    inline bool Element::isNumber() { return isToken() && getToken().isNumber(); }
    inline bool Element::isValue() { return isToken() && getToken().isValue(); }
    inline bool Element::isRaw() { return isToken(); }

    inline Token Element::getToken() {
        Element& e = getSingleElement();
        return e.value.value();
    }
    inline std::string Element::getString() { return getToken().getString(); }
    inline std::string Element::getEscapedString() { return getToken().getEscapedString(); }
    inline std::string Element::getComment() { return getToken().getComment(); }
    inline std::string Element::getName() { return getToken().getName(); }
    inline estd::BigDec Element::getNumber() { return getToken().getNumber(); }
    inline std::string Element::getValue() { return getToken().getValue(); }
    inline std::string Element::getRaw() { return getToken().getRaw(); }
    inline Element& Element::getSingleElement() {
        if (statement != nullptr && statement->size() == 1) { return statement[0].second; }
        return *this;
    }
} // namespace omtl