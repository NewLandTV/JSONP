#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "Json.h"

using namespace JSON;

std::string ParseKey(char* buffer, char*& next)
{
	char* end = std::strchr(buffer, '"');

	if (end == nullptr)
	{
		throw std::runtime_error("Can't find end of string");
	}

	auto ofx = end - buffer;

	next = end + 1;

	return std::string(buffer, ofx);
}

char* SkipSpaces(char* buffer)
{
	if (buffer == nullptr || nullptr)
	{
		return nullptr;
	}

	while (*buffer != '\0')
	{
		switch (*buffer)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			buffer++;

			break;
		default:
			return buffer;
		}
	}

	return nullptr;
}

inline bool CompareFloats(const double& x, const double& y)
{
	return abs(x - y) <= compareError;
}

JsonValue::JsonValue(ValueType type) noexcept : type(type)
{

}

JsonValue* JsonValue::Clone() const
{
	return new JsonNull();
}

JsonNumber::JsonNumber(const double& value) noexcept : JsonValue(ValueType::Number), isFloating(true), floatValue(value)
{

}

JsonNumber::JsonNumber(const int& value) noexcept : JsonValue(ValueType::Number), isFloating(false), intValue(value)
{

}

JsonNumber::JsonNumber(const JsonNumber& object) noexcept : JsonValue(ValueType::Number), isFloating(object.isFloating)
{
	if (isFloating)
	{
		floatValue = object.floatValue;
	}
	else
	{
		intValue = object.intValue;
	}
}

void JsonNumber::Put(const double& value) noexcept
{
	isFloating = true;
	floatValue = value;
}

void JsonNumber::Put(const int& value) noexcept
{
	isFloating = false;
	intValue = value;
}

bool JsonNumber::IsFloating() const noexcept
{
	return isFloating;
}

double JsonNumber::GetAsFloat() const
{
	if (isFloating)
	{
		return floatValue;
	}

	return static_cast<double>(intValue);
}

int JsonNumber::GetAsInt() const
{
	if (!isFloating)
	{
		return intValue;
	}

	return static_cast<int>(floatValue);
}

std::string JsonNumber::Repr() const
{
	return std::to_string(isFloating ? floatValue : intValue);
}

JsonValue* JsonNumber::Clone() const
{
	return new JsonNumber(*this);
}

bool JsonNumber::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	else if (object->type != type)
	{
		return false;
	}

	JsonNumber* number = reinterpret_cast<JsonNumber*>(object);

	if (number->isFloating || isFloating)
	{
		return CompareFloats(number->GetAsFloat(), GetAsFloat());
	}

	return number->intValue == intValue;
}

JsonString::JsonString(const char* str) : JsonValue(ValueType::String), std::string(str)
{

}

JsonString::JsonString(const JsonString& object) : JsonValue(ValueType::String), std::string(object)
{

}

JsonString::~JsonString()
{

}

void JsonString::Put(const char* str)
{
	*this = str;
}

std::string JsonString::Repr() const
{
	return std::string();
}

JsonValue* JsonString::Clone() const
{
	return new JsonString(*this);
}

bool JsonString::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	else if (object->type != type)
	{
		return false;
	}

	return *static_cast<JsonString*>(object) == *this;
}

JsonBoolean::JsonBoolean(const bool& value) noexcept : JsonValue(ValueType::Boolean), value(value)
{

}

JsonBoolean::JsonBoolean(const JsonBoolean& object) noexcept : JsonValue(ValueType::Boolean), value(object.value)
{

}

std::string JsonBoolean::Repr() const
{
	return value ? "true" : "false";
}

JsonValue* JsonBoolean::Clone() const
{
	return new JsonBoolean(*this);
}

bool JsonBoolean::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	else if (object->type != type)
	{
		return false;
	}

	return value == static_cast<JsonBoolean*>(object)->value;
}

JsonArray::JsonArray() : JsonValue(ValueType::Array)
{

}

JsonArray::~JsonArray()
{
	for (auto e : *this)
	{
		delete e;
	}
}

