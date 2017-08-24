#!/bin/sh
# Copyright (C) 2017  Stephan Kreutzer
#
# This file is an extension for the
# digital_publishing_workflow_tools package.
#
# This file is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License version 3 or any later version,
# as published by the Free Software Foundation.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License 3 for more details.
#
# You should have received a copy of the GNU Affero General Public License 3
# along with part of this file. If not, see <http://www.gnu.org/licenses/>.

# Note: you can use any other XSL transformator instead of Apache FOP, for example
# xml_xslt_transformator_1 of the digital_publishing_workflow_tools package or
# xmlstarlet.

rm -f output_pdflatex.pdf
rm -f output_pdflatex.aux
rm -f output_pdflatex.log
rm -f texput.log
rm -f output.tex
rm -f output_fop.pdf
rm -f output.fo
rm -f output.html
rm -f output.xml
./ntl_to_xml_1 input.ntl output.xml
fop -xml output.xml -xsl ntl_to_html_1.xsl -foout output.html
fop -xml output.xml -xsl ntl_to_latex_1.xsl -foout output.tex
pdflatex -jobname output_pdflatex output.tex
fop -xml output.xml -xsl ntl_to_fo_1.xsl -foout output.fo
fop -c fop.xconf -fo output.fo -pdf output_fop.pdf
