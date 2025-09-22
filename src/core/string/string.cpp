#include "core/string/string.h"
#include "core/memory/allocator.h"
#include <cstring>   // <--- เปลี่ยนจาก crt.h
#include <cctype>    // <--- เพิ่มเข้ามาสำหรับ tolower
#include <cmath>     // <--- เพิ่มเข้ามาสำหรับ log10, floor, pow
#include <cstdio>    // <--- เพิ่มเข้ามาสำหรับ sprintf (หรือจะ implement toCString เองก็ได้)

namespace AEngine
{

StringView::StringView(const char* str) 
    : begin(str), end(str ? str + strlen(str) : nullptr) 
{}

StringView::StringView(const char* str, u32 len) 
    : begin(str), end(str + len) 
{}

// ฟังก์ชันพื้นฐานที่คัดลอกมาจาก Lumix แต่ใช้ standard library
char toLower(char c) {
	return (char)tolower(c);
}

bool isLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool isNumeric(char c) { return c >= '0' && c <= '9'; }
bool isUpperCase(char c) { return c >= 'A' && c <= 'Z'; }
bool isWhitespace(char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }

String::String(IAllocator& allocator)
	: m_allocator(allocator)
{
	m_small[0] = '\0';
	m_size = 0;
}

String::String(const String& rhs, u32 start, u32 length)
	: m_allocator(rhs.m_allocator)
{
	m_size = 0;
	*this = StringView(rhs.c_str() + start, length);
}

String::String(StringView rhs, IAllocator& allocator)
	: m_allocator(allocator)
{
	m_size = 0;
	*this = rhs;
}

String::String(const String& rhs)
	: m_allocator(rhs.m_allocator)
{
	m_size = 0;
	*this = rhs;
}

String::String(String&& rhs)
	: m_allocator(rhs.m_allocator)
{
	if (rhs.isSmall()) {
		memcpy(m_small, rhs.m_small, sizeof(m_small));
		rhs.m_small[0] = '\0';
	}
	else {
		m_big = rhs.m_big;
		rhs.m_big = nullptr;
	}
	m_size = rhs.m_size;
	rhs.m_size = 0;
}

void String::operator=(String&& rhs)
{
	if (&rhs == this) return;

	if (!isSmall()) {
		m_allocator.deallocate(m_big);
	}

	if (rhs.isSmall()) {
		memcpy(m_small, rhs.m_small, sizeof(m_small));
		rhs.m_small[0] = '\0';
	}
	else {
		m_big = rhs.m_big;
		rhs.m_big = nullptr;
	}
	m_size = rhs.m_size;
	rhs.m_size = 0;
}

String::~String() { if (!isSmall()) m_allocator.deallocate(m_big); }

char String::operator[](u32 index) const
{
	return isSmall() ? m_small[index] : m_big[index];
}

void String::operator=(const String& rhs)
{
	if (&rhs == this) return;
	*this = StringView{rhs};
}

void String::operator=(StringView rhs) {
	if (!isSmall()) {
		m_allocator.deallocate(m_big);
	}
		
	if (rhs.size() < sizeof(m_small)) {
		if(rhs.begin) memcpy(m_small, rhs.begin, rhs.size());
		m_small[rhs.size()] = '\0';
	}
	else {
		m_big = (char*)m_allocator.allocate(rhs.size() + 1, 1);
		if(rhs.begin) memcpy(m_big, rhs.begin, rhs.size());
		m_big[rhs.size()] = '\0';
	}
	m_size = rhs.size();
}

bool String::operator!=(StringView rhs) const { return !equalStrings(*this, rhs); }
bool String::operator==(StringView rhs) const { return equalStrings(*this, rhs); }

void String::resize(u32 size) {
	if (isSmall()) {
		if (size < sizeof(m_small)) {
			m_small[size] = '\0';
		}
		else {
			char* tmp = (char*)m_allocator.allocate(size + 1, 1);
			memcpy(tmp, m_small, m_size + 1);
			tmp[size] = '\0';
			m_big = tmp;
		}
	}
	else {
		if (size < sizeof(m_small)) {
			char* tmp = m_big;
			memcpy(m_small, tmp, size);
			m_small[size] = '\0';
			m_allocator.deallocate(tmp);
		}
		else {
			m_big = (char*)m_allocator.reallocate(m_big, size + 1, m_size + 1, 1);
			m_big[size] = '\0';
		}
	}
	m_size = size;
}

String& String::add(StringView value) {
	const int old_s = m_size;
	resize(m_size + value.size());
	char* data = getMutableData();
	if(value.begin) memcpy(data + old_s, value.begin, value.size());
	data[old_s + value.size()] = '\0';
	return *this;
}

void String::eraseRange(u32 position, u32 length) {
    if (position + length > m_size) return;

    char* data = getMutableData();
    memmove(data + position, data + position + length, m_size - position - length);
    resize(m_size - length);
}

void String::eraseAt(u32 position) { eraseRange(position, 1); }

void String::insert(u32 position, StringView value) {
    const int old_size = m_size;
    const int len = value.size();
    resize(old_size + len);

    char* data = getMutableData();
    memmove(data + position + len, data + position, old_size - position);
    memcpy(data + position, value.begin, len);
}

void String::insert(u32 position, const char* value) { insert(position, StringView(value)); }

// --- Global string functions ---

int compareStringInsensitive(StringView lhs, StringView rhs) {
    // ... implementation from Lumix ...
    return 0; // Placeholder
}

int compareString(StringView lhs, StringView rhs) {
    return strncmp(lhs.begin, rhs.begin, lhs.size() < rhs.size() ? lhs.size() : rhs.size());
}

bool equalStrings(StringView lhs, StringView rhs) {
	if (rhs.size() != lhs.size()) return false;
    if (rhs.begin == lhs.begin) return true;
    if (!rhs.begin || !lhs.begin) return false;
	return strncmp(lhs.begin, rhs.begin, lhs.size()) == 0;
}

bool equalStrings(const char* lhs, const char* rhs) {
	return strcmp(lhs, rhs) == 0;
}

bool equalIStrings(StringView lhs, StringView rhs) {
    // ... implementation from Lumix ...
    return false; // Placeholder
}

int stringLength(const char* str) { return (int)strlen(str); }

// ... (คัดลอกฟังก์ชันอื่นๆ ที่เหลือจาก string.cpp ของ Lumix มาที่นี่)
// ... (เช่น toCString, fromCString, find, etc.)
// ... (ฟังก์ชันเหล่านี้ส่วนใหญ่ใช้ cstring และ cmath ซึ่งเราได้ include ไว้แล้ว)

} // namespace AEngine