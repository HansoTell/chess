#include "Board.h"


namespace chess{
    Board::Board(const std::string& file) : m_BoardPositions{}, m_BoardView(&m_BoardPositions) {
        m_Figures.reserve(32 * sizeof(GameFigure));

        const json gameConifg = parseJson(file);

        boardinit(gameConifg);
        gameStateInit(gameConifg);   
    }

    json Board::parseJson(const std::string& file){
        std::ifstream Jfile(file);
        if(!Jfile){
            //Exception handeling
            std::cout << "File konnte nicht eingelesen werden\n";
        }

        json j;
        Jfile >> j;
        return j;
    }

    void Board::gameStateInit(const json& gameConfig){
        //json parsen für einstellungen

        threatendSquaresInit();
    }

    void Board::threatendSquaresInit(){
        for(auto& figure : m_Figures){
            auto newThreats = figure.updateThreats(m_BoardView);
            figure.setThreats(newThreats);
            Color figureColor = figure.getColor();

            std::vector<Position>& overallThreats = m_GameState.getThreatendSquares(figureColor);
            const std::vector<Position>& figureThreats = figure.getThreatendSquares();
            overallThreats.insert(overallThreats.end(), figureThreats.begin(), figureThreats.end());
        }
    }

    void Board::updateThreatendSquares(const Move& move){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(move.m_PlayerColor);
        std::vector<Position>& enemyColorThreats = m_GameState.getThreatendSquares(opposite(move.m_PlayerColor));

        GameFigure* movedFigure = m_BoardPositions[move.m_DesiredPosition.index()];
        const auto& old_Figure_Threats = movedFigure->getThreatendSquares();

        //Alte Threats entfernen
        for(const Position& threat : old_Figure_Threats){
            auto toRemoveThreat = std::find(ownColorThreats.cbegin(), ownColorThreats.cend(), threat);
            ownColorThreats.erase(toRemoveThreat);
        }

        //Figur neue Threats hinzufügen --> überlegen anpassen ob sliding oder jumping
        //Frage bei sliding pieces müssen eig ja nicht alle entfernt werdfen --> bei straight: eine reihe sonst gleich außer neue posi und alte posi
        //-->Diagonal ja das gleiche reihe müsste nicht updatet werden aber wie setzt man das um??
        //Überlegen enum mov direc einfügen, um noch mehr zu reduzieren --> funktioniert nicht, weil figur die beides kann jadas zusammen bricht
        auto newThreats = movedFigure->updateThreats(m_BoardView);
        movedFigure->setThreats(newThreats);
        ownColorThreats.insert(ownColorThreats.end(), movedFigure->getThreatendSquares().begin(), movedFigure->getThreatendSquares().end());


        for(auto& figure : m_Figures){
            if(figure.getMovementType() == SLIDING){
                
            }
        }




        //gelten bei enpassant und castling extra regeln
        //was ist mit caputered figuren threats entfernen
    }


    void Board::boardinit(const json& gameConfig){

        auto& figure_position_map= gameConfig["figure_positions"];
        
        for(auto&[figure_Type, data_Figures] : figure_position_map.items()){
            for(auto& data_figure : data_Figures){
                int posX = data_figure[1].get<int>();
                int posY = data_figure[2].get<int>();
                Position piecePosition(posX, posY);
                
                int colorInt = data_figure[0].get<int>();
                Color pieceColor = static_cast<Color>(colorInt);
                GameFigure figure = GameFigureFactory(figure_Type, pieceColor, piecePosition);

                m_Figures.push_back(std::move(figure));
                m_BoardPositions[posX + posY*boardWidth] = &m_Figures.back();
            }
        }

    }

    void Board::printBoard() const {
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
        std::cout << "-----------------------------------------------" << "\n";
        for(int i = 0; i<boardWidth;i++){
            std::cout << i+1 << " "; 
            for(int j = 0; j<boardWidth;j++){
                std::cout << " | ";
                GameFigure* toprintFigure = m_BoardPositions[i+j*boardWidth];
                if(toprintFigure){
                    m_BoardPositions[i+j*boardWidth]->printChar();
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << i+1 <<"\n";
            std::cout << "-----------------------------------------------" << "\n";
        }
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
    }

    bool Board::makeMove(const Move& move){

        MoveResult moveResult = isMoveLegal(move);

        if(!moveResult.m_IsMoveLegal){
            return false;
        }

        if(moveResult.m_MoveType == PROMOTING){
            //Welche figur man will erfragen --> neue klasse die ausgaben macht und sich um sowas kümmert --> eventuell wird sich das aber auch noch erledigen wegen spielablauf deisgn
        }

        FigureType movedFigureType = m_BoardView.getFigureAt(move.m_PiecePosition)->getFigureType();
        executeMove(move, moveResult);
        updateThreatendSquares(move);
        updateGameState(move, moveResult.m_MoveType, movedFigureType);

        return true;
    }

    void Board::updateGameState(const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType){
        updateThreatendSquares(move);

        m_GameState.updateGameState(move, moveType, movedFigureType);

        if(m_GameState.isKingInCheck(move.m_PlayerColor))
            m_GameState.toggleKingInCheck(move.m_PlayerColor);

        if(isInCheck(opposite(move.m_PlayerColor)))
            m_GameState.toggleKingInCheck(opposite(move.m_PlayerColor));
        
    }

    void Board::executeMove(const Move& Move, MoveResult moveResult){
        //nocht vergessen, dass position in figur auch geändert werden muss und raus figuren auch aus vector entfernt werden müssen
    }

    MoveResult Board::isMoveLegal(const Move& move) const{
        if(move.isOutOfBounds())
            return false;

        const GameFigure* movedFigure = m_BoardView.getFigureAt(move.m_PiecePosition);

        if(!movedFigure)
            return false;
        
        if(movedFigure->getColor() != move.m_PlayerColor)
            return false;
        
        if(move.getXOffSet()==0 && move.getYOffSet() == 0)
            return false;

        MoveResult moveResult = movedFigure->isMoveLegal(move, m_BoardView, m_GameState);
        if(!moveResult.m_IsMoveLegal) 
            return false;

        //checken ob eigens board in schach, simulation von neuen threatend positions müssen simuliert werden 
        if(wouldBeInCheck())
            return false;

            
        return moveResult;
    }

    bool Board::isInCheck(Color color) const {
        for(auto& figure : m_Figures){
            if(figure.getFigureType() == KING && figure.getColor() == color){
                const std::vector<Position>& enemyThreats = m_GameState.getThreatendSquares(opposite(color));

                return std::find(enemyThreats.begin(), enemyThreats.end(), figure.getPosition()) != enemyThreats.end();
            }
        }

        std::cout << "No King found kinda weird" << "\n";
        return false;
    }
}
