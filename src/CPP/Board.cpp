#include "Board.h"


namespace chess{

    static bool isInRay(const Position& rayCenter, const Position& positionToCheck){
        //straight movement
        if(rayCenter.x == positionToCheck.x || rayCenter.y == positionToCheck.y)
            return true;
        
        //diagonal Movement
        int x_OffSet = rayCenter.x - positionToCheck.x;
        int y_OffSet = rayCenter.y - positionToCheck.y;
        if(x_OffSet == y_OffSet || x_OffSet == -y_OffSet)
           return true;
           
        return false;
    }

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
            figure.updateThreats(m_BoardView);
            Color figureColor = figure.getColor();

            std::vector<Position>& overallThreats = m_GameState.getThreatendSquares(figureColor);
            const std::vector<Position>& figureThreats = figure.getThreatendSquares();
            overallThreats.insert(overallThreats.end(), figureThreats.begin(), figureThreats.end());
        }
    }

    void Board::updateThreatendSquares(std::optional<GameFigure>& capturedFigure, const Move& move){

        GameFigure* movedFigure = m_BoardPositions[move.m_DesiredPosition.index()];

        if(capturedFigure.has_value()){
            GameFigure* capturedFigure_ptr = &capturedFigure.value();
            removeOldThreats(capturedFigure_ptr);
        }        

        if(movedFigure->getFigureType() == JUMPING){
            removeOldThreats(movedFigure);
            refreshThreats(movedFigure);
        }

        for(auto& figure : m_Figures){
            if(figure.getMovementType() == SLIDING && 
                    (isInRay(move.m_PiecePosition, figure.getPosition()) 
                    || isInRay(move.m_DesiredPosition, figure.getPosition()) 
                    || (capturedFigure.has_value() && isInRay(capturedFigure->getPosition(), figure.getPosition()))))
            {
                GameFigure* figure_ptr = &figure;
                removeOldThreats(figure_ptr);
                refreshThreats(figure_ptr);
            }
        }
    }

    void Board::removeOldThreats(GameFigure* figure){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());
        const auto& old_Figure_Threats = figure->getThreatendSquares();

        for(const Position& threat : old_Figure_Threats){
            auto toRemoveThreat = std::find(ownColorThreats.cbegin(), ownColorThreats.cend(), threat);
            ownColorThreats.erase(toRemoveThreat);
        }
    }    

    void Board::refreshThreats(GameFigure* figure){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());

        figure->updateThreats(m_BoardView);
        ownColorThreats.insert(ownColorThreats.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
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
        auto capturedFigure = executeMove(move, moveResult);
        updateGameState(capturedFigure, move, moveResult.m_MoveType, movedFigureType);

        return true;
    }

    void Board::updateGameState(std::optional<GameFigure>& capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType){
        updateThreatendSquares(capturedFigure, move);

        m_GameState.updateGameState(move, moveType, movedFigureType);

        if(m_GameState.isKingInCheck(move.m_PlayerColor))
            m_GameState.toggleKingInCheck(move.m_PlayerColor);

        if(isInCheck(opposite(move.m_PlayerColor)))
            m_GameState.toggleKingInCheck(opposite(move.m_PlayerColor));
    }

    std::optional<GameFigure> Board::executeMove(const Move& move, MoveResult moveResult){
        std::optional<GameFigure> capturedFigure;
        switch (moveResult.m_MoveType.value())
        {
        case NORMAL:{
            GameFigure* capturedFigure_ptr = m_BoardPositions[move.m_DesiredPosition.index()]; 
            GameFigure* movedFigure_ptr =  m_BoardPositions[move.m_PiecePosition.index()];
            if(capturedFigure_ptr){
                auto capturedFigure_IT = std::find(m_Figures.begin(), m_Figures.end(), *capturedFigure);
                capturedFigure = std::move(*capturedFigure_IT);
                m_Figures.erase(capturedFigure_IT);
            }
            movedFigure_ptr->setPosition(move.m_DesiredPosition);

            capturedFigure_ptr = movedFigure_ptr;
            movedFigure_ptr = nullptr;
            break;
        }
        case EN_PASSANT:{

            break;
        }
        case CASTEL:{
            bool shortCastle = (move.getXOffSet() > 0); 
            Position rook_Position = (shortCastle) ? Position(7, move.m_PiecePosition.y): Position(0, move.m_PiecePosition.y);
            Position rook_DesiredPosition = (shortCastle) ? Position(move.m_DesiredPosition.x -1, move.m_PiecePosition.y) : Position(move.m_DesiredPosition.x+1, move.m_PiecePosition.y);
            GameFigure* moved_King = m_BoardPositions[move.m_PiecePosition.index()];
            GameFigure* moved_Rook = m_BoardPositions[rook_Position.index()];

            moved_King->setPosition(move.m_DesiredPosition);
            moved_Rook->setPosition(rook_DesiredPosition);

            m_BoardPositions[move.m_DesiredPosition.index()] = moved_King;
            moved_King=nullptr;

            m_BoardPositions[rook_DesiredPosition.index()] = moved_Rook;
            moved_Rook=nullptr;
            break;
        }
        case PROMOTING:
            break;
        }

        return capturedFigure;
        //nicht vergessen raus figuren auch aus vector entfernt werden müssen 
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