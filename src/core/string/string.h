#pragma once

#include "core/memory/core.h"
#include "core/memory/allocator.h"
#include "core/memory/span.h" // เราจะสร้างไฟล์นี้ในภายหลัง แต่ใส่ include ไว้ก่อน
#include "core/hash.h" // เราจะสร้างไฟล์นี้ในภายหลัง แต่ใส่ include ไว้ก่อน

// ฟังก์ชันส่วนใหญ่จะอยู่ใน .cpp แต่เราประกาศไว้ที่นี่
namespace AEngine {
	
struct IAllocator;
template <int SIZE> struct StaticString;

int stringLength(const char* str);

struct StringView {
	StringView() {}
	StringView(const char* str);
	StringView(const char* str, u32 len);
	StringView(const char* begin, const char* end);
	template <int N> StringView(const StaticString<N>& str);

	u32 size() const { return u32(end - begin); }
	char operator[](u32 idx) const { return begin[idx]; }
	char back() const { return *(end - 1); }
	void removeSuffix(u32 count) { end -= count; }
	void removePrefix(u32 count) { begin += count; }
	bool empty() const { return begin == end || (begin && !begin[0]); }
	bool operator==(const StringView& rhs) const;

	const char* begin = nullptr;
	const char* end = nullptr;
};

char toLower(char c);
const char* findInsensitive(StringView haystack, StringView needle);
void toCStringHex(u8 value, Span<char> output);
void toCStringPretty(i32 value, Span<char> output);
void toCStringPretty(u32 value, Span<char> output);
void toCStringPretty(u64 value, Span<char> output);
char* toCString(bool value, Span<char> output);
char* toCString(i32 value, Span<char> output);
char* toCString(i64 value, Span<char> output);
char* toCString(u64 value, Span<char> output);
char* toCString(u32 value, Span<char> output);
char* toCString(float value, Span<char> output, int after_point);
char* toCString(double value, Span<char> output, int after_point);

const char* fromCString(StringView input, i32& value);
const char* fromCString(StringView input, u64& value);
const char* fromCString(StringView input, i64& value);
const char* fromCString(StringView input, u32& value);
const char* fromCString(StringView input, u16& value);
const char* fromCString(StringView input, float& value);
const char* fromCString(StringView input, bool& value);
const char* fromCStringOctal(StringView input, u32& value);

char* copyString(Span<char> output, StringView source);
char* catString(Span<char> output, StringView source);
bool makeLowercase(Span<char> output, StringView source);
bool equalStrings(const char* lhs, const char* rhs);
bool equalStrings(StringView lhs, StringView rhs);
bool equalIStrings(StringView lhs, StringView rhs);
int compareString(StringView lhs, StringView rhs);
int compareStringInsensitive(StringView lhs, StringView rhs);
const char* reverseFind(StringView haystack, char c);
const char* find(StringView str, char needle);
const char* find(StringView str, StringView needle);
bool contains(StringView haystack, char needle);
bool startsWith(StringView str, StringView prefix);
bool startsWithInsensitive(StringView str, StringView prefix);
bool endsWith(StringView str, StringView suffix);
bool endsWithInsensitive(StringView str, StringView suffix);
bool isLetter(char c);
bool isNumeric(char c);
bool isUpperCase(char c);
bool isWhitespace(char c);

// String class (เหมือนของ Lumix แต่เปลี่ยน Namespace)
class String {
public:
	explicit String(IAllocator& allocator);
	String(const String& rhs, u32 start, u32 length);
	String(StringView rhs, IAllocator& allocator);
	String(const String& rhs);
	String(String&& rhs);
	~String();

	void resize(u32 size);
	char* getMutableData() { return isSmall() ? m_small : m_big; }
	char operator[](u32 index) const;
	void operator=(const String& rhs);
	void operator=(StringView rhs);
	void operator=(String&& rhs);
	bool operator!=(StringView rhs) const;
	bool operator==(StringView rhs) const;
	operator StringView() const { return StringView{c_str(), m_size}; }
	u32 length() const { return m_size; }
	const char* c_str() const { return isSmall() ? m_small : m_big; }
	
    // --- เพิ่มบรรทัดนี้ ---
    bool empty() const { return m_size == 0; }
    // --------------------

    String& add(StringView value);
	void insert(u32 position, const char* value);
	void insert(u32 position, StringView value);
	void eraseAt(u32 position);
	void eraseRange(u32 position, u32 length);

	IAllocator& m_allocator;
private:
	static bool isSmall(u32 size) { return size < sizeof(m_small); }
	bool isSmall() const { return isSmall(m_size); }

	u32 m_size;
	union {
		char* m_big;
		char m_small[16];
	};
};

template <int N> StringView::StringView(const StaticString<N>& str)
	: begin(str.data)
	, end(str.data + stringLength(str.data))
{}

} // namespace AEngine