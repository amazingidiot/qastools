.TH ${PROGRAM_NAME_UCASE} "1" "2011-12-14" "Linux" "QasTools Manuals"
.SH NAME
${PROGRAM_NAME} \- ALSA configuration tree browser

.SH DESCRIPTION
ALSA's configuration resides in a tree structure which gets built
from the contents of the files
\fI/etc/asound.conf\fP and \fI~/.asoundrc\fP.

\fB${PROGRAM_TITLE}\fP is a graphical browser for this configuration
tree and can help to analyze and debug an ALSA setup.

.SH NOTES
\fB${PROGRAM_TITLE}\fP is part of the \fBQasTools\fP applications collection.

It is written in C++ using the Qt GUI libraries.

.SH SEE ALSO
.B qashctl(1), qasmixer(1)
