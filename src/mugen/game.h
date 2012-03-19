#ifndef paintown_mugen_game_h
#define paintown_mugen_game_h

#include <string>

#include "util.h"

class Bitmap;

namespace Mugen{

class Searcher;
class CharacterSelect;
class Stage;
    
/* Our game definition, this is to facilitate running a game */
class Game {
    public:
        Game(const PlayerType &, const GameType &, const Filesystem::AbsolutePath & systemFile);
        virtual ~Game();

        //! Runs everything
        virtual void run(Searcher & searcher);

        /* just start a training match */
        static void startTraining(const std::string & player1, const std::string & player2, const std::string & stage);
        static void startNetworkVersus(const std::string & player1Name, const std::string & player2Name, const std::string & stageName, bool server, int port);
        /* start an arcade match */
        static void startArcade(const std::string & player1, const std::string & player2, const std::string & stage);
        /* start a watch match */
        static void startWatch(const std::string & player1Name, const std::string & player2Name, const std::string & stageName);
        /* start a scripted match */
        static void startScript(const std::string & player1Name, const std::string & player1Script, const std::string & player2Name, const std::string & player2Script, const std::string & stageName);
    private:
        void doArcade(Searcher &);
        void doTraining(Searcher &);
        void doVersus(Searcher &);
        void doNetworkVersus(bool isServer, Searcher &);
        void doWatch(Searcher &);
        void doSurvival(Searcher &);
        void startDemo(Searcher &);

        //! Player type
        PlayerType playerType;
        //! Game type
        GameType gameType;
        //! System file
        Filesystem::AbsolutePath systemFile;
        //! Motif Base Directory
        Filesystem::AbsolutePath motifDirectory;
};

}

#endif

