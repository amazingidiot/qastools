/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_switch_painter_svg.hpp"
#include "dpe/image_set.hpp"
#include "dpe/image_set_meta.hpp"
#include "dpe/paint_job.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPainter>
#include <QSvgRenderer>
#include <iostream>
#include <sstream>

namespace Wdg
{
namespace Painter
{

DS_Switch_Painter_SVG::DS_Switch_Painter_SVG ()
: ::Wdg::Painter::DS_Widget_Painter ( ::Wdg::DS_SWITCH )
{
  _suffix_bg[ 0 ] = "idle";
  _suffix_bg[ 1 ] = "focus";
  _suffix_bg[ 2 ] = "hover_idle";
  _suffix_bg[ 3 ] = "hover_focus";

  _suffix_handle[ 0 ] = "unchecked_idle";
  _suffix_handle[ 1 ] = "unchecked_focus";
  _suffix_handle[ 2 ] = "unchecked_hover_idle";
  _suffix_handle[ 3 ] = "unchecked_hover_focus";

  _suffix_handle[ 4 ] = "checked_idle";
  _suffix_handle[ 5 ] = "checked_focus";
  _suffix_handle[ 6 ] = "checked_hover_idle";
  _suffix_handle[ 7 ] = "checked_hover_focus";

  _suffix_handle[ 8 ] = "half_checked_focus";
  _suffix_handle[ 9 ] = "half_checked_hover_focus";
}

void
DS_Switch_Painter_SVG::set_base_dir ( const QString & dir_n )
{
  _base_dir = dir_n;
}

void
DS_Switch_Painter_SVG::set_file_prefix_bg ( const QString & file_prefix_n )
{
  _prefix_bg = file_prefix_n;
}

void
DS_Switch_Painter_SVG::set_file_prefix_handle ( const QString & file_prefix_n )
{
  _prefix_handle = file_prefix_n;
}

bool
DS_Switch_Painter_SVG::ready () const
{
  for ( unsigned int ii = 0; ii < num_bg; ++ii ) {
    QString fname ( file_name ( _prefix_bg, _suffix_bg[ ii ] ) );
    if ( !file_ready ( fname ) ) {
      return false;
    }
  }
  for ( unsigned int ii = 0; ii < num_handle; ++ii ) {
    QString fname ( file_name ( _prefix_handle, _suffix_handle[ ii ] ) );
    if ( !file_ready ( fname ) ) {
      return false;
    }
  }
  return true;
}

bool
DS_Switch_Painter_SVG::file_ready ( const QString & file_name_n ) const
{
  QFileInfo finfo ( file_name_n );
  if ( !finfo.exists () ) {
    return false;
  }
  if ( !finfo.isFile () ) {
    return false;
  }
  if ( !finfo.isReadable () ) {
    return false;
  }
  return true;
}

int
DS_Switch_Painter_SVG::paint_image ( ::dpe::Paint_Job * pjob_n )
{
  int res ( 0 );

  res = create_image_data ( &pjob_n->img_set->image ( pjob_n->img_idx ),
                            pjob_n->meta );

  if ( res == 0 ) {
    switch ( pjob_n->meta->type_id ) {
    case 0:
      res = paint_bg ( pjob_n );
      break;
    case 1:
      res = paint_handle ( pjob_n );
      break;
    }
  }

  return res;
}

int
DS_Switch_Painter_SVG::paint_bg ( ::dpe::Paint_Job * pjob_n ) const
{
  const QString fname (
      file_name ( _prefix_bg, _suffix_bg[ pjob_n->img_idx ] ) );

  return render_svg ( pjob_n->img_set->image ( pjob_n->img_idx ), fname );
}

int
DS_Switch_Painter_SVG::paint_handle ( ::dpe::Paint_Job * pjob_n ) const
{
  const QString fname (
      file_name ( _prefix_handle, _suffix_handle[ pjob_n->img_idx ] ) );

  return render_svg ( pjob_n->img_set->image ( pjob_n->img_idx ), fname );
}

QString
DS_Switch_Painter_SVG::file_name ( const QString & prefix_n,
                                   const QString & suffix_n ) const
{
  QString res ( base_dir () );
  if ( res[ res.size () - 1 ] != '/' ) {
    res.append ( '/' );
  }

  res.append ( prefix_n );
  res.append ( suffix_n );
  res.append ( ".svg" );

  return res;
}

int
DS_Switch_Painter_SVG::render_svg ( ::dpe::Image & img_n,
                                    const QString & svg_file_n ) const
{
  int res ( 0 );

  {
    QSvgRenderer svg_render;
    if ( svg_render.load ( svg_file_n ) ) {
      QPainter pnt ( &img_n.qimage () );
      pnt.setRenderHints ( QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform );
      svg_render.render ( &pnt );
    } else {
      img_n.clear ();
      {
        ::std::stringstream msg;
        msg << "SVG image loading failed for " << ::std::endl;
        msg << svg_file_n.toLocal8Bit ().data () << ::std::endl;
        ::std::cerr << msg.str ();
      }
      res = -1;
    }
  }

  return res;
}

} // namespace Painter
} // namespace Wdg
