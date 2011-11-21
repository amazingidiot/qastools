//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "info_texts.hpp"


const char info_text_copying[] = "\
QasMixer - A mixer application for the Linux sound system ALSA.\n\
\n\
Copyright (C) 2011  Sebastian Holtermann <sebholt@xwmw.org>\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\
";


const char info_text_options[] = "\
Options:\n\
\n\
  -h, --help          prints this help\n\
  -c, --card=NUMBER   selects a card\n\
  -D, --device=NAME   selects a mixer device (e.g. hw:1)\n\
\n\
  -t, --tray          start minimized in tray\n\
  -n, --no-single     allow multiple instances\n\
\n\
  -i, --copying       prints copying information\n\
  -v, --version       prints the program version\n\
";
