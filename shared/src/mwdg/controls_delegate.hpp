/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_controls_delegate_hpp__
#define __INC_mwdg_controls_delegate_hpp__

#include <QStyledItemDelegate>

namespace MWdg
{

/// @brief Controls_Delegate
///
class Controls_Delegate : public QStyledItemDelegate
{
  Q_OBJECT

  // Public methods
  public:
  Controls_Delegate ( QObject * parent_n = 0 );

  QSize
  sizeHint ( const QStyleOptionViewItem & option_n,
             const QModelIndex & index_n ) const;

  void
  paint ( QPainter * painter_n,
          const QStyleOptionViewItem & option_n,
          const QModelIndex & index_n ) const;

  // Private attributes
  private:
  unsigned int _hmargin;
  unsigned int _vmargin;
  unsigned int _vspace;
};

} // namespace MWdg

#endif
