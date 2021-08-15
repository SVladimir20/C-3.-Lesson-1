#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <any>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;
struct Person
{
    string first_name;
    string last_name;
    optional<string> patronymic;
};

ostream &operator<<(ostream &out, const Person &person);
bool operator<(const Person &p1, const Person &p2);
bool operator==(const Person &p1, const Person &p2);

struct PhoneNumber
{
    int country_code{};
    int city_code{};
    string number;
    optional<int> extension{};
};

ostream &operator<<(ostream &out, const PhoneNumber &phoneNumber);
bool operator<(const PhoneNumber &pN1, const PhoneNumber &pN2);

class PhoneBook
{
public:
    PhoneBook(ifstream &file);
    void SortByName();
    void SortByPhone();
    void ChangePhoneNumber(const Person &p, const PhoneNumber &pN);
    tuple<string, PhoneNumber> GetPhoneNumber(const string &lastName);
    friend ostream &operator<<(ostream &out, const PhoneBook &book);

private:
    vector<pair<Person, PhoneNumber>> phone_book;
};