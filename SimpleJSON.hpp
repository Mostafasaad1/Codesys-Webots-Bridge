#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>

// Minimal JSON parser for code generator (no external dependencies)
// Supports: objects, arrays, strings, numbers

class SimpleJSON {
public:
    enum Type { OBJECT, ARRAY, STRING, NUMBER, BOOLEAN, NULL_TYPE };
    
    Type type;
    std::string value;
    std::map<std::string, SimpleJSON> object;
    std::vector<SimpleJSON> array;

    SimpleJSON() : type(NULL_TYPE) {}
    
    static SimpleJSON parse(const std::string& json) {
        size_t pos = 0;
        return parseValue(json, pos);
    }

    std::string getString(const std::string& key, const std::string& def = "") const {
        auto it = object.find(key);
        return (it != object.end() && it->second.type == STRING) ? it->second.value : def;
    }

    const std::vector<SimpleJSON>& getArray(const std::string& key) const {
        static std::vector<SimpleJSON> empty;
        auto it = object.find(key);
        return (it != object.end() && it->second.type == ARRAY) ? it->second.array : empty;
    }

    int getInt(const std::string& key, int def = 0) const {
        auto it = object.find(key);
        if (it != object.end() && it->second.type == NUMBER) {
            return std::stoi(it->second.value);
        }
        return def;
    }


private:
    static SimpleJSON parseValue(const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);
        if (pos >= json.size()) throw std::runtime_error("Unexpected end of JSON");

        char c = json[pos];
        if (c == '{') return parseObject(json, pos);
        if (c == '[') return parseArray(json, pos);
        if (c == '"') return parseString(json, pos);
        if (c == '-' || isdigit(c)) return parseNumber(json, pos);
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }

    static SimpleJSON parseObject(const std::string& json, size_t& pos) {
        SimpleJSON obj;
        obj.type = OBJECT;
        pos++; // skip '{'
        
        while (true) {
            skipWhitespace(json, pos);
            if (json[pos] == '}') { pos++; break; }
            
            auto key = parseString(json, pos);
            skipWhitespace(json, pos);
            if (json[pos] != ':') throw std::runtime_error("Expected ':'");
            pos++;
            
            auto val = parseValue(json, pos);
            obj.object[key.value] = val;
            
            skipWhitespace(json, pos);
            if (json[pos] == ',') pos++;
            else if (json[pos] != '}') throw std::runtime_error("Expected ',' or '}'");
        }
        return obj;
    }

    static SimpleJSON parseArray(const std::string& json, size_t& pos) {
        SimpleJSON arr;
        arr.type = ARRAY;
        pos++; // skip '['
        
        while (true) {
            skipWhitespace(json, pos);
            if (json[pos] == ']') { pos++; break; }
            
            arr.array.push_back(parseValue(json, pos));
            
            skipWhitespace(json, pos);
            if (json[pos] == ',') pos++;
            else if (json[pos] != ']') throw std::runtime_error("Expected ',' or ']'");
        }
        return arr;
    }

    static SimpleJSON parseString(const std::string& json, size_t& pos) {
        SimpleJSON str;
        str.type = STRING;
        pos++; // skip opening '"'
        
        while (json[pos] != '"') {
            if (json[pos] == '\\') pos++; // skip escape
            str.value += json[pos++];
        }
        pos++; // skip closing '"'
        return str;
    }

    static SimpleJSON parseNumber(const std::string& json, size_t& pos) {
        SimpleJSON num;
        num.type = NUMBER;
        
        while (pos < json.size() && (isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-' || json[pos] == 'e' || json[pos] == 'E')) {
            num.value += json[pos++];
        }
        return num;
    }

    static void skipWhitespace(const std::string& json, size_t& pos) {
        while (pos < json.size() && isspace(json[pos])) pos++;
    }
};
