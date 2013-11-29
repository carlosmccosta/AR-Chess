#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define UCI_MODE "uci"
#define UCI_OK "uciok"
#define UCI_SET_SKILL_LEVEL "setoption name Skill Level value "
#define UCI_SET_BOOK_FILE "setoption name Book File value "
#define UCI_SET_BEST_BOOK_MOVE "setoption name Best Book Move value true"
#define UCI_SET_OWN_BOOK "setoption name OwnBook value true"
#define UCI_ISREADY "isready"
#define UCI_READYOK "readyok"
#define UCI_NEW_GAME "ucinewgame"
#define UCI_BOARD_POSITION "position startpos moves "
#define UCI_GO "go"
#define UCI_BESTMOVE "bestmove"
#define UCI_QUIT "quit"

#define UCI_BOOK_FILE_PATH "chessengines/book.bin"
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/Referenced>

// boost includes
#include <boost/process.hpp>
//#include <boost/process/mitigate.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

// std includes
#include <string>
#include <sstream>

using namespace boost::process;
using namespace boost::process::initializers;
using namespace boost::iostreams;

// namespace specific imports to avoid namespace pollution
using std::string;
using std::stringstream;
using osg::Referenced;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <UCIProtocol> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class UCIProtocol : public Referenced {
	public:
		UCIProtocol();
		virtual ~UCIProtocol();
		
		void startUCIChessEngine(string enginePath, int skillLevel, string logfile);

		void setupUCIChessEngine(string enginePath);
		void startNewChessGame(int skillLevel);
		void receiveChessEngineInfoAndOptions();
		void setChessEngineSkillLevel(int skillLevel);
		void setChessEngineBookFile(string bookfilePath = UCI_BOOK_FILE_PATH);
		void startNewGame();
		void waitForChessEngine();
		void terminateChessEngine();

		void setChessEngineBoardPosition(string pieceMoves);
		string receiveBestMoveFromChessEngine();

		void sendMessageToEngine(string msg);
		string receiveMessageFromEngine();

		void logCommunicationToEngine(string msg);
		void logCommunicationFromEngine(string msg);


		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		bool isChessEngineReady() const { return _chessEngineReady; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------

	private:
		bool _chessEngineReady;				
		string _chessEnginePath;
		string _logfilePath;
		int _skillLevel;
		stream<file_descriptor_sink>* _guiToEngineStream;
		stream<file_descriptor_source>* _engineToGuiStream;
};
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </UCIProtocol> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
