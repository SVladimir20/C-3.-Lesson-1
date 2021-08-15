#include "PhoneBook.h"
ostream &operator<<(ostream &out, const Person &person)
{
    if (person.patronymic.has_value())
    {
        return out << person.first_name << ' ' << person.last_name << ' ' << person.patronymic.value();
    }
    else
    {
        return out << person.first_name << ' ' << person.last_name;
    }
}
bool operator<(const Person &p1, const Person &p2)
{
    return tie(p1.first_name, p1.last_name, p1.patronymic) < tie(p2.first_name, p2.last_name, p2.patronymic);
}
bool operator==(const Person &p1, const Person &p2)
{
    if (tie(p1.first_name, p1.last_name, p1.patronymic) == tie(p2.first_name, p2.last_name, p2.patronymic))
    {
        return true;
    }
    return false;
}
ostream &operator<<(ostream &out, const PhoneNumber &phoneNumber)
{
    if (phoneNumber.extension.has_value())
    {
        return out << "+" << phoneNumber.country_code << "(" << phoneNumber.city_code << ")"
            << phoneNumber.number << " " << phoneNumber.extension.value();
    }
    else
    {
        return out << "+" << phoneNumber.country_code << "(" << phoneNumber.city_code << ")"
            << phoneNumber.number;
    }
}
bool operator<(const PhoneNumber &pN1, const PhoneNumber &pN2)
{
    if (pN1.extension != nullopt)
    {
        return (tie(pN1.country_code, pN1.city_code, pN1.number, pN1.extension) < tie(pN2.country_code, pN2.city_code, pN2.number, pN2.extension));
    }
    else
    {
        return (tie(pN1.country_code, pN1.city_code, pN1.number) < tie(pN2.country_code, pN2.city_code, pN2.number));
    }
}
ostream &operator<<(ostream &out, const PhoneBook &book)
{
    for (size_t i = 0; i < book.phone_book.size(); i++)
    {
        out << book.phone_book[i].first << " " << book.phone_book[i].second << endl;
    }
    return out;
}
PhoneBook::PhoneBook(ifstream &file)
{
    if (file.is_open())
    {
        string line;
        Person person;
        PhoneNumber phoneNumber;
        while (!file.eof())
        {
            getline(file, line);
            stringstream in(line);
            in >> person.last_name;
            in >> person.first_name;
            if (in.getline(&line[0], in.tellg(), ' '))
            {
                if (in.peek() != '+')
                {
                    in >> person.patronymic.emplace();
                }
                else
                {
                    person.patronymic.reset();
                }
            }
            in >> phoneNumber.country_code;
            if (in.getline(&line[0], in.tellg(), '('))
            {
                in >> phoneNumber.city_code;
            }
            if (in.getline(&line[0], in.tellg(), ')'))
            {
                in >> phoneNumber.number;
            }
            if (!in.eof())
            {
                in >> phoneNumber.extension.emplace();
            }
            else
            {
                phoneNumber.extension.reset();
            }
            phone_book.push_back(make_pair(person, phoneNumber));
        }
        file.close();
    }
    else
    {
        cout << "Не удалось открыть файл";
    }
}
void PhoneBook::SortByName()
{
    sort(phone_book.begin(), phone_book.end(), [](const auto &left, const auto &right) {return left.first < right.first; });
}
void PhoneBook::SortByPhone()
{
    sort(phone_book.begin(), phone_book.end(), [](const auto &left, const auto &right) {return left.second < right.second;});
}
void PhoneBook::ChangePhoneNumber(const Person &p, const PhoneNumber &pN)
{
    auto search = [&](const auto &book)
    {
        return book.first == p;
    };
    auto sName = find_if(phone_book.begin(), phone_book.end(), search);
    if (sName != end(phone_book))
    {
        sName->second = pN;
    }
}
tuple<string, PhoneNumber> PhoneBook::GetPhoneNumber(const string &lastName)
{
    string str;
    PhoneNumber pN;
    size_t counter = 0;
    auto sDuplicate = [&](const auto &pBook) mutable
    {
        if (pBook.first.last_name == lastName)
        {
            counter++;
            pN = pBook.second;
        }
    };
    for_each(phone_book.begin(), phone_book.end(), sDuplicate);
    if (counter == 0)
    {
        str = "Not found";
    }
    else if (counter == 1)
    {
        str.erase();
    }
    else
    {
        str = "Found more than 1";
    }
    return make_tuple(str, pN);
}