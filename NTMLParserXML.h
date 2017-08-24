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



#ifndef _NTMLPARSERXML_H
#define _NTMLPARSERXML_H



#include <string>
#include <list>
#include <fstream>



class NTMLParserXML
{
public:
    NTMLParserXML(std::list<std::string>& aTokens);
    ~NTMLParserXML();

public:
    int Parse(std::ostream& aOutput);

private:
    int ParseInstruction(std::ostream& aOutput);
    int ParseTitle(std::ostream& aOutput);
    int ParseList(std::ostream& aOutput);
    int ParseHighlighted(std::ostream& aOutput);

private:
    bool match(const std::string& strRequired);
    const std::string& GetNextToken();

protected:
    std::list<std::string>& m_aTokens;
    std::list<std::string>::iterator m_aIter;

};


#endif
