#include "util/bitmap.h"
#include "tabbed-box.h"

#include "menu/menu.h"

#include "util/font.h"

#include "gui/context-box.h"

using namespace Gui;

Tab::Tab():
context(new ContextBox()),
active(false){
    // Set alpha to 0 as we are not interested in the box
    context->colors.borderAlpha = 0;
    context->colors.bodyAlpha = 0;
}
Tab::~Tab(){
    delete context;
}

TabbedBox::TabbedBox():
current(0),
fontWidth(24),
fontHeight(24),
inTab(false),
tabWidthMax(0){
}

TabbedBox::TabbedBox( const TabbedBox & b ){
    this->location = b.location;
    this->workArea = b.workArea;
}

TabbedBox::~TabbedBox(){
    for (std::vector<Gui::Tab *>::iterator i = tabs.begin(); i != tabs.end(); ++i){
        Gui::Tab * tab = *i;
        if (tab){
            delete tab;
        }
    }
}

TabbedBox &TabbedBox::operator=( const TabbedBox &copy){
    location = copy.location;
    workArea = copy.workArea;

    return *this;
}

// Logic
void TabbedBox::act(){
    if (!tabs.empty()){
	const Font & vFont = Font::getFont(font, fontWidth, fontHeight);
	//tabWidthMax = location.getWidth()/tabs.size();
	const int width = vFont.textLength(tabs[current]->name.c_str()) + 5;
	tabWidthMax = (location.getWidth() - width) / (tabs.size() - 1);
    } else {
	return;
    }
    if (!tabs[current]->active){
	tabs[current]->active = true;
    }
    tabs[current]->context->act();
}

// Render
void TabbedBox::render(const Bitmap & work){
    const int tabHeight = fontHeight + 5;
    checkWorkArea();
    // Check if we are using a rounded box
    if (location.getRadius() > 0){
        //roundRectFill( *workArea, (int)location.getRadius(), 0, 0, location.getWidth()-1, location.getHeight()-1, colors.body );
        //roundRect( *workArea, (int)location.getRadius(), 0, 0, location.getWidth()-1, location.getHeight()-1, colors.border );
    } else {
        workArea->rectangleFill( 0, tabHeight, location.getWidth()-1, location.getHeight()-1, colors.body );
        workArea->rectangle( 0, tabHeight, location.getWidth()-1, location.getHeight()-1, colors.border );
    }
    
    tabs[current]->context->render(*workArea);
    
    renderTabs(*workArea);
    
    Bitmap::transBlender( 0, 0, 0, colors.bodyAlpha );
    
    workArea->drawTrans(location.getX(), location.getY(), work);
}


// Add tab
void TabbedBox::addTab(const std::string & name, const std::vector<ContextItem *> & list){
    for (std::vector<Tab *>::iterator i = tabs.begin(); i != tabs.end(); ++i){
	Tab * tab = *i;
	if (tab->name == name){
	    return;
	}
    }
    Tab * tab = new Tab();
    tab->name = name;
    tab->context->setList(list);
    tab->context->setFont(font, fontWidth, fontHeight);
    tab->context->location.setPosition(Gui::AbsolutePoint(0, fontHeight));
    tab->context->location.setPosition2(Gui::AbsolutePoint(location.getWidth(), location.getHeight()));
    tab->context->open();
    tabs.push_back(tab);
}

void TabbedBox::up(){
    if (tabs.size() == 0){
        return;
    }
    if (!inTab){
        tabs[current]->context->close();
	tabs[current]->active = false;
        if (current == 0){
            current = tabs.size()-1;
        } else {
            current--;
        }
        tabs[current]->context->open();
	tabs[current]->active = true;
    } else {
        tabs[current]->context->previous();
    }
}

void TabbedBox::down(){
    if (tabs.size() == 0){
        return;
    }
    if (!inTab){
        tabs[current]->context->close();
	tabs[current]->active = false;
        if (current == tabs.size()-1){
            current = 0;
        } else {
            current++;
        }
        tabs[current]->context->open();
	tabs[current]->active = true;
    } else {
        tabs[current]->context->next();
    }
}

void TabbedBox::left(){
    if (tabs.size() == 0){
        return;
    }
    if (!inTab){
        tabs[current]->context->close();
	tabs[current]->active = false;
        if (current == 0){
            current = tabs.size()-1;
        } else {
            current--;
        }
        tabs[current]->context->open();
	tabs[current]->active = true;
    } else {
        tabs[current]->context->adjustLeft();
    }
}

void TabbedBox::right(){
    if (tabs.size() == 0){
        return;
    }
    if (!inTab){
        tabs[current]->context->close();
	tabs[current]->active = false;
        if (current == tabs.size()-1){
            current = 0;
        } else {
            current++;
        }
        tabs[current]->context->open();
	tabs[current]->active = true;
    } else {
        tabs[current]->context->adjustRight();
    }
}

void TabbedBox::toggleTabSelect(){
    inTab = !inTab;
}

unsigned int TabbedBox::getCurrentIndex() const {
    if (tabs.size() == 0){
        return 0;
    }
    return this->tabs[current]->context->getCurrentIndex();
}

void TabbedBox::renderTabs(const Bitmap & bmp){
    const int tabHeight = fontHeight + 5;
    const Font & vFont = Font::getFont(font, fontWidth, fontHeight);
    
    int x = 0;
    
    for (std::vector<Gui::Tab *>::iterator i = tabs.begin(); i != tabs.end(); ++i){
        Gui::Tab * tab = *i;
        const int textWidth = vFont.textLength(tab->name.c_str()) + 5;
        //const int width = (textWidth) > tabWidthMax ? tabWidthMax : textWidth;
	
	if (tab->context->location.getRadius() > 0){
        } else {
            if (tab->active){
		if (!inTab){
		    bmp.rectangleFill( x, 1, x+textWidth-1, tabHeight, colors.body );
		    bmp.vLine(1, x, tabHeight, selectedTabColors.border);
		    bmp.hLine(x, 1, textWidth-1, selectedTabColors.border); 
		    //bmp.vLine(1, x + width-1, tabHeight, selectedTabColors.border);
		} else {
		    bmp.rectangleFill( x, 1, x+textWidth-1, tabHeight, colors.body );
		    bmp.vLine(1, x, tabHeight, runningTabColors.border);
		    bmp.hLine(x, 1, textWidth-1, runningTabColors.border); 
		    //bmp.vLine(1, x + width-1, tabHeight, runningTabColors.border);
		}
		/* FIXME font color */
		bmp.setClipRect(x, 0, x+textWidth, tabHeight-1);
		vFont.printf(x + ((textWidth/2)-((textWidth - 5)/2)), 0, Bitmap::makeColor(255,255,255), bmp, tab->name, 0 );
		x+=textWidth;
            } else {
                bmp.rectangleFill( x, 1, x+tabWidthMax-1, tabHeight-1, tabColors.body );
                bmp.vLine(1, x, tabHeight-1, tabColors.border);
                bmp.hLine(x, 1, tabWidthMax-1, tabColors.border); 
                bmp.vLine(1, x + tabWidthMax-1, tabHeight, tabColors.border);
		/* FIXME font color */
		bmp.setClipRect(x+2, 0, x+tabWidthMax-2, tabHeight-1);
		vFont.printf(x + ((tabWidthMax/2)-(textWidth/2)), 0, Bitmap::makeColor(255,255,255), bmp, tab->name, 0 );
		x+=tabWidthMax;
            }
	    bmp.setClipRect(0, 0, bmp.getWidth(), bmp.getHeight());
        }
    }
}