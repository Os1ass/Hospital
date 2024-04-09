#include "Disease.h"

Disease::Disease()
{
    begin = Date();
    end = Date();
}

Disease::Disease(string disease)
{
    begin = Date();
    end = Date();
    string date;
    int i = 0;
    while (i < disease.length())
    {
        if (disease[i] == '-')
            break;
        date += disease[i];
        i++;
    }

    begin = Date(date);
    if (begin.day == 0)
        return;
    date.clear(); i++;

    while (i < disease.length())
    {
        if (disease[i] == '-')
            break;
        date += disease[i];
        i++;
    }

    end = Date(date);
    if (end.day == 0)
        return;
    date.clear(); i++;

    while (i < disease.length())
    {
        date += disease[i];
        i++;
    }
    name = date;
}

string Disease::getDiseaseString() const
{
    return begin.getDateString() + '-' +
        end.getDateString() + '-' + name;
}

ostream& operator<<(ostream& output, const Disease disease)
{
    output << disease.getDiseaseString();
    return output;
}

istream& operator>>(istream& input, Disease disease)
{
    string begin;
    if (!getline(input, begin, '-')) {
        input.setstate(ios::failbit);
        return input;
    }
    disease.begin = Date(begin);
    string end;
    getline(input, end, '-');
    disease.end = Date(end);
    getline(input, disease.name);

    return input;
}
