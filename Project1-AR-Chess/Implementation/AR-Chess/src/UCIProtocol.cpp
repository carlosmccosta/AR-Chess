#include "UCIProtocol.h"


UCIProtocol::UCIProtocol() : _chessEngineReady(false) {}
UCIProtocol::~UCIProtocol() {}


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
	sendMessageToEngine(UCI_SET_BEST_BOOK_MOVE);
	sendMessageToEngine(UCI_SET_OWN_BOOK);
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


string UCIProtocol::receiveBestMoveFromChessEngine() {
	sendMessageToEngine(UCI_GO);

	string engineOutput;
	do {
		engineOutput = receiveMessageFromEngine();
	} while (engineOutput.find(UCI_BESTMOVE) == string::npos);

	stringstream outputSS(engineOutput);
	string bestMove;

	// best move is 2nd token
	outputSS >> bestMove;
	outputSS >> bestMove;

	return bestMove;
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
	if (_engineToGuiStream->is_open()) {
		std::getline(*_engineToGuiStream, engineOutput);
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
		logfile << "<< " << msg << std::flush;
		logfile.close();
	}
}