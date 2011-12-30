.TH ${PROGRAM_NAME_UCASE} "1" "2011-12-14" "Linux" "QasTools Manuals"
.SH NAME
${PROGRAM_NAME} \- Desktop sound mixer for ALSA

.SH SYNOPSIS
.B ${PROGRAM_NAME} [OPTION]...

.SH DESCRIPTION
.B ${PROGRAM_TITLE}
is a desktop mixer application for the Linux sound system ALSA.

.SH OPTIONS

.TP
\fB\-h\fR, \fB\-\-help\fR
prints a help text.

.TP
\fB\-c\fR, \fB\-\-card=NUMBER\fR
selects a sound card.

.TP
\fB\-D\fR, \fB\-\-device=NAME\fR
selects a mixer device (e.g. hw:1).

.TP
\fB\-t\fR, \fB\-\-tray\fR
start minimized in tray.

.TP
\fB\-n\fR, \fB\-\-no-single\fR
allow multiple instances.

.TP
\fB\-c\fR, \fB\-\-copying\fR
prints copying information.

.TP
\fB\-v\fR, \fB\-\-version\fR
prints the program version.

.SH NOTES
\fB${PROGRAM_TITLE}\fP is part of the \fBQasTools\fP applications collection.

It is written in C++ using the Qt GUI libraries.

.SH SEE ALSO
.B alsamixer(1), qasconfig(1), qashctl(1)
