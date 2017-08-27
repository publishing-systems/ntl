/* Copyright (C) 2017 Stephan Kreutzer
 *
 * This file is part of ntl_to_xml_1, an extension for the
 * digital_publishing_workflow_tools package.
 *
 * ntl_to_xml_1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 or any later version,
 * as published by the Free Software Foundation.
 *
 * ntl_to_xml_1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public License 3
 * along with ntl_to_xml_1. If not, see <http://www.gnu.org/licenses/>.
 */



#include "NTLParserXML.h"

#include <stdexcept>
#include <iostream>
#include <sstream>



NTLParserXML::NTLParserXML(std::list<std::string>& aTokens):
  m_aTokens(aTokens),
  m_aIter(m_aTokens.begin())
{

}

NTLParserXML::~NTLParserXML()
{

}

int NTLParserXML::Parse(std::ostream& aOutput)
{
    m_aIter = m_aTokens.begin();

    aOutput << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    aOutput << "<!-- This file was created by ntl_to_xml_1, which is free software licensed under the GNU Affero General Public License 3 or any later version (see https://github.com/publishing-systems/ntl/ and http://www.publishing-systems.org). -->\n";
    aOutput << "<ntl version=\"0.1\">\n";

    std::string strList;
    std::string strTitle;
    bool bInSection = false;

    while (m_aIter != m_aTokens.end())
    {
        const std::string strToken = *m_aIter;

        if (strToken == "#")
        {
            const std::string& strInstruction = GetNextToken();

            if (strInstruction == "t")
            {
                if (strList.empty() == false)
                {
                    aOutput << "<list>" << strList << "</list>";
                    strList.clear();
                }

                std::stringstream strNoteTitle;

                if (ParseTitle(strNoteTitle) != 0)
                {
                    return -1;
                }

                strTitle += strNoteTitle.str();
            }
            else if (strInstruction == "l")
            {
                std::stringstream strListTitle;

                if (ParseListTitle(strListTitle) != 0)
                {
                    return -1;
                }

                strList += strListTitle.str();
            }
            else if (strInstruction == "s")
            {
                if (strList.empty() == false)
                {
                    aOutput << "<list>" << strList << "</list>";
                    strList.clear();
                }

                if (bInSection == true)
                {
                    aOutput << "</section>";
                }

                bInSection = true;
                aOutput << "<section>";

                if (ParseSectionTitle(aOutput) != 0)
                {
                    return -1;
                }
            }
            else if (strInstruction == "b")
            {
                if (strList.empty() == false)
                {
                    aOutput << "<list>" << strList << "</list>";
                    strList.clear();
                }

                if (ParseHorizontalLine(aOutput) != 0)
                {
                    return -1;
                }
            }
            else
            {
                if (strList.empty() == false)
                {
                    aOutput << "<list>" << strList << "</list>";
                    strList.clear();
                }

                std::cout << "ntl_to_xml_1: Parser: Unknown instruction '" << strInstruction << "'." << std::endl;
                return -1;
            }
        }
        else if (strToken == " ")
        {
            std::stringstream strListItem;

            if (ParseListItem(strListItem) != 0)
            {
                return -1;
            }

            strList += strListItem.str();
        }
        else
        {
            if (strList.empty() == false)
            {
                aOutput << "<list>" << strList << "</list>";
                strList.clear();
            }

            if (ParseParagraph(aOutput) != 0)
            {
                return -1;
            }
        }

        ++m_aIter;
    }

    if (strList.empty() == false)
    {
        aOutput << "<list>" << strList << "</list>";
    }

    if (bInSection == true)
    {
        aOutput << "</section>";
    }

    if (!strTitle.empty())
    {
        // Note title should not appear within a section, and instread of
        // holding the entire collected data back just to output the title
        // first, it goes to the end where the last section, if any, is closed.
        aOutput << strTitle;
    }

    aOutput << "</ntl>" << std::endl;

    return 0;
}

int NTLParserXML::ParseTitle(std::ostream& aOutput)
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

    return 0;
}

int NTLParserXML::ParseListTitle(std::ostream& aOutput)
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

    if (!strTitle.empty())
    {
        aOutput << "<title>" << strTitle << "</title>";
    }

    return 0;
}

