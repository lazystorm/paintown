#include "menu/option_network_join.h"
#include "util/keyboard.h"
#include "util/load_exception.h"
#include "util/token.h"
#include "util/funcs.h"
#include "util/keyboard.h"
#include "return_exception.h"
#include "network/client.h"
#include "globals.h"

#include <iostream>

using namespace std;

OptionNetworkJoin::OptionNetworkJoin(Token *token) throw( LoadException ):
MenuOption(token, event){
	if ( *token != "network-join" ){
		throw LoadException("Not a network-join");
	}
	
	while ( token->hasTokens() ){
		try{
			Token * tok;
			*token >> tok;
			if ( *tok == "name" ){
				// Create an image and push it back on to vector
				std::string temp;
				*tok >> temp;
				this->setText(temp);
			} else {
				Global::debug( 3 ) <<"Unhandled menu attribute: "<<endl;
				tok->print(" ");
			}
		} catch ( const TokenException & ex ) {
			// delete current;
			string m( "Menu parse error: " );
			m += ex.getReason();
			throw LoadException( m );
		} 
	}
	
	if ( getText().empty() ){
		throw LoadException("No name set, this option should have a name!");
	}
}

OptionNetworkJoin::~OptionNetworkJoin(){
	// Nothing
}

void OptionNetworkJoin::logic(){
}

void OptionNetworkJoin::draw(Bitmap *work){
}

void OptionNetworkJoin::run(bool &endGame){
	Keyboard key;
	key.poll();
	key.wait();
	Network::networkClient();
	key.clear();
	key.poll();
	key.wait();
}
