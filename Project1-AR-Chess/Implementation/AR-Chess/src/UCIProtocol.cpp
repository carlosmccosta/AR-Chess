#include "UCIProtocol.h"


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <EngineToGUICommunicationThread> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EngineToGUICommunicationThread::EngineToGUICommunicationThread(stream<file_descriptor_source>* engineToGuiStream) : _engineToGuiStream(engineToGuiStream) {}

int EngineToGUICommunicationThread::cancel() {
	_finished = true;
	while (Thread::isRunning()) Thread::YieldCurrentThread();
	return 0;
}


void EngineToGUICommunicationThread::run() {
	_finished = false;
	_newDataAvailable = false;
	do {
		//Thread::YieldCurrentThread();		

		string engineOutput;
		if (_engineToGuiStream->is_open()) {
			std::getline(*_engineToGuiStream, engineOutput);
		}

		addNewData(engineOutput);
	} while (!_finished);
}

void EngineToGUICommunicationThread::addNewData(string dataReceived) {
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	_dataReceived << dataReceived;
	_newDataAvailable = true;
}


bool EngineToGUICommunicationThread::getDataReceived(string& dataReceived) {
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	if (_newDataAvailable) {
		dataReceived = _dataReceived.str();
		_newDataAvailable = false;

		_dataReceived.str(string());
		_dataReceived.clear();

		return true;
	}

	return false;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </EngineToGUICommunicationThread> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <UCIProtocol> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
UCIProtocol::UCIProtocol() : _chessEngineReady(false), _engineToGUICommunicationThread(NULL) {}
UCIProtocol::~UCIProtocol() {
	if (_engineToGUICommunicationThread != NULL) {
		_engineToGUICommunicationThread->cancel();
		
		terminateChessEngine();
	}
}


void UCIProtocol::setupUCIChessEngine(string enginePath) {
	pipe guiToEnginePipe = create_pipe();
	pipe engineToGuiPipe = create_pipe();

	file_descriptor_sink guiOutputSink(guiToEnginePipe.sink, close_handle);
	file_descriptor_source engineInputSource(guiToEnginePipe.source, close_handle);

	file_descriptor_sink engineOutputSink(engineToGuiPipe.sink, close_handle);
	file_descriptor_source guiInputSource(engineToGuiPipe.source, close_handle);

	child _engineProcess = execute(
		run_exe(enginePath.c_str()),
		bind_stdin(engineInputSource),
		bind_stdout(engineOutputSink)
		);
	
	_guiToEngineStream = new stream<file_descriptor_sink>(guiOutputSink);
	_engineToGuiStream = new stream<file_descriptor_source>(guiInputSource);

	_chessEngineReady = true;

	_engineToGUICommunicationThread = new EngineToGUICommunicationThread(_engineToGuiStream);
	_engineToGUICommunicationThread->startThread();

	sendMessageToEngine(UCI_MODE);
	receiveChessEngineInfoAndOptions();
}


void UCIProtocol::startUCIChessEngine(string enginePath, int skillLevel, string logfile) {	
	_logfilePath = logfile;	

	if (!_chessEngineReady) {
		// clear log	
		std::ofstream log(logfile, std::ofstream::out | std::ofstream::trunc);
		log.close();

		setupUCIChessEngine(enginePath);
	}

	startNewChessGame(skillLevel);
}


void UCIProtocol::startNewChessGame(int skillLevel) {
	setChessEngineSkillLevel(skillLevel);
	setChessEngineBookFile();

	if (skillLevel > 16) {
		sendMessageToEngine(UCI_SET_BEST_BOOK_MOVE);
	} else {
		sendMessageToEngine(UCI_UNSET_BEST_BOOK_MOVE);
	}

	sendMessageToEngine(UCI_SET_OWN_BOOK);

	startNewGame();
	waitForChessEngine();	
}


void UCIProtocol::receiveChessEngineInfoAndOptions() {
	string engineOutput;
	do {
		engineOutput = receiveMessageFromEngine();
	} while (engineOutput.find(UCI_OK) == string::npos);
}


void UCIProtocol::setChessEngineSkillLevel(int skillLevel) {
	_skillLevel = skillLevel;

	stringstream ss;
	ss << UCI_SET_SKILL_LEVEL << skillLevel;
	sendMessageToEngine(ss.str());
}


void UCIProtocol::setChessEngineBookFile(string bookfilePath) {
	stringstream ss;
	ss << UCI_SET_BOOK_FILE << bookfilePath;
	sendMessageToEngine(ss.str());	
}


void UCIProtocol::startNewGame() {
	sendMessageToEngine(UCI_NEW_GAME);	
}


void UCIProtocol::waitForChessEngine() {
	sendMessageToEngine(UCI_ISREADY);

	string engineOutput;
	do {
		engineOutput = receiveMessageFromEngine();
	} while (engineOutput.find(UCI_READYOK) == string::npos);
}


void UCIProtocol::terminateChessEngine() {
	sendMessageToEngine(UCI_QUIT);
}


void UCIProtocol::setChessEngineBoardPosition(string pieceMoves) {
	stringstream ss;
	ss << UCI_BOARD_POSITION << pieceMoves;
	sendMessageToEngine(ss.str());
}


void UCIProtocol::startEngineMoveSearch(int moveTimeInMilleseconds) {
	stringstream goMsg;
	goMsg << UCI_GO;

	if (moveTimeInMilleseconds > 0) {
		goMsg << " " << UCI_MOVE_TIME << " " << moveTimeInMilleseconds;
	}

	sendMessageToEngine(goMsg.str());
}


string UCIProtocol::receiveBestMoveFromChessEngine() {	
	string engineOutput = receiveMessageFromEngine();
	stringstream outputSS(engineOutput);
	
	string bestMove;
	while (outputSS >> bestMove) {
		if (bestMove == UCI_BESTMOVE) {
			// best move is after UCI_BESTMOVE token
			outputSS >> bestMove;
			return bestMove;
		}
	}
	
	// engine still thinking
	return string();
}


bool UCIProtocol::isBoardPositionInCheckMate(string pieceMoves) {
	setChessEngineBoardPosition(pieceMoves);
	string engineBestMove = receiveBestMoveFromChessEngine();	

	if (engineBestMove == UCI_BESTMOVE_CHECK_MATE) {
		return true;
	}

	return false;
}


void UCIProtocol::sendMessageToEngine(string msg) {
	if (_guiToEngineStream->is_open()) {
		(*_guiToEngineStream) << msg;
		(*_guiToEngineStream) << std::endl;
		logCommunicationToEngine(msg);
	}
}


string UCIProtocol::receiveMessageFromEngine() {
	string engineOutput;
	bool newDataReceived = _engineToGUICommunicationThread->getDataReceived(engineOutput);
	if (newDataReceived) {		
		logCommunicationFromEngine(engineOutput);
	}

	return engineOutput;
}


void UCIProtocol::logCommunicationToEngine(string msg) {
	if (msg.empty()) {
		return;
	}

	std::ofstream logfile(_logfilePath, std::ofstream::out | std::ofstream::app);
	if (logfile.is_open()) {
		logfile << ">> " << msg << std::endl;
		logfile.close();
	}
}


void UCIProtocol::logCommunicationFromEngine(string msg) {
	if (msg.empty()) {
		return;
	}

	std::ofstream logfile(_logfilePath, std::ofstream::out | std::ofstream::app);
	if (logfile.is_open()) {
		logfile /*<< "<< "*/ << msg << std::flush;
		logfile.close();
	}
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </UCIProtocol> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
