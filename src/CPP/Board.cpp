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
        m_Figures.reserve(32);

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

    CachedThreats Board::updateThreatendSquares(const GameFigure* capturedFigure, const Move& move, bool caching){

        //Müsste andere posi sein für cachen (move.m_piecePosition)
        GameFigure* movedFigure = m_BoardPositions[move.m_DesiredPosition.index()];

        CachedThreats cachedThreats;

        auto addToAddedThreats = [&](const GameFigure* figure)
        {
            if(figure->getColor() == WHITE){
                cachedThreats.addedThreatsWhite.insert(cachedThreats.addedThreatsWhite.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
            }else{
                cachedThreats.addedThreatsBlack.insert(cachedThreats.addedThreatsBlack.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
            }
        };

        auto addToRemovedThreats  = [&](const GameFigure* figure)
        {
            if(figure->getColor() == WHITE){
                cachedThreats.removedThreatsWhite.emplace_back(figure, figure->getThreatendSquares());
            }else{
                cachedThreats.removedThreatsBlack.emplace_back(figure, figure->getThreatendSquares());
            }
        };

        if( caching ){
            cachedThreats.addedThreatsWhite.reserve(58);
            cachedThreats.addedThreatsBlack.reserve(58);

            cachedThreats.removedThreatsWhite.reserve(8);
            cachedThreats.removedThreatsBlack.reserve(8);
        }

        if(capturedFigure)
            removeOldThreats(capturedFigure, addToRemovedThreats, caching);
                
        if(movedFigure->getMovementType() == JUMPING){
            removeOldThreats(movedFigure, addToRemovedThreats, caching);
            refreshThreats(movedFigure, addToAddedThreats, caching);
        }

        //pass das für caching=true?
        for(auto& figure : m_Figures){
            if(figure.getMovementType() == SLIDING && 
                    (isInRay(move.m_PiecePosition, figure.getPosition()) 
                    || isInRay(move.m_DesiredPosition, figure.getPosition()) 
                    || (capturedFigure && isInRay(capturedFigure->getPosition(), figure.getPosition()))))
            {
                GameFigure* figure_ptr = &figure;
                removeOldThreats(figure_ptr, addToRemovedThreats, caching);
                refreshThreats(figure_ptr, addToAddedThreats, caching);
            }
        }

        return cachedThreats;
    }

    template<typename F>
    void Board::removeOldThreats(const GameFigure* figure, F callback , bool caching){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());
        const auto& old_Figure_Threats = figure->getThreatendSquares();

        if( caching )
            callback(figure);
        
        for(const Position& threat : old_Figure_Threats){
            auto toRemoveThreat = std::find(ownColorThreats.cbegin(), ownColorThreats.cend(), threat);
            if(toRemoveThreat != ownColorThreats.end())
                ownColorThreats.erase(toRemoveThreat);
        }
    }    

    template<typename F>
    void Board::refreshThreats(GameFigure* figure, F callback, bool caching){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());

        figure->updateThreats(m_BoardView);
        
        if( caching )
            callback(figure);
        
        ownColorThreats.insert(ownColorThreats.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
    }

    void Board::boardinit(const json& gameConfig){

        auto& figure_position_map= gameConfig["figure_positions"];

        for(auto& data_Figures : figure_position_map){

            FigureType figure_Type = data_Figures["FigureType"].get<FigureType>();

            addFigureOffJson(data_Figures["WHITE"], figure_Type, WHITE);
            addFigureOffJson(data_Figures["BLACK"], figure_Type, BLACK);
        }
    }

    void Board::addFigureOffJson(const json& posData, FigureType figureType, Color color){

        for(auto& figure_Position_Black : posData){

            int posX = figure_Position_Black[0].get<int>();
            int posY = figure_Position_Black[1].get<int>();
            Position piecePosition(posX, posY);

            GameFigure figure = GameFigureFactory(figureType, color, piecePosition);

            m_Figures.push_back(std::move(figure));
            m_BoardPositions[posX + posY*boardWidth] = &m_Figures.back();
        }
    }

    bool Board::makeMove(const Move& move){

        MoveResult moveResult = isMoveLegal(move);

        if(!moveResult.m_IsMoveLegal){
            return false;
        }

        if(wouldBeInCheck(move))
            return false;

        std::optional<FigureType> promotedFigureType;
        if(moveResult.m_MoveType == PROMOTING){
            promotedFigureType = m_BoardPrinter->getPromotionFigure();
        }

        FigureType movedFigureType = m_BoardView.getFigureAt(move.m_PiecePosition)->getFigureType();
        auto capturedFigure = executeMove(move, moveResult, promotedFigureType);
        const GameFigure* capturedFigure_ptr = (capturedFigure.has_value()) ? &capturedFigure.value() : nullptr;
        updateGameState(capturedFigure_ptr, move, moveResult.m_MoveType, movedFigureType);

        return true;
    }

    void Board::updateGameState(const GameFigure* capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType){
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
            Position promotionPosition = move.m_DesiredPosition;

            GameFigure promotedFigure = GameFigureFactory(promotedFigureType.value(), move.m_PlayerColor, promotionPosition);

            auto deltePawn_IT = std::find(m_Figures.begin(), m_Figures.end(), (**capturedFigure_ptr));
            if(deltePawn_IT != m_Figures.end()){
                removeOldThreats(*capturedFigure_ptr, [&](const GameFigure* figure){}, false);
                m_Figures.erase(deltePawn_IT);
            }

            m_Figures.push_back(std::move(promotedFigure));
            (*capturedFigure_ptr) = &m_Figures.back();

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

    bool Board::wouldBeInCheck(const Move& move){
        //captured Figure heraus finden und speichern


        //nullptr noch wechseln
        CachedThreats cachedThreats = updateThreatendSquares(nullptr, move, true);

        bool wouldbeChecked = isInCheck(move.m_PlayerColor);

        //revert changes


        return wouldbeChecked;
    }
}