void JsonArray::Remove(JsonValue* item)
{
	auto it = find(begin(), end(), item);

	if (it != end())
	{
		erase(it);
	}
}

void JsonArray::Remove(const JsonValue& value)
{
	auto cmp = [&](JsonValue* item)
	{
		return value.Equal(item);
	};

	auto it = std::find_if(begin(), end(), cmp);

	if (it != end())
	{
		erase(it);
	}
}

std::string JsonArray::Repr() const
{
	if (empty())
	{
		return "[]";
	}

	const std::string sep = ", ";

	std::ostringstream rep;

	auto item = cbegin();
	auto end = cend();

	rep << '[' << (*item++)->Repr();

	for (; item != end; item++)
	{
		rep << sep << (*item)->Repr();
	}

	rep << ']';

	return rep.str();
}

JsonValue* JsonArray::Clone() const
{
	auto object = new JsonArray();

	for (auto e : *this)
	{
		object->push_back(e->Clone());
	}

	return object;
}

bool JsonArray::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	else if (object->type != type)
	{
		return false;
	}

	auto o = static_cast<JsonArray*>(object);
	const auto size = this->size();

	if (o->size() != size)
	{
		return false;
	}

	for (size_t i = 0; i < size; i++)
	{
		if (at(i) != o->at(i))
		{
			return false;
		}
	}

	return true;
}

JsonNull::JsonNull() noexcept : JsonValue(ValueType::Null)
{

}

std::string JsonNull::Repr() const
{
	return "null";
}

JsonValue* JsonNull::Clone() const
{
	return new JsonObject();
}

bool JsonNull::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	
	return object->type == type;
}

JsonObject::JsonObject() : JsonValue(ValueType::Object)
{

}

JsonObject::JsonObject(const JsonObject& object) : JsonValue(ValueType::Object)
{
	for (auto item : object)
	{
		insert(make_pair(item.first, item.second->Clone()));
	}
}

JsonObject::~JsonObject()
{
	for (auto item : *this)
	{
		delete item.second;
	}
}

void JsonObject::Put(const std::string& key, JsonValue* value)
{
	auto item = find(key);

	if (item == end())
	{
		insert(make_pair(key, value));
	}
	else
	{
		delete item->second;

		item->second = value;
	}
}

bool JsonObject::Has(const std::string& key) const
{
	return count(key);
}

std::string JsonObject::Repr() const
{
	if (empty())
	{
		return "{}";
	}

	const std::string sep = ", ";

	std::ostringstream rep;

	auto item = cbegin();
	auto end = cend();

	rep << '{' << (item->first) << ':' << item->second->Repr();

	for (; item != end; item++)
	{
		rep << sep << (item->first) << ':' << item->second->Repr();
	}

	rep << '}';

	return rep.str();
}

JsonValue* JsonObject::Clone() const
{
	return new JsonObject(*this);
}

bool JsonObject::Equal(JsonValue* object) const
{
	if (object == this)
	{
		return true;
	}
	else if (object->type != type)
	{
		return false;
	}

	auto o = static_cast<JsonObject*>(object);

	if (o->size() != size())
	{
		return false;
	}

	const auto comparePair = [](value_type const& v1, value_type const& v2)
	{
		return v1.first == v2.first && v1.second->Equal(v2.second);
	};

	return std::equal(begin(), end(), o->begin(), comparePair);
}

JsonValue* JSON::ParseValue(char* buffer, char*& next)
{
	JsonValue* value = nullptr;

	if (std::strncmp(buffer, "true", 4) == 0)
	{
		value = new JsonBoolean(true);
		next = buffer + 4;
	}
	else if (std::strncmp(buffer, "false", 5) == 0)
	{
		value = new JsonBoolean(false);
		next = buffer + 5;
	}
	else if (std::strncmp(buffer, "null", 4) == 0)
	{
		value = new JsonNull();
		next = buffer + 4;
	}
	else
	{
		throw std::runtime_error("Unknown token");
	}

	return value;
}

