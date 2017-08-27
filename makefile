# Copyright (C) 2017  Stephan Kreutzer
#
# This file is part of ntl_to_xml_1, an extension for the
# digital_publishing_workflow_tools package.
#
# ntl_to_xml_1 is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License version 3 or any later version,
# as published by the Free Software Foundation.
#
# ntl_to_xml_1 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License 3 for more details.
#
# You should have received a copy of the GNU Affero General Public License 3
# along with ntl_to_xml_1. If not, see <http://www.gnu.org/licenses/>.



.PHONY: build clean



CFLAGS = -Wall -Werror -Wextra -pedantic



build: ntl_to_xml_1



ntl_to_xml_1: ntl_to_xml_1.cpp NTLParserXML.o
	g++ ntl_to_xml_1.cpp NTLParserXML.o -o ntl_to_xml_1 $(CFLAGS)

NTLParserXML.o: NTLParserXML.h NTLParserXML.cpp
	g++ NTLParserXML.cpp -c $(CFLAGS)

clean:
	rm -f ./ntl_to_xml_1
	rm -f ./NTLParserXML.o