int NTLParserXML::ParseListItem(std::ostream& aOutput)
{
    std::string strItemText;

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken == "\n" ||
            strToken == "\r" ||
            strToken == "\r\n")
        {
            if (strItemText.empty())
            {
                break;
            }

            aOutput << "<item>" << strItemText << "</item>";
            strItemText.clear();

            std::list<std::string>::iterator aIterLookahead = m_aIter;
            ++aIterLookahead;

            if (aIterLookahead == m_aTokens.end())
            {
                break;
            }

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
        else if (strToken.find('\n') == 0 ||
                 strToken.find('\r') == 0)
        {
            // Multiple newlines.

            if (strItemText.empty())
            {
                break;
            }

            aOutput << "<item>" << strItemText << "</item>";
            strItemText.clear();

            break;
        }
        else if (strToken == "::")
        {
            std::stringstream strHighlighted;

            if (ParseHighlighted(strHighlighted) != 0)
            {
                return -1;
            }

            strItemText += strHighlighted.str();
        }
        else
        {
            strItemText += strToken;
        }
    }

    if (strItemText.empty() != true)
    {
        aOutput << "<item>" << strItemText << "</item>";
    }

    return 0;
}

int NTLParserXML::ParseHighlighted(std::ostream& aOutput)
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

int NTLParserXML::ParseSectionTitle(std::ostream& aOutput)
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

    aOutput << "<section-title>" << strTitle << "</section-title>";
    return 0;
}

int NTLParserXML::ParseHorizontalLine(std::ostream& aOutput)
{
    aOutput << "<hr/>";

    // Consuming/ignoring whatever follows on the line.

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken.find('\n') == 0 ||
            strToken.find('\r') == 0)
        {
            break;
        }
    }

    return 0;
}

int NTLParserXML::ParseParagraph(std::ostream& aOutput)
{
    std::string strParagraph;

    if (*m_aIter == "::")
    {
        std::stringstream strHighlighted;

        if (ParseHighlighted(strHighlighted) != 0)
        {
            return -1;
        }

        strParagraph += strHighlighted.str();
    }
    else
    {
        strParagraph += *m_aIter;
    }

    while (true)
    {
        const std::string& strToken = GetNextToken();

        if (strToken.find('\n') == 0 ||
            strToken.find('\r') == 0)
        {
            std::list<std::string>::iterator aIterLookahead = m_aIter;
            ++aIterLookahead;

            if (aIterLookahead == m_aTokens.end())
            {
                break;
            }

            if (*aIterLookahead == "#" ||
                *aIterLookahead == " ")
            {
                break;
            }

            strParagraph += " ";
        }
        else if (strToken.find(" ") == 0)
        {
            std::list<std::string>::iterator aIterLookahead = m_aIter;
            ++aIterLookahead;

            if (aIterLookahead == m_aTokens.end())
            {
                break;
            }

            if (*aIterLookahead == "//")
            {
                m_aIter = aIterLookahead;
                aIterLookahead = m_aIter;
                ++aIterLookahead;

                if (aIterLookahead == m_aTokens.end())
                {
                    break;
                }

                if (aIterLookahead->find('\n') == 0 ||
                    aIterLookahead->find('\r') == 0 ||
                    aIterLookahead->find(' ') == 0)
                {
                    while (true)
                    {
                        const std::string& strToken = GetNextToken();

                        if (strToken.find('\n') == 0 ||
                            strToken.find('\r') == 0 ||
                            strToken.find(' ') == 0)
                        {
                            // Consume/ignore trailing whitespace as "//" specificly asks
                            // for a new paragraph, so there won't follow a list, probably
                            // just an # instruction in case of weird input file.
                            continue;
                        }

                        --m_aIter;
                        break;
                    }
                }

                break;
            }
            else
            {
                strParagraph += strToken;
            }
        }
        else if (strToken == "::")
        {
            std::stringstream strHighlighted;

            if (ParseHighlighted(strHighlighted) != 0)
            {
                return -1;
            }

            strParagraph += strHighlighted.str();
        }
        else
        {
            strParagraph += strToken;
        }
    }

    if (!strParagraph.empty())
    {
        aOutput << "<p>" << strParagraph << "</p>";
    }

    return 0;
}

bool NTLParserXML::match(const std::string& strRequired)
{
    const std::string& strToken = GetNextToken();

    if (strToken == strRequired)
    {
        return true;
    }

    throw std::runtime_error(std::string("Token '") + strRequired + std::string("' was expected, but token '") + strToken + std::string("' was found."));
}

const std::string& NTLParserXML::GetNextToken()
{
    ++m_aIter;

    if (m_aIter == m_aTokens.end())
    {
        throw std::runtime_error("More tokens were expected, but no more tokens were found.");
    }

    return *m_aIter;
}
