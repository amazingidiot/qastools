/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_fill_columns_layout_hpp__
#define __INC_fill_columns_layout_hpp__

#include <QLayout>
#include <QList>
#include <QStyle>

namespace Wdg
{

class Fill_Columns_Layout_Item
{
  public:
  Fill_Columns_Layout_Item ();

  QLayoutItem * item;

  QSize min_size;

  QRect rect;
};

inline Fill_Columns_Layout_Item::Fill_Columns_Layout_Item ()
: item ( 0 )
{
}

class Fill_Columns_Layout : public QLayout
{
  // Public methods
  public:
  Fill_Columns_Layout ( QWidget * parent_n = 0 );

  ~Fill_Columns_Layout ();

  // Size hints

  QSize
  sizeHint () const;

  QSize
  minimumSize () const;

  bool
  hasHeightForWidth () const;

  int
  heightForWidth ( int width ) const;

  // Spacings

  int
  horizontal_spacing () const;

  int
  vertical_spacing () const;

  unsigned int
  horizontal_spacing_default () const;

  unsigned int
  vertical_spacing_default () const;

  unsigned int
  spacing_default ( Qt::Orientation orient_n ) const;

  // Viewport geometry

  const QRect &
  viewport_geometry () const;

  void
  set_viewport_geometry ( const QRect & rect_n );

  // Layout methods

  QLayoutItem *
  itemAt ( int index_n ) const;

  int
  count () const;

  void
  addItem ( QLayoutItem * item_n );

  QLayoutItem *
  takeAt ( int index_n );

  void
  invalidate ();

  void
  setGeometry ( const QRect & rect_n );

  // Protected methods
  protected:
  void
  update_cache_const () const;

  void
  update_cache ();

  QSize
  calc_columns ( unsigned int width_n, unsigned int height_n );

  // Private attributes
  private:
  QList< Fill_Columns_Layout_Item > _items;

  QRect _viewport;

  int _hspace;
  int _vspace;

  bool _cache_dirty;
};

inline const QRect &
Fill_Columns_Layout::viewport_geometry () const
{
  return _viewport;
}

} // namespace Wdg

#endif
