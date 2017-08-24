/* Copyright (C) 2017 Stephan Kreutzer
 *
 * This file is part of ntml_to_xml_1, an extension for the
 * digital_publishing_workflow_tools package.
 *
 * ntml_to_xml_1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 or any later version,
 * as published by the Free Software Foundation.
 *
 * ntml_to_xml_1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public License 3
 * along with ntml_to_xml_1. If not, see <http://www.gnu.org/licenses/>.
 */



#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <locale>

#include "NTMLParserXML.h"



bool IsOtherPunct(const char& cChar, const std::locale& aLocale);
bool IsOtherSpace(const char& cChar, const std::locale& aLocale);



int main(int nArgc, char** pArgv)
{
    std::cout << "ntml_to_xml_1 Copyright (C) 2017 Stephan Kreutzer\n"
              << "This program comes with ABSOLUTELY NO WARRANTY.\n"
              << "This is free software, and you are welcome to redistribute it\n"
              << "under certain conditions. See the GNU Affero General Public License 3\n"
              << "or any later version for details. Also, see the source code repository\n"
              << "https://github.com/publishing-systems/ntml/ and\n"
              << "the project website http://www.publishing-systems.org.\n"
              << std::endl;

    if (nArgc < 3)
    {
        std::cout << "Usage:\n"
                  << "\n"
                  << "\tntml_to_xml_1 input-ntml-file output-xml-file\n"
                  << std::endl;

        return 1;
    }

    std::list<std::string> aTokens;

    {
        std::locale aLocale;
        std::ifstream aInput(pArgv[1], std::ifstream::binary);

        if (aInput.is_open() != true)
        {
            std::cout << "ntml_to_xml_1: Couldn't open input file \"" << pArgv[1] << "\"." << std::endl;
            return -1;
        }

        const int BUFFERSIZE = 256;
        char pBuffer[BUFFERSIZE];
        int nBytesRead = 0;
        std::string strBuffer;

        do
        {
            aInput.read(pBuffer, BUFFERSIZE);

            if (aInput.bad() == true)
            {
                std::cout << "ntml_to_xml_1: An error occurred while reading input file \"" << pArgv[1] << "\"." << std::endl;
                return -1;
            }

            nBytesRead = aInput.gcount();
            char cLast = '\0';

            for (int i = 0; i < nBytesRead; i++)
            {
                char cCurrent = pBuffer[i];

                if (cCurrent == '#')
                {
                    if (strBuffer.empty() == false)
                    {
                        aTokens.push_back(strBuffer);
                        strBuffer.clear();
                    }

                    strBuffer = cCurrent;
                    aTokens.push_back(strBuffer);
                    strBuffer.clear();
                }
                else if (cCurrent == '\n' ||
                         cCurrent == '\r')
                {
                    if (cLast != '\n' &&
                        cLast != '\r' &&
                        strBuffer.empty() == false)
                    {
                        aTokens.push_back(strBuffer);
                        strBuffer.clear();
                    }

                    strBuffer += cCurrent;
                }
                else if (cCurrent == ' ' ||
                         cCurrent == ':')
                {
                    if (cCurrent != cLast &&
                        strBuffer.empty() == false)
                    {
                        aTokens.push_back(strBuffer);
                        strBuffer.clear();
                    }

                    strBuffer += cCurrent;
                }
                else if (std::isalpha(cCurrent, aLocale) == true ||
                         std::isdigit(cCurrent, aLocale) == true ||
                         IsOtherPunct(cCurrent, aLocale) == true ||
                         IsOtherSpace(cCurrent, aLocale) == true)
                {
                    if (std::isalpha(cLast, aLocale) != true &&
                        std::isdigit(cLast, aLocale) != true &&
                        IsOtherPunct(cLast, aLocale) != true &&
                        IsOtherSpace(cLast, aLocale) != true &&
                        strBuffer.empty() == false)
                    {
                        aTokens.push_back(strBuffer);
                        strBuffer.clear();
                    }

                    strBuffer += cCurrent;
                }
                else
                {
                    // TODO: hex.
                    std::cout << "ntml_to_xml_1: Tokenizer: Unexpected character '" << cCurrent << "' in input file \"" << pArgv[1] << "\"." << std::endl;
                    return -1;
                }

                cLast = cCurrent;
            }

        } while (aInput.eof() != true);

        if (strBuffer.empty() == false)
        {
            aTokens.push_back(strBuffer);
        }

        aInput.close();
    }

#if 0
    int i = 0;

    for (std::list<std::string>::iterator iter = aTokens.begin();
         iter != aTokens.end();
         iter++)
    {
        std::cout << i << ": " << *iter << std::endl;

        ++i;
    }
#endif

    std::ofstream aOutput(pArgv[2], std::ios_base::out);

    if (aOutput.is_open() != true)
    {
        std::cout << "ntml_to_xml_1: Couldn't open output file \"" << pArgv[2] << "\"." << std::endl;
        return -1;
    }

    NTMLParserXML aParser(aTokens);

    if (aParser.Parse(aOutput) != 0)
    {
        std::cout << "ntml_to_xml_1: An error occurred while parsing." << std::endl;
        aOutput.close();
        return -1;
    }

    aOutput.close();

    return 0;
}

bool IsOtherPunct(const char& cChar, const std::locale& aLocale)
{
    return std::ispunct(cChar, aLocale) == true &&
           cChar != '#' &&
           cChar != ':';
}

bool IsOtherSpace(const char& cChar, const std::locale& aLocale)
{
    return std::isspace(cChar, aLocale) &&
           cChar != ' ' &&
           cChar != '\n' &&
           cChar != '\r';
}
