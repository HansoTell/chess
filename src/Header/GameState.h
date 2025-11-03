#pragma once

namespace chess{

    struct GameState
    {
        bool m_HasWhiteKingMoved = false;
        bool m_isWhiteKingInCheck = false;
        bool m_HasWhiteARookMoved = false; 
        bool m_HasWhiteHRookMoved = false; 
        bool m_HasBlackARookMoved = false; 
        bool m_HasBlackHRookMoved = false; 
        bool m_HasBlackKingMoved = false;
        bool m_isBlackKingInCheck = false;
    };
}