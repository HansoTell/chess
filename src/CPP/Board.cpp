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

    Board::Board(const std::string& file, std::unique_ptr<BoardPrinter> boardPrinter) : 
        m_BoardPositions{}, 
        m_BoardView(&m_BoardPositions), 
        m_BoardPrinter(std::move(boardPrinter)) 
    {
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

        if(movedFigure->getMovementType() == JUMPING){
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
            if(toRemoveThreat != ownColorThreats.end())
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

        for(auto& data_Figures : figure_position_map){

            FigureType figure_Type = data_Figures["FigureType"].get<FigureType>();

            //updaten auf methode für besser ist nur krampf template oder halt json::bacsic oder so
            //Ganz groß nochmal auf x und y koordinaten schauen sind anscheinende vertauscht. Sind sie bei allem vertauscht??
            for(auto& figure_Position_White : data_Figures["WHITE"]){

                int posY = figure_Position_White[0].get<int>();
                int posX = figure_Position_White[1].get<int>();
                Position piecePosition(posX, posY);
                Color pieceColor = WHITE;

                GameFigure figure = GameFigureFactory(figure_Type, pieceColor, piecePosition);

                m_Figures.push_back(std::move(figure));
                m_BoardPositions[posX + posY*boardWidth] = &m_Figures.back();
            }

            for(auto& figure_Position_Black : data_Figures["BLACK"]){

                int posY = figure_Position_Black[0].get<int>();
                int posX = figure_Position_Black[1].get<int>();
                Position piecePosition(posX, posY);
                Color pieceColor = BLACK;

                GameFigure figure = GameFigureFactory(figure_Type, pieceColor, piecePosition);

                m_Figures.push_back(std::move(figure));
                m_BoardPositions[posX + posY*boardWidth] = &m_Figures.back();
            }
        }
    }

    bool Board::makeMove(const Move& move){

        MoveResult moveResult = isMoveLegal(move);

        if(!moveResult.m_IsMoveLegal){
            return false;
        }

        std::optional<FigureType> promotedFigureType;
        if(moveResult.m_MoveType == PROMOTING){
            promotedFigureType = m_BoardPrinter->getPromotionFigure();
        }

        FigureType movedFigureType = m_BoardView.getFigureAt(move.m_PiecePosition)->getFigureType();
        auto capturedFigure = executeMove(move, moveResult, promotedFigureType);
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

    std::optional<GameFigure> Board::executeMove(const Move& move, MoveResult moveResult, std::optional<FigureType> promotedFigureType){
        std::optional<GameFigure> capturedFigure;
        switch (moveResult.m_MoveType.value())
        {
        case NORMAL:{
            GameFigure** capturedFigure_ptr = &m_BoardPositions[move.m_DesiredPosition.index()]; 
            GameFigure** movedFigure_ptr =  &m_BoardPositions[move.m_PiecePosition.index()];

            capturedFigure = editBoard(movedFigure_ptr, capturedFigure_ptr, move);

            break;
        }
        case EN_PASSANT:{
            int movementDirection = (move.m_PlayerColor == WHITE) ? 1 : -1;
            Position capturedFigurePosition = Position(move.m_DesiredPosition.x, move.m_DesiredPosition.y - movementDirection);
            GameFigure** capturedFigure_ptr = &m_BoardPositions[capturedFigurePosition.index()];
            GameFigure** movedFigure_ptr = &m_BoardPositions[move.m_PiecePosition.index()];

            capturedFigure = editBoard(movedFigure_ptr, capturedFigure_ptr, move);
            break;
        }
        case CASTEL:{
            bool shortCastle = (move.getXOffSet() > 0); 
            Position rook_Position = (shortCastle) ? Position(7, move.m_PiecePosition.y): Position(0, move.m_PiecePosition.y);
            Position rook_DesiredPosition = (shortCastle) ? Position(move.m_DesiredPosition.x -1, move.m_PiecePosition.y) : Position(move.m_DesiredPosition.x+1, move.m_PiecePosition.y);
            GameFigure** moved_King = &m_BoardPositions[move.m_PiecePosition.index()];
            GameFigure** moved_Rook = &m_BoardPositions[rook_Position.index()];

            (*moved_King)->setPosition(move.m_DesiredPosition);
            (*moved_Rook)->setPosition(rook_DesiredPosition);

            m_BoardPositions[move.m_DesiredPosition.index()] = *moved_King;
            (*moved_King)=nullptr;

            m_BoardPositions[rook_DesiredPosition.index()] = *moved_Rook;
            (*moved_Rook)=nullptr;
            break;
        }
        case PROMOTING:
            GameFigure** capturedFigure_ptr = &m_BoardPositions[move.m_DesiredPosition.index()]; 
            GameFigure** movedFigure_ptr =  &m_BoardPositions[move.m_PiecePosition.index()];

            capturedFigure = editBoard(movedFigure_ptr, capturedFigure_ptr, move);

            //Promotin Logic -> m_Figure & m_BoardPositions updaten!!! Neue Figur konstrukten

            break;
        }

        return capturedFigure;
    }

    std::optional<GameFigure> Board::editBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move){
        std::optional<GameFigure> capturedFigure;

        if(capturedFigure_ptr){
            auto capturedFigure_IT = std::find(m_Figures.begin(), m_Figures.end(), (**capturedFigure_ptr));
            if(capturedFigure_IT != m_Figures.end()){
                capturedFigure = std::move(*capturedFigure_IT);
                m_Figures.erase(capturedFigure_IT);
            }
        }
        (*movedFigure_ptr)->setPosition(move.m_DesiredPosition);

        (*capturedFigure_ptr) = nullptr;
        m_BoardPositions[move.m_DesiredPosition.index()] = *movedFigure_ptr;
        (*movedFigure_ptr) = nullptr;

        return capturedFigure;
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

    bool Board::wouldBeInCheck() const{
        return false;
    }
}