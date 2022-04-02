/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Language identification based on trigrams.
 *
 * More info: https://towardsdatascience.com/understanding-cosine-similarity-and-its-application-fd42f585296a
 */

#include <cmath>
#include <codecvt>
#include <locale>
#include <iostream>

#include "Lequel.h"

using namespace std;

/*
 * Builds a trigram profile for a given text.
 *
 * Parameters:
 *  text -      A list of lines (Text).
 *
 * Returns: the trigram profile.
 */
TrigramProfile buildTrigramProfile(const Text &text)
{
    wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;

    TrigramProfile profile;
    WTrigramProfile wprofile;
    
    for (auto i : text) 
    {
        wstring unicodeString = converter.from_bytes(i);        
        
        //agrega un nuevo trigrama al profile si no existe, o le suma 1 al puntaje si existe.
        //completa todo en un map con wsting y float, parecido a TrigramProfile

        for (int j = 0; j < (unicodeString.size() - 2); j++)        
        {
            wprofile[unicodeString.substr(j, 3)] += 1;      
        }
    }

    //copia la informacion del perfil de trigramas con wstring(WTrigramProfile), al que usa string (TrigramProfile)
    //convirtiendo los wstring a string en el proceso
    for (auto l : wprofile)
    {
        string aux = converter.to_bytes(l.first);
        profile[aux]=l.second;

    }

    return profile; 
}



/*
 * Normalizes a trigram profile.
 *
 * Parameters:
 *  trigramProfile -    Trigram profile.
 */
void normalizeTrigramProfile(TrigramProfile &trigramProfile)
{
    float counter = 0.0f;

    for(auto i : trigramProfile){
        counter += i.second * i.second;         // adds the squared frecuencies
    }
    if(counter != 0){                           // avoids dividing by 0
        for(auto &element : trigramProfile){
            element.second = element.second / sqrtf(counter);       //normalize every frecuency
        }
    }
}

/*
 * Calculates the cosine similarity between a text trigram profile
 * and a language trigram profile.
 *
 * Parameters:
 *  textProfile -       Text trigram profile.
 *  languageProfile -   Language trigram profile.
 *
 * Returns: the cosine similarity score.
 */
float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile)
{
   float counter = 0.0f;
    for(auto &i : textProfile)
    {
        for (auto& k : languageProfile)
        {
            if ((i.first == k.first))
            {
                counter += i.second * k.second;         
            }
        }
    }  
    return counter;
}

/*
 * Identifies the language of a text.
 *
 * Parameters:
 *  text -      A list of lines (Text).
 *
 * Returns: The language code of the most likely language.
 */
string identifyLanguage(const Text &text, Languages &languages)
{
    float MostSimilar;
    float auxSimilarity;
    string languageCode;

    TrigramProfile triProfile = buildTrigramProfile(text);

    normalizeTrigramProfile(triProfile);

    float maxSimilarity = 0;

    for(auto &i : languages)
    {
        float similarity = getCosineSimilarity(triProfile, i.trigramProfile);

        if (similarity > maxSimilarity)         //checks if the program found a new more similar profile
        {
            maxSimilarity = similarity;
            languageCode = i.languageCode;
        }
    }
    return languageCode; 
}
