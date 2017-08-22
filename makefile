# Copyright (C) 2017  Stephan Kreutzer
#
# This file is part of digital_publishing_workflow_tools.
#
# digital_publishing_workflow_tools is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License version 3 or any later version,
# as published by the Free Software Foundation.
#
# digital_publishing_workflow_tools is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License 3 for more details.
#
# You should have received a copy of the GNU Affero General Public License 3
# along with digital_publishing_workflow_tools. If not, see <http://www.gnu.org/licenses/>.



.PHONY: build clean



build: ntml_to_xml_1



ntml_to_xml_1: ntml_to_xml_1.cpp NTMLParserXML.o
	g++ ntml_to_xml_1.cpp NTMLParserXML.o -o ntml_to_xml_1

NTMLParserXML.o: NTMLParserXML.h NTMLParserXML.cpp
	g++ NTMLParserXML.cpp -c

clean:
	rm -f ./ntml_to_xml_1
	rm -f ./NTMLParserXML.o
