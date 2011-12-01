.TH ${PROGRAM_NAME} 1 "December 01, 2011" ${PROGRAM_NAME} 1
.SH NAME
${PROGRAM_NAME} \- A graphical sound mixer application for ALSA
.SH SYNOPSIS
.B ${PROGRAM_NAME} [OPTION]...
.SH DESCRIPTION
.B ${PROGRAM_NAME}
is a graphical mixer application for the Linux sound system ALSA using the Qt GUI libraries.
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
.SH SEE ALSO
.B alsamixer amixer
.SH AUTHOR
.B ${PROGRAM_NAME}
was written by Sebastian Holtermann <sebholt@xwmw.org>
