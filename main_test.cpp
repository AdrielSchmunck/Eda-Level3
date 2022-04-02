/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Tests
 */

#include <iostream>
#include <string>

#include "Lequel.h"
#include "Text.h"

using namespace std;

void print(string s)
{ 
    cout << s;
}

int fail()
{ 
    cout << "FAIL" << endl;
    return 1;
}

int pass()
{ 
    cout << "PASS" << endl;
    return 0;
}

int main()
{
    Text text;

    TrigramProfile profile;
    TrigramProfile profile2;

    float value;

    Languages languages;
    Language language;

    print("buildTrigramProfile() works when text length is shorter than trigram length... ");
    getText("hi", text);
    profile = buildTrigramProfile(text);
    if (profile.size() != 0)
        return fail();
    pass();

    print("buildTrigramProfile() works when text length matches trigram length... ");
    getText("^_^", text);
    profile = buildTrigramProfile(text);
    if ((profile.size() != 1) ||
        (profile["^_^"] != 1))
        return fail();
    pass();

    print("buildTrigramProfile() works when text length is trigram length plus one... ");
    getText("meco", text);
    profile = buildTrigramProfile(text);
    if ((profile.size() != 2) ||
        (profile["mec"] != 1) ||
        (profile["eco"] != 1))
        return fail();
    pass();

    print("buildTrigramProfile() works with repeated characters... ");
    getText("aaaa", text);
    profile = buildTrigramProfile(text);
    if ((profile.size() != 1) ||
        (profile["aaa"] != 2))
        return fail();
    pass();

    print("buildTrigramProfile() works with non-ASCII characters... ");
    getText("áéí", text);
    profile = buildTrigramProfile(text);
    if ((profile.size() != 1) ||
        (profile["áéí"] != 1))
        return fail();
    pass();

    print("normalizeTrigramProfile() does not add or remove keys... ");
    profile.clear();
    profile["ABC"] = 1;
    profile["BCD"] = 1;
    profile["CDE"] = 1;
    profile["DEF"] = 1;
    normalizeTrigramProfile(profile);
    if (profile.size() != 4)
        return fail();
    pass();

    print("normalizeTrigramProfile() works with equal frequencies... ");
    profile.clear();
    profile["ABC"] = 1;
    profile["BCD"] = 1;
    profile["CDE"] = 1;
    profile["DEF"] = 1;
    normalizeTrigramProfile(profile);
    if ((profile["ABC"] != 0.5F) ||
        (profile["BCD"] != 0.5F) ||
        (profile["CDE"] != 0.5F) ||
        (profile["DEF"] != 0.5F))
        return fail();
    pass();

    print("normalizeTrigramProfile() works with different frequencies... ");
    profile.clear();
    profile["ABC"] = 4;
    profile["BCD"] = 3;
    normalizeTrigramProfile(profile);
    if ((profile["ABC"] != 0.8F) ||
        (profile["BCD"] != 0.6F))
        return fail();
    pass();

    print("normalizeTrigramProfile() works with large numbers... ");
    profile.clear();
    profile["ABC"] = 1E9;
    normalizeTrigramProfile(profile);
    if (profile["ABC"] != 1)
        return fail();
    pass();

    print("getCosineSimilarity() works with one shared key... ");
    profile.clear();
    profile2.clear();
    profile["ABC"] = 1;
    profile2["ABC"] = 1;
    value = getCosineSimilarity(profile, profile);
    if (value != 1)
        return fail();
    pass();

    print("getCosineSimilarity() works with non-overlapping keys... ");
    profile.clear();
    profile2.clear();
    profile["ABC"] = 1;
    profile2["BCD"] = 2;
    value = getCosineSimilarity(profile, profile2);
    if (value != 0)
        return fail();
    pass();

    print("getCosineSimilarity() works with keys present in textProfile but not languageProfile... ");
    profile.clear();
    profile2.clear();
    // These aren't normalized but we are just testing...
    profile["ABC"] = 1;
    profile["BCD"] = 2;
    profile["CDE"] = 3;
    profile2["BCD"] = 4;
    profile2["CDE"] = 5;
    value = getCosineSimilarity(profile, profile2);
    if (value != 23)
        return fail();
    pass();

    print("getCosineSimilarity() works with keys mising in both textProfile and languageProfile... ");
    profile.clear();
    profile2.clear();
    profile["BCD"] = 3;
    profile["CDE"] = 4;
    profile2["ABC"] = 4;
    profile2["BCD"] = 5;
    value = getCosineSimilarity(profile, profile2);
    if (value != 15)
        return fail();
    pass();

    print("identifyLanguage() works with perfect similarity... ");
    languages.clear();
    language.languageCode = "L1";
    language.trigramProfile.clear();
    language.trigramProfile["ABC"] = 1;
    languages.push_back(language);
    language.languageCode = "L2";
    language.trigramProfile.clear();
    language.trigramProfile["BCD"] = 1;
    languages.push_back(language);
    language.languageCode = "L3";
    language.trigramProfile.clear();
    language.trigramProfile["CDE"] = 1;
    languages.push_back(language);
    language.languageCode = "L4";
    language.trigramProfile.clear();
    language.trigramProfile["DEF"] = 1;
    languages.push_back(language);
    getText("CDE", text);
    if (identifyLanguage(text, languages) != "L3")
        return fail();
    pass();

    return 0;
}
