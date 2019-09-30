/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_model_keys_hpp__
#define __INC_qsnd_model_keys_hpp__

#include <QAbstractItemModel>


namespace QSnd
{

static const unsigned int MKEY_DB_INDEX ( Qt::UserRole + 1 );
static const unsigned int MKEY_L10N_ARGS ( Qt::UserRole + 3 );

static const unsigned int MKEY_CARD_INDEX ( Qt::UserRole + 10 );
static const unsigned int MKEY_CARD_NAME ( Qt::UserRole + 11 );
static const unsigned int MKEY_CARD_MIXER_NAME ( Qt::UserRole + 12 );


} // End of namespace

#endif
