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
        m_Figures.reserve(40);

        const json gameConifg = parseJson(file);

        AttackTabels::initAttackTabels();
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
        m_GameState.parseGameStateJson(gameConfig);

        threatendSquaresInit();
        allLegalMovesInit();
    }

    void Board::threatendSquaresInit(){
        for(auto& figure : m_Figures){
            if( figure.getIsActive() ){
                figure.updateThreats(m_BoardView);
                Color figureColor = figure.getColor();

                std::vector<Position>& overallThreats = m_GameState.getThreatendSquares(figureColor);

                const std::vector<Position>& figureThreats = figure.getThreatendSquares();

                overallThreats.insert(overallThreats.end(), figureThreats.begin(), figureThreats.end());
            }
        }
    }

    void Board::allLegalMovesInit(){
        for(auto& figure : m_Figures){
            if( figure.getIsActive() ){
                figure.updateAllLegalMoves(m_BoardView, m_GameState);
            }
        }
    }

    bool Board::isCheckmate(Color playerColor) const {
        return false;
    }

    bool Board::isStalemate() const {
        return false;
    }

    void Board::updateThreatendSquares(const GameFigure* capturedFigure, const Move& move){

        GameFigure* movedFigure = m_BoardPositions[move.m_DesiredPosition.index()];
        GameFigure* pWhiteKing = nullptr;
        GameFigure* pBlackKing = nullptr;

        if(capturedFigure)
            removeOldThreats(capturedFigure);
                
        if(movedFigure->getMovementType() == JUMPING){
            removeOldThreats(movedFigure);
            refreshThreats(movedFigure);
        }

        for(auto& figure : m_Figures){
            if( figure.getFigureType() == KING ){
                if(figure.getColor() == WHITE)
                    pWhiteKing = &figure;
                else
                    pBlackKing = &figure;
            }

            if( figure.getIsActive() &&  figure.getMovementType() == SLIDING && 
                    (isInRay(move.m_PiecePosition, figure.getPosition()) 
                    || isInRay(move.m_DesiredPosition, figure.getPosition()) 
                    || (capturedFigure && isInRay(capturedFigure->getPosition(), figure.getPosition()))) )
            {
                GameFigure* figure_ptr = &figure;
                removeOldThreats(figure_ptr);
                refreshThreats(figure_ptr);
            }
        }

        removeThreatendKingThreats(pWhiteKing, WHITE);
        removeThreatendKingThreats(pBlackKing, BLACK);
    }

    CachedThreats Board::simulateUpdateThreatendSquares(GameFigure* capturedFigure, const Move& move){

        GameFigure* movedFigure = m_BoardPositions[move.m_DesiredPosition.index()];

        CachedThreats cachedThreats;

        cachedThreats.addedThreatsWhite.reserve(58);
        cachedThreats.addedThreatsBlack.reserve(58);

        cachedThreats.removedThreatsWhite.reserve(8);
        cachedThreats.removedThreatsBlack.reserve(8);

        if(capturedFigure)
            simulateRemoveOldThreats(capturedFigure, cachedThreats);
                
        if(movedFigure->getMovementType() == JUMPING){
            simulateRemoveOldThreats(movedFigure, cachedThreats);
            simulateRefreshThreats(movedFigure, cachedThreats);
        }

        for(auto& figure : m_Figures){
            if(figure.getIsActive() &&  figure.getMovementType() == SLIDING && 
                    (isInRay(move.m_PiecePosition, figure.getPosition()) 
                    || isInRay(move.m_DesiredPosition, figure.getPosition()) 
                    || (capturedFigure && isInRay(capturedFigure->getPosition(), figure.getPosition()))))
            {
                GameFigure* figure_ptr = &figure;
                simulateRemoveOldThreats(figure_ptr, cachedThreats);
                simulateRefreshThreats(figure_ptr, cachedThreats);
            }
        }

        return cachedThreats;
    }

    void Board::removeOldThreats(const GameFigure* figure){
        const auto old_Figure_Threats = figure->getThreatendSquares();

        m_GameState.removeThreatsFormThreatMap(old_Figure_Threats, figure->getColor());
    }    

    void Board::refreshThreats(GameFigure* figure){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());

        figure->updateThreats(m_BoardView);
        
        ownColorThreats.insert(ownColorThreats.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
    }

    void Board::simulateRemoveOldThreats(GameFigure* figure, CachedThreats& cachedThreats){
        const auto old_Figure_Threats = figure->getThreatendSquares();

        if(figure->getColor() == WHITE){
            cachedThreats.removedThreatsWhite.emplace_back(figure, figure->getThreatendCopySquares());
        }else{
            cachedThreats.removedThreatsBlack.emplace_back(figure, figure->getThreatendCopySquares());
        }

        m_GameState.removeThreatsFormThreatMap(old_Figure_Threats, figure->getColor());
    }    

    void Board::simulateRefreshThreats(GameFigure* figure, CachedThreats& cachedThreats){
        std::vector<Position>& ownColorThreats = m_GameState.getThreatendSquares(figure->getColor());

        figure->updateThreats(m_BoardView);
        
        if(figure->getColor() == WHITE){
            cachedThreats.addedThreatsWhite.insert(cachedThreats.addedThreatsWhite.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
        }else{
            cachedThreats.addedThreatsBlack.insert(cachedThreats.addedThreatsBlack.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
        }
        
        ownColorThreats.insert(ownColorThreats.end(), figure->getThreatendSquares().begin(), figure->getThreatendSquares().end());
    }

    void Board::removeThreatendKingThreats(GameFigure* pKing, Color kingColor){
        if( !pKing )
            return;

        std::vector<Position>& kingThreats = pKing->getThreatendSquares();

        std::vector<Position>& ownColorThreatMap = m_GameState.getThreatendSquares(kingColor);
        std::vector<Position>& enemyColorThreatMap = m_GameState.getThreatendSquares(opposite(kingColor));

        for(Position kingThreat : kingThreats){
            if( std::find(enemyColorThreatMap.begin(), enemyColorThreatMap.end(), kingThreat) != enemyColorThreatMap.end() ){
                auto threatMap_IT = std::find( ownColorThreatMap.begin(), ownColorThreatMap.end(), kingThreat);
                if( threatMap_IT != ownColorThreatMap.end() )
                    ownColorThreatMap.erase(threatMap_IT);
                
                auto kingThreat_IT = std::find(kingThreats.begin(), kingThreats.end(), kingThreat);
                if( kingThreat_IT != kingThreats.end() )
                    kingThreats.erase(kingThreat_IT);
            }
        }
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
        //check das jeweils ein König da ist und auch nur genau einer

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

        if(!moveResult.m_IsMoveLegal)
            return false;

        if(wouldBeInCheck(move, moveResult))
            return false;

        std::optional<FigureType> promotedFigureType;
        if(moveResult.m_MoveType == PROMOTING)
            promotedFigureType = m_BoardPrinter->getPromotionFigure();
        
        FigureType movedFigureType = m_BoardView.getFigureAt(move.m_PiecePosition)->getFigureType();
        const GameFigure* capturedFigure = executeMove(move, moveResult, promotedFigureType);

        updateGameState(capturedFigure, move, moveResult.m_MoveType, movedFigureType);

        return true;
    }

    void Board::updateGameState(const GameFigure* capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType){
        updateThreatendSquares(capturedFigure, move);
        updateAllLegalMoves(capturedFigure, move);

        m_GameState.updateGameState(move, moveType, movedFigureType);

        if(m_GameState.isKingInCheck(move.m_PlayerColor))
            m_GameState.toggleKingInCheck(move.m_PlayerColor);

        if(isInCheck(opposite(move.m_PlayerColor)))
            m_GameState.toggleKingInCheck(opposite(move.m_PlayerColor));
    }

    const GameFigure* Board::executeMove(const Move& move, MoveResult moveResult, std::optional<FigureType> promotedFigureType){
        switch (moveResult.m_MoveType.value())
        {
        case NORMAL:
            return ExecuteNormalMove(move);
        
        case EN_PASSANT:
            return ExecuteEnPassantMove(move);
        
        case CASTEL:
            return ExecuteCastelingMove(move);
         
        case PROMOTING:
            return ExecutePromotingMove(move, promotedFigureType.value());
        }

        return {};
    }

    void Board::updateAllLegalMoves(const GameFigure* pCapturedFigure, const Move& move){

        GameFigure* pMovedFigure = m_BoardPositions[move.m_DesiredPosition.index()];

        if( pMovedFigure && pMovedFigure->getFigureType() == KNIGHT )
            pMovedFigure->updateAllLegalMoves(m_BoardView, m_GameState);

        for(auto& figure : m_Figures){
            if( figure.getIsActive() && figure.getFigureType() != KNIGHT && 
                (isInRay(move.m_PiecePosition, figure.getPosition())
                || isInRay(move.m_DesiredPosition, figure.getPosition())
                || (pCapturedFigure && isInRay(pCapturedFigure->getPosition(), figure.getPosition()))))
            {
                figure.updateAllLegalMoves(m_BoardView, m_GameState);
            }
        }
    }

    ChangedPieces Board::simulateMove(const Move& move, MoveResult moveResult, std::optional<FigureType> promotedFigureType){
        switch (moveResult.m_MoveType.value())
        {
        case NORMAL:
            return simulateNormalMove(move);
        case EN_PASSANT:
            return simulateEnPassantMove(move);
        case CASTEL:
            return simulateCastelingMove(move);
        case PROMOTING:
            return simulatePromotingMove(move);
        }

        return ChangedPieces(nullptr, {}, nullptr, nullptr, {});
    }

    const GameFigure* Board::editBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move){
        const GameFigure* pCapturedFigure = (capturedFigure_ptr) ? (*capturedFigure_ptr) : nullptr;

        (*movedFigure_ptr)->setPosition(move.m_DesiredPosition);
        if( pCapturedFigure ){
            (**capturedFigure_ptr).toggleIsActive();
            (*capturedFigure_ptr) = nullptr;
        }
        m_BoardPositions[move.m_DesiredPosition.index()] = *movedFigure_ptr;
        (*movedFigure_ptr) = nullptr;

        return pCapturedFigure;
    }

    ChangedPieces Board::simulateEditBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move){

        GameFigure* capturedFigure = (capturedFigure_ptr) ? (*capturedFigure_ptr) : nullptr;
        GameFigure* movedFigure = (movedFigure_ptr) ? (*movedFigure_ptr) : nullptr;

        std::optional<Position> movedFigurePos = {};

        if(capturedFigure_ptr){
            capturedFigure->toggleIsActive();
            (*capturedFigure_ptr) = nullptr;
        }
        if(movedFigure_ptr){
            movedFigurePos = movedFigure->getPosition();
            movedFigure->setPosition(move.m_DesiredPosition);
            m_BoardPositions[move.m_DesiredPosition.index()] = *movedFigure_ptr;
            (*movedFigure_ptr) = nullptr;
        }

        return ChangedPieces( movedFigure, movedFigurePos, capturedFigure , nullptr, {} );
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

    bool Board::wouldBeInCheck(const Move& move, MoveResult moveResult){
        const ChangedPieces changedPieces = simulateMove(move, moveResult, {});

        const CachedThreats cachedThreats = simulateUpdateThreatendSquares(changedPieces.m_CapturedPiece , move);

        bool wouldbeChecked = isInCheck(move.m_PlayerColor);

        revertSimulatedMove(move, changedPieces);
        revertSimulatedThreats(cachedThreats);

        return wouldbeChecked;
    }

    void Board::revertSimulatedMove(const Move& move, const ChangedPieces changedPieces){

        GameFigure* pMovedPiece = changedPieces.m_MovedPiece;
        GameFigure* pCapturedPiece = changedPieces.m_CapturedPiece;
        GameFigure* pCastelingRook = changedPieces.m_CastelingRook;

        if( pMovedPiece && changedPieces.m_MovedPiecePos.has_value() ){
            pMovedPiece->setPosition(changedPieces.m_MovedPiecePos.value());
            placeFigureOnBoard(pMovedPiece);
            m_BoardPositions[move.m_DesiredPosition.index()] = nullptr;
        }
        if( pCapturedPiece ){
            pCapturedPiece->toggleIsActive();
            placeFigureOnBoard(pCapturedPiece);
        }
        if( pCastelingRook && changedPieces.m_CastelingRookPos.has_value()){
            pCastelingRook->setPosition(changedPieces.m_CastelingRookPos.value());
            placeFigureOnBoard(pCastelingRook);
            int row = (move.m_PlayerColor) ? 0 : 7;
            int castleDirection = (move.getXOffSet() > 0) ? 5 : 3;
            Position rookToRemove(castleDirection, row);
            m_BoardPositions[rookToRemove.index()] = nullptr;
        }
    }

    void Board::placeFigureOnBoard(GameFigure* figureToPlace){
        m_BoardPositions[figureToPlace->getPosition().index()] = figureToPlace;
    }

    void Board::revertSimulatedThreats(CachedThreats cachedThreats){

        revertAddedThreats(cachedThreats, WHITE);
        revertAddedThreats(cachedThreats, BLACK);

        revertRemovedThreats(cachedThreats, WHITE);
        revertRemovedThreats(cachedThreats, BLACK);
    }

    void Board::revertAddedThreats(const CachedThreats cachedThreats, Color color){
        const std::vector<Position>& threatsToRemove = cachedThreats.getAddedThreats(color);

        m_GameState.removeThreatsFormThreatMap(threatsToRemove, color);
    }

    void Board::revertRemovedThreats(CachedThreats cachedThreats, Color color){
        std::vector<std::tuple<GameFigure*, std::vector<Position>>>& threatsToAdd = cachedThreats.getRemovedThreats(color);

        for(auto&[pFigure, removedThreats] : threatsToAdd){
            pFigure->setThreats(removedThreats);

            m_GameState.removeThreatsFormThreatMap(removedThreats, color);
        }
    }

    const GameFigure* Board::ExecuteNormalMove(const Move& move){

        GameFigure** movedFigure_ptr =  &m_BoardPositions[move.m_PiecePosition.index()];
        GameFigure** capturedFigure_ptr = ( m_BoardPositions[move.m_DesiredPosition.index()] ) ? &m_BoardPositions[move.m_DesiredPosition.index()] : nullptr;

        return editBoard(movedFigure_ptr, capturedFigure_ptr, move);
    }
    const GameFigure* Board::ExecuteCastelingMove(const Move& move){

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

        return nullptr;
    }
    const GameFigure* Board::ExecuteEnPassantMove(const Move& move){
        int movementDirection = (move.m_PlayerColor == WHITE) ? 1 : -1;
        Position capturedFigurePosition(move.m_DesiredPosition.x, move.m_DesiredPosition.y - movementDirection);
        GameFigure** movedFigure_ptr = &m_BoardPositions[move.m_PiecePosition.index()];
        GameFigure** capturedFigure_ptr =  ( m_BoardPositions[capturedFigurePosition.index()] ) ? &m_BoardPositions[capturedFigurePosition.index()]  : nullptr;

        return editBoard(movedFigure_ptr, capturedFigure_ptr, move);
    }
    const GameFigure* Board::ExecutePromotingMove(const Move& move, FigureType promotedFigureType){

        const GameFigure* capturedFigure = ExecuteNormalMove(move); 

        GameFigure* pDesiredPos = m_BoardPositions[move.m_DesiredPosition.index()];

        GameFigure promotedFigure = GameFigureFactory(promotedFigureType, move.m_PlayerColor, move.m_DesiredPosition);

        auto deltePawn_IT = std::find(m_Figures.begin(), m_Figures.end(), (*pDesiredPos));
        if(deltePawn_IT != m_Figures.end()){
            pDesiredPos->toggleIsActive();
            removeOldThreats(pDesiredPos);
        }
        m_Figures.push_back(std::move(promotedFigure));
        m_BoardPositions[move.m_DesiredPosition.index()] = &m_Figures.back();

        return capturedFigure;
    }
    ChangedPieces Board::simulateNormalMove(const Move& move){

        GameFigure** movedFigure_ptr =  &m_BoardPositions[move.m_PiecePosition.index()];
        GameFigure** capturedFigure_ptr = ( m_BoardPositions[move.m_DesiredPosition.index()] ) ? &m_BoardPositions[move.m_DesiredPosition.index()] : nullptr;
        
        return simulateEditBoard(movedFigure_ptr, capturedFigure_ptr, move);
    }
    ChangedPieces Board::simulateCastelingMove(const Move& move){

        bool shortCastle = (move.getXOffSet() > 0); 
        Position rook_Position = (shortCastle) ? Position(7, move.m_PiecePosition.y): Position(0, move.m_PiecePosition.y);
        Position rook_DesiredPosition = (shortCastle) ? Position(move.m_DesiredPosition.x -1, move.m_PiecePosition.y) : Position(move.m_DesiredPosition.x+1, move.m_PiecePosition.y);
        GameFigure** moved_King = &m_BoardPositions[move.m_PiecePosition.index()];
        GameFigure** moved_Rook = &m_BoardPositions[rook_Position.index()];

        GameFigure* pMovedKing = (*moved_King);
        Position movedKingPos = pMovedKing->getPosition();
        GameFigure* pMovedRook = (*moved_Rook);

        pMovedKing->setPosition(move.m_DesiredPosition);
        m_BoardPositions[move.m_DesiredPosition.index()] = *moved_King;
        (*moved_King)=nullptr;

        pMovedRook->setPosition(rook_DesiredPosition);
        m_BoardPositions[rook_DesiredPosition.index()] = *moved_Rook;
        (*moved_Rook)=nullptr;

        return ChangedPieces( pMovedKing, {movedKingPos} , nullptr , pMovedRook, {rook_Position} );
    }
    ChangedPieces Board::simulateEnPassantMove(const Move& move){
        int movementDirection = (move.m_PlayerColor == WHITE) ? 1 : -1;
        Position capturedFigurePosition(move.m_DesiredPosition.x, move.m_DesiredPosition.y - movementDirection);
        GameFigure** movedFigure_ptr = &m_BoardPositions[move.m_PiecePosition.index()];
        GameFigure** capturedFigure_ptr =  ( m_BoardPositions[capturedFigurePosition.index()] ) ? &m_BoardPositions[capturedFigurePosition.index()]  : nullptr;

        return  simulateEditBoard(movedFigure_ptr, capturedFigure_ptr, move);
    }
    ChangedPieces Board::simulatePromotingMove(const Move& move){
        return simulateNormalMove(move);
    }
}