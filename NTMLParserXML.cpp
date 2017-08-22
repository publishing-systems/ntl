/* Copyright (C) 2017 Stephan Kreutzer
 *
 * This file is part of ntml_to_xml_1, an extension for the
 * digital_publishing_workflow_tools package.
 *
 * part of ntml_to_xml_1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 or any later version,
 * as published by the Free Software Foundation.
 *
 * part of ntml_to_xml_1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public License 3
 * along with part of ntml_to_xml_1. If not, see <http://www.gnu.org/licenses/>.
 */



#include "NTMLParserXML.h"

#include <stdexcept>
#include <iostream>
#include <sstream>



NTMLParserXML::NTMLParserXML(std::list<std::string>& aTokens):
  m_aTokens(aTokens),
  m_aIter(m_aTokens.begin())
{

}

NTMLParserXML::~NTMLParserXML()
{

}

int NTMLParserXML::Parse(std::ostream& aOutput)
{
    m_aIter = m_aTokens.begin();

    aOutput << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    aOutput << "<!-- This file was created by ntml_to_xml_1, which is free software licensed under the GNU Affero General Public License 3 or any later version (see https://github.com/publishing-systems/ntml/ and http://www.publishing-systems.org). -->\n";
    aOutput << "<ntml version=\"0.2\">\n";

    std::string strParagraph;

    while (m_aIter != m_aTokens.end())
    {
        const std::string strToken = *m_aIter;

        if (strToken == "#")
        {
            if (strParagraph.empty() == false)
            {
                aOutput << "<p>" << strParagraph << "</p>";
                strParagraph.clear();
            }

            try
            {
                if (ParseInstruction(aOutput) != 0)
                {
                    return -1;
                }
            }
            catch (std::runtime_error ex)
            {
                std::cout << "ntml_to_xml_1: Parser: " << ex.what() << std::endl;
                return -1;
            }
        }
        else if (strToken == "::")
        {
            std::stringstream aHighlighted;

            if (ParseHighlighted(aHighlighted) != 0)
            {
                return -1;
            }

            strParagraph += aHighlighted.str();
        }
        else
        {
            strParagraph += *m_aIter;
        }

        ++m_aIter;
    }

    if (strParagraph.empty() == false)
    {
        aOutput << "<p>" << strParagraph << "</p>";
    }

    aOutput << "</ntml>" << std::endl;

    return 0;
}

int NTMLParserXML::ParseInstruction(std::ostream& aOutput)
{
    std::string strInstruction = GetNextToken();

    if (strInstruction == "t")
    {
        if (ParseTitle(aOutput) != 0)
        {
            return -1;
        }
    }
    else if (strInstruction == "b")
    {
        if (ParseList(aOutput) != 0)
        {
            return -1;
        }
    }
    else
    {
        std::cout << "ntml_to_xml_1: Parser: Unknown instruction '" << strInstruction << "'." << std::endl;
        return -1;
    }

    return 0;
}

int NTMLParserXML::ParseTitle(std::ostream& aOutput)
{
    match(" ");

    std::string strTitle;

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken.find('\n') == 0 ||
            strToken.find('\r') == 0)
        {
            break;
        }

        strTitle += strToken;
    }

    aOutput << "<title>" << strTitle << "</title>";
}

int NTMLParserXML::ParseList(std::ostream& aOutput)
{
    std::string strToken = GetNextToken();

    if (strToken != "\n" &&
        strToken != "\r" &&
        strToken != "\r\n")
    {
        std::cout << "ntml_to_xml_1: Parser: List instruction isn't followed by a newline character." << std::endl;
        return -1;
    }

    aOutput << "<list>";

    match(" ");

    std::string strItemText;

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken == "\n" ||
            strToken == "\r" ||
            strToken == "\r\n")
        {
            aOutput << "<item>" << strItemText << "</item>";
            strItemText.clear();

            std::list<std::string>::iterator aIterLookahead = m_aIter;
            ++aIterLookahead;

            if (*aIterLookahead == " ")
            {
                m_aIter = aIterLookahead;
                continue;
            }
            else
            {
                break;
            }
        }

        strItemText += strToken;
    }

    aOutput << "</list>";
    return 0;
}

int NTMLParserXML::ParseHighlighted(std::ostream& aOutput)
{
    aOutput << "<highlighted>";

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken == "::")
        {
            break;
        }

        aOutput << strToken;
    }

    aOutput << "</highlighted>";
    return 0;
}

bool NTMLParserXML::match(const std::string& strRequired)
{
    const std::string& strToken = GetNextToken();

    if (strToken == strRequired)
    {
        return true;
    }

    throw std::runtime_error(std::string("Token '") + strRequired + std::string("' was expected, but token '") + strToken + std::string("' was found."));
}

const std::string& NTMLParserXML::GetNextToken()
{
    ++m_aIter;

    if (m_aIter == m_aTokens.end())
    {
        throw std::runtime_error("More tokens were expected, but no more tokens were found.");
    }

    return *m_aIter;
}
