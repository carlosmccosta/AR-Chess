# [Augmented Reality Chess](http://carlosmccosta.github.io/AR-Chess/)


## Overview
AR-Chess is an augmented reality chess GUI implemented using osgART.
It allows users to play chess games against a chess engine or against another human.



## Main features
* Uses Universal Chess Interface (UCI) protocol to communicate with the chess engine (currently uses Stockfish, but any UCI engine can be used)
* Allows 3 different game modes: Human vs Human, Human vs Computer and Computer vs Computer
* Allows going back and forward in the player chess piece moves
* Implements and enforces all chess rules
* Possible moves are shown for the chess piece the player wants to move
* Player isn't allowed to make invalid moves
* Uses ARToolkit for marker detection and tracking (and as such, requires a live video feed)
* All user interaction is done with [2 markers](https://github.com/carlosmccosta/AR-Chess/tree/master/Source/AR-Chess/patterns) (1 for the board and 1 for the selector)
* Uses OpenSceneGraph for 3D rendering (press h for help and to see keyboard shortcuts)


[![AR-Chess demo](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/AR-Chess.png)](http://www.youtube.com/watch?v=GOzPtKTbHsU)
Vídeo 1 - AR-Chess gameplay


![Front view](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/FrontSide.png)
Figure 1 - Front view

![Left view](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/LeftSide.png)
Figure 2 - Left view

![Game modes](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/GameModes.png)
Figure 3 - Game modes

![Skill levels](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/SkillLevels.png)
Figure 4 - Skill levels

![Game timers](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/GameTimers.png)
Figure 5 - Game timers

![Pawn en passant capture](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/PawnEnPassantCapture.png)
Figure 6 - Pawn en passant capture

![Pawn promotion](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/PawnPromotion.png)
Figure 7 - Pawn promotion

![King castle](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/KingCastle.png)
Figure 8 - Castling

![King check](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/KingCheck.png)
Figure 9 - King check

![King check mate](https://raw.github.com/carlosmccosta/AR-Chess/master/Screenshots/EndGame.png)
Figure 10 - King check mate



## Releases
[Windows 8.1 release](https://github.com/carlosmccosta/AR-Chess/tree/master/Releases/Windows)



## Building and developing
The setup instructions on how to build and develop in Visual Studio is available [here](https://github.com/carlosmccosta/AR-Chess/blob/master/Source/Configuration/Visual%20Studio%20configuration.txt)