JsonNumber* JSON::ParseNumber(char* buffer, char*& next)
{
	std::string number;

	while (*buffer != '\0')
	{
		if (!(isdigit(*buffer) || *buffer == '.'))
		{
			break;
		}

		number.push_back(*(buffer++));
	}

	auto fi = number.find_first_of('.');

	if (fi != std::string::npos)
	{
		if (fi != number.find_last_of('.'))
		{
			throw std::runtime_error("Unknown number format");
		}

		try
		{
			double value = std::stof(number);

			next = buffer;

			return new JsonNumber(value);
		}
		catch (std::exception ex)
		{
			throw std::runtime_error("Unknown number format");
		}
	}
	else
	{
		try
		{
			int value = std::stoi(number);

			next = buffer;

			return new JsonNumber(value);
		}
		catch (std::exception ex)
		{
			throw std::runtime_error("Unknown number format");
		}
	}
}

JsonString* JSON::ParseString(char* buffer, char*& next)
{
	auto str = ParseKey(buffer, next);

	return new JsonString(str.c_str());
}

JsonArray* JSON::ParseArray(char* buffer, char*& next)
{
	auto object = new JsonArray();

	buffer = SkipSpaces(buffer + 1);

	while (*buffer != ']' && *buffer != '\0')
	{
		try
		{
			if (*buffer == '{')
			{
				object->push_back(ParseObject(buffer, buffer));
			}
			else if (*buffer == '"')
			{
				object->push_back(ParseString(buffer + 1, buffer));
			}
			else if (isdigit(*buffer))
			{
				object->push_back(ParseNumber(buffer, buffer));
			}
			else if (*buffer == '[')
			{
				object->push_back(ParseArray(buffer, buffer));
			}
			else if (isalpha(*buffer))
			{
				object->push_back(ParseValue(buffer, buffer));
			}
			else
			{
				throw std::runtime_error("Uncompleted Array");
			}
		}
		catch (std::exception ex)
		{
			delete object;

			throw ex;
		}

		buffer = SkipSpaces(buffer);

		if (*buffer == ',')
		{
			buffer++;

			buffer = SkipSpaces(buffer);
		}
	}

	next = buffer + 1;

	return object;
}

JsonObject* JSON::ParseObject(char* buffer, char*& next)
{
	auto const origin = buffer;
	auto object = new JsonObject();
	bool parseKey = true;

	std::string key;

	buffer = SkipSpaces(buffer);

	if (*buffer != '{')
	{
		delete object;

		throw std::runtime_error("No Start");
	}

	buffer++;

	while ((*buffer != '}' && *buffer != '\0'))
	{
		if (parseKey)
		{
			if (*buffer == '\"')
			{
				key = ParseKey(buffer + 1, buffer);
				buffer = std::strchr(buffer, ':');
				buffer = SkipSpaces(buffer);

				if (buffer == nullptr)
				{
					delete object;

					throw std::runtime_error("Can't find end of token");
				}

				if (*buffer != ':')
				{
					delete object;

					throw std::runtime_error("Can't find end of key");
				}

				buffer = SkipSpaces(++buffer);
				parseKey = false;
			}
			else
			{
				buffer = SkipSpaces(buffer);
			}
		}
		else
		{
			try
			{
				if (*buffer == '{')
				{
					object->Put(key, ParseObject(buffer, buffer));
				}
				else if (*buffer == '"')
				{
					object->Put(key, ParseString(buffer + 1, buffer));
				}
				else if (isdigit(*buffer))
				{
					object->Put(key, ParseNumber(buffer, buffer));
				}
				else if (*buffer == '[')
				{
					object->Put(key, ParseArray(buffer, buffer));
				}
				else if (isalpha(*buffer))
				{
					object->Put(key, ParseValue(buffer, buffer));
				}
				else
				{
					throw std::runtime_error("Uncompleted Object");
				}
			}
			catch (std::exception ex)
			{
				delete object;

				throw ex;
			}

			buffer = SkipSpaces(buffer);

			if (*buffer == ',')
			{
				buffer++;
				parseKey = true;
				buffer = SkipSpaces(buffer);
			}
			else
			{
				break;
			}
		}
	}

	next = buffer + 1;

	return object;
}