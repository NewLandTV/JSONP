#ifndef __JSON_H__
#define __JSON_H__

#define _MBCS

#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

constexpr double compareError = 1e-3;

inline bool CompareFloats(const double& x, const double& y);

namespace JSON
{
	enum class ValueType
	{
		Number,
		String,
		Boolean,
		Array,
		Null,
		Object
	};

	class JsonValue
	{
	protected:
		JsonValue(ValueType type) noexcept;

	public:
		const ValueType type;

		virtual ~JsonValue() {}

		virtual std::string Repr() const = 0;
		virtual JsonValue* Clone() const = 0;
		virtual bool Equal(JsonValue* object) const = 0;
	};

	class JsonNumber : public JsonValue
	{
	private:
		bool isFloating;

		union
		{
			double floatValue;
			int intValue;
		};

	public:
		JsonNumber() = delete;
		JsonNumber(const double& value) noexcept;
		JsonNumber(const int& value) noexcept;
		JsonNumber(const JsonNumber& object) noexcept;
		JsonNumber(JsonNumber&&) = delete;

		void Put(const double& value) noexcept;
		void Put(const int& value) noexcept;
		bool IsFloating() const noexcept;
		double GetAsFloat() const;
		int GetAsInt() const;

		std::string Repr() const override;
		virtual JsonValue* Clone() const;
		virtual bool Equal(JsonValue* object) const;
	};

	class JsonString : public JsonValue, public std::string
	{
	public:
		JsonString() = delete;
		JsonString(const char* str);
		JsonString(const JsonString& object);
		JsonString(JsonString&&) = delete;
		~JsonString();

		using std::string::operator =;
		using std::string::operator +=;
		using std::string::operator [];

		void Put(const char* str);

		std::string Repr() const override;
		virtual JsonValue* Clone() const;
		virtual bool Equal(JsonValue* object) const;
	};

	class JsonBoolean : public JsonValue
	{
	private:
		bool value;

	public:
		JsonBoolean() = delete;
		JsonBoolean(const bool& value) noexcept;
		JsonBoolean(const JsonBoolean& object) noexcept;
		JsonBoolean(JsonBoolean&&) = delete;

		std::string Repr() const override;
		virtual JsonValue* Clone() const override;
		virtual bool Equal(JsonValue* object) const override;
	};

	class JsonArray : public JsonValue, public std::vector<JsonValue*>
	{
	public:
		JsonArray();
		~JsonArray();

		using std::vector < JsonValue*>::operator [];

		void Remove(JsonValue* item);
		void Remove(const JsonValue& value);

		std::string Repr() const override;
		virtual JsonValue* Clone() const override;
		virtual bool Equal(JsonValue* object) const override;
	};

	class JsonNull : public JsonValue
	{
	public:
		JsonNull() noexcept;
		JsonNull(const JsonNull&) = delete;
		JsonNull(JsonNull&&) = delete;

		std::string Repr() const override;
		virtual JsonValue* Clone() const override;
		virtual bool Equal(JsonValue* object) const override;
	};

	class JsonObject : public JsonValue, public std::map<std::string, JsonValue*>
	{
	public:
		JsonObject();
		JsonObject(const JsonObject& object);
		JsonObject(JsonObject&&) = delete;
		~JsonObject();

		void Put(const std::string& key, JsonValue* value);
		bool Has(const std::string& key) const;

		std::string Repr() const override;
		virtual JsonValue* Clone() const override;
		virtual bool Equal(JsonValue* object) const override;
	};

	JsonValue* ParseValue(char* buffer, char*& next);
	JsonNumber* ParseNumber(char* buffer, char*& next);
	JsonString* ParseString(char* buffer, char*& next);
	JsonArray* ParseArray(char* buffer, char*& next);
	JsonObject* ParseObject(char* buffer, char*& next);
}

#endif