#ifndef _paintown_gui_widget_h
#define _paintown_gui_widget_h

#include "gui/rectarea.h"
#include "gui/coordinate.h"

class Bitmap;

namespace Gui{
    
struct ColorInfo{
    int body;
    int bodyAlpha;
    int border;
    int borderAlpha;
};

class Widget{
    public:
        Widget();
        Widget( const Widget & w );
        virtual ~Widget();
        
        // copy
        Widget &operator=( const Widget &);
        
        //! position info
        RectArea position;
        
        //! New position data
        Coordinate location;
        
        //! Colors
        ColorInfo colors;
        
        // Logic
        virtual void act()=0;
        
        // Render
        virtual void render(const Bitmap &)=0;
    
    protected:
        void arc( Bitmap *work, int x, int y, double startAngle, int radius, int color );
        void roundRect( Bitmap *work, int radius, int x1, int y1, int x2, int y2, int color );
        void roundRectFill( Bitmap *work, int radius, int x1, int y1, int x2, int y2, int color );
        
        void checkWorkArea();
        
        Bitmap *workArea;
};

}

#endif
